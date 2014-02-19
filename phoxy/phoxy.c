/*      phoxy.c v1.3
		test w/:
 				gcc -Wall -ansi -o phoxy phoxy.c -lcrypto
				./proxy -t 10000 -u 62000 -r 127.0.0.1 -p 63000 [-e]
				./proxy -t 10000 -u 63000 -r 127.0.0.1 -p 62000 -c 127.0.0.1 [-e]

		usage: 	./proxy -t portTCP -u portUDP -r IPtoSEND -p portTOsendUDP [-e] [-c ipTOconnect] [-b BUFFSIZE=200000] [-m MAXPKTS=1] [-w waitTOresend=3500000] [-s SIZEPKTS=10000]

      Copyright 2012-2014 phra

      This program is free software; you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation; either version 2 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program; if not, write to the Free Software
      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
      MA 02110-1301, USA.


2013-01-29  phra  <phra@home>

 * v1.3 aggiunta cifratura AES



 2012-04-25  phra  <phra@linux-ipyd>

 * v1.2 fixati tutti i memory leaks e bug minori.



 2012-04-25  phra  <phra@linux-ipyd>

 * v1.1 aggiunta cache per memorizzare i pacchetti gia' serializzati.



 2012-04-25  phra  <phra@linux-ipyd>

 * v1.0 ripristinato flusso bidirezionale in tutte le situazioni.
 */


#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/param.h>
#include <sys/time.h>
#include <assert.h>
#include <getopt.h>
#include <openssl/evp.h>

#define BUFFSIZE 100
#define MAX_CONN 1
#define BODY (char)0
#define ACK (char)1
#define AES_BLOCK_SIZE 256
#define MAXSIZEPW 100

#define pulisci(p,size) memset((void*)p,0,size)
#define acapo() putchar('\n')
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define alloc(a) malloc(sizeof(a))

struct sockaddr_in way1;
/**
 * \var sentTCP
 * \brief variabile globale per statistiche.
 */
long unsigned int sentTCP = 0;
/**
 * \var sentUDP
 * \brief variabile globale per statistiche.
 */
long unsigned int sentUDP = 0;
/**
 * \var receivedTCP
 * \brief variabile globale per statistiche.
 */
long unsigned int receivedTCP = 0;
/**
 * \var receivedUDP
 * \brief variabile globale per statistiche.
 */
long unsigned int receivedUDP = 0;

/* SETTINGS */
/**
 * \var RECVBUF
 * \brief dimensione buffer in lettura da TCP.
 */
int RECVBUF = 200000;

/**
 * \var SIZEPACK
 * \brief dimensione pacchetti udp.
 */
int SIZEPACK = 10000;
/**
 * \var max
 * \brief massimo numero di pacchetti da mandare per volta.
 */
int max = 1;
/**
 * \var timer
 * \brief microsecondi da attendere prima di rinviare.
 */
int timer = 3500000;

/**
 * \var en
 * \brief struttura dati di openssl per l'encrypting.
 */

EVP_CIPHER_CTX en;

/**
 * \var de
 * \brief struttura dati di openssl per il decrypting.
 */

EVP_CIPHER_CTX de;

/**
 * \var have2crypt
 * \brief cifratura attiva.
 */

int have2crypt = 0;



/**
 * Create an 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/
int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx,
		EVP_CIPHER_CTX *d_ctx)
{
	int i, x,  nrounds = 5;
	unsigned char key[32], iv[32];

	/*
	 * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
	 * nrounds is the number of times the we hash the material. More rounds are more secure but
	 * slower.
	 */
	i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);
	if (i != 32) {
		printf("Key size is %d bits - should be 256 bits\n", i);
		return -1;
	}

	for(x = 0; x<32; ++x)
		printf("Key: %x iv: %x \n", key[x], iv[x]);

	for(x = 0; x<8; ++x)
		printf("salt: %x\n", salt[x]);

	EVP_CIPHER_CTX_init(e_ctx);
	EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
	EVP_CIPHER_CTX_init(d_ctx);
	EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

	return 0;
}

/*
 * Encrypt *len bytes of data
 * All data going in & out is considered binary (unsigned char[])
 */
unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
{
	/* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
	int c_len = *len + AES_BLOCK_SIZE - 1, f_len = 0;
	unsigned char *ciphertext = (unsigned char *)malloc(c_len);

	/* allows reusing of 'e' for multiple encryption cycles */
	if(!EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL)){
		printf("ERROR in EVP_EncryptInit_ex \n");
		return NULL;
	}

	/* update ciphertext, c_len is filled with the length of ciphertext generated,
	 *len is the size of plaintext in bytes */
	if(!EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len)){
		printf("ERROR in EVP_EncryptUpdate \n");
		return NULL;
	}

	/* update ciphertext with the final remaining bytes */
	if(!EVP_EncryptFinal_ex(e, ciphertext+c_len, &f_len)){
		printf("ERROR in EVP_EncryptFinal_ex \n");
		return NULL;
	}

	*len = c_len + f_len;
	return ciphertext;
}

/*
 * Decrypt *len bytes of ciphertext
 */
unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len)
{
	/* plaintext will always be equal to or lesser than length of ciphertext*/
	int p_len = *len, f_len = 0;
	unsigned char *plaintext = (unsigned char *)malloc(p_len);
	if(!EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL)){
		printf("ERROR in EVP_DecryptInit_ex \n");
		return NULL;
	}

	if(!EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len)){
		printf("ERROR in EVP_DecryptUpdate\n");
		return NULL;
	}

	if(!EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len)){
		printf("ERROR in EVP_DecryptFinal_ex\n");
		return NULL;
	}

	*len = p_len + f_len;
	return plaintext;
}


/**
 * funzione per stampare un messaggio d'errore ed uscire.
 * \param str stringa da stampare.
 */
void error(char* str){
	perror(str);
	fflush(stderr);
	exit(EXIT_FAILURE);
}

/**
 * funzione per stampare un messaggio d'errore ed uscire.
 * \param timeout indirizzo alla struttura timeval da cambiare.
 * \param sec secondi da impostare.
 * \param usec microsecondi da impostare.
 */
void set_timeout(struct timeval* timeout,int sec, int usec){

	pulisci(timeout,sizeof(struct timeval));
	timeout->tv_sec = sec;
	timeout->tv_usec = usec;
}
/**
 * funzione per aggiornare un timeout e verifica se e' scaduto.
 * \param timeout indirizzo alla struttura timeval da cambiare.
 * \param timepass timeval attuale.
 * \return 1 | 0
 */
int refreshtimeout(struct timeval* timeout, struct timeval timepass){
	timeout->tv_sec = timeout->tv_sec - timepass.tv_sec;
	timeout->tv_usec = timeout->tv_usec - timepass.tv_usec;
#ifdef DEBUG
	printf("refresh timeout: timeout contiene sec %d, usec %d.\n",timeout->tv_sec,timeout->tv_usec);
#endif
	if (((timeout->tv_sec * 1000000) + timeout->tv_usec) <= 0) return 1;
	else return 0;
}

/**
 * struttura del client.
 * \details la struttura memorizza un nick, l'indirizzo IP e l'fd associato.
 */
typedef struct client_t{
	/*@{*/
	char* nick; /**< nome del client */
	char* ip; /**< ip del client */
	int fd; /**< fd del client */
	/*@}*/
}client;

/**
 * funzione per creare un client.
 * \param nick nome del client.
 * \param addr indirizzo del client.
 * \param fd file descriptor associato al client.
 * \return puntatore al client
 */
client* crea_client(char* nick, struct in_addr addr, int fd){
	client* p = malloc(sizeof(client));
	pulisci(p,sizeof(*p));
	asprintf(&p->nick,"%s",nick);
	asprintf(&p->ip,"%s",inet_ntoa(addr));
	p->fd = fd;
	return p;
}

/**
 * funzione per aggiungere un client.
 * \param clients destinazione.
 * \param fdset puntatore a fd_set a cui aggiungere l'attuale fd.
 * \param fd file descriptor associato al client.
 * \param maxfd puntatore al valore fd massimo.
 * \param nick nome del client.
 * \param addr indirizzo del client.
 */
void aggiungi_client(int fd, client** clients,fd_set* fdset,int* maxfd, char* nick, struct in_addr addr){
	char* n = nick;
	struct in_addr a = addr;
	if (*clients == NULL){
		*clients = crea_client(n,a,fd);
		printf("[[ aggiunto %s con fd=%d(%s)]]\n",n,fd,(*clients)->ip);
		*maxfd = MAX(*maxfd,fd);
#ifdef DEBUG
		printf("maxfd = %d\n",*maxfd);
#endif
		FD_SET(fd,fdset);
	} else error("errore aggiungi client.");
}

/**
 * distruttore del client.
 * \param c puntatore al client di cui liberare le risorse allocate.
 */
void freeclient(client* c){
	free(c->nick);
	free(c->ip);
	free(c);
}

/**
 * funzione per rimuovere un client.
 * \param clients sorgente.
 * \param allset puntatore a fd_set da cui rimuovere l'attuale client.
 * \param maxfd puntatore al valore fd massimo.
 */
void rimuovi_client(client** clients,fd_set* allset,int* maxfd){
	if (*clients != NULL){
		close((*clients)->fd);
		FD_CLR((*clients)->fd,allset);
		if ((*clients)->fd == *maxfd) *maxfd -= 1;
		printf("[[ rimosso clients con fd=%d. ]]\n",(*clients)->fd);
		fflush(stdout);
		freeclient(*clients);
		*clients = NULL;
	}
}

/**
 * struttura del pacco.
 * \details la struttura memorizza l'id del segmento, il tipo, il payload originale, la forma serializzata e le loro dimensioni.
 */
typedef struct pacco_t {
	/*@{*/
	int id; /**< l'id del pacco */
	int sizepayload; /**< dimensione segmento */
	int sizebin; /**< dimensione forma serializzata */
	char type; /**< tipo */
	char* payload; /**< puntatore ai dati crudi */
	char* bin; /**< puntatore alla forma serializzata */
	/*@}*/
}pacco;

/**
 * funzione per creare un pacco.
 * \param id id del pacco.
 * \param size dimensione del pacco.
 * \param type tipo di pacco.
 * \param body puntatore al payload.
 * \return puntatore al pacco.
 */
pacco* crea_pacco(int id, int size, char* body, char type){
	char* b = malloc(size*sizeof(char));
	pacco* p = malloc(sizeof(pacco));
	pulisci(p,sizeof(pacco));
	p->id = id;
	p->sizepayload = size;
	memcpy(b,body,size);
	p->payload = b;
	p->type = type;
	p->bin = NULL;
	p->sizebin = 0;
	return p;
}

/**
 * distruttore del pacco.
 * \param p puntatore al pacco di cui liberare le risorse allocate.
 */
void freepacco(pacco* p){
	free(p->payload);
	free(p->bin);
	free(p);
}

/**
 * stampa informazioni sul pacco.
 * \param p puntatore al pacco da stampare.
 */
void stampapacco(pacco* p){
	printf("id=%d,type=%d,sizepayload=%d.\n",p->id,p->type,p->sizepayload);
#ifdef SUPERDEBUG
	int i = 0;
	for (;i<p->sizepayload;i++) printf("%c",p->payload[i]);
	acapo();
#endif
}

/**
 * struttura della lista dei pacchi.
 * \details si tratta di una lista con sentinella linkata in avanti, terminata da un puntatore a NULL. ogni entry ha un timeout associato per il resend.
 */
typedef struct listapacchi_t {
	/*@{*/
	pacco* this; /**< puntatore al pacco */
	struct listapacchi_t* next; /**< puntatore al successivo */
	struct timeval timeout; /**< timeout */
	/*@}*/
}listapacchi;

/**
 * funzione per aggiungere un pacco in coda ad una lista di pacchi.
 * \param root puntatore alla lista.
 * \param pkt pacco da aggiungere.
 */
void aggiungi_pacco_coda(listapacchi* root,pacco* pkt){
	listapacchi* p = root;
	pacco* pack = pkt;
	listapacchi* ultimo = malloc(sizeof(listapacchi));
	pulisci(ultimo,sizeof(*ultimo));
	while(p->next != NULL) p = p->next;
	ultimo->this = pack;
	ultimo->next = NULL;
	p->next = ultimo;
	set_timeout(&ultimo->timeout,0,0);
#ifdef DEBUG
	printf("aggiunto pacco alla lista con id=%d.\n",pkt->id);
#endif
}

/**
 * funzione per aggiungere un pacco in testa ad una lista di pacchi.
 * \param root puntatore alla lista.
 * \param pkt pacco da aggiungere.
 */
void aggiungi_pacco_testa(listapacchi* root,pacco* pkt){
	listapacchi* p = root;
	pacco* pack = pkt;
	listapacchi* primo = malloc(sizeof(listapacchi));
	pulisci(primo,sizeof(listapacchi));
	primo->this = pack;
	primo->next = p->next;
	p->next = primo;
	set_timeout(&primo->timeout,0,30000);
#ifdef DEBUG
	printf("aggiunto pacco alla lista con id=%d.\n",pkt->id);
#endif
}

/**
 * funzione per aggiungere un pacco ad una lista ordinata di pacchi.
 * \param root puntatore alla lista.
 * \param pkt pacco da aggiungere.
 */
void aggiungi_pacco_ordinato(listapacchi* root, pacco* pkt){
	listapacchi* p = root;
	listapacchi* add = alloc(listapacchi);
	pulisci(add,sizeof(listapacchi));
	while((p->next != NULL)&&(p->next->this->id < pkt->id))	p = p->next;
	add->this = pkt;
	add->next = p->next;
	p->next = add;
	set_timeout(&p->timeout,0,30000);
#ifdef DEBUG
	printf("aggiunto pacco alla lista con id=%d.\n",pkt->id);
#endif
}

/**
 * funzione per rimuovere un pacco da una lista.
 * \param root puntatore alla lista.
 * \param id id del pacco da rimuovere.
 */
void rimuovi_pacco(listapacchi* root,int id){
	listapacchi* p = root;
#ifdef DEBUG
	printf("rimuovi_pacco con id=%d!\n",id);
#endif
	while ((p->next != NULL) && (p->next->this->id != id)) p = p->next;
	if ((p->next != NULL) && (p->next->this->id == id)){
		listapacchi* new = p->next->next;
		freepacco(p->next->this);
		free(p->next);
		p->next = new;
	}
	else {
#ifdef DEBUG
		printf("doppio ACK con id=%d.\n",id);
#endif
	}
}

/**
 * funzione per verificare la presenza di un dato pacco.
 * \param root puntatore alla lista.
 * \param id id del pacco da cercare.
 * \return puntatore al pacco | NULL
 */
listapacchi* inlista(listapacchi* root, int id){
	listapacchi* p = root;
	if (p->next == NULL) return NULL;
	do {
		p = p->next;
		if (p->this->id == id) return p;
	} while (p->next != NULL);
	return NULL;
}

/**
 * funzione per stampare una lista di pacchi.
 * \param root puntatore alla lista.
 */
void stampalista(listapacchi* root){
	int i = 0;
	listapacchi* pointer = root;
	while(pointer->next != NULL){
		pointer = pointer->next;
		printf("IN POSIZIONE %d: ",i++);
		stampapacco(pointer->this);
	}
}

/**
 * funzione per serializzare un pacco.
 * \param pkt puntatore al pacco da serializzare.
 * \return dimensione del pacco in forma serializzata.
 */
ssize_t serializepacco(pacco* pkt){
	char B = 'B';
	char A = 'A';
	int id = htonl(pkt->id);
	int sizepayload = htonl(pkt->sizepayload);
	char type = pkt->type;
	char* payload = pkt->payload;
	char* serial = malloc((10+pkt->sizepayload)*sizeof(char));
	char* p;
	int temp = *((int*)pkt->payload);
	int sizenc;
	pacco* pack = pkt;
	assert(serial != NULL);
	p = serial;
	memcpy(p,&id,4);
	p += 4;
	memcpy(p,&B,1);
	p += 1;
#ifdef DEBUG
	printf("serial is %lu.\ntype is %d.\npkt->sizepayload is = %d.\n",serial,type,pkt->sizepayload);
	fflush(stdout);
#endif
	switch(type){
		case 0:
			memcpy(p,&B,1);
			memcpy(p+1,&sizepayload,4);
			p += 5;
			memcpy(p,payload,pkt->sizepayload);
			p += pkt->sizepayload;
			break;
		case 1:
			memcpy(p,&A,1);
			p += 1;
			memcpy(p,&temp,pkt->sizepayload);
			p += pkt->sizepayload;
			break;
		default:
			error("pacchetto fuori dal protocollo in uscita.\n");
	}
	sizenc = p - serial;
	if (have2crypt){
		pack->bin = (char*)aes_encrypt(&en,(unsigned char*)serial,&sizenc);
		free(serial);
	} else 	pack->bin = serial;
	pack->sizebin = sizenc;
	return pack->sizebin;
}

/**
 * funzione per deserializzare un pacco.
 * \param bin puntatore al buffer ricevuto.
 * \param pkt puntatore al puntatore da assegnare.
 * \param size dimensione ciphertext
 */
void deserializepacco(char* bin, pacco** pkt, int size){
	int len = size, id;
	char* plain,* b;
	char type1;
	if (have2crypt){
		plain = (char *)aes_decrypt(&de,(unsigned char *)bin,&len);
		b = plain;
	} else b = bin;
	id = ntohl(*(int*)b);
	type1 = b[4];
	if (type1 == 'B') {
		char type2 = b[5];
		if (type2 == 'B'){
			int sizepayload = ntohl(*((int*)&b[6]));
			char* payload = &b[10];
#ifdef DEBUG
			printf("deserializzo un pacchetto body.\n");
#endif
			*pkt = crea_pacco(id,sizepayload,payload,BODY);
		} else if (type2 == 'A') {
			int* ptoi = (int*)&b[6];
			int temp = *ptoi;
#ifdef DEBUG
			printf("ricevuto un ACK con id=%d.\n",ntohl(temp));
#endif
			*pkt = crea_pacco(id,4,(char*)&temp,ACK);
		} else error("pacchetto fuori dal protocollo in entrata.\n");
	} else error("pacchetto fuori dal protocollo in entrata.\n");
	if (have2crypt) free(plain);
}

/**
 * funzione per ricevere dati da TCP.
 * \param fd fd da cui ricevere.
 * \param id puntatore alla variabile di conteggio.
 * \param root puntatore alla lista a cui aggiungere i segmenti arrivati.
 * \return dimensione del pacco in forma serializzata.
 */
int recvtcp(int fd, int* id, listapacchi* root){
	char* buf = malloc(RECVBUF*sizeof(char));
	ssize_t rd;
	listapacchi* rootTOnet = root;
	pulisci(&rd,sizeof(rd));
	do {
		if (((rd = read(fd,buf,RECVBUF)) < 0) && (errno != EINTR)) error("errore nella recv del pacchetto tcp.\n");
	} while (errno == EINTR);
	if (rd > 0){
		int i,n;
		pacco* p;
		int num = rd / SIZEPACK;
		i = n = 0;
#ifdef DEBUG
		printf("recvtpc: ricevuto pacco di dimensione rd=%d\n",rd);
#endif
		for(;i<num;i++){
			p = crea_pacco(*id,SIZEPACK,&buf[n],BODY);
			aggiungi_pacco_testa(rootTOnet,p);
			*id = *id +1;
			n += SIZEPACK;
#ifdef DEBUG
			stampapacco(p);
#endif
		}
		p = crea_pacco(*id,rd-n,&buf[n],BODY);
		aggiungi_pacco_testa(rootTOnet,p);
		*id = *id +1;
		n += SIZEPACK;
#ifdef DEBUG
		stampapacco(p);
#endif
		receivedTCP += rd;
		free(buf);
		return 0;
	}
	else { free(buf); return -1; }
}

/**
 * funzione di basso livello per inviare pacchi su TCP.
 * \param fd fd su cui inviare.
 * \param p puntatore al pacco da inviare.
 */
void _sendtcp(int fd, pacco* p){
	ssize_t wr,n;
	int size = p->sizepayload;
	pulisci(&wr,sizeof(wr));
	pulisci(&n,sizeof(n));
#ifdef DEBUG
	printf("_sendtcp pkt id=%d.\n",p->id);
#endif
	do {
		if ((wr = send(fd,&p->payload[n],size-n,MSG_NOSIGNAL)) < 0) {
			if (errno == EINTR) continue;
			else error("errore nella send del pacchetto tcp.\n");
		}
		n += wr;
	} while (n < size);
	sentTCP += n;
}

/**
 * funzione per inviare un segmento ricostruito.
 * \param fd fd su cui inviare.
 * \param p puntatore al pacco da inviare.
 * \param id puntatore al contatore globale per sapere quali pacchi poter inviare.
 * \param lista puntatore alla lista dei pacchi.
 */
void sendtcp(int fd, pacco* p, int* id, listapacchi* lista){
	listapacchi* pointer = lista;
	_sendtcp(fd,p);
	rimuovi_pacco(lista,*id);
	*id += 1;
	while((pointer->next != NULL) && (pointer->next->this->id == *id)){
		_sendtcp(fd,pointer->next->this);
		rimuovi_pacco(lista,*id);
		*id += 1;
	}
}

/**
 * funzione per stampare a schermo la forma serializzata di un pacco.
 * \param pkt pacco da stampare.
 */
void stampabinario(pacco* pkt){
	int temp = 0;
	printf("il binario contiene (id e size stampati in host order):\n");
	printf("%d",ntohl(*(int*)pkt->bin));
	temp = 4;
	printf("%c",pkt->bin[temp++]);
	printf("%c",pkt->bin[temp++]);
	printf("%d",ntohl(*(int*)&pkt->bin[temp]));
	temp += 4;
	while (temp < pkt->sizebin){
		printf("%c",pkt->bin[temp++]);
	}
	acapo();
	fflush(stdout);
}

/**
 * funzione di basso livello per inviare un pacco su una delle tre vie.
 * \param fd fd su cui inviare.
 * \param address canale da utilizzare.
 * \param p pacco da inviare.
 */
void _sendudp(int fd, pacco* p, struct sockaddr_in* address) { /*this is the problem*/
	ssize_t wr,n,size;
	pacco* pkt = p;
	struct sockaddr_in* addr = address;
#ifdef DEBUG
	printf("_sendudp con parametri: fd=%d, p=%lu, address=%lu.\n",fd,p,address);
	fflush(stdout);
#endif
	pulisci(&wr,sizeof(wr));
	pulisci(&n,sizeof(n));
	pulisci(&size,sizeof(size));
	if (pkt->bin == NULL) serializepacco(pkt);
#ifdef DEBUG
	printf("size=%d.\n",pkt->sizebin);
#endif
#ifdef SUPERDEBUG
	stampabinario(pkt);
#endif
	do {
		if ((wr = sendto(fd,pkt->bin,pkt->sizebin,MSG_NOSIGNAL,(struct sockaddr*)addr,sizeof(*addr))) < 0) {
			if (errno == EINTR) continue;
			else error("errore nella send del pacchetto udp.\n");
		}
	} while (errno == EINTR);
#ifdef DEBUG
	assert(wr == pkt->sizebin);
#endif
	sentUDP += wr;
}

/**
 * funzione per inviare un pacco.
 * \param fd fd su cui inviare.
 * \param p pacco da inviare.
 */
void sendudp(int fd, pacco* p) {
	pacco* pkt = p;
	_sendudp(fd,pkt,&way1);
}

/**
 * funzione per ricevere pacchi su UDP.
 * \param fd da cui ricevere.
 */
pacco* recvudp(int fd){
	char* buf = malloc(RECVBUF*sizeof(char));
	ssize_t rd;
	pacco* p;
	pulisci(&rd,sizeof(rd));
	pulisci(buf,RECVBUF);
	do {
		if (((rd = read(fd,buf,RECVBUF)) < 0) && (errno != EINTR)) error("errore nella recv del pacchetto tcp.\n");
	} while (errno == EINTR);
	deserializepacco(buf,&p,rd);
	receivedUDP += rd;
	free(buf);
	return p;
}

/**
 * funzionare per inviare ack.
 * \param fd su cui inviare.
 * \param id id del pacco da confermare.
 */
void sendack(int fd, int id){
	int idn = htonl(id);
	pacco* acked = crea_pacco(0,4,(char*)&idn,ACK);
#ifdef DEBUG
	printf("mando ack con id=%d.\n",id);
#endif
	sendudp(fd,acked);
	freepacco(acked);
}

/**
 * funzionare stampare statistiche e uscire.
 * \param sign numero segnale
 */
void qstats(int sign){
	EVP_CIPHER_CTX_cleanup(&de);
	EVP_CIPHER_CTX_cleanup(&en);
	printf("\nricevuto segnale %d.\n",sign);
	printf("sentTCP=%lu\nreceivedTCP=%lu\nsentUDP=%lu\nreceivedUDP=%lu\n",sentTCP,receivedTCP,sentUDP,receivedUDP);
	exit(0);
}


/**
 * ALL STARTS HERE!
 * \details usage: ./proxy -t portTCP -u portUDP -r IPtoSEND -p portTOsendUDP [-e] [-c ipTOconnect] [-b BUFFSIZE=200000] [-m MAXPKTS=1] [-w waitTOresend=3500000] [-s SIZEPKTS=60000]
 */
int main(int argc, char **argv)
{
	struct sockaddr_in addrserv,addrcli;
	int fdcli,fdlisten,fdudp,i,ready,maxfd,maxclient,IDtoCLIENT,IDtoNET,have2connect, maxpkts, c, option_index,port,port2,port3,keysize;
	client* CLIENT;
	char buf[BUFFSIZE],pw[MAXSIZEPW];
	char* IPrit, *IPconnect;
	fd_set allset;
	listapacchi rootTOnet,rootTOclient;
	extern char* optarg;
	extern int optind, opterr, optopt;
	struct sigaction new_actionq;
	unsigned char salt[] = {255,3,1,23,63,129,90,248};
	struct option long_options[] = {
		{"tcp", required_argument, 0, 't'},
		{"udp", required_argument, 0, 'u'},
		{"target", required_argument, 0, 'r'},
		{"port", required_argument, 0, 'p'},
		{"connect", required_argument, 0, 'c'},
		{"buffer", required_argument, 0, 'b'},
		{"maxpkts", required_argument, 0, 'm'},
		{"sizepkts", required_argument, 0, 's'},
		{"wait", required_argument, 0, 'w'},
		{"encrypt", no_argument, 0, 'e'},
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};
	pulisci(&rootTOnet,sizeof(listapacchi));
	pulisci(&rootTOclient,sizeof(listapacchi));
	pulisci(&way1,sizeof(way1));
	pulisci(&addrserv,sizeof(addrserv));
	pulisci(&addrcli,sizeof(addrcli));
	option_index = have2connect = IDtoCLIENT = IDtoNET = fdcli = fdlisten = i = ready = maxfd = maxclient = 0;
	rootTOnet.this = NULL;
	rootTOnet.next = NULL;
	rootTOclient.this = NULL;
	rootTOclient.next = NULL;
	CLIENT = NULL;
	if (argc == 1) error("usage: ./proxy -t portTCP -u portUDP -r IPtoSEND -p portTOsendUDP [-e] [-c ipTOconnect] [-b BUFFSIZE=200000] [-m MAXPKTS=1] [-w waitTOresend=3500000] [-s SIZEPKTS=10000]");
	new_actionq.sa_handler = qstats;
	sigemptyset(&new_actionq.sa_mask);
	new_actionq.sa_flags = 0;
	if (sigaction(SIGINT, &new_actionq, NULL) < 0) error("errore sigaction. (SIGINT)");
	if (sigaction(SIGHUP, &new_actionq, NULL) < 0) error("errore sigaction. (SIGHUP)");
	if (sigaction(SIGQUIT, &new_actionq, NULL) < 0) error("errore sigaction. (SIGQUIT)");
	for(;;){
		if ((c = getopt_long(argc, argv, "t:u:r:p:c:a:b:m:w:s:eh", long_options, &option_index)) == -1) break;
		switch (c){
			case 't':
				port = atoi(optarg);
#ifdef DEBUG
				printf("port=%d ",port);
#endif
				break;
			case 'u':
				port2 = atoi(optarg);
#ifdef DEBUG
				printf("port2=%d ",port2);
#endif
				break;
			case 'r':
				IPrit = optarg;
				way1.sin_addr.s_addr = inet_addr(optarg);
#ifdef DEBUG
				printf("addr=%s ",optarg);
#endif
				break;
			case 'p':
				port3 = atoi(optarg);
#ifdef DEBUG
				printf("port3=%d ",port3);
#endif
				break;
			case 'c':
				have2connect = 1;
				IPconnect = optarg;
				addrserv.sin_addr.s_addr = inet_addr(optarg);
#ifdef DEBUG
				printf("connect to %s ",optarg);
#endif
				break;
			case 'b':
				RECVBUF = atoi(optarg);
#ifdef DEBUG
				printf("buffer di %d bytes.\n",RECVBUF);
#endif
				break;
			case 's':
				if (atoi(optarg) < 65000) SIZEPACK = atoi(optarg);
#ifdef DEBUG
				printf("pkts di %d bytes.\n",SIZEPACK);
#endif
				break;
			case 'm':
				max = atoi(optarg);
#ifdef DEBUG
				printf("maxpakts = %d \n",max);
#endif
				break;
			case 'w':
				if (atoi(optarg) > 0) timer = atoi(optarg);
#ifdef DEBUG
				printf("attendo %d microsecondi prima di rimandare il pacchetto.\n",timer);
#endif
				break;
			case 'e':
				have2crypt = 1;
#ifdef DEBUG
				printf("cifratura attiva.\n");
#endif
				break;
			case 'h':
			default:
				error("usage: ./proxy -t portTCP -u portUDP -r IPtoSEND -p portTOsendUDP [-c ipTOconnect] [-b BUFFSIZE=100000] [-m MAXPKTS=1] [-w waitTOresend=3500000] [-s SIZEPKTS=10000]");
		}
	}
	printf("~SETTINGS: portaTCP=%d, portaUDP=%d, IPtoSEND=%s, FIRSTportTosendUDP=%d, ",port,port2,IPrit,port3);
	if (have2connect) printf("ipTOconnect=%s ",IPconnect);
	printf("BUFFSIZEinLETTURA=%d bytes, MAX_PKTS_EACH_TIME=%d, ATTESAprimaDIrimandare=%d microsecondi, DIMENSIONEpacchettiUDP=%d bytes, ",RECVBUF,max,timer,SIZEPACK);

	way1.sin_family = AF_INET;
	way1.sin_port = htons(port3);
	addrserv.sin_family = AF_INET;
	addrserv.sin_port = htons(port);
	if (have2crypt){
		printf("INSERISCI PW\n");
		fgets(pw,MAXSIZEPW,stdin);
		keysize = strlen(pw);
		if (pw[keysize-1] == '\n'){
			pw[keysize-1] = '\0';
			keysize--;
		}
		if (aes_init((unsigned char*)pw,keysize,salt,&en,&de))
			error("\ninizializzazione del cifrario AES fallita.");
	}
	/* connessione tcp */
	printf("\n[[ configurazione, ");
	if (!have2connect){
		int optval = 1;
		addrserv.sin_addr.s_addr = htonl(INADDR_ANY);
		printf("creo il socket, ");
		if ((fdlisten = socket(AF_INET,SOCK_STREAM,0)) < 0) error("errore socket tcp.");
		printf("faccio la bind, ");
		setsockopt(fdlisten,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
		if (bind(fdlisten,(struct sockaddr*) &addrserv, sizeof(addrserv)) < 0) error("errore nella bind.");
		printf("faccio la listen ");
		if (listen(fdlisten,MAX_CONN) < 0) error("errore nella listen.");
		printf("e rimango in attesa di una connessione sulla porta %d. ]]\n",port);
	} else {
		if ((fdcli = socket(AF_INET,SOCK_STREAM,0)) < 0) error("errore socket tcp.");
		if (connect(fdcli,(struct sockaddr*)&addrserv,sizeof(addrserv)) < 0) error("errore nella connect.");
		printf("[[ client connesso. ]]\n");
		strncpy(buf,"CLIENT",strlen("CLIENT")+1);
		aggiungi_client(fdcli,&CLIENT,&allset,&maxfd,buf,addrserv.sin_addr);
	}
	/* connessione udp */
	pulisci((void*)&addrserv,sizeof(addrserv));
	addrserv.sin_family = AF_INET;
	addrserv.sin_addr.s_addr = htonl(INADDR_ANY);
	addrserv.sin_port = htons(port2);
	if ((fdudp = socket(AF_INET,SOCK_DGRAM,0)) < 0) error("errore socket udp.");
	if (bind(fdudp,(struct sockaddr*) &addrserv, sizeof(addrserv)) < 0) error("errore nella bind.");
	/*configurazione select*/
	FD_ZERO(&allset);
	FD_SET(STDIN_FILENO,&allset);
	if (!have2connect){
		FD_SET(fdlisten,&allset);
	} else {
		FD_SET(fdcli,&allset);
	}
	FD_SET(fdudp,&allset);
	maxfd = fdudp;

	for (;;){
		fd_set fdset = allset;
		struct timeval timeout,timeofdaypre,timeofdaypost,timepass;
		listapacchi* listout = &rootTOnet;
		maxpkts = max;
		set_timeout(&timeout,0,1000000);
		gettimeofday(&timeofdaypre,NULL);
		do {
			ready = select(maxfd+1,&fdset,NULL,NULL,&timeout);
		} while (errno == EINTR);
		if (ready < 0) error("errore nella select.");
		if (!ready){
#ifdef DEBUG
			printf("[[ nessuna attivita' nei 1 secondi precedenti. ]]\n");
#endif
		}
		gettimeofday(&timeofdaypost,NULL);
		timepass.tv_sec = timeofdaypost.tv_sec - timeofdaypre.tv_sec;
		timepass.tv_usec = timeofdaypost.tv_usec - timeofdaypre.tv_usec;
#ifdef DEBUG
		printf("ci sono %d fd pronti.\n",ready);
		printf("timepass contiene: sec: %d,usec: %d.\n",timepass.tv_sec,timepass.tv_usec);
		printf("IDtoNET=%d,IDtoCLIENT=%d.\n",IDtoNET,IDtoCLIENT);
#endif
		if (ready != 0) {
			if (!have2connect){
				if (FD_ISSET(fdlisten,&fdset)){
					socklen_t len = sizeof(addrcli);
					pulisci(buf,BUFFSIZE);
					fdcli = accept(fdlisten,(struct sockaddr*)&addrcli,&len);
					printf(" client connesso. ]]\n");
					strncpy(buf,"CLIENT",strlen("CLIENT")+1);
					aggiungi_client(fdcli,&CLIENT,&allset,&maxfd,buf,addrcli.sin_addr);
					ready--;
				}
			}
			if (FD_ISSET(fdcli,&fdset)) {
#ifdef DEBUG
				printf("IDtoNET = %d.\n",IDtoNET);
#endif
				if (recvtcp(fdcli,&IDtoNET,&rootTOnet)) rimuovi_client(&CLIENT,&allset,&maxfd);
			}
			if (FD_ISSET(fdudp,&fdset)) {
				pacco* pkt = recvudp(fdudp);
				if (pkt != NULL){
					int temp;
					switch (pkt->type){
						case BODY:
#ifdef TEST
							printf("[[ arrivato un pacchetto di tipo BODY. ]]\n");
#endif
							/*arrivato un pacchetto body*/
							if (pkt->id >= IDtoCLIENT){
								if (pkt->id == IDtoCLIENT){
#ifdef TEST
									printf("[[ mando al client pkt con id=%d e successivi. ]]\n",pkt->id);
#endif
									sendtcp(fdcli,pkt,&IDtoCLIENT,&rootTOclient);
									sendack(fdudp,pkt->id);
									freepacco(pkt);
								}
								else if (!inlista(&rootTOclient,pkt->id)){
									aggiungi_pacco_ordinato(&rootTOclient,pkt);
									sendack(fdudp,pkt->id);
#ifdef TEST
									printf("[[ mancano dei pkt prima di inviare questo (id=%d). accodo. ]]\n",pkt->id);
#endif
#ifdef DEBUG
									stampalista(&rootTOclient);
#endif
								} else {
									/*pacco doppio*/
#ifdef TEST
									printf("[[ ricevuto doppio pkt (id=%d). rimando ack. ]]\n",pkt->id);
#endif
									sendack(fdudp,pkt->id);
									freepacco(pkt);
								}
							} else {
								/*pacco doppio*/
#ifdef TEST
								printf("[[ ricevuto doppio pkt (id=%d). rimando ack. ]]\n",pkt->id);
#endif
								sendack(fdudp,pkt->id);
								freepacco(pkt);
							}
							break;
						case ACK:
							/*arrivato un ack*/
							temp = ntohl(*(int*)pkt->payload);
							rimuovi_pacco(listout,temp);
							freepacco(pkt);
							break;
						default:
							error("ricevuto pacchetto fuori dal protocollo.\n");
					}
				}
			}
		}
		while ((listout->next != NULL) && (maxpkts > 0)) {
			listout = listout->next;
#ifdef DEBUG
			printf("ciao,sono un pacco in lista at %lu con id=%d.\n",listout->this,listout->this->id);
#endif
			if (refreshtimeout(&listout->timeout,timepass)) {
#ifdef TEST
				printf("timer scaduto, inviamo pacco con id=%d.\n",listout->this->id);
#endif
				sendudp(fdudp,listout->this);
				set_timeout(&listout->timeout,0,timer);
				maxpkts -= 1;
			}
		}
	}
	return 0;
}

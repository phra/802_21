//==============================================================================
// Brief   : MIH-User
// Authors : Bruno Santos <bsantos@av.it.pt>
//------------------------------------------------------------------------------
// ODTONE - Open Dot Twenty One
//
// Copyright (C) 2009-2013 Universidade Aveiro
// Copyright (C) 2009-2013 Instituto de Telecomunicações - Pólo Aveiro
//
// This software is distributed under a license. The full license
// agreement can be found in the file LICENSE in this distribution.
// This software may not be copied, modified, sold or distributed
// other than expressed in the named license agreement.
//
// This software is distributed without any warranty.
//==============================================================================

#include <odtone/base.hpp>
#include <odtone/debug.hpp>
#include <odtone/logger.hpp>
#include <odtone/mih/request.hpp>
#include <odtone/mih/response.hpp>
#include <odtone/mih/indication.hpp>
#include <odtone/mih/confirm.hpp>
#include <odtone/mih/tlv_types.hpp>
#include <odtone/sap/user.hpp>

#include <boost/utility.hpp>
#include <boost/bind.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream>

///////////////////////////////////////////////////////////////////////////////

static const char* const kConf_MIH_Commands = "user.commands";
///////////////////////////////////////////////////////////////////////////////

namespace po = boost::program_options;

using odtone::uint;
using odtone::ushort;

odtone::logger log_("mih_usr", std::cout);

///////////////////////////////////////////////////////////////////////////////
void __trim(odtone::mih::octet_string &str, const char chr)
{
    str.erase(std::remove(str.begin(), str.end(), chr), str.end());
}

/**
 * Parse supported commands.
 *
 * @param cfg Configuration options.
 * @return A bitmap mapping the supported commands.
 */
boost::optional<odtone::mih::mih_cmd_list> parse_supported_commands(const odtone::mih::config &cfg)
{
    using namespace boost;

    odtone::mih::mih_cmd_list commands;
    bool has_cmd = false;

    std::map<std::string, odtone::mih::mih_cmd_list_enum> enum_map;
    enum_map["mih_link_get_parameters"]       = odtone::mih::mih_cmd_link_get_parameters;
    enum_map["mih_link_configure_thresholds"] = odtone::mih::mih_cmd_link_configure_thresholds;
    enum_map["mih_link_actions"]              = odtone::mih::mih_cmd_link_actions;
    enum_map["mih_net_ho_candidate_query"]    = odtone::mih::mih_cmd_net_ho_candidate_query;
    enum_map["mih_net_ho_commit"]             = odtone::mih::mih_cmd_net_ho_commit;
    enum_map["mih_n2n_ho_query_resources"]    = odtone::mih::mih_cmd_n2n_ho_query_resources;
    enum_map["mih_n2n_ho_commit"]             = odtone::mih::mih_cmd_n2n_ho_commit;
    enum_map["mih_n2n_ho_complete"]           = odtone::mih::mih_cmd_n2n_ho_complete;
    enum_map["mih_mn_ho_candidate_query"]     = odtone::mih::mih_cmd_mn_ho_candidate_query;
    enum_map["mih_mn_ho_commit"]              = odtone::mih::mih_cmd_mn_ho_commit;
    enum_map["mih_mn_ho_complete"]            = odtone::mih::mih_cmd_mn_ho_complete;

    std::string tmp = cfg.get<std::string>(kConf_MIH_Commands);
    __trim(tmp, ' ');

    char_separator<char> sep1(",");
    tokenizer< char_separator<char> > list_tokens(tmp, sep1);

    BOOST_FOREACH(std::string str, list_tokens) {
        if(enum_map.find(str) != enum_map.end()) {
            commands.set((odtone::mih::mih_cmd_list_enum) enum_map[str]);
            has_cmd = true;
        }
    }

    boost::optional<odtone::mih::mih_cmd_list> supp_cmd;
    if(has_cmd)
        supp_cmd = commands;

    return supp_cmd;
}


#include "boost/date_time/posix_time/posix_time.hpp"
#define MAX_LINKS 5
#define MAX_BUF 66000
#define LINK_TIMEOUT 10L
#define HB_TIMEOUT 3
#define IP_SENDBACK "127.0.0.1"
#define PORT_SENDBACK 10001
#define PORT_LSOCK 10000
#define PORT_DEST 11000
#define PORT_DATA_SOCK 12000
#define MIH_LISTEN 12345
#define MIH_SEND 1025
/*
#define PORT_SENDBACK 9998
#define PORT_LSOCK 9999
#define PORT_DEST 12000
#define PORT_DATA_SOCK 11000
#define MIH_LISTEN 1234
#define MIH_SEND 1025*/


class interfaces : boost::noncopyable {

    private:
        struct endpoint {
            bool up;
            boost::asio::ip::udp::endpoint* dest;
            boost::posix_time::ptime lasthb;
        };

        struct info {
            bool up;
            odtone::mih::mac_addr* mac;
            odtone::mih::link_type type;
            boost::asio::ip::udp::socket* sock;
            char* buf;
        };

        struct info data[MAX_LINKS];
        struct endpoint destinations[MAX_LINKS];
        char buf[MAX_BUF];
        boost::asio::io_service io,iol,iot;
        boost::asio::ip::udp::socket lsock = boost::asio::ip::udp::socket(iol); 
        boost::asio::ip::udp::endpoint src = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(IP_SENDBACK),PORT_SENDBACK);
        boost::asio::deadline_timer* timer;
        boost::asio::ip::udp::endpoint sender,lsender;
        bool sendback_active = false;
        odtone::uint port_counter = 0;

        void sendback(char* buf, odtone::uint len) {
            log_(0,__FUNCTION__, " sending back ",len," bytes");
            lsock.send_to(boost::asio::buffer(buf,len),src);
        }

        odtone::sint ip_to_index(boost::asio::ip::address addr) {
            odtone::sint i = 0;
            for (; i < MAX_LINKS; i++) {
                if (destinations[i].dest->address() == addr)
                    return i;
            }
            return -1;
        }

        void handle_receive_pacco(const boost::system::error_code& error, size_t received_bytes, odtone::uint i) {
            odtone::sint j = ip_to_index(this->sender.address());
            log_(0,__FUNCTION__, "received pkt from ", this->sender.address(),":",this->sender.port());
            if (error) {
                log_(0,__FUNCTION__," errore callback",error);
                return;
            } 
            if (received_bytes == 0) {
                log_(0,__FUNCTION__," heartbeat");
                destinations[j].up = true;
                destinations[j].lasthb = boost::posix_time::second_clock::local_time();
            } else if (this->data[i].buf != NULL && received_bytes > 0) {
                log_(0,__FUNCTION__," ", received_bytes," bytes received");
                sendback(this->data[i].buf,received_bytes);
            }
            if (this->data[i].sock != NULL) 
                this->data[i].sock->async_receive_from(
                        boost::asio::buffer(this->data[i].buf,MAX_BUF), this->sender,
                        boost::bind(&interfaces::handle_receive_pacco, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred,i));

        }

        void handle_receive(const boost::system::error_code& error, size_t received_bytes) {
            this->sendto(buf,received_bytes);
            log_(0,__FUNCTION__);
            lsock.async_receive_from(
                    boost::asio::buffer(buf,MAX_BUF), this->lsender,
                    boost::bind(&interfaces::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
        }

    public:
        interfaces() {
            memset(data,0,MAX_LINKS*sizeof(struct info));
            memset(destinations,0,MAX_LINKS*sizeof(struct endpoint));
            memset(buf,0,MAX_BUF);
            for (odtone::uint i = 0; i < MAX_LINKS; i++) {
                data[i].up = destinations[i].up = false;
            }
            lsock.open(boost::asio::ip::udp::v4());
            lsock.set_option(boost::asio::ip::udp::socket::reuse_address(true));
            lsock.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"),PORT_LSOCK));
            lsock.async_receive_from(
                    boost::asio::buffer(buf,MAX_BUF), this->lsender,
                    boost::bind(&interfaces::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
            destinations[1].up = true;
            destinations[1].dest = new boost::asio::ip::udp::endpoint(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("192.168.1.147"),PORT_DEST+1));
            destinations[0].up = true;
            destinations[0].dest = new boost::asio::ip::udp::endpoint(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("192.168.2.2"),PORT_DEST));
            this->timer = new boost::asio::deadline_timer(iot, boost::posix_time::seconds(HB_TIMEOUT));
            timer->async_wait(boost::bind(&interfaces::send_hb_timer, this, boost::asio::placeholders::error));
            boost::thread t = (boost::bind(&boost::asio::io_service::run, &iol));
            boost::thread t1 = (boost::bind(&boost::asio::io_service::run, &iot));
        }

        ~interfaces() {
            this->remove_interfaces();
        }

        odtone::sint find_best_interface() {
            odtone::sint i = 0;
            for (; i < MAX_LINKS; i++) {
                if (this->data[i].up) {
                    log_(0,__FUNCTION__, " i = ",i);
                    return i;
                }
            }
            return -1;
        }

        odtone::sint find_best_destination() {
            odtone::sint i = 0;
            for (; i < MAX_LINKS; i++) {
                boost::posix_time::time_duration diff = boost::posix_time::second_clock::local_time() - this->destinations[i].lasthb;
                log_(0, __FUNCTION__, " total seconds: ",diff.total_seconds());
                if (diff.total_seconds() > LINK_TIMEOUT) {
                    log_(0,__FUNCTION__," link timeout");
                    this->destinations[i].up = false;
                }
                if (this->destinations[i].up) {
                    log_(0,__FUNCTION__, " i = ",i);
                    return i;
                }
            }
            return -1;
        }

        void sendto(char* buf, int len) {
            odtone::sint i = this->find_best_interface();
            odtone::sint l = this->find_best_destination();
            if (i == -1 || l == -1) {
                log_(0, __FUNCTION__, " no available interfaces, discarding packet");
                return;
            }
            try {
                this->data[i].sock->send_to(boost::asio::buffer(buf,len),*destinations[l].dest);
            } catch(...) {
                this->data[i].up = false;
            }
        }

        std::string mac_to_ip(odtone::mih::mac_addr& mac) {
            odtone::mih::mac_addr eth0mac("f4:6d:04:9c:11:38");
            odtone::mih::mac_addr eth1mac("f4:6d:04:9c:0d:4d");
            if (mac == eth0mac)
                return "192.168.1.145";
            else if (mac == eth1mac)
                return "192.168.2.1";
            log_(0,__FUNCTION__, " -> 127.0.0.1");
            return "127.0.0.1";
        } 

        odtone::sint find_interface(odtone::mih::mac_addr& mac) {
            odtone::uint i = 0;
            for (; i < MAX_LINKS; i++) {
                if (this->data[i].mac != NULL && *this->data[i].mac == mac)
                    return i;
            }
            return -1;
        }

        void send_hb_timer(const boost::system::error_code& /*error*/) {
            /*if (error) {
              log_(0,__FUNCTION__," error");
              return;
              }*/
            //log_(0,__FUNCTION__, " timer scaduto, invio hb");
            send_hb();
            this->timer->expires_at(this->timer->expires_at() + boost::posix_time::seconds(HB_TIMEOUT));
            this->timer->async_wait(boost::bind(&interfaces::send_hb_timer, this,  boost::asio::placeholders::error));
        }

        uint32_t IPToUInt(const std::string ip) {
            int a, b, c, d;
            uint32_t addr = 0;

            if (sscanf(ip.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
                return 0;

            addr = a << 24;
            addr |= b << 16;
            addr |= c << 8;
            addr |= d;
            return addr;
        }

        bool IsIPInRange(const std::string ip, const std::string network, const std::string mask) {
            uint32_t ip_addr = IPToUInt(ip);
            uint32_t network_addr = IPToUInt(network);
            uint32_t mask_addr = IPToUInt(mask);

            uint32_t net_lower = (network_addr & mask_addr);
            uint32_t net_upper = (net_lower | (~mask_addr));

            if (ip_addr >= net_lower &&
                    ip_addr <= net_upper)
                return true;
            return false;
        }


        void send_hb() {
            odtone::uint i,j;
            for (i = 0; i < MAX_LINKS; i++) {
                if (!this->data[i].up)
                    continue;
                for (j = 0; j < MAX_LINKS; j++) {
                    if (destinations[j].dest != NULL) {
                        bool temp1 = IsIPInRange(this->data[i].sock->local_endpoint().address().to_string(),"192.168.0.0","255.255.0.0");
                        bool temp2 = IsIPInRange(this->destinations[j].dest->address().to_string(),"192.168.0.0.","255.255.0.0");
                        if (temp1 && temp2 && !IsIPInRange(this->data[i].sock->local_endpoint().address().to_string(),this->destinations[j].dest->address().to_string(),"255.255.255.0")) {
                            log_(0,"scarto hb verso diversa subnet locale");
                            continue;
                        }

                        //log_(0,__FUNCTION__, "sending hb to j = ", j, " ", destinations[j].dest.address(), ":",destinations[j].dest.port());
                        try {
                            this->data[i].sock->send_to(boost::asio::buffer("",0), *destinations[j].dest);
                        } catch (...) {
                            this->data[i].up = false;
                        }
                    }
                }
            }
        }

        odtone::uint add_interface(odtone::mih::mac_addr& mac, odtone::mih::link_type& type) {
            odtone::sint i = 0;
            if ((i = this->find_interface(mac)) >= 0) {
                log_(0,__FUNCTION__, i);
                this->data[i].up = true;
                return i;
            } else i = 0;
            while (this->data[i].mac != NULL && i < MAX_LINKS) i++;
            if (i >= MAX_LINKS) return -1;
            this->data[i].up = true;
            this->data[i].mac = new odtone::mih::mac_addr(mac.address());
            this->data[i].type = type;
            this->data[i].sock = new boost::asio::ip::udp::socket(this->io);
            this->data[i].sock->open(boost::asio::ip::udp::v4());
            this->data[i].sock->bind(boost::asio::ip::udp::endpoint(
                        boost::asio::ip::address::from_string(
                            this->mac_to_ip(mac).c_str()),PORT_DATA_SOCK+port_counter++));


            this->data[i].buf = new char[MAX_BUF];
            log_(0,__FUNCTION__," binding and receive on ", this->data[i].sock->local_endpoint().address(),":",this->data[i].sock->local_endpoint().port());
            this->data[i].sock->async_receive_from(
                    boost::asio::buffer(this->data[i].buf,MAX_BUF), this->sender,
                    boost::bind(&interfaces::handle_receive_pacco, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred,i));
            if (!this->sendback_active) { 
                log_(0,__FUNCTION__," activating send_back");
                boost::thread t = boost::thread (boost::bind(&boost::asio::io_service::run, &io));
                this->sendback_active = true;
            }
            return i;
        }

        void set_interface_down(odtone::mih::mac_addr& mac){
            odtone::sint i = find_interface(mac);
            if (i >= 0) {
                this->data[i].up = false;
            }
        }

        void remove_interfaces() {
            odtone::uint i = 0;
            for (; i < MAX_LINKS; i++)
                if (this->data[i].up) remove_interface(i);
        }

        void remove_interface(odtone::uint i) {
            if (data[i].mac != NULL) delete data[i].mac;
            if (data[i].sock != NULL) {
                data[i].sock->close();
                delete data[i].sock;
            }
            if (data[i].buf != NULL) delete data[i].buf;
            memset(&data[i],0,sizeof(struct info));
        }

        void remove_interface(odtone::mih::mac_addr& mac) {
            odtone::sint i = 0;
            if ((i = this->find_interface(mac)) >= 0)
                return remove_interface(i);
        }

        void print_interface(odtone::uint i) {
            if (i >= MAX_LINKS)
                log_(0,"invalid index");
            else if (this->data[i].type == odtone::mih::link_type_802_11)
                log_(0, "interface[", i, "] address: ", this->data[i].mac->address(), " type = 802.11");
            else if (this->data[i].type == odtone::mih::link_type_ethernet)
                log_(0, "interface[", i, "] address: ", this->data[i].mac->address(), " type = 802.3");
            else
                log_(0, "interface[", i, "] address: ", this->data[i].mac->address(), " type = OTHER");
        }

        void print_interfaces() {
            odtone::uint i = 0;
            for (; i < MAX_LINKS; i++)
                if (this->data[i].mac != NULL) print_interface(i);
        }
};



///////////////////////////////////////////////////////////////////////////////
/**
 * This class provides an implementation of an IEEE 802.21 MIH-User.
 */
class mih_user : boost::noncopyable {
    public:
        /**
         * Construct the MIH-User.
         *
         * @param cfg Configuration options.
         * @param io The io_service object that the MIH-User will use to
         * dispatch handlers for any asynchronous operations performed on the socket. 
         */
        mih_user(const odtone::mih::config& cfg, boost::asio::io_service& io);

        /**
         * Destruct the MIH-User.
         */
        ~mih_user();

    protected:
        /**
         * User registration handler.
         *
         * @param cfg Configuration options.
         * @param ec Error Code.
         */
        void user_reg_handler(const odtone::mih::config& cfg, const boost::system::error_code& ec);

        /**
         * Default MIH event handler.
         *
         * @param msg Received message.
         * @param ec Error code.
         */
        void event_handler(odtone::mih::message& msg, const boost::system::error_code& ec);

        /**
         * Capability Discover handler.
         *
         * @param msg Received message.
         * @param ec Error Code.
         */
        void capability_discover_confirm(odtone::mih::message& msg, const boost::system::error_code& ec);

        /**
         * Event subscribe handler.
         *
         * @param msg Received message.
         * @param ec Error Code.
         */
        void event_subscribe_response(odtone::mih::message& msg, const boost::system::error_code& ec);
        void send_link_get_parameters_request();
        void link_get_parameters_request_handler(odtone::mih::message& msg, const boost::system::error_code& ec);

    private:
        odtone::sap::user _mihf;	/**< User SAP helper.		*/
        odtone::mih::id   _mihfid;	/**< MIHF destination ID.	*/
        interfaces links;
};

/**
 * Construct the MIH-User.
 *
 * @param cfg Configuration options.
 * @param io The io_service object that the MIH-User will use to
 * dispatch handlers for any asynchronous operations performed on the socket. 
 */
    mih_user::mih_user(const odtone::mih::config& cfg, boost::asio::io_service& io)
: _mihf(cfg, io, boost::bind(&mih_user::event_handler, this, _1, _2))
{
    odtone::mih::message m;
    boost::optional<odtone::mih::mih_cmd_list> supp_cmd = parse_supported_commands(cfg);

    m << odtone::mih::indication(odtone::mih::indication::user_register)
        & odtone::mih::tlv_command_list(supp_cmd);
    m.destination(odtone::mih::id("local-mihf"));

    _mihf.async_send(m, boost::bind(&mih_user::user_reg_handler, this, boost::cref(cfg), _2));
}


/**
 * Destruct the MIH-User.
 */
mih_user::~mih_user()
{
}

/**
 * User registration handler.
 *
 * @param cfg Configuration options.
 * @param ec Error Code.
 */
void mih_user::user_reg_handler(const odtone::mih::config& cfg, const boost::system::error_code& ec)
{
    log_(0, "MIH-User register result: ", ec.message());

    odtone::mih::message msg;

    odtone::mih::octet_string destination = cfg.get<odtone::mih::octet_string>(odtone::sap::kConf_MIH_SAP_dest);
    _mihfid.assign(destination.c_str());

    //
    // Let's fire a capability discover request to get things moving
    //
    msg << odtone::mih::request(odtone::mih::request::capability_discover, _mihfid);

    _mihf.async_send(msg, boost::bind(&mih_user::capability_discover_confirm, this, _1, _2));

    log_(0, "MIH-User has sent a Capability_Discover.request towards its local MIHF");
}

/**
 *   * Create and send a MIH_Link_Get_Parameters.request message.
 *     *
 *       * @param sap The SAP helper.
 *         * @param dst The destination MIH ID.
 *           */
void mih_user::send_link_get_parameters_request()
{
    odtone::mih::message p;

    odtone::mih::link_tuple_id  li,li1;
    odtone::mih::link_id_list   lil;
    odtone::mih::mac_addr       mac,mac1;

    log_(0, "sending link_get_parameters_request");
    mac.address("00:26:b6:4e:af:6c");
    mac1.address("00:13:77:bf:f0:ef");
    li.type = odtone::mih::link_type_802_11;
    li.addr = mac;
    lil.push_back(li);

    odtone::mih::link_status_req    lsr;
    //odtone::mih::link_param_802_11  lp = odtone::mih::link_param_802_11_rssi;
    lsr._states_req.set(odtone::mih::link_states_req_op_mode);
    //lsr._param_type_list.push_back(lp);
    // lsr._desc_req.set(odtone::mih::link_desc_req_classes_of_service_supported);

    odtone::mih::link_param_gen lpgs = odtone::mih::link_param_gen_signal_strength;
    lsr._param_type_list.push_back(lpgs);

    odtone::mih::link_param_gen lpgd = odtone::mih::link_param_gen_data_rate;
    lsr._param_type_list.push_back(lpgd);

    odtone::mih::link_param_gen lpgp = odtone::mih::link_param_gen_packet_error_rate;
    lsr._param_type_list.push_back(lpgp);

    p << odtone::mih::request(odtone::mih::request::link_get_parameters, _mihfid)
        & odtone::mih::tlv_link_id_list(lil)
        & odtone::mih::tlv_get_status_req_set(lsr);

    _mihf.async_send(p, boost::bind(&mih_user::link_get_parameters_request_handler, this, _1, _2));

}

void mih_user::link_get_parameters_request_handler(odtone::mih::message& msg, const boost::system::error_code& ec) {
    log_(0, __FUNCTION__);
    if (ec) {
        log_(0, __FUNCTION__, " error : ", ec.message());
        return;
    }
    odtone::mih::status st;
    odtone::mih::status_rsp_list srl;

    odtone::uint iter,iter1;

    log_(0, "receive_MIH_Link_Get_Parameters_confirm - Begin");

    msg >> odtone::mih::confirm()
        & odtone::mih::tlv_status(st)
        & odtone::mih::tlv_get_status_rsp_list(srl);

    odtone::mih::link_addr addr;

    //log_(0, "\t- STATUS RSP LIST - Length: ", srl.size());

    /*for (odtone::mih::status_rsp_list::iterator i = srl->begin(); i != srl->end(); ++i) {
      log_(0, "inizio");
      log_(0, *i);
      log_(0, "fine");
      }*/

    for(iter=0; iter<srl.size(); iter++)
    {
        log_(0, "LINK ID ", iter, ":");
        log_(0, "LINK STATUS RSP List - Element: ", iter);

        odtone::uint iter1;

        log_(0, "States Rsp List - Length: ", srl[iter].rsp.states_rsp_list.size());
        //States Rsp List (vector)
        for(iter1=0; iter1<srl[iter].rsp.states_rsp_list.size(); iter1++)
        {
            //each element op_mode or channel_id
            log_(0, "Element:", iter1, " - Value:", srl[iter].rsp.states_rsp_list[iter1]);
        }

        log_(0, "Param List - Length: ", srl[iter].rsp.param_list.size());
        //Param List
        for(iter1=0; iter1<srl[iter].rsp.param_list.size(); iter1++)
        {
            odtone::mih::link_param_val val = boost::get<odtone::mih::link_param_val>(srl[iter].rsp.param_list[iter1].value);
            if (boost::get<odtone::mih::link_param_gen>(srl[iter].rsp.param_list[iter1].type) == odtone::mih::link_param_gen_signal_strength)
                log_(0,"RSSI: ",std::to_string((odtone::sint8)val));
            else log_(0, "Element:", iter, ", Type: ", srl[iter].rsp.param_list[iter1].type ,", Value: ", val = boost::get<odtone::mih::link_param_val>(srl[iter].rsp.param_list[iter1].value));
            break;
            // 1 - link_param_type
            // 2 - boost::variant<link_param_val, qos_param_val>
        }

        log_(0, "Desc_rsp_list - Length: ", srl[iter].rsp.desc_rsp_list.size());
        //Desc_rsp_list
        for(iter1=0; iter1<srl[iter].rsp.desc_rsp_list.size(); iter1++)
        {
            //log_(0, "Element:", iter+1, "Value", srl[iter1].rsp.desc_rsp_list[iter1]);
            //boost::variant<num_cos, num_queue>
            //uint8 num_cos - uint8 num_queue
        }
    }

    /*for(iter=0; iter<srl.size(); iter++)
      {
      log_(0, "\t  LINK ID ", iter+1, ":");
      addr =  srl[iter].id.addr;
      odtone::mih::link_param_list parmslist = srl[iter].rsp.param_list;
      for (iter1 = 0; iter1 < parmslist.size(); iter1++) {
//odtone::mih::link_param_val rssi = boost::get<odtone::mih::link_param_val>(parmslist[iter].value);
if (iter1 == 0) {
odtone::sint8 rssi = boost::get<odtone::mih::link_param_val>(parmslist[iter1].value);
log_(0,parmslist[iter1].type," : ",std::to_string(rssi));
} else {
odtone::uint data = boost::get<odtone::mih::link_param_val>(parmslist[iter1].value);
log_(0, parmslist[iter1].type," : ",std::to_string(data));
}
}
log_(0, "\t  LINK STATUS RSP List - Element: ", addr);

}*/
}
/**
 * Default MIH event handler.
 *
 * @param msg Received message.
 * @param ec Error code.
 */
void mih_user::event_handler(odtone::mih::message& msg, const boost::system::error_code& ec)
{
    if (ec) {
        log_(0, __FUNCTION__, " error : ", ec.message());
        send_link_get_parameters_request();
        return;
    }
    odtone::mih::link_tuple_id li;
    msg >> odtone::mih::indication()
        & odtone::mih::tlv_link_identifier(li);
    odtone::mih::mac_addr mac = boost::get<odtone::mih::mac_addr>(li.addr);
    odtone::mih::link_type type = boost::get<odtone::mih::link_type>(li.type);
    switch (msg.mid()) {
        case odtone::mih::indication::link_up:
            log_(0, "MIH-User has received a local event \"link_up\"");
            this->links.add_interface(mac,type);
            this->links.send_hb();
            break;

        case odtone::mih::indication::link_down:
            log_(0, "MIH-User has received a local event \"link_down\"");
            //this->links.remove_interface(mac);
            this->links.set_interface_down(mac);
            this->links.send_hb();
            break;

        case odtone::mih::indication::link_detected:
            log_(0, "MIH-User has received a local event \"link_detected\"");
            break;

        case odtone::mih::indication::link_going_down:
            log_(0, "MIH-User has received a local event \"link_going_down\"");
            break;

        case odtone::mih::indication::link_handover_imminent:
            log_(0, "MIH-User has received a local event \"link_handover_imminent\"");
            break;

        case odtone::mih::indication::link_handover_complete:
            log_(0, "MIH-User has received a local event \"link_handover_complete\"");
            break;

        default:
            log_(0, "MIH-User has received a local event \"unknown\"");
            break;
    }
    this->links.print_interfaces();
}

/**
 * Capability Discover handler.
 *
 * @param msg Received message.
 * @param ec Error Code.
 */
void mih_user::capability_discover_confirm(odtone::mih::message& msg, const boost::system::error_code& ec)
{
    if (ec) {
        log_(0, __FUNCTION__, " error: ", ec.message());
        return;
    }

    odtone::mih::status st;
    boost::optional<odtone::mih::net_type_addr_list> ntal;
    boost::optional<odtone::mih::mih_evt_list> evt;

    msg >> odtone::mih::confirm()
        & odtone::mih::tlv_status(st)
        & odtone::mih::tlv_net_type_addr_list(ntal)
        & odtone::mih::tlv_event_list(evt);

    log_(0, "MIH-User has received a Capability_Discover.response with status ",
            st.get(), " and the following capabilities:");

    /*odtone::uint i = 0;
      odtone::uint size = ntal.size();
      for (;i < size; i++) {
      odtone::mih::mac_addr mac = boost::get<odtone::mih::mac_addr>(ntal[i].addr);
      odonte::mih::network_type type = ntal[i].nettype;
      switch type:
      case odtone::mih::link_type_802_11:
      log_(0, "detected wifi w/ addr = ", mac);
      break;
      case odtone::mih::link_type_ethernet:
      log_(0, "detected ethernet w/ addr = ", mac);
      break;
      }*/




    //log_(0, "prima for");
    for (odtone::mih::net_type_addr_list::iterator i = ntal->begin(); i != ntal->end(); ++i) {
        /*log_(0, "inizio");
          log_(0, *i);
          log_(0, "fine");*/
        //odtone::mih::link_tuple_id li;

        if (i->nettype.link.which() == 1)
        {
            odtone::mih::mac_addr mac = boost::get<odtone::mih::mac_addr>(i->addr);
            odtone::mih::link_type type = boost::get<odtone::mih::link_type>(i->nettype.link);
            this->links.add_interface(mac,type);
        }
    }
    this->links.print_interfaces();
    //
    // event subscription
    //
    // For every interface the MIHF sent in the
    // Capability_Discover.response send an Event_Subscribe.request
    // for all availabe events
    //
    if (ntal && evt) {
        for (odtone::mih::net_type_addr_list::iterator i = ntal->begin(); i != ntal->end(); ++i) {
            odtone::mih::message req;
            odtone::mih::link_tuple_id li;

            if (i->nettype.link.which() == 1)
            {
                li.addr = i->addr;
                li.type = boost::get<odtone::mih::link_type>(i->nettype.link);

                req << odtone::mih::request(odtone::mih::request::event_subscribe, _mihfid)
                    & odtone::mih::tlv_link_identifier(li)
                    & odtone::mih::tlv_event_list(evt);
                req.destination(msg.source());

                _mihf.async_send(req, boost::bind(&mih_user::event_subscribe_response, this, _1, _2));

                log_(0, "MIH-User has sent Event_Subscribe.request to ", req.destination().to_string());
            }
        }
    }
}

/**
 * Event subscribe handler.
 *
 * @param msg Received message.
 * @param ec Error Code.
 */
void mih_user::event_subscribe_response(odtone::mih::message& msg, const boost::system::error_code& ec)
{
    log_(0, __FUNCTION__, "(", msg.tid(), ")");

    if (ec) {
        log_(0, __FUNCTION__, " error: ", ec.message());
        return;
    }

    odtone::mih::status st;

    msg >> odtone::mih::response()
        & odtone::mih::tlv_status(st);

    log_(0, "status: ", st.get());
}

int main(int argc, char** argv)
{
    odtone::setup_crash_handler();

    try {
        boost::asio::io_service ios;

        // declare MIH Usr available options
        po::options_description desc(odtone::mih::octet_string("MIH Usr Configuration"));
        desc.add_options()
            ("help", "Display configuration options")
            (odtone::sap::kConf_File, po::value<std::string>()->default_value("mih_usr.conf"), "Configuration file")
            (odtone::sap::kConf_Receive_Buffer_Len, po::value<uint>()->default_value(4096), "Receive buffer length")
            (odtone::sap::kConf_Port, po::value<ushort>()->default_value(MIH_LISTEN), "Listening port")
            (odtone::sap::kConf_MIH_SAP_id, po::value<std::string>()->default_value("user"), "MIH-User ID")
            (kConf_MIH_Commands, po::value<std::string>()->default_value(""), "MIH-User supported commands")
            (odtone::sap::kConf_MIHF_Ip, po::value<std::string>()->default_value("127.0.0.1"), "Local MIHF IP address")			
            (odtone::sap::kConf_MIHF_Local_Port, po::value<ushort>()->default_value(MIH_SEND), "Local MIHF communication port")
            (odtone::sap::kConf_MIH_SAP_dest, po::value<std::string>()->default_value(""), "MIHF destination");

        odtone::mih::config cfg(desc);
        cfg.parse(argc, argv, odtone::sap::kConf_File);

        if (cfg.help()) {
            std::cerr << desc << std::endl;
            return EXIT_SUCCESS;
        }

        mih_user usr(cfg, ios);

        ios.run();

    } catch(std::exception& e) {
        log_(0, "exception: ", e.what());
    }
}

// EOF ////////////////////////////////////////////////////////////////////////

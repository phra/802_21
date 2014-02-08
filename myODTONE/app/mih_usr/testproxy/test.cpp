#include <iostream>
#include <boost/asio.hpp>

int main( int argC, char *argV[] ) {
    boost::asio::io_service ioService;
    boost::asio::ip::udp::socket socket(ioService);

    boost::asio::ip::udp::endpoint localEndpoint(
        boost::asio::ip::address::from_string("192.168.2.1"), 50000);
    boost::asio::ip::udp::endpoint remoteEndpoint(boost::asio::ip::address::from_string("127.0.0.1"),9999);
    /*boost::asio::ip::tcp::resolver resolver(ioService);
    boost::asio::ip::tcp::resolver::iterator remoteEndpoint = 
        resolver.resolve(boost::asio::ip::tcp::resolver::query("haatschii.de", "80"));
*/
    socket.open(boost::asio::ip::udp::v4());

    std::cout   << "Before binding socket has local endpoint: " 
                << socket.local_endpoint().address().to_string() 
                << ":" << socket.local_endpoint().port() << std::endl;

    socket.bind(localEndpoint);

    std::cout   << "Before sending socket has local endpoint: " 
                << socket.local_endpoint().address().to_string() 
                << ":" << socket.local_endpoint().port() << std::endl;

    /*boost::asio::connect(socket, remoteEndpoint);

    std::cout   << "After connecting socket has local endpoint: " 
                << socket.local_endpoint().address().to_string() 
                << ":" << socket.local_endpoint().port() << std::endl;
*/
    //Test request to a page that echos our IP address.
    socket.send_to(
        boost::asio::buffer("GET /ip.php HTTP/1.1\r\nHost: haatschii.de\r\nAccept: */*\r\n\r\n", 57), remoteEndpoint);
    std::cout   << "After sending socket has local endpoint: " 
                << socket.local_endpoint().address().to_string() 
                << ":" << socket.local_endpoint().port() << std::endl;


    //Parse server response (not important for this code example)
    return 0;
}

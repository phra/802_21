/** async_send_to udp example 
 * compile with
 * g++ -o udpserver2 udpserver2.cpp -lboost_system -lboost_signals -lboost_thread
 */

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

boost::asio::io_service io_service;

void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    std::cout << "async_send_to return " << error << ": " << bytes_transferred << " transmitted" << std::endl;
    // stop must be executed only inside a callback
    io_service.stop();
}

/* ./udpserver <host> <port> */
int main(int argc, char *argv[])
{
    if(argc==3)
    {
        boost::asio::ip::udp::socket socket(io_service);
        boost::asio::ip::udp::endpoint remote_endpoint;
        
        boost::asio::ip::udp::endpoint localEndpoint(boost::asio::ip::address::from_string("192.168.2.1"), 50000);
        boost::asio::ip::udp::endpoint remoteEndpoint(boost::asio::ip::address::from_string("127.0.0.1"),9999);
        // create the phisical socket
        socket.open(boost::asio::ip::udp::v4());
        socket.bind(localEndpoint);
        if(false)
        {
            boost::asio::socket_base::broadcast option(true);
            socket.set_option(option);
        }

        // create a ip:port destination address
        remote_endpoint = boost::asio::ip::udp::endpoint(
                boost::asio::ip::address::from_string(argv[1]),  boost::lexical_cast<int>(argv[2]));

        std::cout << "Send to " << remote_endpoint << " from " << localEndpoint << std::endl;

        // send async data
        socket.async_send_to(boost::asio::buffer("message", 7), remote_endpoint, 
                boost::bind(&handle_send, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

        // io_service.run();
        boost::thread t = boost::thread (boost::bind(&boost::asio::io_service::run, &io_service));

        // wait thread terminates:
        t.join();
    }
    return 0;
}

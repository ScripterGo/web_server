#include <iostream>
#include<string>
#include<boost/asio.hpp>
#include<boost/array.hpp>

using boost::asio::ip::tcp;

int main(){
    std::cout << "Server initiating..." << std::endl;
    try{
        boost::asio::io_context context;
        boost::asio::ip::tcp::acceptor acc(context, tcp::endpoint (tcp::v4(), 1337));

        while(true){
            boost::asio::ip::tcp::socket client_socket(context);
            acc.accept(client_socket);

            std::cout << "Established connection to a client" << std::endl;

            boost::system::error_code dispatch_error;


            boost::array<char, 2056> client_request_buf;
            size_t byte_cnt = client_socket.read_some(boost::asio::buffer(client_request_buf));
            std::cout << std::string(client_request_buf.data(), byte_cnt) << std::endl;

            std::string message_to_client = "HTTP/1.1 200 \r\n";
            client_socket.write_some(boost::asio::buffer(message_to_client), dispatch_error);

            if(dispatch_error.failed()){
                std::cout << "--Dispatch failed--" << std::endl;
                std::cout << dispatch_error.what() << std::endl;
            }
            //break;
        }
    }catch (std::exception& e){
        std::cout << "-----ERROR-----" << std::endl;
        std::cout << e.what() << std::endl;
    }
    return 0;
}
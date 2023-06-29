//
// Created by Anton magnusson on 2023-06-22.
//

#include<iostream>
#include<boost/asio.hpp>
#include<boost/array.hpp>
#include<string>

using boost::asio::ip::tcp;

int main(){
    try{
        boost::asio::io_context context;
        tcp::resolver resolver(context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "1337");

        tcp::socket server_socket(context);

        try{
            boost::asio::connect(server_socket, endpoints);
        }catch(boost::system::error_code e){
            std::cout << "Could not connect" << std::endl;
            std::cout << e.what() << std::endl;
            return 1;
        }
        while(true){
            boost::array<char, 128> buf;
            boost::system::error_code read_error;
            size_t byte_cnt = server_socket.read_some(boost::asio::buffer(buf), read_error);

            if(read_error == boost::asio::error::eof){
                std::cout << "Connection was closed by server!" << std::endl;
            }else if(read_error){
                std::cout << "Could not read from socket!" << std::endl;
                std::cout << read_error.what() << std::endl;
                throw boost::system::system_error(read_error);
            }

            std::string recieved_message(buf.data(), byte_cnt);
            std::cout << recieved_message << std::endl;
            break;
        }
    }catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }


    return 0;
}


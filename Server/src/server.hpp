//
// Created by SidHCH on 2020/6/5.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_SERVER_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_SERVER_HPP

#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <task.hpp>
#include <request.hpp>
#include <file_manager.hpp>


class server {
private:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::acceptor acceptor;

    void read(boost::asio::ip::tcp::socket &socket) {
        char buffer[1024];
        boost::system::error_code erc;
        socket.read_some(boost::asio::buffer(buffer), erc);
        std::unique_ptr<request> req{nullptr};
        try {
            req = std::make_unique<request>(buffer);
        } catch (const invalid_data_stream &e) {
            printf("接收包时出现异常: %s", e.what());
        }
        printf("receive pack size: %d\n", req->size);
    }

public:
    server() : acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 11209)) {

    }


    [[noreturn]] void listen() {

        for (;;) {
            boost::asio::ip::tcp::socket socket(io_service);
            acceptor.accept(socket);
            read(socket);
        }
    }
};


#endif //NAIVETASKDISTRIBUTIONSYSTEM_SERVER_HPP

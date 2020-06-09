//
// Created by SidHCH on 2020/6/5.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_CLIENT_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_CLIENT_HPP

#include <boost/asio.hpp>
#include <request.hpp>
#include <memory>

using boost::asio::ip::tcp;

/* 套接客户端 直接使用tcp连接 */
class client {
private:
    boost::asio::io_service service;
    tcp::endpoint ep;
public:
    /*
     * 根据远程的ip地址以及端口进行初始化
     * 用例：
     * client* c = new client("127.0.0.1", 11209);
     * */
    client(const std::string &ip, u_int16_t port) : ep{boost::asio::ip::address::from_string(ip), port} {
    }


    /*
     * 发送数据包，格式
     * 获取所有的数据信息
     * */
    void send(const std::shared_ptr<request> &req) {
        tcp::socket skt(service);
        skt.connect(ep);
        char data_with_size[req->size];
        req->serialize(data_with_size);
        skt.send(boost::asio::buffer(data_with_size, req->size));
        printf("发送大小%dB的数据\n", req->size);
    }
};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_CLIENT_HPP

//
// Created by SidHCH on 2020/6/5.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_CLIENT_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_CLIENT_HPP

#include <boost/asio.hpp>

/* 套接客户端 直接使用tcp连接 */
class client {
private:
    boost::asio::io_context service;
    boost::asio::ip::tcp::endpoint ep;
    boost::asio::ip::tcp::socket socket;
public:
    /*
     * 根据远程的ip地址以及端口进行初始化
     * 用例：
     * client* c = new client("127.0.0.1", 11209);
     * */
    client(const std::string &ip, u_int16_t port) : ep{boost::asio::ip::address::from_string(ip), port},
                                                    socket{service} {
    }

    /* 创建连接 */
    void connect() {
        socket.connect(ep);
    }

    /*
     * 发送数据包，
     * 通过\n作为分隔符
     * 获取所有的数据信息
     * */
    void send(char *data, size_t size) {
        if (data[size - 1] != '\n') {
            char data_[size + 1];
            memcpy(data_, data, size);
            data_[size] = '\n';
            socket.send(boost::asio::buffer(data_, size));
        } else {
            socket.send(boost::asio::buffer(data, size));
        }
    }

};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_CLIENT_HPP

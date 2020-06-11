//
// Created by SidHCH on 2020/6/5.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_CLIENT_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_CLIENT_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
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
     * 返回接收到的参数
     * */
    std::unique_ptr<request> send(const std::shared_ptr<request> &req) {
        tcp::socket skt(service);
        skt.connect(ep);
        char data_with_size[req->size];
        req->serialize(data_with_size);
        skt.send(boost::asio::buffer(data_with_size, req->size));
        printf("发送大小%dB的数据\n", req->size);
        return read(skt);
    }

    std::unique_ptr<request> read(tcp::socket &socket) {
        boost::system::error_code erc;
        char buffer[1024 * 1024 * 3];
        // 读取四字节的魔法值验证
        boost::asio::read(socket, boost::asio::buffer(buffer), boost::asio::transfer_exactly(4), erc);
        if (!request::validate_header(buffer)) {
            printf("头部魔法值验证错误");
            socket.read_some(boost::asio::buffer(buffer), erc);
            return nullptr;
        }
        uint32_t size;
        boost::asio::read(socket, boost::asio::buffer(buffer), boost::asio::transfer_exactly(4), erc);
        memcpy(&size, buffer, 4);
        boost::asio::read(socket, boost::asio::buffer(buffer), boost::asio::transfer_exactly(size - 8), erc);
        std::unique_ptr<request> req{std::make_unique<request>()};
        try {
            req->deserialize(buffer, size, 0);
        } catch (const invalid_data_stream &e) {
            printf("接收包时出现异常: %s\n", e.what());
            return nullptr;
        }
        printf("receive pack size: %d\n", req->size);
        return req;
    }
};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_CLIENT_HPP

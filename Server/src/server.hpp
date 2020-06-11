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
#include <task_manager.hpp>


class server {
private:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::acceptor acceptor;

    void merge_data() {
        Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('./')");
        PyObject * module, *func, *ret, *list;
        list = PyList_New(0);
        PyObject * args = PyTuple_New(1);
        for (auto &tsk:task_manager::get_instance()->list_tasks()) {
            for (auto &f:tsk.second->files) {
                PyList_Append(list, Py_BuildValue("s", ("_" + std::to_string(f) + "_.sav").c_str()));
            }
        }
        module = PyImport_ImportModule("merge_all");
        func = PyObject_GetAttrString(module, "merge_all");
        PyTuple_SetItem(args, 0, list);
        ret = PyEval_CallObject(func, args);
        int code = -2;
        PyArg_Parse(ret, "i", &code);
        if (!code) {
            printf("合并执行成功!\n");
            exit(0);
        } else {
            printf("文件合并执行失败!\n");
        }
        Py_Finalize();
    }

    void read(boost::asio::ip::tcp::socket &socket) {
        char buffer[1024 * 1024 * 3];
        boost::system::error_code erc;
        // 读取四字节的魔法值验证
        boost::asio::read(socket, boost::asio::buffer(buffer), boost::asio::transfer_exactly(4), erc);
        if (!request::validate_header(buffer)) {
            printf("头部魔法值验证错误");
            socket.read_some(boost::asio::buffer(buffer), erc);
            return;
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
            return;
        }
        if (req->sr->type == data_type::COMMAND) {
            execute(std::dynamic_pointer_cast<command>(req->sr), socket);
        } else {
            send(socket, std::make_shared<command>(1));
        }
        if (task_manager::get_instance()->update_done()) {
            printf("merging...");
            merge_data();
        }
        printf("receive pack size: %d\n", req->size);
    }

    void send(boost::asio::ip::tcp::socket &socket, const std::shared_ptr<serializable> &ser) {
        request req(ser);
        char buf[req.size];
        req.serialize(buf);
        socket.send(boost::asio::buffer(buf, req.size));
    }

    /* 执行command */
    void execute(const std::shared_ptr<command> &cmd, boost::asio::ip::tcp::socket &socket) {
        switch (cmd->code) {
            case FETCH_FILE:
                if (!cmd->operand) {
                    send(socket, std::make_shared<command>(MSG_ERROR));
                    printf("operand缺失");
                } else {
                    send(socket, file_manager::get_instance()->get_file(cmd->operand));
                    printf("回传file...");
                }
                break;
            case FETCH_TASK:
                auto tsk = task_manager::get_instance()->get_task();
                if (tsk) {
                    send(socket, task_manager::get_instance()->get_task());
                    printf("回传id=%d任务", tsk->get_id());
                    task_manager::get_instance()->set_fetched(tsk->get_id());
                } else {
                    send(socket, std::make_shared<command>(NO_MORE_TASK));
                    printf("无任务");
                }
                break;
        }
        printf("\n");
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

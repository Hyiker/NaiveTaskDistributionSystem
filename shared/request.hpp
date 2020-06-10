//
// Created by SidHCH on 2020/6/6.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_REQUEST_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_REQUEST_HPP
#define HEADER_MAGIC 11209

#include <serializable.hpp>
#include <file.hpp>
#include <memory>
#include <invalid_data_stream.hpp>
#include <file_manager.hpp>
#include <task_manager.hpp>
#include <command.hpp>

struct request {
    std::shared_ptr<serializable> sr;
    uint32_t size;

    explicit request(const std::shared_ptr<serializable> &sr) : sr(sr), size(sr->struct_size + 8) {

    }

    /* 传入带header的数据流 */
    explicit request(const char *src, int type = 0) {
        uint32_t tmp;
        memcpy(&tmp, src, 4);
        /* 验证 */
        if (tmp != HEADER_MAGIC) {
            throw invalid_data_stream("头部魔法值验证错误");
        }
        memcpy(&size, src + 4, 4);
        memcpy(&tmp, src + 8, 4);
        switch (tmp) {
            case static_cast<uint32_t>(data_type::TASK):
                sr = std::shared_ptr<serializable>(new task(src + 8, size - 8));
                printf("接收到task,其description字段为%s\n", std::dynamic_pointer_cast<task>(sr)->description.c_str());
                task_manager::get_instance()->add_task(std::dynamic_pointer_cast<task>(sr));
                break;
            case static_cast<uint32_t>(data_type::FILE):
                sr = std::shared_ptr<serializable>(new file(src + 8, size - 8));
                printf("接收到file,其大小为%lluB\n", std::dynamic_pointer_cast<file>(sr)->size);
                file_manager::get_instance()->add_file(std::dynamic_pointer_cast<file>(sr));
                std::dynamic_pointer_cast<file>(sr)->save(type);
                break;
            case static_cast<uint32_t>(data_type::COMMAND):
                sr = std::shared_ptr<serializable>(new command(src + 8));
                printf("接收到一条命令\n");
                break;
            default:
                throw invalid_data_stream("未识别的序列化类型");
        }
    }

    /* 序列化: 4字节header 4字节size size大小的结构体 */
    void serialize(char *const dest) {
        uint32_t header{HEADER_MAGIC};
        memcpy(dest, &header, 4);
        memcpy(dest + 4, &size, 4);
        sr->serialize(dest + 8);
    }
};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_REQUEST_HPP

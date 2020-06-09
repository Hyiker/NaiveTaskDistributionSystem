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

struct request {
    std::shared_ptr<serializable> sr;
    uint32_t size;

    explicit request(const std::shared_ptr<serializable> &sr) : sr(sr), size(sr->struct_size + 8) {

    }

    /* 传入带header的数据流 */
    explicit request(const char *src) {
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
                printf("接收到task,其description字段为%s\n", reinterpret_cast<task *>(sr.get())->description.c_str());
                break;
            case static_cast<uint32_t>(data_type::FILE):
                sr = std::shared_ptr<serializable>(new file(src + 8, size - 8));
                printf("接收到file,其大小为%lluB\n", reinterpret_cast<file *>(sr.get())->size);
                reinterpret_cast<file *>(sr.get())->save();
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

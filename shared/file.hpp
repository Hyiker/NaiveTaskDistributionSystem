//
// Created by SidHCH on 2020/6/4.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_FILE_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_FILE_HPP

#include <cstdint>
#include <serializable.hpp>

/* 最大允许10KB的文件大小 */
const int MAX_FILE_SIZE = 1024 * 10;

/* 任务对象 */
struct file : public serializable {
    /* 前4字节为id */
    int id;
    /* 文件的大小 */
    uint64_t size;
    /* 文件的签名，SHA1 */
    char SHA1[40]{};
    /* 后可变长的大小，具体文件的字节流 */
    char data[MAX_FILE_SIZE];

    /* struct_size组成：type+id+size+SHA1+data */
    file(int id, uint64_t size, const char *SHA1, char *data) : serializable(4 + 4 + 4 + 40 + size, data_type::FILE),
                                                                id(id),
                                                                size(size){
        memcpy(this->SHA1, SHA1, 40);
    }

    file(const char *src, uint32_t struct_size) : serializable(struct_size, data_type::FILE) {
        memcpy(&id, src + 4, 4);
        memcpy(&size, src + 8, 8);
        memcpy(SHA1, src + 16, 40);
        memcpy(data, src + 56, size);
    }

    /* 序列化文件为字节流 */
    void serialize(char *const dest) override {
        int tp = static_cast<int>(type);
        memcpy(dest, &tp, 4);
        memcpy(dest + 4, &id, 4);
        memcpy(dest + 8, &size, 8);
        memcpy(dest + 16, SHA1, 40);
        memcpy(dest + 56, data, size);
    }
};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_FILE_HPP

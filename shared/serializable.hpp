//
// Created by SidHCH on 2020/6/6.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_SERIALIZABLE_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_SERIALIZABLE_HPP

#include <cstring>
#include <cstdint>

/* 可序列化类 */
enum class data_type {
    FILE = 0, TASK = 1
};

struct serializable {
    uint32_t struct_size;
    data_type type;

    /* 序列化方法，格式要求：data_type+正文 */
    virtual void serialize(char *const dest) = 0;

protected:
    serializable(uint64_t struct_size, data_type type) : struct_size(struct_size), type(type) {
    }
};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_SERIALIZABLE_HPP

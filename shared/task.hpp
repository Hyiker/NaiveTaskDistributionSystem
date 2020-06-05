//
// Created by SidHCH on 2020/6/4.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_TASK_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_TASK_HPP

#include <string>
#include <cstring>
#include <iostream>

/*
 * task结构体
 * 负责实现task信息的保存与序列化、反序列化
 */
struct task {
    /* 任务文件的id，占四个字节 */
    uint32_t file_id;
    /* 任务文件的大小，单位为字节，占八个字节 */
    uint64_t file_size;
    /* 文件的签名，进行安全性核验，占八个字节 */
    std::string signature;
    /* 发送时间戳，占八个字节 */
    uint64_t timestamp;
    /* 总大小为28B */
    const int BYTE_SIZE = sizeof(char);

    task(uint32_t file_id, uint64_t file_size, std::string signature, uint64_t timestamp) : file_id{file_id},
                                                                                            file_size{file_size},
                                                                                            signature{std::move(
                                                                                                    signature)},
                                                                                            timestamp{timestamp} {

    }

    /* 反序列化data字节流 */
    explicit task(char *data) {
        memcpy(&file_id, data, 4 * BYTE_SIZE);
        memcpy(&file_size, data + 4, 8 * BYTE_SIZE);
        char tmp[16];
        memcpy(tmp, data + 12, 8 * BYTE_SIZE);
        signature = tmp;
        memcpy(&timestamp, data + 20, 8 * BYTE_SIZE);
    }

    /* 序列化为data字节流 */
    void serialize(char *const dest) {
        memcpy(dest, &file_id, 4 * BYTE_SIZE);
        memcpy(dest + 4, &file_size, 8 * BYTE_SIZE);
        memcpy(dest + 12, signature.c_str(), 8 * BYTE_SIZE);
        memcpy(dest + 20, &timestamp, 8 * BYTE_SIZE);
    }

    friend std::ostream &operator<<(std::ostream &os, task &tsk) {
        os << "file_id: " << tsk.file_id << '\n'
           << "file_size: " << tsk.file_size << '\n'
           << "signature: " << tsk.signature << '\n'
           << "timestamp: " << tsk.timestamp;
        return os;
    }
};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_TASK_HPP


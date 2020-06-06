//
// Created by SidHCH on 2020/6/4.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_TASK_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_TASK_HPP

#include <string>
#include <utility>
#include <vector>
#include <serializable.hpp>

/*
 * 负责实现任务信息的保存与序列化、反序列化
 */
struct task : public serializable {
    /* 任务的id */
    uint32_t id;
    /* 任务文件的数量 */
    uint32_t file_number;
    /* 任务所需要的文件，因为可能有多个所以使用vector保存 */
    std::vector<uint32_t> files;
    /* 对于任务的描述长度 */
    uint32_t description_size;
    /* 对于任务的描述 */
    std::string description;

    /* struct_size组成: type+id+file_number+files+description_size+description */
    task(int id, int file_number, std::vector<uint32_t> files, int description_size, std::string &description)
            : id(id), file_number(file_number), files(std::move(files)), description_size(description_size),
              description(description),
              serializable(4 + 4 + 4 + file_number * 4 + 4 + description_size, data_type::TASK) {

    }

    task(const char *src, uint32_t size) : serializable(size, data_type::TASK) {
        memcpy(&id, src + 4, 4);
        memcpy(&file_number, src + 8, 4);
        for (int i = 0; i < file_number; ++i) {
            uint32_t fn;
            memcpy(&fn, src + 12 + i * 4, 4);
            files.push_back(fn);
        }
        memcpy(&description_size, src + 12 + file_number * 4, 4);
        char buffer[256];
        memcpy(buffer, src + 16 + file_number * 4, description_size);
        buffer[description_size] = '\0';
        description = buffer;
    }


    void serialize(char *const dest) override {
        int tp = static_cast<int>(type);
        memcpy(dest, &tp, 4);
        memcpy(dest + 4, &id, 4);
        memcpy(dest + 8, &file_number, 4);
        for (int i = 0; i < file_number; ++i) {
            memcpy(dest + 12 + i * 4, &files[i], 4);
        }
        memcpy(dest + 12 + file_number * 4, &description_size, 4);
        memcpy(dest + 16 + file_number * 4, description.c_str(), description_size);
    }
};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_TASK_HPP


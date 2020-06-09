//
// Created by SidHCH on 2020/6/4.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_TASK_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_TASK_HPP

#include <string>
#include <utility>
#include <vector>
#include <serializable.hpp>
#include <cstring>

/*
 * 负责实现任务信息的保存与序列化、反序列化
 */
struct task : public serializable {
private:
    static uint32_t AUTO_GEN_ID_SER;
    /* 任务的id */
    uint32_t id;
public:
    /* 任务文件的数量 */
    uint32_t file_number;
    /* 任务所需要的文件，因为可能有多个所以使用vector保存 */
    std::vector<uint32_t> files;
    /* 对于任务的描述长度 */
    uint32_t description_size;
    /* 对于任务的描述 */
    std::string description;

    /* struct_size组成: type+id+file_number+files+description_size+description */
    task(int file_number, std::vector<uint32_t> files, int description_size, std::string description)
            : id(AUTO_GEN_ID_SER++), file_number(file_number), files(std::move(files)),
              description_size(description_size),
              description(std::move(description)),
              serializable(4 + 4 + 4 + file_number * 4 + 4 + description_size, data_type::TASK) {
    }

    task(const std::string &description) : id(AUTO_GEN_ID_SER++), description(description),
                                           description_size(strlen(description.c_str())), file_number(0),
                                           serializable(4 + 4 + 4 + 4 + strlen(description.c_str()), data_type::TASK) {

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

    uint32_t get_id() const {
        return id;
    }

    void add_file(uint32_t f) {
        file_number++;
        files.push_back(f);
        struct_size = 4 + 4 + 4 + file_number * 4 + 4 + description_size;
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

uint32_t task::AUTO_GEN_ID_SER = 1;


#endif //NAIVETASKDISTRIBUTIONSYSTEM_TASK_HPP


//
// Created by SidHCH on 2020/6/4.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_FILE_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_FILE_HPP

#include <cstdint>
#include <serializable.hpp>
#include <invalid_file.hpp>
#include <fstream>
#include <invalid_file.hpp>
#include <openssl/sha.h>
#include <cstring>

/* 最大允许10KB的文件大小 */
const int MAX_FILE_SIZE = 1024 * 10;

/* 任务对象 */
struct file : public serializable {
private:
    static uint32_t AUTO_GEN_ID_SER;
    /* 前4字节为id id自动生成 */
    uint32_t id;
public:

    /* 文件的大小 */
    uint64_t size;
    /* 文件的签名，FILE_SIGNATURE */
    u_char FILE_SIGNATURE[SHA_DIGEST_LENGTH]{};
    /* 后可变长的大小，具体文件的字节流 */
    char data[MAX_FILE_SIZE];

    /* struct_size组成：type+id+size+FILE_SIGNATURE+data */
    file(uint64_t size, const char *SHA1, char *data) : serializable(4 + 4 + 8 + SHA_DIGEST_LENGTH + size,
                                                                     data_type::FILE),
                                                        id(AUTO_GEN_ID_SER++),
                                                        size(size) {
        if (size > MAX_FILE_SIZE) {
            printf("超出文件大小限制，最大为%dKB\n", MAX_FILE_SIZE / 1024);
            throw invalid_file("超出文件大小限制");
        }
        memcpy(this->FILE_SIGNATURE, SHA1, SHA_DIGEST_LENGTH);
        memcpy(this->data, data, size);
    }

    file(const char *src, uint32_t struct_size) : serializable(struct_size, data_type::FILE) {
        memcpy(&id, src + 4, 4);
        memcpy(&size, src + 8, 8);
        memcpy(FILE_SIGNATURE, src + 16, SHA_DIGEST_LENGTH);
        memcpy(data, src + 16 + SHA_DIGEST_LENGTH, size);
//        if (!checksum()) {
//            throw invalid_file("文件SHA1验证出错");
//        }
    }

    file(const std::string &file_name) : id(AUTO_GEN_ID_SER++), serializable(-1, data_type::FILE) {
        std::ifstream ifs(file_name, std::ios::binary);
        if (ifs.bad()) {
            ifs.close();
            throw invalid_file("不正确的文件/文件格式");
        }
        char buffer[MAX_FILE_SIZE * 2];
        ifs.read(buffer, MAX_FILE_SIZE);
        int real_size = ifs.gcount();
        if (real_size > MAX_FILE_SIZE) {
            throw invalid_file("文件大小过大");
        }
        size = real_size;
        memcpy(data, buffer, real_size);
        ::SHA1(reinterpret_cast<const unsigned char *>(data), size, FILE_SIGNATURE);
        struct_size = 4 + 4 + 8 + SHA_DIGEST_LENGTH + size;
        ifs.close();
    }

    uint32_t get_id() const {
        return id;
    }

    bool checksum() {
        u_char sum[SHA_DIGEST_LENGTH];
        ::SHA1(reinterpret_cast<const u_char *>(data), size, sum);
        int cmp = strncmp(reinterpret_cast<const char *>(sum), reinterpret_cast<const char *>(FILE_SIGNATURE),
                          SHA_DIGEST_LENGTH);
        return cmp == 0;
    }

    void save() {
        std::ofstream ofs(std::to_string(id), std::ios::binary);
        ofs.write(data, size);
        ofs.close();
    }

    /* 序列化文件为字节流 */
    void serialize(char *const dest) override {
        int tp = static_cast<int>(type);
        memcpy(dest, &tp, 4);
        memcpy(dest + 4, &id, 4);
        memcpy(dest + 8, &size, 8);
        memcpy(dest + 16, FILE_SIGNATURE, SHA_DIGEST_LENGTH);
        memcpy(dest + 16 + SHA_DIGEST_LENGTH, data, size);
    }
};

uint32_t file::AUTO_GEN_ID_SER = 1;

#endif //NAIVETASKDISTRIBUTIONSYSTEM_FILE_HPP

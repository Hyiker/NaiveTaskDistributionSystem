//
// Created by SidHCH on 2020/6/9.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_COMMAND_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_COMMAND_HPP

#include <serializable.hpp>

#define FETCH_TASK 0
#define FETCH_FILE 1
#define NO_MORE_TASK 2
#define MSG_ERROR 3

struct command : public serializable {
public:
    /* 命令类型 */
    uint32_t code;
    /* 操作码 */
    uint32_t operand;

    command(uint32_t code, uint32_t operand = 0) : code(code), operand(operand), serializable(12, data_type::COMMAND) {

    }

    command(const char *src) : serializable(12, data_type::COMMAND) {
        memcpy(&code, src + 4, 4);
        memcpy(&operand, src + 8, 4);
    }

    void serialize(char *const dest) override {
        int tp = static_cast<int>(type);
        memcpy(dest, &tp, 4);
        memcpy(dest + 4, &code, 4);
        memcpy(dest + 8, &operand, 4);
    };
};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_COMMAND_HPP

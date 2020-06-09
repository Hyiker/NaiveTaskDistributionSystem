//
// Created by SidHCH on 2020/6/7.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_INVALID_FILE_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_INVALID_FILE_HPP

#include <stdexcept>
#include <string>

/* 针对接受数据流格式不正确所抛出的错误 */
class invalid_file : public std::runtime_error {
public:
    explicit invalid_file(const std::string &message) : std::runtime_error(message) {

    }
};


#endif //NAIVETASKDISTRIBUTIONSYSTEM_INVALID_FILE_HPP

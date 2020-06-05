//
// Created by SidHCH on 2020/6/5.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_UTILS_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_UTILS_HPP

#include <cstdint>
#include <chrono>

namespace utils {
    uint64_t get_timestamp() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }
};
#endif //NAIVETASKDISTRIBUTIONSYSTEM_UTILS_HPP

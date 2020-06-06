//
// Created by SidHCH on 2020/6/6.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_REQUEST_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_REQUEST_HPP

#include <serializable.hpp>


struct request {
    serializable *sr;
    uint32_t size;

    explicit request(serializable *sr) : sr(sr), size(sr->struct_size + 4) {

    }

    void serialize(char *const dest) {
        memcpy(dest, &size, 4);
        sr->serialize(dest + 4);
    }
};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_REQUEST_HPP

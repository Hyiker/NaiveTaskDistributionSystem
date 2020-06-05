//
// Created by SidHCH on 2020/6/4.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_FILE_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_FILE_HPP

#include <cstdint>

/* 最大允许10KB的文件大小 */
const int MAX_FILE_SIZE = 1024 * 10;
/* 任务对象 */
struct file {
    /* 前4字节为id */
    int id;
    uint64_t size;
    /* 后可变长的大小，具体文件的字节流 */
    char *data[MAX_FILE_SIZE];

};
#endif //NAIVETASKDISTRIBUTIONSYSTEM_FILE_HPP

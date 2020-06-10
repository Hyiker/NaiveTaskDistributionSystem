//
// Created by SidHCH on 2020/6/4.
//
#include <iostream>
#include <server.hpp>
#include <file_manager.hpp>
#include <task_manager.hpp>

using namespace std;

int main() {
    unique_ptr<file_manager> fm = unique_ptr<file_manager>(file_manager::get_instance());
    unique_ptr<task_manager> tm = unique_ptr<task_manager>(task_manager::get_instance());
    uint32_t task_id = tm->add_task("中文测试");
    uint32_t file_id = fm->add_file_to_task("/Users/sidhch/tmp/tmpp.py", tm->get_task(task_id));
    server svr;
    svr.listen();
}
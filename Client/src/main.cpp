//
// Created by SidHCH on 2020/6/4.
//
#include <iostream>
#include <task.hpp>
#include <utils.hpp>
#include <client.hpp>
#include <file_manager.hpp>
#include <task_manager.hpp>

int main() {

    client c("127.0.0.1", 11209);

    file_manager *fm = file_manager::get_instance();
    task_manager *tm = task_manager::get_instance();
    uint32_t task_id = tm->add_task("中文测试");
    uint32_t file_id = fm->add_file_to_task("/Users/sidhch/tmp/ceshi2.txt", tm->get_task(task_id));
    c.send(std::make_shared<request>(tm->get_task(task_id)));
    c.send(std::make_shared<request>(fm->get_file(file_id)));
}
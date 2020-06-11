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
    uint32_t t1 = tm->add_task("排序1");
    uint32_t t2 = tm->add_task("排序2");
    uint32_t t3 = tm->add_task("排序3");
    fm->add_file_to_task("/Users/sidhch/tmp/ms/ms1.py", tm->get_task(t1));
    fm->add_file_to_task("/Users/sidhch/tmp/ms/ms2.py", tm->get_task(t1));
    fm->add_file_to_task("/Users/sidhch/tmp/ms/ms3.py", tm->get_task(t2));
    fm->add_file_to_task("/Users/sidhch/tmp/ms/ms4.py", tm->get_task(t2));
    fm->add_file_to_task("/Users/sidhch/tmp/ms/ms4.py", tm->get_task(t2));
    fm->add_file_to_task("/Users/sidhch/tmp/ms/ms4.py", tm->get_task(t3));
    fm->add_file_to_task("/Users/sidhch/tmp/ms/ms5.py", tm->get_task(t3));
    server svr;
    svr.listen();
}
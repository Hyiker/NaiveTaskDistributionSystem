//
// Created by SidHCH on 2020/6/4.
//
#include <iostream>
#include <task.hpp>
#include <utils.hpp>
#include <client.hpp>
#include <file_manager.hpp>
#include <task_manager.hpp>
#include <command.hpp>
#include <cstring>
#include <iostream>

using namespace std;
client c("127.0.0.1", 11209);

void print_tasks() {
    auto tasks = task_manager::get_instance()->list_tasks();
    if (tasks.empty()) {
        cout << "当前没有可用task" << endl;
    }
    for (auto &it: tasks) {
        cout << "task_id:" << it.first << "|" << it.second->description << "|文件列表:" << endl;
        for (auto &file:it.second->files) {
            cout << file
                 << ((file_manager::get_instance()->has_file_local("_" + to_string(file) + ".py")) ? "√" : "×")
                 << ((file_manager::get_instance()->has_file_local("_" + to_string(file) + ".sav")) ? "√" : "×")
                 << " ";
        }
        cout << endl;
    }
}

bool has(const std::string &str, const std::string &child) {
    return str.find(child) != string::npos;
}

void upload_data(uint32_t i) {
    shared_ptr<task> tsk_ptr;
    if ((tsk_ptr = task_manager::get_instance()->get_task())) {
        for (auto &f:tsk_ptr->files) {
            auto ff = make_shared<file>("_" + to_string(f) + ".sav");
            ff->set_file_name("_" + to_string(f) + "_.sav");
            c.send(make_shared<request>(ff));
            cout << "文件" << ff->filename << "上传成功" << endl;
        }
    } else {
        cout << "不存在对应的task" << endl;
    }
}

[[noreturn]] void keymap() {
    char buffer[128];
    while (true) {
        cin.getline(buffer, 128);
        string input(buffer);
        if (has(input, "exit")) {
            task_manager::get_instance()->save();
            exit(0);
        } else if (has(input, "test")) {
            if (c.test()) {
                printf("有连接\n");
            } else {
                printf("连接失败\n");
            }
        } else if (has(input, "fetch all in")) {
            cout << "fetching..." << endl;
            for (auto f:task_manager::get_instance()->get_task((uint32_t) atoi(buffer + 12))->files) {
                std::shared_ptr<command> ft = std::make_shared<command>(FETCH_FILE, f);
                auto ptr = c.send(std::make_shared<request>(ft));
                if (!ptr) {
                    break;
                }
                shared_ptr<file> fcv = std::dynamic_pointer_cast<file>(ptr->sr);
            }
            print_tasks();
        } else if (has(input, "fetch tasks")) {
            cout << "fetching..." << endl;
            std::shared_ptr<command> ft = std::make_shared<command>(FETCH_TASK);
            auto ptr = c.send(std::make_shared<request>(ft));
            if (!ptr) {
                continue;
            }
            shared_ptr<task> tsk = std::dynamic_pointer_cast<task>(ptr->sr);
            task_manager::get_instance()->add_task(tsk);
            print_tasks();
        } else if (has(input, "fetch file")) {
            cout << "fetching..." << endl;
            std::shared_ptr<command> ft = std::make_shared<command>(FETCH_FILE, atoi(buffer + 11));
            auto ptr = c.send(std::make_shared<request>(ft));
            if (!ptr) {
                continue;
            }
            shared_ptr<file> fcv = std::dynamic_pointer_cast<file>(ptr->sr);
            print_tasks();
        } else if (has(input, "clear")) {
            system("clear");
        } else if (has(input, "show tasks")) {
            print_tasks();
        } else if (has(input, "execute task")) {
            shared_ptr<task> tsk_ptr;
            if ((tsk_ptr = task_manager::get_instance()->get_task(atoi(buffer + 12)))) {
                cout << "executing..." << endl;
                tsk_ptr->run();
                upload_data(atoi(buffer + 12));
            } else {
                cout << "不存在对应的task" << endl;
            }
        } else if (has(input, "remove all")) {
            task_manager::get_instance()->remove_all();
            print_tasks();
        } else if (has(input, "upload data")) {
            upload_data(atoi(buffer + 11));
        } else {
            cout << "未知的指令" << endl;
        }
    }
}

int main() {
    task_manager::get_instance();
    print_tasks();
    printf("\n");
    keymap();
}
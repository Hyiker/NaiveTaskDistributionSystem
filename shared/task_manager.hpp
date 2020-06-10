//
// Created by SidHCH on 2020/6/7.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_TASK_MANAGER_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_TASK_MANAGER_HPP
#define TASKS_SAVE_POS "tasks_save.dat"

#include <map>
#include <task.hpp>
#include <memory>
#include <fstream>

/* 任务管理器，单例模式 */
class task_manager {
private:
    std::map<uint32_t, std::shared_ptr<task>> tasks;
    static std::unique_ptr<task_manager> instance;

    /* 如果文件内有记录，那么使用文件内的记录初始化 */
    task_manager() {
        std::ifstream ifs(TASKS_SAVE_POS, std::ios::binary);
        if (!ifs || ifs.bad()) {
            return;
        }
        char buf[1024];
        while (!ifs.eof()) {
            uint32_t id, size_;
            ifs.read(buf, 4);
            memcpy(&id, buf, 4);
            ifs.read(buf, 4);
            memcpy(&size_, buf, 4);
            ifs.read(buf, size_);
            tasks[id] = std::make_shared<task>(buf, size_);
        }
    };

public:
    static task_manager *get_instance() {
        if (instance == nullptr) {
            instance = std::unique_ptr<task_manager>(new task_manager());
        }
        return instance.get();
    }

    bool update_done() {
        std::ifstream ifs;
        bool all_done{true};
        for (auto &tsk:tasks) {
            bool done{true};
            for (auto &f:tsk.second->files) {
                ifs.open("_" + std::to_string(f) + "_.sav");
                done = done && ifs.good();
            }
            tsk.second->finished = done;
            all_done = all_done && done;
        }
        return all_done;
    }

    uint32_t add_task(const std::shared_ptr<task> &tsk) {
        tasks[tsk->get_id()] = tsk;
        task::change_auto_gen(tsk->get_id());
        return tsk->get_id();
    }

    uint32_t add_task(const std::string &description) {
        std::shared_ptr<task> tsk = std::make_shared<task>(description);
        tasks[tsk->get_id()] = tsk;
        return tsk->get_id();
    }

    std::shared_ptr<task> get_task(uint32_t id) {

        if (tasks.find(id) == tasks.end()) {
            printf("无可用的task\n");
            return nullptr;
        }
        return tasks[id];
    }

    std::shared_ptr<task> get_task() {
        for (auto &task : tasks) {
            if (!task.second->finished) {
                return task.second;
            }
        }
        return nullptr;
    }

    std::map<uint32_t, std::shared_ptr<task>> list_tasks() {
        return tasks;
    }

    void save() {
        std::ofstream ofs(TASKS_SAVE_POS, std::ios::binary);
        char buf[1024];
        for (auto &it:tasks) {
            /* 首先写入4B的id */
            memcpy(buf, &(it.first), 4);
            ofs.write(buf, 4);
            /* 其次写入4B的大小 */
            memcpy(buf, &(it.second->struct_size), 4);
            ofs.write(buf, 4);
            /* 最后写入结构体 */
            it.second->serialize(buf);
            ofs.write(buf, it.second->struct_size);
        }
    }

    std::shared_ptr<task> &operator[](int id) {
        return tasks[id];
    }
};

std::unique_ptr<task_manager> task_manager::instance{nullptr};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_TASK_MANAGER_HPP

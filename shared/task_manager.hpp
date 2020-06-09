//
// Created by SidHCH on 2020/6/7.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_TASK_MANAGER_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_TASK_MANAGER_HPP

#include <map>
#include <task.hpp>
#include <memory>

/* 任务管理器，单例模式 */
class task_manager {
private:
    std::map<uint32_t, std::shared_ptr<task>> tasks;
    static std::unique_ptr<task_manager> instance;

    task_manager() = default;

public:
    static task_manager *get_instance() {
        if (instance == nullptr) {
            instance = std::unique_ptr<task_manager>(new task_manager());
        }
        return instance.get();
    }

    uint32_t add_task(const std::shared_ptr<task> &tsk) {
        tasks[tsk->get_id()] = tsk;
        return tsk->get_id();
    }

    uint32_t add_task(const std::string &description) {
        std::shared_ptr<task> tsk = std::make_shared<task>(description);
        tasks[tsk->get_id()] = tsk;
        return tsk->get_id();
    }


    // TODO 完成task的执行与回送
    std::shared_ptr<task> get_task(uint32_t id) {

        if (tasks.find(id) == tasks.end()) {
            printf("无可用的task\n");
            return nullptr;
        }
        return tasks[id];
    }

    std::shared_ptr<task> &operator[](int id) {
        return tasks[id];
    }
};

std::unique_ptr<task_manager> task_manager::instance{nullptr};

#endif //NAIVETASKDISTRIBUTIONSYSTEM_TASK_MANAGER_HPP

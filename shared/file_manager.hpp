//
// Created by SidHCH on 2020/6/7.
//

#ifndef NAIVETASKDISTRIBUTIONSYSTEM_FILE_MANAGER_HPP
#define NAIVETASKDISTRIBUTIONSYSTEM_FILE_MANAGER_HPP

#include <fstream>
#include <file.hpp>
#include <memory>
#include <task.hpp>
#include <map>

/* 任务文件管理器，同样适用单例模式 */
class file_manager {
private:
    std::map<uint32_t, std::shared_ptr<file>> files;
    static std::unique_ptr<file_manager> instance;
    /* 用来验证文件是否存在的tester */
    std::ifstream tester;

    file_manager() = default;

public:
    static file_manager *get_instance() {
        if (instance == nullptr) {
            instance = std::unique_ptr<file_manager>(new file_manager());
        }
        return instance.get();
    }

    /* 向任务中添加文件并且返回对应的file_id */
    uint32_t add_file(const std::shared_ptr<file> &f) {
        files[f->get_id()] = f;
        file::change_auto_gen(f->get_id());
        return f->get_id();
    }

    /*
     * 使用ifstream添加文件
     * 根据文件名，向对应的task中添加任务文件
     * */
    uint32_t add_file_to_task(const std::string &file_name, const std::shared_ptr<task> &tsk) {
        std::shared_ptr<file> f{nullptr};
        try {
            f = std::make_shared<file>(file_name);
        } catch (const invalid_file &e) {
            printf("%s\n", e.what());
            return -1;
        }
        files[f->get_id()] = f;
        tsk->add_file(f->get_id());
        return f->get_id();
    }

    std::shared_ptr<file> get_file(uint32_t id) {
        if (files.find(id) == files.end()) {
            printf("无可用的task\n");
            return nullptr;
        }
        return files[id];
    }

    bool has_file_local(const std::string &filename) {
        tester.open(filename);
        bool res = tester.good();
        tester.close();
        return res;
    }

    std::shared_ptr<file> &operator[](int id) {
        return files[id];
    }
};

std::unique_ptr<file_manager> file_manager::instance{nullptr};
#endif //NAIVETASKDISTRIBUTIONSYSTEM_FILE_MANAGER_HPP

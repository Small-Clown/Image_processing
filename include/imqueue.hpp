#include <filesystem>
#include "improc.hpp"
#include <map>
#include <iostream>
#include <string>

#ifndef IMAGE_PROCESSOR__SKELETON_IMQUEUE_HPP
#define IMAGE_PROCESSOR__SKELETON_IMQUEUE_HPP


class TaskData{
public:
    std::filesystem::path input_dir;
    std::function<Image(Image)> p_function;
    std::filesystem::path output_dir;
    void execute_task(std::string _id);
};

using queue_type = std::map<std::string, TaskData>;

class TaskQueue{
private:
    queue_type _queue;
public:
    TaskQueue() = default;
    ~TaskQueue() = default;
    void add_task(std::string task_id, TaskData task){
        _queue.insert({task_id , task});
    };
    void execute_all();
    const queue_type& get_all_tasks(){ return _queue; }
};

#endif //IMAGE_PROCESSOR__SKELETON_IMQUEUE_HPP

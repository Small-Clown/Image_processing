#include "imqueue.hpp"
#include "improc.hpp"

void TaskQueue::execute_all() {
    for(auto task : _queue){
        task.second.execute_task(task.first);
    }
}

void TaskData::execute_task(std::string _id) {
    Image pre_exec = load_bitmap(input_dir.string());
    Image post_exec = p_function(pre_exec);
    if(!exists(output_dir)){
        bool x = create_directory(output_dir);
        std::cout << x << std::endl;
    }

    std::string path_file = output_dir.append(input_dir.filename().string()).string();
    path_file = path_file.substr(0, path_file.length() - 4) + "__" + _id + input_dir.extension().string();
    int exit = save_bitmap( path_file, post_exec);
    if(exit == EXIT_FAILURE){
        std::cout << "SOMETHING WENT WRONG! SAVING FAILED!" << std::endl;
    }

    std::cout << "File saved in: " << std::filesystem::absolute(output_dir).string() << std::endl;
};
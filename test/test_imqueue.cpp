#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "imqueue.hpp"

#include <filesystem>
namespace fs = std::filesystem;

// -----------------

TEST(ImQueueTest, ExecuteAll) {
    fs::path img_root = "../imgs/";
    fs::path img_root_out = "../out_test/";

    if (fs::exists(img_root_out)) {
        fs::remove_all(img_root_out);
    }

    fs::path input_filename = fs::path(img_root).append("1x1_g127_win.bmp");

    std::function<byte(byte)> f_inverse = [](byte b) -> byte { return (byte) (255U - b); };

    TaskQueue task_queue;
    task_queue.add_task("transform", TaskData{input_filename,
                                            [&f_inverse](const Image &im) -> Image { return transform(im, f_inverse); }, img_root_out});
    task_queue.add_task("filter", TaskData{input_filename,
                                           [](const Image &im) -> Image { return filter(im, get_averaging_mask(3)); }, img_root_out});
    task_queue.execute_all();

    fs::path p1 = fs::path(img_root_out).append("1x1_g127_win__transform.bmp");
    ASSERT_TRUE(fs::exists(p1) && fs::is_regular_file(p1));
    fs::path p2 = fs::path(img_root_out).append("1x1_g127_win__filter.bmp");
    ASSERT_TRUE(fs::exists(p2) && fs::is_regular_file(p2));

    fs::remove_all(img_root_out);
}

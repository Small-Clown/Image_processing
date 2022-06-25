#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "improc.hpp"

#include <cstdio>

// -----------------

TEST(AuxTest, ToString) {
    Image input_image(2, 2, nullptr, BITMAPFILEHEADER());
    input_image[0][0] = 65U;
    input_image[0][1] = 66U;
    input_image[1][0] = 67U;
    input_image[1][1] = 68U;

    std::string im_str_char = to_string(input_image, ImagePrintMode::CHARS);
    EXPECT_EQ(im_str_char, "AB\nCD");

    std::string im_str_num = to_string(input_image, ImagePrintMode::NUMS);
    EXPECT_EQ(im_str_num, "  65  66\n  67  68");
}

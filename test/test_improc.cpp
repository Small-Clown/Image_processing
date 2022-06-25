#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "improc.hpp"

// -----------------

TEST(ImProcTest, Transform_1x1) {
    Image input_image(1, 1, nullptr, BITMAPFILEHEADER());
    input_image[0][0] = 1U;

    std::function<byte(byte)> f_inverse = [](byte b) -> byte { return (byte) (255U - b); };
    Image transformed_image = transform(input_image, f_inverse);

    EXPECT_EQ(transformed_image[0][0], 254U);
}

TEST(ImProcTest, Transform_1x2) {
    Image input_image(1, 2, nullptr, BITMAPFILEHEADER());
    input_image[0][0] = 1U;
    input_image[0][1] = 2U;

    std::function<byte(byte)> f_inverse = [](byte b) -> byte { return (byte) (255U - b); };
    Image transformed_image = transform(input_image, f_inverse);

    EXPECT_EQ(transformed_image[0][0], 254U);
    EXPECT_EQ(transformed_image[0][1], 253U);
}

TEST(ImProcTest, Filter_1x1_n3) {
    Image input_image(1, 1, nullptr, BITMAPFILEHEADER());
    input_image[0][0] = 1U;

    Image filtered_image = filter(input_image, get_averaging_mask(3));

    EXPECT_EQ(filtered_image[0][0], 0U);
}

TEST(ImProcTest, Filter_1x1_n5) {
    Image input_image(1, 1, nullptr, BITMAPFILEHEADER());
    input_image[0][0] = 1U;

    Image filtered_image = filter(input_image, get_averaging_mask(5));

    EXPECT_EQ(filtered_image[0][0], 0U);
}

TEST(ImProcTest, Filter_3x3_n3) {
    Image input_image(3, 3, nullptr, BITMAPFILEHEADER());
    input_image[1][1] = 9U;

    Image filtered_image = filter(input_image, get_averaging_mask(3));

    for (std::size_t i = 0; i < filtered_image.get_nrows(); i++) {
        for (std::size_t j = 0; j < filtered_image.get_ncols(); j++) {
            EXPECT_EQ(filtered_image[i][j], 1U);
        }
    }
}

TEST(ImProcTest, Filter_3x5_n3) {
    Image input_image(3, 5, nullptr, BITMAPFILEHEADER());
    input_image[1][1] = 18U;
    input_image[1][2] = 9U;
    input_image[1][3] = 9U;

    Image filtered_image = filter(input_image, get_averaging_mask(3));

    for (std::size_t i = 0; i < filtered_image.get_nrows(); i++) {
        for (std::size_t j = 0; j < filtered_image.get_ncols(); j++) {
            if (j == 0 || j == 3)
                EXPECT_EQ(filtered_image[i][j], 2U);
            else if (j == 1)
                EXPECT_EQ(filtered_image[i][j], 3U);
            else if (j == 2)
                EXPECT_EQ(filtered_image[i][j], 4U);
            else
                EXPECT_EQ(filtered_image[i][j], 1U);
        }
    }
}

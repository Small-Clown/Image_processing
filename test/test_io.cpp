#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "improc.hpp"

#include <cstdio>

// -----------------

TEST(ImIOTest, ReadImage) {
    std::string img_root = "../imgs/";
    std::string input_filename = img_root + "1x1_g127_win.bmp";
    Image input_image = load_bitmap(input_filename);

    ASSERT_NE(input_image.get_bitmapinfo(), nullptr);



    ASSERT_EQ(input_image.get_nrows(), 1);
    ASSERT_EQ(input_image.get_ncols(), 1);
    EXPECT_EQ(input_image[0][0], 127U);

#ifdef WIN32

    BITMAPFILEHEADER file_header = input_image.get_bitmapheader();
    BITMAPINFOHEADER info_header = input_image.get_bitmapinfo()->bmiHeader;

    EXPECT_EQ(file_header.bfType, 19778);
    EXPECT_EQ(file_header.bfSize, 1084);
    EXPECT_EQ(file_header.bfReserved1, 0);
    EXPECT_EQ(file_header.bfReserved2, 0);
    EXPECT_EQ(file_header.bfOffBits, 1078);

    EXPECT_EQ(info_header.biSize, 40);
    EXPECT_EQ(info_header.biWidth, 1);
    EXPECT_EQ(info_header.biHeight, 1);
    EXPECT_EQ(info_header.biPlanes, 1);
    EXPECT_EQ(info_header.biBitCount, 8);
    EXPECT_EQ(info_header.biCompression, 0);
    EXPECT_EQ(info_header.biSizeImage, 6);
    EXPECT_EQ(info_header.biXPelsPerMeter, 2834);
    EXPECT_EQ(info_header.biYPelsPerMeter, 2834);
    EXPECT_EQ(info_header.biClrUsed, 0);
    EXPECT_EQ(info_header.biClrImportant, 0);
#else
#endif
}

TEST(ImIOTest, WriteImage) {
    std::string img_root = "../imgs/";

    std::string reference_filepath = img_root + "1x1_g127_win.bmp";
    Image reference_image = load_bitmap(reference_filepath);

    std::string test_filpath = img_root + "test_out.bmp";
    save_bitmap(test_filpath, reference_image);
    Image input_image = load_bitmap(test_filpath);
    std::remove(test_filpath.c_str());

    ASSERT_NE(input_image.get_bitmapinfo(), nullptr);


    ASSERT_EQ(input_image.get_nrows(), reference_image.get_nrows());
    ASSERT_EQ(input_image.get_ncols(), reference_image.get_ncols());
    EXPECT_EQ(input_image[0][0], reference_image[0][0]);

#ifdef WIN32

    BITMAPFILEHEADER file_header_ref = reference_image.get_bitmapheader();
    BITMAPINFOHEADER info_header_ref = reference_image.get_bitmapinfo()->bmiHeader;

    BITMAPFILEHEADER file_header = input_image.get_bitmapheader();
    BITMAPINFOHEADER info_header = input_image.get_bitmapinfo()->bmiHeader;



    EXPECT_EQ(file_header.bfType, file_header_ref.bfType);
    EXPECT_EQ(file_header.bfSize, file_header_ref.bfSize);
    EXPECT_EQ(file_header.bfReserved1, file_header_ref.bfReserved1);
    EXPECT_EQ(file_header.bfReserved2, file_header_ref.bfReserved2);
    EXPECT_EQ(file_header.bfOffBits, file_header_ref.bfOffBits);

    EXPECT_EQ(info_header.biSize, info_header_ref.biSize);
    EXPECT_EQ(info_header.biWidth, info_header_ref.biWidth);
    EXPECT_EQ(info_header.biHeight, info_header_ref.biHeight);
    EXPECT_EQ(info_header.biPlanes, info_header_ref.biPlanes);
    EXPECT_EQ(info_header.biBitCount, info_header_ref.biBitCount);
    EXPECT_EQ(info_header.biCompression, info_header_ref.biCompression);
    EXPECT_EQ(info_header.biSizeImage, info_header_ref.biSizeImage);
    EXPECT_EQ(info_header.biXPelsPerMeter, info_header_ref.biXPelsPerMeter);
    EXPECT_EQ(info_header.biYPelsPerMeter, info_header_ref.biYPelsPerMeter);
    EXPECT_EQ(info_header.biClrUsed, info_header_ref.biClrUsed);
    EXPECT_EQ(info_header.biClrImportant, info_header_ref.biClrImportant);
#else
#endif
}

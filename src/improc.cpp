#include "../include/improc.hpp"
#include "../include/bitmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <exception>


Image load_bitmap(const std::string& filepath){
    BITMAPINFO* inf = new BITMAPINFO();
    BITMAPFILEHEADER header;
    byte* bitmapBytes = LoadDIBitmap(filepath.c_str(), &inf, &header);
    const size_t h = (size_t) (inf)->bmiHeader.biHeight;
    const size_t w = (size_t) (inf)->bmiHeader.biWidth;
    const size_t bits_per_pixel = (inf)->bmiHeader.biBitCount;

    Image return_image(h, w, inf, header);
    size_t row_size = (bits_per_pixel * w + 31) / 32 * 4;
    byte* reader = bitmapBytes;

    for (size_t i = 0; i < h; ++i) {
        /* Copy values of pixels in an image row. */
        for (size_t j = 0; j < w; ++j) {
            return_image[h - i - 1][j] = (byte) *reader;
            ++reader;
        }
        /* Skip padding bytes. */
        reader += row_size - w;
    }
    free(bitmapBytes);
    return return_image;
}

extern int save_bitmap(const std::string& filename, const Image& image){
    const size_t h = (size_t) (image.get_bitmapinfo())->bmiHeader.biWidth;
    const size_t w = (size_t) (image.get_bitmapinfo())->bmiHeader.biHeight;
    const size_t bits_per_pixel = (image.get_bitmapinfo())->bmiHeader.biBitCount;
    const size_t row_size = (bits_per_pixel * w + 31) / 32 * 4;

    byte* bitmapBytes = (byte*) malloc(sizeof(byte) * (image.get_bitmapinfo())->bmiHeader.biSizeImage);
    byte* writer = bitmapBytes;

    const size_t padding = row_size - w;

    for (size_t i = 0; i < h; ++i) {

        /* Przepisz wartosci pikseli wiersza obrazu. */
        for (size_t j = 0; j < w; j++) {
            *writer++ = image[h - i - 1][j];
        }

        /* Ustaw bajty wyrownania. */
        for (size_t j = 0; j < padding; j++) {
            *writer++ = 0;
        }
    }

    int status = SaveDIBitmap(filename.c_str(), image.get_bitmapinfo(), bitmapBytes);
    free(bitmapBytes);


    return status;
}

std::string to_string(const Image& image, ImagePrintMode printType){
    std::ostringstream print;
    for(std::size_t rows = 0; rows < image.get_nrows(); rows++){
        for(std::size_t colls = 0; colls < image.get_ncols(); colls++){
            if(printType == CHARS){
                print << (char) image[rows][colls];
            }else{
                print << std::setw(4) << (int) image[rows][colls];
            }
        }
        if(rows < image.get_nrows() -1){
            print << std::endl;
        }
    }
    return print.str();
}

Image transform(const Image& im_in, std::function<byte(byte)> func){
    Image finished(im_in);
    for(std::size_t row = 0; row < finished.get_nrows(); row++){
        for(std::size_t collumn = 0; collumn < finished.get_ncols(); collumn++) {
            finished[row][collumn] = func(finished[row][collumn]);
        }
    }
    return finished;
}

Mask get_averaging_mask(std::size_t n){
    return Mask(n, n, (double) (1.0 / ((double)n * (double)n)));
}

Image filter(const Image& im_in, const Mask& mask){
    Image finished(im_in);
    std::size_t mask_range = (mask.get_ncols() - 1) / 2;
    for(std::size_t row = 0; row < finished.get_nrows(); row++){
        for(std::size_t collumn = 0; collumn < finished.get_ncols(); collumn++){
            byte sum = 0;


            for(std::size_t r_m = row; r_m < row + mask.get_nrows(); r_m++){
                for(std::size_t c_m = collumn; c_m < collumn + mask.get_ncols(); c_m++){
                    std::size_t pixel_value;
                    if(r_m + 1 > finished.get_nrows() + mask_range || c_m + 1> finished.get_ncols() + mask_range  || c_m < mask_range || r_m < mask_range) { // handles out of bounds exceptions
                        pixel_value = 0;
                    }else {
                        pixel_value = im_in[r_m - mask_range][c_m - mask_range];
                    }

                    sum += (byte) ((double) pixel_value * mask[r_m - row][c_m - collumn]);
                }
            }


            finished[row][collumn] = sum;
        }
    }
    return finished;
}

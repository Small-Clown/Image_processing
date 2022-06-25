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

std::string to_string(const Image& im, ImagePrintMode mode){
    std::ostringstream print;
    for(std::size_t r = 0; r < im.get_nrows(); r++){
        for(std::size_t c = 0; c < im.get_ncols(); c++){
            if(mode == CHARS){
                print << (char) im[r][c];
            }else{
                print << std::setw(4) << (int) im[r][c];
            }
        }
        if(r < im.get_nrows() -1){
            print << std::endl;
        }
    }
    return print.str();
}

Image transform(const Image& im_in, std::function<byte(byte)> func){
    Image processed(im_in);
    for(std::size_t r = 0; r < processed.get_nrows(); r++){
        for(std::size_t c = 0; c < processed.get_ncols(); c++) {
            processed[r][c] = func(processed[r][c]);
        }
    }
    return processed;
}

Mask get_averaging_mask(std::size_t n){
    return Mask(n, n, (double) (1.0 / ((double)n * (double)n)));
}

Image filter(const Image& im_in, const Mask& mask){
    Image processed(im_in);
    std::size_t mask_range = (mask.get_ncols() - 1) / 2; // the length at which the maks goes either way e.g. 3x3 -> 1 -> 1 left 1 right 1 top 1 bottom
    for(std::size_t r = 0; r < processed.get_nrows(); r++){ // r -> the actual rows of the matrix being processed
        for(std::size_t c = 0; c < processed.get_ncols(); c++){ // c -> the actual columns of the matrix being processed
            byte sum = 0; //sum that is to be put in each byte (bytes from the averaging mask)
            for(std::size_t r_m = r; r_m < r + mask.get_nrows(); r_m++){ // r_m -> current row of the mask being iterated over (goes from 0 to length)
                /// example we start on 3x3 mask so on 1st pixel r_m is 0 and we go to double the range because we then subtract
                for(std::size_t c_m = c; c_m < c + mask.get_ncols(); c_m++){ // c_m current column of the mask being iterated over (from 0 to length)
                    std::size_t pixel_value; //value of the pixel that is to be assigned
                    if(r_m < mask_range || r_m + 1 > processed.get_nrows() + mask_range || c_m < mask_range || c_m + 1> processed.get_ncols() + mask_range) {
                        // 1st condition is [-range + iter >= 0] transformed and negated (left overflow)
                        // 2nd condition is [iter - range < length] negated and transformed (right overflow)
                        // 3rd condition is 1st condition but on columns (top overflow)
                        // 4th condition is 2nd condition but on columns (bottom overflow)
                        pixel_value = 0;
                    }else {
                        pixel_value = im_in[r_m - mask_range][c_m - mask_range];
                    }

                    sum += (byte) ((double) pixel_value * mask[r_m - r][c_m - c]);
                }
            }
            processed[r][c] = sum;
        }
    }
    return processed;
}
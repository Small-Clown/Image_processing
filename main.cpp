#include <stdlib.h>

#include "include/bitmap.h"
#include "include/improc.hpp"

int main(void) {
    // FIXME: Replace it with the absolute path to an existing image.
    const char* input_filepath = "/home/image_processor/imgs/lena_128.bmp";
    // FIXME: Replace it with the absolute path to an existing image.
    const char* output_filepath = "/home/image_processor/imgs/lena_128__out.bmp";

    BITMAPINFO* bitmap_info = NULL;
    byte** image_array = load_bitmap(input_filepath, &bitmap_info);
    save_bitmap(output_filepath, image_array, bitmap_info);

    // Cleanup.

    int h = bitmap_info->bmiHeader.biHeight;

    if (image_array) {
        for (int i = 0; i < h; i++) {
            free(image_array[i]);
        }

        free(image_array);
        image_array = NULL;
    }

    if (bitmap_info) {
        free(bitmap_info);
    }

    return EXIT_SUCCESS;
}

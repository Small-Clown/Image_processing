/*
 * Windows BMP file definitions.
 *
 * Written by Pawel Kleczek [pkleczek@agh.edu.pl]
 * (based on the code by Michael Sweet).
 */

#ifndef _BITMAP_H_
#define _BITMAP_H_

/*
 * Include necessary headers.
 */

#ifdef WIN32

#include <windows.h>
#include <wingdi.h>

#endif /* WIN32 */

/*
 * Make this header file work with C and C++ source code...
 */

#  ifdef __cplusplus
extern "C" {
#  endif /* __cplusplus */

/*
 * Bitmap file data structures (these are defined in <wingdi.h> under
 * Windows...)
 *
 * Note that most Windows compilers will pack the following structures, so
 * when reading them under MacOS or UNIX we need to read individual fields
 * to avoid differences in alignment...
 */

#include <stdlib.h>
#include <stdint.h>

#  ifndef WIN32
typedef struct /**** BMP file header structure ****/
{
    uint16_t bfType; /* Magic number for file */
    uint32_t bfSize; /* Size of file */
    uint16_t bfReserved1; /* Reserved */
    uint16_t bfReserved2; /* ... */
    uint32_t bfOffBits; /* Offset to bitmap data */
} BITMAPFILEHEADER;

extern const uint16_t BF_TYPE;  /* 0x4D42 == "MB" */

typedef struct /**** BMP file info structure ****/
{
    uint32_t biSize; /* Size of info header */
    int32_t biWidth; /* Width of image */
    int32_t biHeight; /* Height of image */
    uint8_t biPlanes; /* Number of color planes */
    uint8_t biBitCount; /* Number of bits per pixel */
    uint8_t biCompression; /* Type of compression to use */
    uint32_t biSizeImage; /* Size of image data */
    int32_t biXPelsPerMeter; /* X pixels per meter */
    int32_t biYPelsPerMeter; /* Y pixels per meter */
    uint32_t biClrUsed; /* Number of colors used */
    uint32_t biClrImportant; /* Number of important colors */
} BITMAPINFOHEADER;

/*
 * Constants for the biCompression field...
 */

#  define BI_RGB       0             /* No compression - straight BGR data */
#  define BI_RLE8      1             /* 8-bit run-length compression */
#  define BI_RLE4      2             /* 4-bit run-length compression */
#  define BI_BITFIELDS 3             /* RGB bitmap with RGB masks */

typedef struct /**** Colormap entry structure ****/
{
    uint8_t rgbBlue; /* Blue value */
    uint8_t rgbGreen; /* Green value */
    uint8_t rgbRed; /* Red value */
    uint8_t rgbReserved; /* Reserved */
} RGBQUAD;

typedef struct /**** Bitmap information structure ****/
{
    BITMAPINFOHEADER bmiHeader; /* Image header */
    RGBQUAD bmiColors[256]; /* Image colormap */
} BITMAPINFO;
#  endif /* !WIN32 */

typedef uint8_t byte;

/**
 * Load a DIB/BMP file from disk.
 *
 * @param filename File to load. [in]
 * @param info Bitmap information. [out]
 * @param header Bitmap header. [out]
 * @return A pointer to the bitmap if successful, NULL otherwise.
 */
extern byte* LoadDIBitmap(const char* filename, BITMAPINFO** info, BITMAPFILEHEADER* header);

/**
 * Save a DIB/BMP file to disk.
 *
 * @param filename File to save. [in]
 * @param info Bitmap information. [in]
 * @param bits Bitmap data. [in]
 * @return EXIT_SUCCESS on success or EXIT_FAILURE on failure.
 */
extern int SaveDIBitmap(const char* filename, BITMAPINFO* info, byte* bits);

#  ifdef __cplusplus
}
#  endif /* __cplusplus */
#endif /* !_BITMAP_H_ */

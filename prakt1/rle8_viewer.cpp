// Build using g++ from GNU Compiler Collection (GCC, e.g. from https://nuwen.net/mingw.html):
// g++ rle8_viewer.cpp -lopengl32 -lgdi32 -mwindows -o rle8_viewer.exe


#include "tigr.c"  // Beware! Bad style: directly include C file. Advantage: Saves separate compilation and linking of module tigr.c.
#include <stdint.h>


#pragma pack(push, 1)

struct BmpFileHeader
{
    uint16_t bfType;            // specifies the file type
    uint32_t bfSize;            // specifies the size in bytes of the bitmap file
    uint16_t bfReserved1;       // reserved; must be 0
    uint16_t bfReserved2;       // reserved; must be 0
    uint32_t bfOffBits;         // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
};

struct BmpInfoHeader
{
    uint32_t biSize;            // specifies the number of bytes required by the struct
    int32_t  biWidth;           // specifies width in pixels
    int32_t  biHeight;          // species height in pixels
    uint16_t biPlanes;          // specifies the number of color planes, must be 1
    uint16_t biBitCount;        // specifies the number of bit per pixel
    uint32_t biCompression;     // specifies the type of compression
    uint32_t biSizeImage;       // size of image in bytes
    int32_t  biXPelsPerMeter;   // number of pixels per meter in x axis
    int32_t  biYPelsPerMeter;   // number of pixels per meter in y axis
    uint32_t biClrUsed;         // number of colors used by the bitmap
    uint32_t biClrImportant;    // number of colors that are important
};

struct BmpPaletteEntry
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
};

#pragma pack(pop)


int main(int argc, char* argv[])
{
    // size of the drawable area of the window in pixels
    // (should later be adapted to size of bitmap to be displayed)
    int screen_width = 800;
    int screen_height = 600;

    // initialize window and screen surface, fill with black color
    Tigr* screen = tigrWindow(screen_width, screen_height, "Praktikum DKK: RLE8-Viewer", TIGR_FIXED);
    tigrClear(screen, tigrRGB(0x00, 0x00, 0x00));

    for (int y = 0; y < screen_height; ++y) {
        for (int x = 0; x < screen_width; ++x) {
            // choose a pixel color (triplet of values 0...255)
            TPixel color = tigrRGB((x + y) % 256, (y - x) % 256, (x - y) % 256);
            // put pixel on screen surface
            tigrPlot(screen, x, y, color);
        }
    }

    // wait until user closes window (or Escape is pressed)
    while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)) {
        tigrUpdate(screen);
    }

    tigrFree(screen);
}

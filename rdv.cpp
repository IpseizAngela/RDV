#include <iostream>
#include <vector>
#include "data.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WIDTH 200
#define HEIGHT 200

void render() {
    Color c = {127,127,127};
    std::vector<unsigned char> pixmap(WIDTH*HEIGHT*3);
    for (size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        for (size_t j = 0; j<3; j++) {
            pixmap[i*3+j] = (unsigned char)127;
        }
    }
    stbi_write_jpg("out.jpg", WIDTH, HEIGHT, 3, pixmap.data(), 100);
}

int main() {
    //std::cout << "Hello, World!" << std::endl;
    render();
    return 0;
}

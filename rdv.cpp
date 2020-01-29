#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "data.hpp"

#define WIDTH 10
#define HEIGHT 10

void writepgm(const unsigned char* image, const char* filename){
    int i;
    FILE* F = fopen(filename,"w");
    if (F) {
        fprintf(F, "P3\n%d %d\n255\n", WIDTH, HEIGHT);
        unsigned char r, v, b;
        for (i = 0; i < WIDTH * HEIGHT; i++) {
            fprintf(F, "%d %d %d ", image[i*3 + 0], image[i*3 + 1], image[i*3 + 2]);
        }
        fclose(F);
    }
}

void render() {
    Color c = {127,127,127};
    std::vector<unsigned char> pixmap(WIDTH*HEIGHT*3);
    for (size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        for (size_t j = 0; j<3; j++) {
            pixmap[i*3+j] = (unsigned char)127;
        }
    }
    writepgm(pixmap.data(), "test.ppm");
}

int main() {
    render();
    return 0;
}

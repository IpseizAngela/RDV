#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "data.hpp"
#include "model.h"

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
    Color c;
    //c.tab= {127,127,127};
    c.tab[0] = 127;
    c.tab[1] = 127;
    c.tab[2] = 127;
    std::vector<unsigned char> pixmap(WIDTH*HEIGHT*3);
    for (size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        for (size_t j = 0; j<3; j++) {
            pixmap[i*3+j] = (unsigned char)c.tab[j];
        }
    }
    writepgm(pixmap.data(), "test.ppm");
}

int main(int argc, char** argv) {
    std::cout << "Usage: " << argv[0] << " model.obj tangentnormals.jpg diffuse.jpg specular.jpg" << std::endl;
    std::string file_obj ("../lib/obj/diablo3_pose/diablo3_pose.obj");
    std::string file_nm  ("../lib/obj/diablo3_pose/diablo3_pose_nm_tangent.tga");
    std::string file_diff("../lib/obj/diablo3_pose/diablo3_pose_diffuse.tga");
    std::string file_spec("../lib/obj/diablo3_pose/diablo3_pose_spec.tga");
    if (5==argc) {
        file_obj  = std::string(argv[1]);
        file_nm   = std::string(argv[2]);
        file_diff = std::string(argv[3]);
        file_spec = std::string(argv[4]);
    }
    Model model(file_obj.c_str());
    std::cout << "Number of points in the model: " << model.nbvertex() << std::endl;
    render();
    return 0;
}

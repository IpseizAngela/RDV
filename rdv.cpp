#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "data.hpp"
#include "model.h"

#define WIDTH 800
#define HEIGHT 800

using namespace std;

 void writeppm(unsigned char* image, const char* filename){
    int i, j;
    FILE* F = fopen(filename,"w");
    if (F) {
        fprintf(F, "P3\n%d %d\n255\n", WIDTH, HEIGHT);
        unsigned char r, v, b;
        for (i = 0; i < WIDTH * HEIGHT; i++) {
            for (j = 0; j < 3; j++) {
                fprintf(F, "%d %d %d ", image[i*3 + 0], image[i*3 + 1], image[i*3 + 2]);
            }
        }
        fclose(F);
    }
}



void render(Model m) {
    std::vector<Color> pixels(WIDTH*HEIGHT*3);
    Color black;
    black = {0, 0, 0};
    Color white = {255,255,255};
    for (size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        pixels[i] = black;
    }
    
    Point pt;
    int col, lig;
    float h = HEIGHT / 2.;
    float w = WIDTH / 2.;
    for (int i = 0; i < m.nbvertex(); i++) {
        pt = m.point(i);
        
        if (pt.dim[0] < 0) col = (int) (w - abs(pt.dim[0]*w));
        else col = (int) (w + pt.dim[0]*w);
        
        if (pt.dim[1] < 0) lig = (int) (h + abs(pt.dim[1]*h));
        else lig = (int) (h - pt.dim[1]*h);
        
        pixels[lig*WIDTH + col] = white;
    }
    std::vector<unsigned char> pixmap(WIDTH*HEIGHT*3);
    for (size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        for (size_t j = 0; j<3; j++) {
            pixmap[i*3+j] = (unsigned char)/*(255 * std::max(0.f, std::min(1.f,*/ pixels[i].get(j);//)));
        }
    }
    writeppm(pixmap.data(), "test1.ppm");
}

int main(int argc, char** argv) {
    cout << "Usage: " << argv[0] << " model.obj tangentnormals.jpg diffuse.jpg specular.jpg" << endl;
    string file_obj ("../lib/obj/diablo3_pose/diablo3_pose.obj");
    string file_nm  ("../lib/obj/diablo3_pose/diablo3_pose_nm_tangent.tga");
    string file_diff("../lib/obj/diablo3_pose/diablo3_pose_diffuse.tga");
    string file_spec("../lib/obj/diablo3_pose/diablo3_pose_spec.tga");
    if (5==argc) {
        file_obj  = std::string(argv[1]);
        file_nm   = std::string(argv[2]);
        file_diff = std::string(argv[3]);
        file_spec = std::string(argv[4]);
    }
    Model model(file_obj.c_str());
    cout << "Number of points in the model: " << model.nbvertex() << " Number of faces in the model " << model.nbfaces() << endl;
    render(model);
    return 0;
}

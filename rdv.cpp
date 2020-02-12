#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "data.hpp"
#include "model.h"

#define WIDTH 800
#define HEIGHT 800

using namespace std;

void segment(Pointi A, Pointi B, Color coul, std::vector<Color> &pixels)
{
	cout << "A = " << A.x << "; " << A.y << endl;
	cout << "B = " << B.x << "; " << B.y << endl;
    int dlig = B.y - A.y;
    int dcol = B.x - A.x;
    int absdcol = abs(dcol);
    int absdlig = abs(dlig);
    int col = A.x;
    int lig = A.y;
    int sensLig = 1;
    int sensCol = 1;
    int cumul;

    if (dcol < 0) {
        sensCol = -1;
    }
    if (dlig < 0) {
        sensLig = -1;
    }

    if (absdcol >= absdlig) {
        cumul = absdcol;
        while (col != B.x + sensCol) {
			// cout << "lig = " << lig << "; col = " << col << endl;
            pixels[lig*WIDTH + col] = coul;
            cumul = cumul + (2*absdlig);
            if (cumul >= 2*absdcol) {
                lig = lig + sensLig;
                cumul = cumul - (2*absdcol);
            }
            col = col + sensCol;
        }
    } else {
        cumul = absdlig;
        while (lig != B.y + sensLig) {
			// cout << "lig = " << lig << "; col = " << col << endl;
            pixels[lig*WIDTH + col ] = coul;
            cumul = cumul + (2*absdcol);
            if (cumul >= 2*absdlig) {
                col = col + sensCol;
                cumul = cumul - (2*absdlig);
            }
            lig = lig + sensLig;
        }
    }
}

void writeppm(unsigned char* image, const char* filename){
    int i, j;
    FILE* F = fopen(filename,"w");
    if (F) {
        fprintf(F, "P3\n%d %d\n255\n", WIDTH, HEIGHT);
        unsigned char r, v, b;
        for (i = 0; i < WIDTH * HEIGHT; i++) {
            //for (j = 0; j < 3; j++) {
                fprintf(F, "%d %d %d ", image[i*3 + 0], image[i*3 + 1], image[i*3 + 2]);
            //}
        }
        fclose(F);
    }
}



void render(Model m) {
    std::vector<Color> pixels(WIDTH*HEIGHT*3);
    Color black;
    black = {0, 0, 0, 0};
    Color white = {255,255 ,255, 0};
    for (size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        pixels[i] = black;
    }
    
    /*Point pt;
    int col, lig;
    float h = HEIGHT / 2.;
    float w = WIDTH / 2.;
    for (int i = 0; i < m.nbvertex(); i++) {
        pt = m.point(i);
        
        if (pt.x < 0) col = (int) (w - abs(pt.x*w));
        else col = (int) (w + pt.x*w);
        
        if (pt.y < 0) lig = (int) (h + abs(pt.y*h));
        else lig = (int) (h - pt.y*h);
        
        pixels[lig*WIDTH + col] = white;
    }*/
	Pointi p1 = {10,10,10};
	Pointi p2 = {700,700,0};
	segment(p1, p2, white, pixels);
	
	Pointi p3 = {600,10,10};
	Pointi p4 = {450,700,0};
	segment(p3, p4, white, pixels);
    
    
    std::vector<unsigned char> pixmap(WIDTH*HEIGHT*3);
    for (size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        for (size_t j = 0; j<3; j++) {
            pixmap[i*3+j] = (unsigned char)/*(255 * std::max(0.f, std::min(1.f,*/ pixels[i][j];//)));
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

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <vector>
#include "data.hpp"
#include "model.h"

#define WIDTH 800
#define HEIGHT 800
#define PROF 100


Point dir_lum = {0, 0, -1};
Color col_diablo;

using namespace std;

void segment(Pointi A, Pointi B, float coulA, float coulB, std::vector<Color> &pixels, std::vector<float> &zbuffer)
{
	float i, z, gamma;
	int index;
	Color c;
	
	if (A.x > B.x) { 
		std::swap(A, B);  
		std::swap(coulA, coulB); 
	}
	
	for(int col = A.x; col <= B.x; col++) {
		gamma = B.x==A.x ? 1. : (float)(col - A.x) / (float)(B.x - A.x);
		z = A.z + gamma*(B.z - A.z);
		i = (1-gamma)*coulA + gamma*coulB; 
		if (i > 0) {
			c = col_diablo*i; 
			index = A.y*WIDTH + col;
			if (zbuffer[index] < z) {
				pixels[index] = c;
				zbuffer[index] = z;
			}
		}
	}
}


void trianglePlein(std::vector<Pointi> pts, std::vector<float> ins, std::vector<Color> &pixels, std::vector<float> &zbuffer) {

	Pointi A = pts[0], B = pts[1], C = pts[2];
	float inA = ins[0], inB = ins[1], inC = ins[2];
	if (A.y > B.y) { std::swap(A,B); std::swap(inA,inB); }
	if (B.y > C.y) { std::swap(B,C); std::swap(inB,inC); }
	if (A.y > B.y) { std::swap(A,B); std::swap(inA,inB); }

	Pointi AB = {B.x - A.x, B.y - A.y, B.z - A.z}; 
	Pointi AC = {C.x - A.x, C.y - A.y, C.z - A.z};
	Pointi BC = {C.x - B.x, C.y - B.y, C.z - B.z};
	Pointi pAB, pAC, pBC;

	float alpha, beta, gamma, inG, inD;
	int index;
	if (AB.y > 0) {
		for (int l=0; l <= AB.y; l++) {
			alpha = (float)l / (float)AB.y;
			beta = (float)l / (float)AC.y;
			
			pAB.x = A.x + AB.x * l / AB.y;
			pAB.y = A.y + l;
			pAB.z = A.z + alpha * (B.z - A.z);

			pAC.x = A.x + AC.x * l / AC.y;
			pAC.y = pAB.y;
			pAC.z = A.z + beta * (C.z - A.z);

			inG = (1-alpha)*inA + alpha*inB;
			inD = (1-beta)*inA + beta*inC;
			
			segment(pAB, pAC, inG, inD, pixels, zbuffer);
		}
	} else segment(A, B, inA, inB, pixels, zbuffer);
	
	if (BC.y > 0) {
		for (int l = AB.y; l <= AC.y; l++) {
			alpha = (float)(l - AB.y) / (float)BC.y;
			beta = (float)l / (float)AC.y;
			
			pBC.x = B.x + BC.x * (l-AB.y) / BC.y;
			pBC.y = A.y + l;
			pBC.z = B.z + alpha * (C.z - B.z);
			
			pAC.x = A.x + AC.x * l / AC.y;
			pAC.y = pBC.y;
			pAC.z = A.z + beta * (C.z - A.z);
			
			inG = (1-alpha)*inB + alpha*inC;
			inD = (1-beta)*inA + beta*inC;

			segment(pBC, pAC, inG, inD, pixels, zbuffer);
		}
	} else segment(B, C, inB, inC, pixels, zbuffer);
}


void writeppm(unsigned char* image, const char* filename){
    int i, j;
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


Pointi pointToPointi(Point p, float d) {
	Pointi pi;
	float h = HEIGHT / 2.;
    float w = WIDTH / 2.;
	float pr = PROF /2.;
	
	if (p.x < 0) pi.x = (int) (w - abs(p.x*w));
	else pi.x = (int) (w + p.x*w);
        
	if (p.y < 0) pi.y = (int) (h + abs(p.y*h));
	else pi.y = (int) (h - p.y*h);
	
	if (p.z < 0) pi.z = (int) (pr - abs(p.z*pr));
	else pi.z = (int) (pr + p.z*pr);
	
	pi.x += d;
	
	return pi;
}


void render(Model m) {
	int NB_PT = 3;
	int TAILLE = WIDTH*HEIGHT;
    
    Color blanc = {255,255 ,255, 0};
    Color noir = {0, 0, 0, 0};
	col_diablo = blanc;
	
	std::vector<Color> pixels(TAILLE),pixelsR(TAILLE),pixelsB(TAILLE);
	std::vector<float> zBufferR(TAILLE), zBufferB(TAILLE);
	
	std::vector<float> intensites(NB_PT);
	std::vector<Point> points_modele(NB_PT);
	std::vector<Pointi> points_ecranR(NB_PT), points_ecranB(NB_PT);
	
    for (size_t i = 0; i < TAILLE; ++i) {
        pixelsR[i] = noir;
        pixelsB[i] = noir;
		zBufferR[i] = -TAILLE*1000;
        zBufferB[i] = -TAILLE*1000;
    }
	
	int delta = 10; 	
	for (int nface = 0; nface < m.nbfaces(); nface++) {
		for (int i = 0; i < NB_PT; i++) {
			points_modele[i] = m.point(m.vert(nface, i));
			points_ecranR[i] = pointToPointi(points_modele[i], - delta);
			points_ecranB[i] = pointToPointi(points_modele[i], delta);
			intensites[i] = -(m.normal(nface, i) * dir_lum);
		}
		
		trianglePlein(points_ecranR, intensites, pixelsR, zBufferR);
		trianglePlein(points_ecranB, intensites, pixelsB, zBufferB);
	}
    
	for (size_t i = 0; i < WIDTH*HEIGHT; i++) {
        Color coulfinale = {(pixelsR[i].r+pixelsR[i].g+pixelsR[i].b)/3, 0, (pixelsB[i].r+pixelsB[i].g+pixelsB[i].b)/3, 0};
        pixels[i] = coulfinale;
    }
    
    std::vector<unsigned char> pixmap(WIDTH*HEIGHT*3);
    for (size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        for (size_t j = 0; j<3; j++) {
            pixmap[i*3+j] = (unsigned char) pixels[i][j];
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

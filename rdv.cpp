#include <iostream>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <cstring>
#include "data.hpp"
#include "model.h"
#include "matrix.h"
#include "tgaimage.h"

#define WIDTH 800
#define HEIGHT 800
#define PROF 100
#define DELTA 15


Point dir_lum = {0, 0, -1};
Point cam(0,0,3);
Color col_diablo;
TGAImage texture;
bool isDiffuse;


using namespace std;

Pointi mat2pt(Matrix m) {
    Pointi p = {(int)(m[0][0]/m[3][0]), (int)(m[1][0]/m[3][0]), (int)(m[2][0]/m[3][0])};
	return p;
}

Matrix pt2mat(Point v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = PROF/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = PROF/2.f;
    return m;
}

Matrix lookat(Point eye, Point center, Point up) {
    Point z = (eye-center).normalize();
    Point x = cross(up,z).normalize();
    Point y = cross(z,x).normalize();
    Matrix Minv = Matrix::identity(4);
    Matrix Tr   = Matrix::identity(4);
    for (int i=0; i<3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = -center[i];
    }
    return Minv*Tr;
}

void segment(Pointi A, Pointi B, float inA, float inB, Point2d ctA, Point2d ctB, std::vector<Color> &pixels, std::vector<float> &zbuffer)
{
	float i, z, gamma;
	int index;
	Point2d ctP;
	Color c;
	TGAColor tgac;
	
	if (A.x > B.x) { 
		std::swap(A, B);  
		std::swap(inA, inB); 
		std::swap(ctA, ctB); 
	}
	
	for(int col = A.x; col <= B.x; col++) {
		gamma = B.x==A.x ? 1. : (float)(col - A.x) / (float)(B.x - A.x);
		z = A.z + gamma*(B.z - A.z);
		i = (1-gamma)*inA + gamma*inB; 
		ctP = (1-gamma)*ctA + gamma*ctB; 
		tgac = texture.get(ctP.x*texture.get_width(),texture.get_height() - (ctP.y*texture.get_height()));
		if (i > 0) {
			if (isDiffuse) c = {(float)tgac.bgra[2], (float)tgac.bgra[1], (float)tgac.bgra[0], 0};
			else c = col_diablo;
			c = c*i; 
			index = A.y*WIDTH + col;
			if (zbuffer[index] < z) {
				pixels[(HEIGHT-A.y)*WIDTH + col] = c;
				zbuffer[index] = z;
			}
		}
	}
}


void trianglePlein(std::vector<Pointi> pts, std::vector<Point2d> text,std::vector<float> ins, std::vector<Color> &pixels, std::vector<float> &zbuffer) {

	Pointi A = pts[0], B = pts[1], C = pts[2];
	float inA = ins[0], inB = ins[1], inC = ins[2];
	Point2d ctA = text[0], ctB = text[1], ctC = text[2];
	if (A.y > B.y) { std::swap(A,B); std::swap(inA,inB); std::swap(ctA,ctB); }
	if (B.y > C.y) { std::swap(B,C); std::swap(inB,inC); std::swap(ctB,ctC); }
	if (A.y > B.y) { std::swap(A,B); std::swap(inA,inB); std::swap(ctA,ctB); }

	Pointi AB = {B.x - A.x, B.y - A.y, B.z - A.z}; 
	Pointi AC = {C.x - A.x, C.y - A.y, C.z - A.z};
	Pointi BC = {C.x - B.x, C.y - B.y, C.z - B.z};
	Pointi pAB, pAC, pBC;

	float alpha, beta, gamma, inG, inD;
	int index;
	Point2d ctG, ctD;
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
			ctG = (1-alpha)*ctA + alpha*ctB;
			ctD = (1-beta)*ctA + beta*ctC;
			
			segment(pAB, pAC, inG, inD, ctG, ctD, pixels, zbuffer);
		}
	} else segment(A, B, inA, inB, ctA, ctB, pixels, zbuffer);
	
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
			ctG = (1-alpha)*ctB + alpha*ctC;
			ctD = (1-beta)*ctA + beta*ctC;

			segment(pBC, pAC, inG, inD, ctG, ctD, pixels, zbuffer);
		}
	} else segment(B, C, inB, inC, ctB, ctC, pixels, zbuffer);
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


std::vector<Color> draw(Point camera, Model m) {
	int NB_PT = 3;
	int TAILLE = WIDTH*HEIGHT + DELTA;
	Color noir = {0, 0, 0, 0};
	Matrix proj = Matrix::identity(4);
	Matrix view = viewport(WIDTH/8, HEIGHT/8, WIDTH*3/4, HEIGHT*3/4);
	proj[3][2] = -1.f/camera.z;
	
	Point eye = {camera.x, camera.y, 0};
	Point center = {0, 0, 0};
	Point up = {camera.x,camera.y+3,camera.z};
	Matrix look = lookat(camera, center, up);
	
	std::vector<Color> pixels(TAILLE);
	std::vector<float> zBuffer(TAILLE);
	
	std::vector<float> intensites(NB_PT);
	std::vector<Point> points_modele(NB_PT);
	std::vector<Pointi> points_ecran(NB_PT);
	std::vector<Point2d> coord_text(NB_PT);
	
    for (size_t i = 0; i < TAILLE; ++i) {
        pixels[i] = noir;
		zBuffer[i] = -TAILLE*1000;
    }
		
	for (int nface = 0; nface < m.nbfaces(); nface++) {
		for (int i = 0; i < NB_PT; i++) {
			points_modele[i] = m.point(m.vert(nface, i));
			points_ecran[i] = mat2pt(view*proj*look*pt2mat(points_modele[i]));
			intensites[i] = -(m.normal(nface, i) * dir_lum);
			coord_text[i] = m.uv(nface, i);
		}
		trianglePlein(points_ecran, coord_text, intensites, pixels, zBuffer);
	}
	
	return pixels;
}


void render(Model m) {
	int TAILLE = WIDTH*HEIGHT;
	float dcam = .1;
   
	col_diablo = {255,255 ,255, 0};
	
	Point camR = {cam.x + (dcam/2.f), cam.y, cam.z};
	Point camB = {cam.x - (dcam/2.f), cam.y, cam.z};
	
	std::vector<Color> pixelsR = draw(camR, m);
	std::vector<Color> pixelsB = draw(camB, m);

	std::vector<unsigned char> pixmap(WIDTH*HEIGHT*3);
    for (size_t j = 0; j<HEIGHT; j++) {
        for (size_t i = 0; i<WIDTH; i++) {
            Color R = pixelsR[i+DELTA+j*WIDTH];
            Color B = pixelsB[i+j*WIDTH];
			Color finale = {(R.r+R.g+R.b)/3.f, 0, (B.r+B.g+B.b)/3.f, 0};
			
			for (size_t k = 0; k<3; k++) {
				pixmap[(j*WIDTH+i)*3+k] = (unsigned char) finale[k];
			}
        }
    }
	
    writeppm(pixmap.data(), "anaglyphe.ppm");
}


int main(int argc, char** argv) {
	string name = "../lib/obj/diablo3_pose/diablo3_pose.obj";
	string texture_name = "../lib/obj/diablo3_pose/diablo3_pose_diffuse.tga";
	if (argc >= 2) {
		name = argv[1];		
		texture_name = name.substr(0, name.size() - strlen(".obj"));
		texture_name = texture_name + "_diffuse.tga";
	}
    Model model(name.c_str());
	isDiffuse = texture.read_tga_file(texture_name.c_str());
    cout << "Number of points in the model: " << model.nbvertex() << " Number of faces in the model " << model.nbfaces() << endl;
    render(model);
    return 0;
}

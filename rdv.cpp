#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include "data.hpp"
#include "model.h"

#define WIDTH 800
#define HEIGHT 800
#define PROF 100


Point dir_lum = {0, 0, -1};

using namespace std;

void segment(Pointi A, Pointi B, Color coul, std::vector<Color> &pixels, std::vector<float> &zbuffer)
{
	//La distance de la colonne entre les deuc points
	int dcol = B.x - A.x;
	//Le point de départ
	int col = A.x;
	int lig = A.y;
	//le sens de parcours
	int sensCol = 1;

	//si le parcours est de droite vers la gauche
	if (dcol < 0) {
		//le sens de parcours devient négatif
		sensCol = -1;
	}


	float z = A.z;
	double delta;
	int index;

	while (col != B.x + sensCol) {
		index = lig*WIDTH + col;
		if (zbuffer[index] < z) {
			pixels[index] = coul;
			zbuffer[index] = z;
		}
		col = col + sensCol;
		delta = (double)(col - A.x) / (double)(B.x - A.x);
		 
		z = (1-delta)*A.z + delta*B.z;
	}
}

void triangle(Pointi A, Pointi B, Pointi C, Color c, std::vector<Color> &pixels, std::vector<float> &zbuffer) {
	segment(A, B, c, pixels, zbuffer);
	segment(B, C, c, pixels, zbuffer);
	segment(A, C, c, pixels, zbuffer);
}


void trianglePlein(Pointi A, Pointi B, Pointi C, Color coul, std::vector<Color> &pixels, std::vector<float> &zbuffer) {
	
	//Calcul de la normal d'un triangle vecteur(AB) * vecteur(BC)
	//Calcul du vecteur AB -> xB - xA ; yB - yA
	
	Pointi tmp;

	if (A.y > B.y) {
		tmp = A; A = B; B = tmp;
	}
	if (B.y > C.y) {
		tmp = C; C = B; B = tmp;
	}
	if (A.y > B.y) {
		tmp = A; A = B; B = tmp;
	}

	Pointi AB = {B.x - A.x, B.y - A.y, B.z - A.z}; //vecteur AB
	Pointi AC = {C.x - A.x, C.y - A.y, C.z - A.z};
	Pointi BC = {C.x - B.x, C.y - B.y, C.z - B.z};

	Pointi pAB ;
	Pointi pAC ;

	double alpha, beta;
	if (AB.y > 0) {
		for (int l=0; l <= AB.y; l++) {
			pAB.x = A.x + AB.x * l / AB.y;
			pAB.y = A.y + l;
			// pAB.z = A.z;
			pAC.x = A.x + AC.x * l / AC.y;
			pAC.y = pAB.y;
			// pAC.z = A.z + AC.z;
			
			alpha = (double)(pAB.x - A.x) / (double)AB.x;
			beta = (double)(pAC.x - A.x) / (double)AC.x;
			
			pAB.z = (1-alpha)*A.z + alpha * B.z;
			pAC.z = (1-beta)*A.z + beta * C.z;
			
			segment(pAB, pAC, coul, pixels, zbuffer);
		}
	} else {
		segment(A, B, coul, pixels, zbuffer);
	}

	Pointi pBC;
	if (BC.y > 0) {
		for (int l = AB.y; l <= AC.y; l++) {
			pBC.x = B.x + BC.x * (l-AB.y) / BC.y;
			pBC.y = A.y + l;
			// pBC.z = B.z;
			pAC.x = A.x + AC.x * l / AC.y;
			pAC.y = pBC.y;
			// pAC.z = B.z + AC.z;
			
			alpha = (double)(pBC.x - B.x) / (double)BC.x;
			beta = (double)(pAC.x - A.x) / (double)AC.x;
		
			
			pBC.z = (1-alpha)*B.z + alpha * C.z;
			pAC.z = (1-beta)*A.z + beta * C.z;
			
			segment(pBC, pAC, coul, pixels, zbuffer);
		}
	} else {
		segment(B, C, coul, pixels, zbuffer);
	}
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

Pointi pointToPointi(Point p) {
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
	
	
	return pi;
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
    
    Color black = {0, 0, 0, 0};
    Color white = {255,255 ,255, 0};
    Color vert = {0,255 ,0, 0};
    Color bleu = {0,0 ,255, 0};
    Color rouge = {255,0, 0, 0};
	
	std::vector<Color> pixels(WIDTH*HEIGHT);
	std::vector<Color> pixelsR(WIDTH*HEIGHT);
	std::vector<Color> pixelsB(WIDTH*HEIGHT);
    for (size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        //pixels[i] = black;
        pixelsR[i] = black;
        pixelsB[i] = black;
    }
	
	std::vector<float> zBuffer(WIDTH*HEIGHT);
	std::vector<float> zBufferR(WIDTH*HEIGHT);
	std::vector<float> zBufferB(WIDTH*HEIGHT);
	for(size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        //zBuffer[i] = -WIDTH*HEIGHT*1000;
        zBufferR[i] = -WIDTH*HEIGHT*1000;
        zBufferB[i] = -WIDTH*HEIGHT*1000;
    }
	
	/*Pointi p1 = {50,100,10};
    Pointi p2 = {100,300,10};
    Pointi p3 = {200,200,20};

    Pointi p4 = {100,50,15};
    Pointi p5 = {150,300,15};
    Pointi p6 = {200,100,15};
	
	Pointi p7 = {0,50,0};
    Pointi p8 = {50,0,0};
    Pointi p9 = {300,300,60};
	
	trianglePlein(p1, p2, p3, rouge, pixels, zBuffer);
	trianglePlein(p4, p5, p6, vert, pixels, zBuffer);
	trianglePlein(p7, p8, p9, bleu, pixels, zBuffer);*/
	
	int delta = 20; 	
	for (int nface = 0; nface < m.nbfaces(); nface++) {
		Point p1 = m.point(m.vert(nface, 0));
		Point p2 = m.point(m.vert(nface, 1));
		Point p3 = m.point(m.vert(nface, 2));
		
		Pointi p1i = pointToPointi(p1, -delta);
		Pointi p2i = pointToPointi(p2, -delta);
		Pointi p3i = pointToPointi(p3, -delta);
		
		/*Pointi p1i = pointToPointi(p1);
		Pointi p2i = pointToPointi(p2);
		Pointi p3i = pointToPointi(p3);*/
		
		Point p12 = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z}; //vecteur AB
		Point p13 = {p3.x - p1.x, p3.y - p1.y, p3.z - p1.z};
		Point p23 = {p3.x - p2.x, p3.y - p2.y, p3.z - p2.z};
		
		Point normale = (p3 - p1)^(p2 - p1);
		normale.normalize();
		float intensite = normale*dir_lum;
		// Color coul = white;
		Color coulR = rouge;
		Color coulB = bleu;
		if (intensite > 0) {
			// coul = {intensite*white.r, intensite*white.g, intensite*white.b, 0};
			coulR = {intensite*rouge.r, intensite*rouge.g, intensite*rouge.b, 0};
			coulB = {intensite*bleu.r, intensite*bleu.g, intensite*bleu.b, 0};
		}
	
		// trianglePlein(p1i, p2i, p3i, coul, pixels, zBuffer);
		trianglePlein(p1i, p2i, p3i, coulR, pixelsR, zBufferR);
		
		p1i = pointToPointi(p1, delta);
		p2i = pointToPointi(p2, delta);
		p3i = pointToPointi(p3, delta);
		
		trianglePlein(p1i, p2i, p3i, coulB, pixelsB, zBufferB);
	}
    
	for (size_t i = 0; i < WIDTH*HEIGHT; i++) {
        Color coulfinale = {pixelsR[i].r, 0, pixelsB[i].b, 0};
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

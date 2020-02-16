#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <limits>
#include "data.hpp"
#include "model.h"

#define WIDTH 800
#define HEIGHT 800


Point dir_lum = {0, 800, -1};

using namespace std;

void segment(Pointi A, Pointi B, Color coul, std::vector<Color> &pixels)
{
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

void triangle(Pointi A, Pointi B, Pointi C, Color c, std::vector<Color> &pixels) {
	segment(A, B, c, pixels);
	segment(B, C, c, pixels);
	segment(A, C, c, pixels);
}

void trianglePlein(Pointi A, Pointi B, Pointi C, Color c, std::vector<Color> &pixels) {
	
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
	
	Point normale = {(float)AB.x*BC.x, (float)AB.y*BC.y, (float)AB.z*BC.z};
	normale.normalize();
	float intensite = normale*dir_lum;
	Color coul = c;
	if (intensite > 0) {
		coul = {intensite*c.r, intensite*c.g, intensite*c.b, 0};
	}

	Pointi pAB ;
	Pointi pAC ;

	if (AB.y > 0) {
		for (int l=0; l < AB.y; l++) {
			pAB.x = A.x + AB.x * l / AB.y;
			pAB.y = A.y + l;
			pAC.x = A.x + AC.x * l / AC.y;
			pAC.y = pAB.y;

			segment(pAB, pAC, coul, pixels);
		}
	} else {
		segment(A, B, coul, pixels);
	}

	Pointi pBC;
	if (BC.y > 0) {
		for (int l = AB.y; l < AC.y; l++) {
			pBC.x = B.x + BC.x * (l-AB.y) / BC.y;
			pBC.y = A.y + l;
			pAC.x = A.x + AC.x * l / AC.y;
			pAC.y = pBC.y;
			
			segment(pBC, pAC, coul, pixels);
		}
	} else {
		segment(B, C, coul, pixels);
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
	if (p.x < 0) pi.x = (int) (w - abs(p.x*w));
	else pi.x = (int) (w + p.x*w);
        
	if (p.y < 0) pi.y = (int) (h + abs(p.y*h));
	else pi.y = (int) (h - p.y*h);
	
	return pi;
}

// TRP
Point world2screen(Point v) {
    return Point(int((v.x+1.)*WIDTH/2.+.5), int((v.y+1.)*HEIGHT/2.+.5), v.z);
}
// TRP
Point barycentric(Point A, Point B, Point C, Point P) {
    Point s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    Point u = cross(s[0], s[1]);
    if (std::abs(u[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return Point(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Point(-1,1,1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

void render(Model m) {
    int taillePixels = WIDTH*HEIGHT*3;
    std::vector<Color> pixels(taillePixels);
    Color black = {0, 0, 0, 0};
    Color white = {255,255 ,255, 0};
    Color vert = {0,255 ,0, 0};
    Color bleu = {0,0 ,255, 0};
    Color rouge = {255,0, 0, 0};
    for (size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        pixels[i] = black;
    }

	/*Pointi p1 = {300,100,10};
	Pointi p2 = {50,300,0};	
	Pointi p3 = {200,400,10};
	Pointi p4 = {400,200,0};
	Pointi p5 = {600,200,0};
	Pointi p6 = {300,400,0};
	Pointi p7 = {400,220,0};
	Pointi p8 = {400,490,0};
	Pointi p9 = {300,650,0};

	trianglePlein(p1, p2, p3, rouge, pixels);
	trianglePlein(p4, p5, p6, vert, pixels);
	trianglePlein(p7, p8, p9, bleu, pixels);*/


    int *zbuffer = new int[WIDTH*HEIGHT];
    for (int i=WIDTH*HEIGHT; i--; zbuffer[i] = -std::numeric_limits<int>::max());

    // Depart
	/*for (int nface = 0; nface < m.nbfaces(); nface++) {
		Point p1 = m.point(m.vert(nface, 0));
		Point p2 = m.point(m.vert(nface, 1));
		Point p3 = m.point(m.vert(nface, 2));
		
		Pointi p1i = pointToPointi(p1);
		Pointi p2i = pointToPointi(p2);
		Pointi p3i = pointToPointi(p3);
		
		trianglePlein(p1i, p2i, p3i, white, pixels);
	}*/

	// Moi
    /*for(int i = 0; i < taillePixels ; i++){
        Color p = pixels[i];

        bool cache = p.z <= zbuffer[int(p.x+p.y*WIDTH)];
        if(!cache){
            zbuffer[int(p.x+p.y*WIDTH)] = p.z;
            pixels[int(p.x+p.y*WIDTH)] = white;
        }
    }*/

    // TRP

    for (int k=0; k<m.nbfaces(); k++) {
        Point pts[3];
        for (int i = 0; i < 3; i++){
            pts[i] = world2screen(m.point(m.vert(k,i)));
        }
        Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
        Vec2f clamp(WIDTH - 1, HEIGHT - 1);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 2; j++) {
                bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
                bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
            }
        }
        Point P;
        for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
            for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
                Point bc_screen = barycentric(pts[0], pts[1], pts[2], P);
                if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
                P.z = 0;
                for (int i = 0; i < 3; i++) P.z += pts[i][2] * bc_screen[i];
                if (zbuffer[int(P.x + P.y * WIDTH)] < P.z) {
                    zbuffer[int(P.x + P.y * WIDTH)] = P.z;
                    pixels[int(P.x+P.y*WIDTH)] = white;
                }
            }
        }
    }

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

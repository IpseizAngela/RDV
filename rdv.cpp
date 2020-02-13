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


void trianglePlein(Pointi A, Pointi B, Pointi C, Color coul, std::vector<Color> &pixels) {
  Pointi tmp;
  /*Couleur coulT;
  Couleur coulA = couls[0];
  Couleur coulB = couls[1];
  Couleur coulC = couls[2];*/

  if (A.y > B.y) {
    tmp = A;
    A = B;
    B = tmp;
    /*coulT = coulA;
    coulA = coulB;
    coulB = coulT;*/
  }
  if (B.y > C.y) {
    tmp = C;
    C = B;
    B = tmp;
    /*coulT = coulC;
    coulC = coulB;
    coulB = coulT;*/
  }
  if (A.y > B.y) {
    tmp = A;
    A = B;
    B = tmp;
    /*coulT = coulA;
    coulA = coulB;
    coulB = coulT;*/
  }

  Pointi AB = {B.x - A.x, B.y - A.y, 0};
  Pointi AC = {C.x - A.x, C.y - A.y, 0};
  Pointi BC = {C.x - B.x, C.y - B.y, 0};

  Pointi pAB ;//= PointImage();
  Pointi pAC ;//= PointImage();

  /*double alpha;
  double beta;
  Couleur coulG;
  Couleur coulD;*/

  if (AB.y > 0) {
    for (int l=0; l < AB.y; l++) {
      pAB.x = A.x + AB.x * l / AB.y;
      pAB.y = A.y + l;
      pAC.x = A.x + AC.x * l / AC.y;
      pAC.y = pAB.y;

      /*alpha = (double)(pAB.lig - A.lig) / (double)AB.lig;
      beta = (double)(pAC.lig - A.lig) / (double)AC.lig;

      coulG.R = (1-alpha)*coulA.R + alpha * coulB.R;
      coulG.V = (1-alpha)*coulA.V + alpha * coulB.V;
      coulG.B = (1-alpha)*coulA.B + alpha * coulB.B;
      coulG.A = (1-alpha)*coulA.A + alpha * coulB.A;

      coulD.R = (1-beta)*coulA.R + beta * coulC.R;
      coulD.V = (1-beta)*coulA.V + beta * coulC.V;
      coulD.B = (1-beta)*coulA.B + beta * coulC.B;
      coulD.A = (1-beta)*coulA.A + beta * coulC.A;*/

      //Segment(pAB, pAC, coulG, coulD,image);
      segment(pAB, pAC, coul, pixels);
    }
  } else {
    //Segment(A, B, coulA, coulB, image);
    segment(A, B, coul, pixels);
  }

  Pointi pBC ;//= PointImage();
    if (BC.y > 0) {
		for (int l = AB.y; l < AC.y; l++) {
			pBC.x = B.x + BC.x * (l-AB.y) / BC.y;
			pBC.y = A.y + l;
			pAC.x = A.x + AC.x * l / AC.y;
			pAC.y = pBC.y;

			/*alpha = (double)(pBC.lig - B.lig) / (double)BC.lig;
			beta = (double)(pAC.lig - A.lig)/ (double)AC.lig;

			coulG.R = (1-alpha)*coulB.R + alpha * coulC.R;
			coulG.V = (1-alpha)*coulB.V + alpha * coulC.V;
			coulG.B = (1-alpha)*coulB.B + alpha * coulC.B;
			coulG.A = (1-alpha)*coulB.A + alpha * coulC.A;

			coulD.R = (1-beta)*coulA.R + beta * coulC.R;
			coulD.V = (1-beta)*coulA.V + beta * coulC.V;
			coulD.B = (1-beta)*coulA.B + beta * coulC.B;
			coulD.A = (1-beta)*coulA.A + beta * coulC.A;*/

			segment(pBC, pAC, coul, pixels);
		//Segment(pBC, pAC, coulG, coulD, image);
		}
	} else {
		//Segment(B, C, coulB, coulC, image);
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


void render(Model m) {
    std::vector<Color> pixels(WIDTH*HEIGHT*3);
    Color black = {0, 0, 0, 0};
    Color white = {255,255 ,255, 0};
    Color vert = {0,255 ,0, 0};
    Color bleu = {0,0 ,255, 0};
    Color rouge = {255,0, 0, 0};
    for (size_t i = 0; i < HEIGHT*WIDTH; ++i) {
        pixels[i] = black;
    }
	
	/*for (int i = 0; i < m.nbvertex(); i++) {
		Point p = m.point(i);
		Pointi pi = pointToPointi(p);
		pixels[pi.y*WIDTH + pi.x] = white;
	}*/
    
	Pointi p1 = {300,100,10};
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
	trianglePlein(p7, p8, p9, bleu, pixels);
	
	/*for (int nface = 0; nface < m.nbfaces(); nface++) {
		Point p1 = m.point(m.vert(nface, 0));
		Point p2 = m.point(m.vert(nface, 1));
		Point p3 = m.point(m.vert(nface, 2));
		
		Pointi p1i = pointToPointi(p1);
		Pointi p2i = pointToPointi(p2);
		Pointi p3i = pointToPointi(p3);
		
		triangle(p1i, p2i, p3i, white, pixels);
	}*/
    
    
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

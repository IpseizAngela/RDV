//
// Created by Dylanlicho on 30/01/2020.
//

#ifndef PROJETRDV_MODEL_H
#define PROJETRDV_MODEL_H

#include <vector>
#include "data.hpp"

class Model {
private:
    std::vector<Point> vertex;
    std::vector<std::vector<Pointi> > faces;
    std::vector<Point> norms;
    std::vector<Point2d> texcoords;

public:
    Model(const char *filename);
    int nbvertex();
    int nbfaces();
    Point &point(int i);
    int vert(int fi, int li);
    Point normal(int fi, int li);
	Point2d uv(int fi, int li);
};


#endif //PROJETRDV_MODEL_H

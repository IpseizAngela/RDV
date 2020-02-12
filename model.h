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
    std::vector<std::vector<Pointi> > faces; // attention, this Vec3i means vertex/uv/normal ids

public:
    Model(const char *filename);
    int nbvertex();
    int nbfaces();
    Point &point(int i);
	std::vector<Pointi> face(int i);
    Pointi vert(int fi, int li);
};


#endif //PROJETRDV_MODEL_H

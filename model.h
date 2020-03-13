#ifndef PROJETRDV_MODEL_H
#define PROJETRDV_MODEL_H

#include <vector>
#include "data.hpp"

class Model {
private:
    std::vector<Point> vertex;
    std::vector<std::vector<Pointi> > faces;
    std::vector<Point> norms;

public:
    Model(const char *filename);
    int nbvertex();
    int nbfaces();
    Point &point(int i);
    int vert(int numFace, int numPoint);
    Point normal(int numFace, int numPoint);

};


#endif //PROJETRDV_MODEL_H

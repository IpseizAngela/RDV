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
public:
    Model(const char *filename);
    int nbvertex();
    Point &point(int i);
};


#endif //PROJETRDV_MODEL_H

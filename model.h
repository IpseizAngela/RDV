//
// Created by Dylanlicho on 30/01/2020.
//

#ifndef PROJETRDV_MODEL_H
#define PROJETRDV_MODEL_H

#include <vector>
#include "data.hpp"

class Model {
private:
    std::vector<Color> vertex;
public:
    Model(const char *filename);
    int nbvertex();
    Color &point(int i);
};


#endif //PROJETRDV_MODEL_H

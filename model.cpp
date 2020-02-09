//
// Created by Dylanlicho on 30/01/2020.
//
#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include "model.h"

Model::Model(const char *filename) : vertex(){
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if(in.fail()){
        std::cerr << "Failled to open" << filename << std::endl;
        return;
    }
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream cur(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            cur >> trash;
            Point v;
            for (int i=0;i<3;i++) cur >> v.dim[i];
            vertex.push_back(v);
        }
    }
}

int Model::nbvertex() {
    return (int)vertex.size();
}

Point &Model::point(int i) {
    assert(i>=0 && i<nbvertex());
    return vertex[i];
}
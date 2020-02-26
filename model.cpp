//
// Created by Dylanlicho on 30/01/2020.
//
#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include "model.h"

Model::Model(const char *filename) : vertex(), faces(){
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
            for (int i=0;i<3;i++) cur >> v[i];
            vertex.push_back(v);
        }else if (!line.compare(0, 2, "f ")) {
            std::vector<Pointi> f;
            Pointi tmp;
            cur >> trash;
            while (cur >> tmp.x >> trash >> tmp.y >> trash >> tmp.z) {
                //for (int i=0; i<3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                tmp.x--;
                tmp.y--;
                tmp.z--;
                f.push_back(tmp);
            }
            assert(3==f.size());
            faces.push_back(f);
        }
    }
}

int Model::nbvertex() {
    return (int)vertex.size();
}

int Model::nbfaces() {
    return (int)faces.size();
}

Point &Model::point(int i) {
    assert(i>=0 && i<nbvertex());
    return vertex[i];
}

std::vector<Pointi> Model::face(int i) {
	assert(i>=0 && i<nbvertex());
    return faces[i];
}

int Model::vert(int fi, int li) {
    assert(fi>=0 && fi<nbfaces() && li>=0 && li<3);
    return faces[fi][li].x;
}
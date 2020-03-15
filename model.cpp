
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
        }else if (!line.compare(0, 3, "vn ")) {
            cur >> trash >> trash;
            Point n;
            for (int i=0;i<3;i++) cur >> n[i];
            norms.push_back(n);
		}else if (!line.compare(0, 3, "vt ")) {
            cur >> trash >> trash;
            Vec2f uv;
            for (int i=0;i<2;i++) cur >> uv[i];
            texcoords.push_back(uv);
        }else if (!line.compare(0, 2, "f ")) {
            std::vector<Pointi> f;
            Pointi tmp;
            cur >> trash;
            while (cur >> tmp.x >> trash >> tmp.y >> trash >> tmp.z) {
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

int Model::vert(int numFace, int numPoint) {
    assert(numFace >= 0 && numFace < nbfaces() && numPoint >= 0 && numPoint < 3);
    return faces[numFace][numPoint].x;
}

Vec2f Model::uv(int numFace, int numPoint) {
    assert(numFace>=0 && numFace<nbfaces() && numPoint>=0 && numPoint<3);
    return texcoords[faces[numFace][numPoint].y];
}

Point Model::normal(int numFace, int numPoint) {
    assert(numFace >= 0 && numFace < nbfaces() && numPoint >= 0 && numPoint < 3);
    return norms[faces[numFace][numPoint].z];
}

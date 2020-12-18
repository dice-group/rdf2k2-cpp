//
// Created by minimal on 12/18/20.
//

#ifndef RDF2K2_CPP_PATH_H
#define RDF2K2_CPP_PATH_H

#include <vector>
#include "Point.h"

class Path {
public:
    Path(u_int32_t h);

    std::vector<std::vector<u_char>> &getPaths();
    bool isEmpty(u_int32_t i);
    void pop(u_int32_t i);
    Point calculatePoint();
    bool hasLast();
    void add(u_int32_t i, u_char newpath);
    void addLast();

private:
    std::vector<std::vector<u_char>> paths;
    u_char last;
    bool hasLastChar;
};


#endif //RDF2K2_CPP_PATH_H

//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_LABLEDMATRIX_H
#define RDF2K2_CPP_LABLEDMATRIX_H

#include <vector>
#include "Point.h"

class LabledMatrix {
public:
    LabledMatrix();
    LabledMatrix(long pID);
    LabledMatrix(long pID, long size);
    void set(long sID, long oID);
    std::vector<Point> getPoints();
    double getH();
    long getLabel();
    void clear();
private:
    long pID;
    std::vector<Point> matrix;
    double h=-1;
    long pointCount=0;
};


#endif //RDF2K2_CPP_LABLEDMATRIX_H

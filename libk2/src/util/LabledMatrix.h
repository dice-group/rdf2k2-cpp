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
    const std::vector<Point> &getPoints();
    double getH();
    long getLabel();
    void clear();

    std::vector<Point> matrix;

    void addPoint(Point &p);

private:
    //TODO it so unlikely that pID will get over Int, we can safely use u_int32_t
    long pID;
    double h=-1;
    long pointCount=0;

};


#endif //RDF2K2_CPP_LABLEDMATRIX_H

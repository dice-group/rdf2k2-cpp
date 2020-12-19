//
// Created by minimal on 12/14/20.
//

#include "LabledMatrix.h"
#include <vector>
#include "Point.h"
#include <math.h>

LabledMatrix::LabledMatrix(long pID) {
    this->pID=pID;
}

LabledMatrix::LabledMatrix(long pID, long size) {
    this->pID=pID;
    this->matrix = std::vector<Point>(size);
}

LabledMatrix::LabledMatrix() {}

void LabledMatrix::addPoint(Point &p){
    matrix.push_back(p);
}

void LabledMatrix::set(long sID, long oID) {
    matrix[pointCount++] =Point(sID, oID);
    if(sID>h){
        h = sID;
    }
    if(oID>h){
        h =oID;
    }
}

long LabledMatrix::getLabel() {
    return pID;
}

const  std::vector<Point> &LabledMatrix::getPoints(){
    return this->matrix;
}

double LabledMatrix::getH(){
    return ceil(log2(h+1));
}

void LabledMatrix::clear() {
    this->matrix.clear();

}
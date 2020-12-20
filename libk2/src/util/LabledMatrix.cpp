//
// Created by minimal on 12/14/20.
//

#include "LabledMatrix.h"
#include <vector>
#include "Point.h"
#include <math.h>

k2::LabledMatrix::LabledMatrix(u_int32_t pID) {
    this->pID=pID;
}

k2::LabledMatrix::LabledMatrix(u_int32_t pID, long size) {
    this->pID=pID;
    this->matrix = std::vector<Point>(size);
}

k2::LabledMatrix::LabledMatrix() {}

void k2::LabledMatrix::addPoint(Point &p){
    matrix.push_back(p);
}

void k2::LabledMatrix::set(unsigned long sID, unsigned long oID) {
    matrix[pointCount++] =Point(sID, oID);
    if(sID>h){
        h = sID;
    }
    if(oID>h){
        h =oID;
    }
    if(sID<min){
        min =sID;
    }
    if(oID<min){
        min =oID;
    }
}

u_int32_t k2::LabledMatrix::getLabel() {
    return pID;
}

const  std::vector<k2::Point> &k2::LabledMatrix::getPoints(){
    return this->matrix;
}

double k2::LabledMatrix::getH(){
    return ceil(log2(h+1));
}

void k2::LabledMatrix::clear() {
    this->matrix.clear();

}
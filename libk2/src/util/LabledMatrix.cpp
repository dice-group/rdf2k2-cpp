//
// Created by minimal on 12/14/20.
//

#include "LabledMatrix.h"
#include <vector>
#include "Point.h"
#include <math.h>
using namespace std;

LabledMatrix::LabledMatrix(long pID) {
    this->pID=pID;
    this->matrix = vector<Point>();
}

LabledMatrix::LabledMatrix(long pID, long size) {
    this->pID=pID;
    this->matrix = vector<Point>(size);
}

LabledMatrix::LabledMatrix() {}



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

vector<Point> LabledMatrix::getPoints(){
    return this->matrix;
}

double LabledMatrix::getH(){
    return ceil(log2(h+1));
}

void LabledMatrix::clear() {
    this->matrix.clear();

}
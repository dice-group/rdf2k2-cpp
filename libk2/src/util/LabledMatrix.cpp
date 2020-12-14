//
// Created by minimal on 12/14/20.
//

#include "LabledMatrix.h"
#include <vector>
#include "Point.h"
using namespace std;

LabledMatrix::LabledMatrix(long pID) {
    this->pID=pID;
    this->matrix = vector<Point>();
}

LabledMatrix::LabledMatrix(long pID, long size) {
    this->pID=pID;
    this->matrix = vector<Point>();
}

LabledMatrix::LabledMatrix() {}



void LabledMatrix::set(long sID, long oID) {
    matrix.push_back(Point(sID, oID));
    if(sID>h){
        h = sID;
    }
    if(oID>h){
        h =oID;
    }
}

vector<Point> LabledMatrix::getPoints(){
    return this->matrix;
}

double LabledMatrix::getH(){
    return h;
}

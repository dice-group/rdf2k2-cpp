//
// Created by minimal on 12/14/20.
//

#include "Point.h"

k2::Point::Point(unsigned long sID, unsigned long oID) {
    this->sID=sID;
    this->oID=oID;
}

k2::Point::Point() {}

unsigned long k2::Point::getCol() const {
    return oID;
}

unsigned long k2::Point::getRow() const {
    return sID;
}

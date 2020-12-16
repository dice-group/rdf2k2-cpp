//
// Created by minimal on 12/14/20.
//

#include "Point.h"

Point::Point(long sID, long oID) {
    this->sID=sID;
    this->oID=oID;
}

Point::Point() {}

long Point::getCol() const {
    return oID;
}

long Point::getRow() const {
    return sID;
}

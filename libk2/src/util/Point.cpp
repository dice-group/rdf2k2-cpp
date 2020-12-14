//
// Created by minimal on 12/14/20.
//

#include "Point.h"

Point::Point(long sID, long oID) {
    this->sID=sID;
    this->oID=oID;
}

long Point::getCol() {
    return oID;
}

long Point::getRow() {
    return sID;
}

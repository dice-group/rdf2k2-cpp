//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_POINT_H
#define RDF2K2_CPP_POINT_H


class Point {
public:
    Point(long sID, long oID);
    Point();
    long getCol() const;
    long getRow() const;
private:
    long sID;
    long oID;
};


#endif //RDF2K2_CPP_POINT_H

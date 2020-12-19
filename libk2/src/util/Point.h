//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_POINT_H
#define RDF2K2_CPP_POINT_H

namespace k2 {

    class Point {
    public:
        Point(unsigned long sID, unsigned long oID);

        Point();

        unsigned long getCol() const;

        unsigned long getRow() const;

    private:
        unsigned long sID;
        unsigned long oID;
    };
}

#endif //RDF2K2_CPP_POINT_H

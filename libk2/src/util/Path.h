//
// Created by minimal on 12/18/20.
//

#ifndef RDF2K2_CPP_PATH_H
#define RDF2K2_CPP_PATH_H

#include <vector>
#include "Point.h"

namespace k2 {
    class Path {
    public:
        Path(u_int32_t h);

        bool isEmpty(u_int32_t i) const;

        void pop(u_int32_t i);

        std::vector<Point> calculatePoints();

        Point calculatePoint();

        bool hasLast() const;

        void add(u_int32_t i, u_char newpath);

        void addLast(u_int32_t i);

    private:
        std::vector<std::vector<u_char>> paths;
        u_char last = '\00';
        u_int32_t h;
    };
}

#endif //RDF2K2_CPP_PATH_H

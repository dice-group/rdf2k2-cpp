//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_TRIPLE_H
#define RDF2K2_CPP_TRIPLE_H

#include <cstdlib>

namespace k2 {

    class Triple {
    public:
        Triple(size_t s, size_t p, size_t o);

        Triple();

        size_t get(u_char i);

    private:
        long sID;
        long pID;
        long oID;
    };
}

#endif //RDF2K2_CPP_TRIPLE_H

//
// Created by minimal on 12/19/20.
//

#ifndef RDF2K2_CPP_BYTEBUFFER_H
#define RDF2K2_CPP_BYTEBUFFER_H

#include <istream>

namespace k2 {
    class ByteBuffer {
    public:
        explicit ByteBuffer(std::istream &istream, size_t len, size_t initalSize = 1024);

        u_char next();

        u_char *next(size_t len);

        bool eof();

    private:
        u_char *buffer;
        size_t pointer = 0;
    };

}
#endif //RDF2K2_CPP_BYTEBUFFER_H

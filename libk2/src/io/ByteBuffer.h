//
// Created by minimal on 12/19/20.
//

#ifndef RDF2K2_CPP_BYTEBUFFER_H
#define RDF2K2_CPP_BYTEBUFFER_H

#include <istream>

namespace k2 {
    class ByteBuffer {
    public:
        explicit ByteBuffer(std::istream *istream, size_t len, size_t initalSize = 1024);

        void next(size_t length, u_char *putIn);

        bool eos();

    private:
        u_char buffer[1024];
        size_t pointer = 0;
        std::istream *istream;
        size_t len;
        size_t bufferSize;
        size_t count=0;

        void fillBuffer();
    };

}
#endif //RDF2K2_CPP_BYTEBUFFER_H

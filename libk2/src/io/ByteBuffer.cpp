//
// Created by minimal on 12/19/20.
//

#include "ByteBuffer.h"
#include <cmath>
k2::ByteBuffer::ByteBuffer(std::istream *stream, size_t length, size_t initalSize){
    this->pointer=0;
    this->bufferSize = initalSize;
    this->buffer[bufferSize];
    this->len=length;
    this->istream=stream;
    fillBuffer();
}

void k2::ByteBuffer::fillBuffer() {
    buffer[bufferSize];
    istream->read((char *)buffer, bufferSize);
}


void k2::ByteBuffer::next(size_t length, u_char *putIn){
    for(size_t i=0; i<length;i++){
        if(this->pointer>=this->bufferSize){
            fillBuffer();
            this->pointer=0;
        }
        putIn[i]= buffer[this->pointer++];
    }
}

bool k2::ByteBuffer::eos(){
    return len<=pointer;
}

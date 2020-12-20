//
// Created by minimal on 12/14/20.
//

#include "Triple.h"

k2::Triple::Triple(size_t s, size_t p, size_t o) {
    sID=s;
    pID=p;
    oID=o;
}


k2::Triple::Triple() {}

size_t k2::Triple::get(u_char i) {
    if(i==0){
        return sID;
    }
    if(i==1){
        return pID;
    }
    if(i==2){
        return oID;
    }
    return -1;
}
//
// Created by minimal on 12/14/20.
//

#include "Triple.h"

Triple::Triple(long s, long p, long o) {
    sID=s;
    pID=p;
    oID=o;
}

long Triple::get(int i) {
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
//
// Created by minimal on 12/18/20.
//

#include "Path.h"
#include <iostream>
#include <cmath>

k2::Path::Path(u_int32_t h) {
    this->h=h;
    paths.resize(h);
}

std::vector<std::vector<u_char>> &k2::Path::getPaths(){
    return paths;
}

bool k2::Path::isEmpty(u_int32_t i){
    return paths[i].empty();
}

void k2::Path::pop(u_int32_t i){
    paths[i].pop_back();

}

std::vector<k2::Point> k2::Path::calculatePoints(){
    std::vector<Point> ret{};
    while(!isEmpty(h-1)) {
        ret.push_back(calculatePoint());
        check();
        pop(h - 1);
        check();
    }
    return ret;
}

void k2::Path::check(){
    for(u_int32_t i=0;i<h-1;i++){
        if(paths[i].empty() & !paths[i+1].empty()){
            std::cerr << "weird" << std::endl;
        }
    }
    }

k2::Point k2::Path::calculatePoint(){
    long row=0, col=0;
    u_char x=0;
    for(u_int32_t i=h;i>0;i--){
        //FIXME files produces SIGSEV because some vecotrs in paths are empty, despite that they shouldn't be.
        u_char p =paths[i-1][paths[i-1].size()-1];
        if(p==1){col+=pow(2, x);}
        if(p==2){row+=pow(2, x);}
        if(p==3){row+=pow(2, x);col+=pow(2, x);}
        x++;
    }
    return Point(row, col);
}

bool k2::Path::hasLast(){
    return last!='\00';
}

void k2::Path::add(u_int32_t i, u_char newpath){
    //dissasmble newpath
    if(i>0 && paths[i-1].empty()){
        i;
    }
    u_char x=3;
    for(u_char j=128;j>=16;j/=2){
        if(newpath & j){
            paths[i].push_back(x);
        }
        x--;
    }
    //we were already at the last point
    if(i+1==h) {
        //shift;
        u_char newLast = newpath & 15;
        this->last=newLast;
    }else {
        u_char x=3;
        for (u_char j = 8; j >= 1; j /= 2) {
            if(newpath & j){
                this->paths[i+1].push_back(x);
            }
            x--;
        }
    }

}

void k2::Path::addLast(u_int32_t i){
    u_char x=3;
    for (u_char j = 8; j >= 1; j /= 2) {
        if(last & j){
            this->paths[i].push_back(x);
        }
        x--;
    }
    last='\00';
}

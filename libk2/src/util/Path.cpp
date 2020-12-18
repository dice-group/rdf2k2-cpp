//
// Created by minimal on 12/18/20.
//

#include "Path.h"
#include <cmath>

Path::Path(u_int32_t h) {
    this->h=h;
    paths.resize(h);
}

std::vector<std::vector<u_char>> &Path::getPaths(){
    return paths;
}

bool Path::isEmpty(u_int32_t i){
    return paths[i].empty();
}

void Path::pop(u_int32_t i){
    paths[i].pop_back();

}

std::vector<Point> Path::calculatePoints(){
    std::vector<Point> ret{};
    while(!isEmpty(h-1)) {
        ret.push_back(calculatePoint());
        pop(h - 1);

    }
    return ret;
}


Point Path::calculatePoint(){
    long row=0, col=0;
    u_char x=0;
    for(int i=h-1;i>=0;i--){
        if(paths[i].empty()){
            x;
        }
        u_char p =paths[i][paths[i].size()-1];
        if(p==1){row+=pow(2, x);}
        if(p==2){col+=pow(2, x);}
        if(p==3){row+=pow(2, x);col+=pow(2, x);}
        x++;
    }
    return Point(row, col);
}

bool Path::hasLast(){
    return last!='\00';
}

void Path::add(u_int32_t i, u_char newpath){
    //dissasmble newpath
    if(i>0 && paths[i-1].empty()){
        i;
    }
    u_char x=0;
    for(u_char j=128;j>=16;j/=2){
        if(newpath & j){
            paths[i].push_back(x);
        }
        x++;
    }
    //we were already at the last point
    if(i+1==h) {
        //shift;
        u_char newLast = newpath & 15;
        this->last=newLast;
    }else {
        u_char x=0;
        for (u_char j = 8; j >= 1; j /= 2) {
            if(newpath & j){
                this->paths[i+1].push_back(x);
            }
            x++;
        }
    }

}

void Path::addLast(u_int32_t i){
    u_char x=0;
    for (u_char j = 8; j >= 1; j /= 2) {
        if(last & j){
            this->paths[i].push_back(x);
        }
        x++;
    }
    last='\00';
}
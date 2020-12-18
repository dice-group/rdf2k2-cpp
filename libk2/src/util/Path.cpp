//
// Created by minimal on 12/18/20.
//

#include "Path.h"

std::vector<std::vector<u_char>> &Path::getPaths(){
    return paths;
}

bool Path::isEmpty(u_int32_t i){
    return paths[i].size();
}

void Path::pop(u_int32_t i){
    //maybe we should reverse the list. thus we can use pop_back()
    paths[i].erase(paths.begin()->begin());
}
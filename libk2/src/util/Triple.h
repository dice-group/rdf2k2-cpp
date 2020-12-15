//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_TRIPLE_H
#define RDF2K2_CPP_TRIPLE_H


class Triple {
public:
    Triple(long s, long p, long o);
    Triple();
    long get(int i);

private:
    long sID;
    long pID;
    long oID;
};


#endif //RDF2K2_CPP_TRIPLE_H

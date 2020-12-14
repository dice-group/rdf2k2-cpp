#include "ThreadedKD2TreeSerializer.hpp"
#include <iostream>
#include "../util/LabledMatrix.h"
#include <memory>
#include "../util/Triple.h"
#include <thread>
#include <fstream>
#include "math.h"
#include <future>
#include <functional>

#include "../util/TreeNode.h"

using namespace std;

ThreadedKD2TreeSerializer::ThreadedKD2TreeSerializer(bool threaded, long noOfPredicates, long noOfTriples) {
    triples = std::vector<Triple>();
    matrices = std::vector<LabledMatrix>(noOfPredicates);
    int threads = std::thread::hardware_concurrency();

    threadedMatrices = vector<vector<long>>(threads);
    int count=0;
    for(long i=0;i<threads;i++){
        threadedMatrices.push_back(vector<long>());
    }
    for(long i=0;i<noOfPredicates;i++){
        threadedMatrices[count++].push_back(i);
        if(count>=threads){
            count =0;
        }
    }
}

void ThreadedKD2TreeSerializer::serialize(char *out){
    future<vector<shared_ptr<char[]>>> futures[threadedMatrices.size()];

    using TaskType = vector<shared_ptr<char[]>>(vector<long> , vector<LabledMatrix>);

    for(int i=0;i<threadedMatrices.size();i++){
        packaged_task<TaskType> pt {&ThreadedKD2TreeSerializer::threadedCreation};
        futures[i] = pt.get_future();
        thread thr {move(pt), threadedMatrices[i], matrices};
    }

    ofstream outfile;
    outfile.open(out, ios::out | ios::trunc );

     for(int i=0;i<threadedMatrices.size();i++){
         for(shared_ptr<char[]> treePtr : futures[i].get()){
           outfile << treePtr.get();
        }
      }

    outfile.close();
}

vector<shared_ptr<char[]>> ThreadedKD2TreeSerializer::threadedCreation(vector<long> &use, vector<LabledMatrix> &matrices){
    long x=0;
    vector<shared_ptr<char[]>> trees = vector<shared_ptr<char[]>>();
    for(long current : use){
        LabledMatrix &matrix = matrices[current];
        shared_ptr<char[]> treePtr = createTree(matrix);
        trees.push_back(treePtr);
        x++;
        if(x%10==0) {
            cout << "Created " << x << " k2 trees of " << use.size() << endl;
        }
    }
    return trees;
}

shared_ptr<char[]> ThreadedKD2TreeSerializer::createTree(LabledMatrix &matrix){
    TreeNode root = TreeNode();
    double h = matrix.getH();
    double size = pow(2, h);
    long mSize=matrix.getPoints().size();
    long count=0;

    for(Point p : matrix.getPoints()){
        long c1=0 ;
        long r1=0;
        long c2=size;
        long r2=size;
        count++;

        TreeNode pnode = root;

        if(count % 100000 ==0){
            cout << "\r" << count << "/" << mSize << endl;
        }

        for(int i=0;i<h;i++){
            char node = getNode(p, c1, r1, c2, r2);

            TreeNode cnode = TreeNode();

            cnode = pnode.setChildIfAbsent(node, cnode);
            pnode = cnode;

            if(node==0){
                r2 = (r2 - r1) / 2 + r1;
                c2 = (c2 - c1) / 2 + c1;
            }
            else if(node==1){
                ///y=row, x =col
                r2 = (r2 - r1) / 2 + r1;
                c1 = (c2 - c1) / 2 + c1;
            }
            else if(node==2){
                r1 = (r2 - r1) / 2 + r1;
                c2 = (c2 - c1) / 2 + c1;
            }
            else if(node==3){
                r1 = (r2 - r1) / 2 + r1;
                c1 = (c2 - c1) / 2 + c1;
            }

        }

    }
    matrix.getPoints().clear();
    //TODO merge
}

char ThreadedKD2TreeSerializer::getNode(Point &p, long c1, long r1, long c2, long r2){
    long rCenter = (r2 - r1) / 2 + r1;
    long cCenter = (c2 - c1) / 2 + c1;
    if(p.getCol()<cCenter && p.getRow() <rCenter){
        return 0;
    }
    if(p.getCol()>=cCenter && p.getRow() <rCenter){
        return 1;
    }
    if(p.getCol()<cCenter && p.getRow() >= rCenter){
        return 2;
    }
    else{
        return 3;
    }
}

void ThreadedKD2TreeSerializer::initSpace(shared_ptr<long[]> sizeList) {
    for(int i=0;i<sizeof(sizeList.get())/sizeof(sizeList[0]);i++){
        matrices.push_back(LabledMatrix(i, sizeList[i]));
    }
}

void ThreadedKD2TreeSerializer::flush() {
    long count=0;
    for(long i=0; i<triples.size(); i++){
        Triple triple = triples[i];
        matrices[triple.get(1)].set(triple.get(0), triple.get(2));
        count++;
        if(count %100000==0){
            cout << "\rwrote " << count << " triples to matrix.";
        }
    }
    triples.clear();
}

void ThreadedKD2TreeSerializer::add(long sID, long pID, long oID){
    Triple triple = Triple(sID, pID, oID);
    triples.push_back(triple);
}
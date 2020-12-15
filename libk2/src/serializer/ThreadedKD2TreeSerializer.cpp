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
    triples = std::vector<Triple>(noOfTriples);
    matrices = std::vector<LabledMatrix>(noOfPredicates);
    int threads=1;
    if(threaded) {
        threads = std::thread::hardware_concurrency();
    }
    threadedMatrices = vector<vector<long>>();
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
    future<vector<vector<unsigned char>>> futures[threadedMatrices.size()];

    for(int i=0;i<threadedMatrices.size();i++){
        promise<vector<vector<unsigned char>>> pt;
        futures[i] = pt.get_future();
        thread thr {&ThreadedKD2TreeSerializer::threadedCreationThread, this, ref(threadedMatrices[i]), ref(matrices), move(pt)};
        thr.detach();
    }

    ofstream outfile;
    outfile.open(out, ios::out | ios::trunc );

     for(int i=0;i<threadedMatrices.size();i++){
         vector<vector<unsigned char>> threadPtr = futures[i].get();
         for(vector<unsigned char> treePtr : threadPtr){
             for(unsigned char b : treePtr) {
                 outfile << b;
             }
        }
      }
    outfile.flush();
    outfile.close();
}

void ThreadedKD2TreeSerializer::threadedCreationThread(vector<long> &use, vector<LabledMatrix> &matrices, promise<vector<vector<unsigned char>>> promise) {
    vector<vector<unsigned char>> vec = threadedCreation(use, matrices);
    promise.set_value(vec);
}

vector<vector<unsigned char>> ThreadedKD2TreeSerializer::threadedCreation(vector<long> &use, vector<LabledMatrix> &matrices){
    long x=0;
    vector<vector<unsigned char>> trees = vector<vector<unsigned char>>();
    for(long current : use){
        LabledMatrix &matrix = matrices[current];
        vector<unsigned char> treePtr = createTree(matrix);
        trees.push_back(treePtr);
        x++;
        if(x%10==0) {
            cout << "Created " << x << " k2 trees of " << use.size() << endl;
        }
    }
    return trees;
}



vector<unsigned char> ThreadedKD2TreeSerializer::createTree(LabledMatrix &matrix){
    TreeNode *root = new TreeNode();
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

        TreeNode *pnode = root;

        if(count % 100000 ==0){
            cout << "\r" << count << "/" << mSize << endl;
        }

        for(int i=0;i<h;i++){
            char node = getNode(p, c1, r1, c2, r2);

            TreeNode *cnode = new TreeNode();

            cnode = pnode->setChildIfAbsent(node, cnode);
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

    vector<vector<unsigned char>> hMap = vector<vector<unsigned char>>();
    for(int i=0;i<h;i++){
        hMap.push_back(vector<unsigned char>());
    }
    merge(root, hMap, 0, h);

    vector<unsigned char> baos = vector<unsigned char>();
    // convert from an unsigned long int to a 4-byte array
    long label = matrix.getLabel();
    baos.push_back((int)((label >> 24) & 0xFF));
    baos.push_back((int)((label >> 16) & 0xFF)) ;
    baos.push_back((int)((label >> 8) & 0XFF));
    baos.push_back((int)((label & 0XFF)));


    bool shift=true;
    unsigned char last=0;
    for(int i=0; i<hMap.size();i++){
        for(unsigned char b : hMap[i]) {
            //tree.addNodeNeighbor(b);
            if(shift) {
                last = b << 4;
                shift=false;
            }
            else{
                baos.push_back( last | b);
                shift=true;
            }
        }
        hMap[i].clear();
    }
    if(!shift){
        baos.push_back(last);
    }
    baos.push_back(0);
    hMap.clear();
    return baos;
}

void ThreadedKD2TreeSerializer::merge(TreeNode *root, std::vector<vector<unsigned char>> &hMap, int h, double max){
    if(root == nullptr || h>=max){
        return;
    }

    vector<unsigned char> *arr = &hMap[h];
    arr->push_back(root->getRawValue(true));
    TreeNode *c0 = root->getChild(0);
    TreeNode *c1 = root->getChild(1);
    TreeNode *c2 = root->getChild(2);
    TreeNode *c3 = root->getChild(3);
    root->clear();

    merge(c0, hMap, h+1, max);
    merge(c1, hMap, h+1, max);
    merge(c2, hMap, h+1, max);
    merge(c3, hMap, h+1, max);
}


vector<unsigned char> intToBytes(int paramInt)
{
    vector<unsigned char> arrayOfByte(4);
    for (int i = 0; i < 4; i++)
        arrayOfByte[3 - i] = (paramInt >> (i * 8));
    return arrayOfByte;
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
    for(int i=0;i<matrices.size();i++){
        matrices[i]=(LabledMatrix(i, sizeList[i]));
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
    triples[tripleCount++] = triple;
}
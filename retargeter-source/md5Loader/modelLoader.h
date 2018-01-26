//
//  modelLoader.h
//  DrawEngine
//
// Based on the work by David Henry
// http://tfc.duke.free.fr/coding/md5-specs-en.html
//

#ifndef __DrawEngine__modelLoader__
#define __DrawEngine__modelLoader__

#include <iostream>
#include <fstream>
#include <utility>
#include "structs.h"
#include "model.h"
#include "entity.h"

using std::ifstream;
using std::pair;

class modelLoader{
private:
    int numJoints, numMeshes;
    
    void buildFrameSkeletons();
    Skeleton populateJoints(ifstream &infile);
    AnimModel * populateMesh(ifstream &infile);
    
public:
    modelLoader();
    AnimEntity * loadModel(string filePath);
    
};

#endif /* defined(__DrawEngine__modelLoader__) */

//
//  animLoader.h
//  DrawEngine
//
// Based on the work by David Henry
// http://tfc.duke.free.fr/coding/md5-specs-en.html
//

#ifndef __DrawEngine__animLoader__
#define __DrawEngine__animLoader__

#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include "model.h"

using std::map;
using std::pair;

class animLoader{
private:
    vector<frame> frames;
    Skeleton baseFrame;
    Anim* buildFrameSkeletons();
    
    void clearResources();
    
public:
    animLoader(){}
    ~animLoader(){}
    Anim* loadAnim(string filePath);
    

};



#endif /* defined(__DrawEngine__animLoader__) */

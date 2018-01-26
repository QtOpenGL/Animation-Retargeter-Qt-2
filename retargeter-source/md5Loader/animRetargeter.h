//
//  animRetargeter.h
//  DrawEngine
//
//  Created by Alex Handby on 28/07/2015.
//  Copyright (c) 2015 Alex Handby. All rights reserved.
//

#ifndef __DrawEngine__animRetargeter__
#define __DrawEngine__animRetargeter__

#include <stdio.h>
#include <map>
#include <utility>
#include <string>
#include <memory>
#include "entity.h"

using std::string;
using std::pair;
using std::map;
using std::shared_ptr;

class animRetargeter {

    void buildBindPose(Skeleton &bind);
    quaternion calcRotationBetweenVecs(vec3 & A, vec3 & B);
    bool inVector(int target, vector<int> vector);
    
public:
    Anim * retargetAnim( Anim * anim, AnimEntity * ent, map<int, int> * userMap, vector<int> roots, Skeleton bind);
};

#endif /* defined(__DrawEngine__animRetargeter__) */

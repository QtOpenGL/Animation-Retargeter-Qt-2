//
//  animLoader.cpp
//  DrawEngine
//
// Based on the work by David Henry
// http://tfc.duke.free.fr/coding/md5-specs-en.html
//

#include "animLoader.h"

using std::ifstream;
using std::ios;
using std::cerr;
using std::cout;
using std::endl;

Anim* animLoader::buildFrameSkeletons(){
    Anim * anim = new Anim;
        
    //for every frame
    for(int frameIndex = 0; frameIndex < frames.size(); ++frameIndex){
        
        Skeleton skel;
        skel.copy_data(baseFrame);
        
        
        //for every joint of the baseframe data
        for(int jointIndex = 0; jointIndex < baseFrame.size(); ++jointIndex){
            float px, py, pz; //glm::vec3 is an immutable datastructure
            
            int floatIndex = skel[jointIndex].getStartIndex();
            
            //find what needs to be replaced indicated by flags
            //check bit 1
            if((1 & skel[jointIndex].getFlags()) == 1){
                px = frames[frameIndex].getComponentVals()[floatIndex];
                floatIndex++;
            }else
                px = baseFrame[jointIndex].getLocalPos().x;
            
            //check bit 2
            if((2 & skel[jointIndex].getFlags()) == 2){
                py = frames[frameIndex].getComponentVals()[floatIndex];
                floatIndex++;
            }else
                py = baseFrame[jointIndex].getLocalPos().y;
            
            //check bit 3
            if((4 & skel[jointIndex].getFlags()) == 4){
                pz = frames[frameIndex].getComponentVals()[floatIndex];
                floatIndex++;
            }else
                pz = baseFrame[jointIndex].getLocalPos().z;
            
            //check bit 4
            if((8 & skel[jointIndex].getFlags()) == 8){
                skel[jointIndex].getLocalOrient().setX(frames[frameIndex].getComponentVals()[floatIndex]);
                floatIndex++;
            }
            
            //check bit 5
            if((16 & skel[jointIndex].getFlags()) == 16){
                skel[jointIndex].getLocalOrient().setY(frames[frameIndex].getComponentVals()[floatIndex]);
                floatIndex++;
            }
            
            //check bit 6
            if((32 & skel[jointIndex].getFlags()) == 32){
                skel[jointIndex].getLocalOrient().setZ(frames[frameIndex].getComponentVals()[floatIndex]);
                floatIndex++;
            }
            
            //make the position vector
            vec3 pos(px, py, pz);
            skel[jointIndex].setLocalPos(pos);
            
            //compute the w component of the joint.
            skel[jointIndex].getLocalOrient().computeW();
            
            skel.recomputeObjectSpace();
        }
                
        //save this frame skeleton to the anim.
        anim->getSkeletons().push_back(skel);
    }
    
    clearResources();
    return anim;
}

void animLoader::clearResources(){
    frames.clear();
    baseFrame.clear();
}

Anim* animLoader::loadAnim(string filePath){
    int numJoints(0), numFrames(0), frameRate(0), numComponents(0);
    
    cout << "path: " << filePath << endl;
    
    ifstream infile;
    infile.open(filePath.c_str(), ios::in);
    
    if(infile.fail())
        throw MD5Exception("Unable to load Anim: ", filePath);
    
    while(!infile.eof()){
        string token, buffer;
        
        infile >> token;
        
        if(token == "//")
            getline(infile, buffer);
        
        else if(token == "numFrames"){
            infile >> numFrames;
            frames.reserve(numFrames);
        }
        else if(token == "numJoints"){
            infile >> numJoints;
            baseFrame.reserve(numJoints);
        }
        else if(token == "numAnimatedComponents")
            infile >> numComponents;
        
        else if(token == "frameRate")
            infile >> frameRate;
        
        else if(token == "hierarchy"){
            infile >> token; //{
            
            //read the joints
            for(int i = 0; i < numJoints; ++i){
                string name;
                int parent, flags, startIndex;
                joint temp;
                
                infile >> name;
                temp.setName(name);
                infile >> parent;
                temp.setParent(parent);
                infile >> flags;
                temp.setFlags(flags);
                infile >> startIndex;
                temp.setStartIndex(startIndex);
                
                baseFrame.push_back(temp);
                
                //get the rest of the line.
                getline(infile, buffer);
            }
            
            infile >> token; // }
        }
        
        else if(token == "baseframe"){
            infile >> token; //{
            
            //read baseframe data
            for(int i = 0; i < numJoints; ++i){
                float pX, pY, pZ, val;
                
                infile >> token;//(
                
                infile >> pX;
                infile >> pY;
                infile >> pZ;
                
                baseFrame[i].setLocalPos(vec3(pX, pY, pZ));
                
                infile >> token; //)
                infile >> token; //(
                
                infile >> val;
                baseFrame[i].getLocalOrient().setX(val);
                
                infile >> val;
                baseFrame[i].getLocalOrient().setY(val);
                
                infile >> val;
                baseFrame[i].getLocalOrient().setZ(val);
                
                infile >> token; //)
                
                baseFrame[i].getLocalOrient().computeW();
                
                //Eat up the rest of the line 
                std::getline (infile, buffer);
            }
            
            infile >> token; //} 
        }
        else if(token == "frame"){
            int index(0);
            float floatVal(0);
            vector<float> floats;
            
            infile >> index; //index
            
            infile >> token; //{
            
            for(int i = 0; i < numComponents; ++i){
                infile >> floatVal;
                floats.push_back(floatVal);
            }
            
            frames.push_back(frame(index, floats));
            
            infile >> token; //} 
        }
        
    }
    
    return buildFrameSkeletons();
}

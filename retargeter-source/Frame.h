//
//  main.h
//  wxTest
//
//  Created by Alex Handby on 30/07/2015.
//  Copyright (c) 2015 Alex Handby. All rights reserved.
//

#ifndef WxSDL2_myFrame_h
#define WxSDL2_myFrame_h

#include <wx/wx.h>
#include <memory>
#include "entity.h"
#include "animRetargeter.h"

using std::shared_ptr;

class Frame;


/*===============================
 * App
 * @description: runs the main program loop and recieves messges from 
 * the frame class to modify the program data.
 *===============================*/

class App: public wxApp
{
private:
    
    //action flags
    bool loop;
    bool doretarget;
    bool doFromMesh;
    bool doFromAnim;
    bool doToMesh;
    bool bones;
    bool hide1;
    bool hide2;
    bool play1;
    bool play2;
    bool bind1;
    bool bind2;
    
    //referenced objects
    Frame * frame;
    vector<int> roots;
    DrawEngine *engine;
    modelGenerator *modelGen;
    camera *cam;
    modelLoader *mLoader;
    animLoader *aLoader;
    animRetargeter *retarg;
    map<int, int> * userMap;
    
    AnimEntity* ant;
    AnimEntity* ant2;
    Anim * anim;
    Anim * anim1;
    Anim * anim2;
    
    string toMesh;
    string fromMesh;
    string fromAnim;
    string toMeshTex;
    string fromMeshTex;
    
    void LoadToMesh();
    void LoadFromMesh();
    void LoadFromAnim();
    void PlayToAnim();
    void PlayFromAnim();
    void BindFrom();
    void BindTo();
    void checkUpdates();
    
    const int windowHeight = 768;
    const int windowWidth = 1024;
    
    
public:
    virtual bool OnInit();
    void retarget();
    void main();
    void close();
    void render(double time);
    void resize();
    void genUserMap(shared_ptr<AnimEntity> ent, shared_ptr<Anim> anim);
    void deleteResources();
    void setToMesh(wxString filepath, wxString texpath){ this->toMesh = string(filepath.mb_str()); this->toMeshTex = string(texpath.mb_str()); doToMesh = true; }
    void setFromAnim(wxString filepath){ this->fromAnim = string(filepath.mb_str()); doFromAnim = true; }
    void setFromMesh(wxString filepath, wxString texpath){ this->fromMesh = string(filepath.mb_str()); this->fromMeshTex = string(texpath.mb_str()); doFromMesh = true; }
    void setBones(bool setting){ this->bones = setting; }
    
    void setRetarget(vector<int> nroots){ roots = nroots; doretarget = true; }
    void setHideFrom(){ if(hide1) hide1 = false; else hide1 = true; }
    void setHideTo(){ if(hide2) hide2 = false; else hide2 = true; }
    void setPlayFrom(){ if(play1) play1 = false; else play1 = true; }
    void setPlayTo(){ if(play2) play2 = false; else play2 = true; }
    void setBindFrom(){ if(bind1) bind1 = false; else bind1 = true; }
    void setBindTo(){ if(bind2) bind2 = false; else bind2 = true; }
};


/*===============================
 * Frame
 * @description: Handles the user input. Sends Messages and updates to App.
 *===============================*/
class Frame: public wxFrame
{
public:
    Frame(const wxString& title, const wxPoint& pos, const wxSize& size, App * app, AnimEntity * ent, Anim * anim, map<int, int> *map);
    void Exit();
    void resetRetargetButtons(Skeleton & ent, Skeleton & anim);
    void sendWarning(string message);
    
    
private:
    App * parent;
    map<int, int> *userMap;
    vector<wxStaticText*> labels;
    vector<wxChoice*> choices;
    vector<wxCheckBox*> radio;
    map<int, int> * map;
    
    wxScrolledWindow* w;
    wxGridSizer* is;
    wxButton *applyButton; 
    
    void regenButtons( Skeleton & ent, Skeleton & anim);
    
    void OnExit(wxCommandEvent& event);
    void OnClose(wxCloseEvent & event);
    void OnRetarget(wxCommandEvent& event);
    void OnOpenMesh(wxCommandEvent& event);
    void OnToMesh(wxCommandEvent& event);
    void OnViewRetarget(wxCommandEvent& event);
    void OnBindFrom(wxCommandEvent& event);
    void OnBindTo(wxCommandEvent& event);
    void OnHideFrom(wxCommandEvent& event);
    void OnHideTo(wxCommandEvent& event);
    void OnPlayFrom(wxCommandEvent& event);
    void OnPlayTo(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};


enum
{
    ID_Retarget = 1,
    ID_From = 2,
    ID_SaveAnim = 3,
    ID_Rframe = 4,
    ID_Bind1 = 5,
    ID_Bind2 = 6,
    ID_ToMesh = 7,
    ID_ShowBones = 8,
    ID_HideBones = 9,
    ID_Hide1 = 10,
    ID_Hide2 = 11,
    ID_Play1 = 12,
    ID_Play2 = 13
};

/*
 * Event table for all the events.
 */
wxBEGIN_EVENT_TABLE(Frame, wxFrame)
EVT_CLOSE(Frame::OnClose)
EVT_BUTTON(ID_Retarget, Frame::OnRetarget)
EVT_MENU(ID_From, Frame::OnOpenMesh)
EVT_MENU(ID_ToMesh, Frame::OnToMesh)
EVT_MENU(wxID_EXIT,  Frame::OnExit)
EVT_MENU(ID_Rframe, Frame::OnViewRetarget)
EVT_MENU(ID_Bind1, Frame::OnBindFrom)
EVT_MENU(ID_Bind2, Frame::OnBindTo)
EVT_MENU(ID_Hide1, Frame::OnHideFrom)
EVT_MENU(ID_Hide2, Frame::OnHideTo)
EVT_MENU(ID_Play1, Frame::OnPlayFrom)
EVT_MENU(ID_Play2, Frame::OnPlayTo)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(App);

#endif

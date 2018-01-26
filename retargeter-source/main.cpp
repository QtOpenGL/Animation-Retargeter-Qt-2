//
//  main.cpp
//  DrawEngine
//
//  Created by Alex Handby on 14/07/2015.
//  Copyright (c) 2015 Alex Handby. All rights reserved.
//

#include <iostream>
#include <string>
#include <strings.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h> 
#include <map>
#include "engine.h"
#include "modelLoader.h"
#include "animLoader.h"
#include "Frame.h"
#include "animRetargeter.h"
#include "loadDialog.h"
#include <unistd.h>


using std::string;
using std::vector;
using std::cerr;
using std::endl;
using std::cout;
using std::map;


//
// < App Class Definition >
// * wxWidgets requires that both the main frame of an application and the App class be defined together in the same .h file.
// * Seperating the definitions of the frame and app classes into their own respective .cpp or .h results in duplicate symbol
// * linker errors. There has been several attempts to seperate the two.
//

/*===============================
 * App
 * @description: runs the main program loop and recieves messges from
 * the frame class to modify the program data.
 *===============================*/


//
//  Model Modification Functions
// * These functions maniplate the program data i.e. the models and class variables.
//
//


/*
 * retarget
 * retarget the animation from one model to another
 */
void App::retarget(){
    
    cout << "roots:" << endl;
    for(int i : roots){
        cout << i << endl;
    }
    
    cout << "map:" << endl;
    for(auto const &entry : *userMap){
        cout << entry.first << " : " << entry.second << endl;
    }
    
    anim2 = retarg->retargetAnim(ant2->getAnim(), ant, userMap, roots, ant2->getBindPose());
    ant->setAnim(anim2);
    ant->Pause(); //this should set the first frame of animation
}


/*
 * LoadToMesh
 * loads the mesh(model) which the animation is going to be retargeted to.
 */
void App::LoadToMesh(){
    
    AnimEntity * prev = ant;
    
    try{
        ant = mLoader->loadModel(toMesh);
    }catch(MD5Exception e){
        frame->sendWarning(e.getMessage());
        ant = prev;
        return;
    }
    
    delete prev;
    
    frame->resetRetargetButtons(ant->getBindPose(), ant2->getBindPose());
    
    ant->getSubEntities()[0].setTexBID(engine->storeTexture(toMeshTex));
    ant->getSubEntities()[1].setVisible(false);
    ant->getSubEntities()[2].setVisible(false);
    ant->getSubEntities()[3].setVisible(false);
    ant->rotate(30.0f, vec3(1,0,0));
    ant->translate(vec3(-15, -20, -120));
    ant->BindPose();
}


/*
 * LoadFromMesh
 * loads the mesh(model) that the animation is going to be retargeted from.
 */
void App::LoadFromMesh(){
    
    AnimEntity * prev = ant2;
    
    try{
        ant2 = mLoader->loadModel(fromMesh);
    }catch(MD5Exception e){
        frame->sendWarning(e.getMessage());
        ant2 = prev;
        return;
    }
    
    delete prev;
    
    frame->resetRetargetButtons(ant->getBindPose(), ant2->getBindPose());
    
    ant2->getSubEntities()[0].setTexBID(engine->storeTexture(fromMeshTex));
    ant2->getSubEntities()[1].setVisible(false);
    ant2->getSubEntities()[2].setVisible(false);
    ant2->getSubEntities()[3].setVisible(false);
    ant2->rotate(30.0f, vec3(1,0,0));
    ant2->translate(vec3(-100, -20, -120));
    ant2->BindPose();
}


/*
 * LoadFromAnim
 * Load the animation to be retargeted.
 */
void App::LoadFromAnim(){
    
    Anim * prev = anim;
    
    try{
        anim = aLoader->loadAnim(fromAnim);
    }catch(MD5Exception e){
        frame->sendWarning(e.getMessage());
        anim = prev;
        return;
    }
    
    delete prev;
    
    ant2->setAnim(anim);
    ant2->Pause();
}


/*
 * PlayToAnim
 * play the retargeted animation on the model that it was retargeted to.
 */
void App::PlayToAnim(){

    if(ant->isPaused())
        ant->Play(); //if in bindpose will have no effect.
    else
        ant->Pause();
    
}


/*
 * PlayFromAnim
 * play the animation on the model that it was originally from.
 */
void App::PlayFromAnim(){

    if(ant2->isPaused())
        ant2->Play(); //if in bindpose will have no effect.
    else
        ant2->Pause();
}


/*
 * BindFrom
 * put the model that the animation was originally from in bind pose.
 */
void App::BindFrom(){

    if(ant2->isBindPose())
        ant2->Pause();
    else
        ant2->BindPose();
}

/*
 * BindTo
 * put the model that the animation is being retargeted to in bind pose.
 */
void App::BindTo(){

    if(ant->isBindPose())
        ant->Pause();
    else
        ant->BindPose();
    
}

/*
 * render
 * send the models to the DrawEngine to be rendered
 * @param double time - the current time.
 */
void App::render(double time){
    engine->BeginDraw();
    if(!hide2){
        ant->update(time);
        engine->BufferedDrawTex(*ant);
    }
    
    if(!hide1){
        ant2->update(time);
        engine->BufferedDrawTex(*ant2);
    }
    
    
}


/*
 * checkUpdates()
 * check action flags. If the flags have been set, perform the corresponding action then 
 * set the flags back.
 */

void App::checkUpdates(){
    
    if(doretarget){
        doretarget = false;
        retarget();
    }
    if(doFromMesh){
        doFromMesh = false;
        LoadFromMesh();
    }
    if(doFromAnim){
        doFromAnim = false;
        LoadFromAnim();
    }
    if(doToMesh){
        doToMesh = false;
        LoadToMesh();
    }
    if(play1){
        play1 = false;
        PlayFromAnim();
    }
    if(play2){
        play2 = false;
        PlayToAnim();
    }
    if(bind1){
        bind1 = false;
        BindFrom();
    }
    if(bind2){
        bind2 = false;
        BindTo();
    }
    
}


//
//  Main Application Functions
//
//

/*
 * close
 * close the application by terminating the main application loop
 */
void App::close(){
    loop = false;
}

/*
 * resize
 * inform the DrawEngine the the window is being resized.
 */
void App::resize(){
    engine->resize(windowWidth, windowHeight);
}


/*
 * deleteResources
 * ensure all the memory allocated for the child objects is freed.
 */
void App::deleteResources(){
    
    delete engine;
    delete modelGen;
    delete cam;
    delete mLoader;
    delete aLoader;
    delete retarg;
    userMap->clear();
    delete userMap;
    delete ant;
    delete ant2;
    delete anim;
    delete anim2;
    
    
}


/*
 * OnInit
 * wxApp does provide a constructor however OnInit perform's a similar.
 * @return bool success - whether the initialization of App is a success.
 */
bool App::OnInit()
{
    loop = true;
    doretarget = false;
    doFromMesh = false;
    doFromAnim = false;
    doToMesh = false;
    bones = false;
    hide1 = false;
    hide2 = false;
    play1 = false;
    play2 = false;
    bind1 = false;
    bind2 = false;
    main();
    return true;
}


/*
 * main()
 * The main application loop
 */
void App::main() {
    
    SDL_Window *mainWindow;
    SDL_GLContext mainContext;
    
    if( SDL_Init( SDL_INIT_VIDEO ) != 0 ){
        cerr << "could not initialise SDL" << endl;
        exit(0);
        return;
    }
    
    char filepath[500];
    
    getcwd(filepath, sizeof(filepath));
    
    cout << "fpath: " << filepath << endl;
    
    //set buffers and versions
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
    //create the window
    mainWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    
    mainContext = SDL_GL_CreateContext(mainWindow);
    
    cam = new camera(vec3(0, 0, 10));
    engine = new DrawEngine(windowWidth, windowHeight, cam);
    modelGen = new modelGenerator();
    mLoader = new modelLoader();
    aLoader = new animLoader();
    retarg = new animRetargeter();
    
    ant = mLoader->loadModel("Demo/zombies/zfat/zfat.md5mesh");
    ant2 = mLoader->loadModel("Demo/imp/imp.md5mesh");
    anim = aLoader->loadAnim("Demo/imp/walk1.md5anim");
    anim2 = new Anim();

    userMap = new map<int, int>;
    
    
    frame = new Frame( "Retarget", wxPoint(50, 50), wxSize(400, 500), this, ant, anim, userMap );
    frame->Show( true );
    
    ant->getSubEntities()[0].setTexBID(engine->storeTexture("Demo/zombies/zfat/fatty_d.tga"));
    ant->getSubEntities()[1].setVisible(false);
    ant->getSubEntities()[2].setVisible(false);
    ant->getSubEntities()[3].setVisible(false);
    ant->BindPose();
    ant->rotate(30.0f, vec3(1,0,0));
    ant->translate(vec3(20, -20, -120));
    
    ant2->setAnim(anim);
    ant2->getSubEntities()[0].setTexBID(engine->storeTexture("Demo/imp/imp_d.tga"));
    ant2->getSubEntities()[1].setVisible(false);
    ant2->getSubEntities()[2].setVisible(false);
    ant2->getSubEntities()[3].setVisible(false);
    ant2->Pause();
    ant2->rotate(30.0f, vec3(1,0,0));
    ant2->translate(vec3(-50, -20, -120));

    
    double time = 0.0f;
    
    while(loop){
        
        SDL_Event event;
        
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    loop = false;
                    break;
                    
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_w:
                            cam->moveForward();
                            break;
                            
                        case SDLK_s:
                            cam->moveBackward();
                            break;
                            
                        case SDLK_d:
                            cam->strafeRight();
                            break;
                            
                        case SDLK_a:
                            cam->strafeLeft();
                            break;
                            
                        case SDLK_r:
                            cam->moveUp();
                            break;
                            
                        case SDLK_f:
                            cam->moveDown();
                            break;
                            
                        case SDLK_UP:
                            cam->lookUp();
                            break;
                            
                        case SDLK_DOWN:
                            cam->lookDown();
                            break;
                            
                        case SDLK_LEFT:
                            cam->lookLeft();
                            break;
                            
                        case SDLK_RIGHT:
                            cam->lookRight();
                            break;
                            
                        case SDLK_SPACE:
                            //play anim
                            break;
                            
                        case SDLK_ESCAPE:
                            loop = false;
                            break;
                            
                        default:
                            break;
                            
                    }
                    
            }
        }
        checkUpdates();
        time = (double) SDL_GetTicks() / 1000.0;
        render(time);
        SDL_GL_SwapWindow(mainWindow);
        
    }
    frame->Exit();
    SDL_Quit();
    deleteResources();
    
}



//
// < Frame Class Definition >
// * wxWidgets requires that both the main frame of an application and the App class be defined together in the same .h file.
// * Seperating the definitions of the frame and app classes into their own respective .cpp or .h results in duplicate symbol
// * linker errors. There has been several attempts to seperate the two.
//

/*===============================
 * Frame
 * @description: Handles the user input. Sends Messages and updates to App.
 *===============================*/

/*
 * Frame Constructor
 * @param wxString title - frame title.
 * @param wxPoint pos - beginning position.
 * @param wxString size - wxSize size.
 * @param App * app - parent.
 * @param AnimEntity * ent - entity used to populate window/dialog information.
 * @param Anim * anim - anim used to populate window/dialog information.
 * @param map - used to map joints of the animation to joints of the model being retargeted to.
 */
Frame::Frame(const wxString& title, const wxPoint& pos, const wxSize& size, App * app, AnimEntity * ent, Anim * anim, std::map<int, int> * map)
: wxFrame(NULL, wxID_ANY, title, pos, size, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
    parent = app;
    userMap = map;
    
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_From, "&Retarget From... ", "Load a mesh and animation to retarget from");
    menuFile->Append(ID_ToMesh, "&Retarget To...", "Load a mesh to retarget to");
    //menuFile->Append(ID_OpenAnim, "&Save Anim... \tCtrl-S", "brings up the Save Anim dialog");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    
    wxMenu *menuView = new wxMenu;
    menuView->Append(ID_Rframe, "&Retarget Panel", "opens the retargeting panel");
    
    wxMenu *menuAnim = new wxMenu;
    menuAnim->Append(ID_Play1, "&Play 'From'", "play the animation");
    menuAnim->Append(ID_Play2, "&Play 'To'", "play the animation");
    menuAnim->Append(ID_Bind1, "&Bind Pose 'From'", "Bind Pose");
    menuAnim->Append(ID_Bind2, "&Bind Pose 'To'", "Bind Pose");
    menuAnim->Append(ID_Hide1, "&Hide 'From' Model", "hides or shows the 'From' model");
    menuAnim->Append(ID_Hide2, "&Hide 'To' Model", "hides or shows the 'To' model");
    
    
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuAnim, "&Animation");
    SetMenuBar( menuBar );

    
    //create the main sizer
    wxBoxSizer * main_sizer = new wxBoxSizer(wxVERTICAL);
    
    //create the scrollwindow
    wxScrolledWindow * wnd = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL, wxT("scroll"));
    w = wnd;
    
    wxGridSizer * innersizer = new wxGridSizer(20, 3, 0, 0);
    is = innersizer;
    
    regenButtons(ent->getBindPose(), anim->getSkeletons()[0]);
    
    //set the second sizer to the scolledwindow
    wnd->SetSizer(innersizer);
    wnd->SetAutoLayout(true);
    wnd->Layout();
    wnd->SetScrollbars(20,20,50,50);
    
    //add the scrolledwindow to the main sizer
    main_sizer->Add(wnd, 1, wxEXPAND | wxALL, 5);
    
    SetSizer(main_sizer);
    SetAutoLayout(true);
    Layout();
}

/*
 * regenButtons
 * if a new 'To' or 'From' model is loaded this function makes the necessary changes to the retargeting panel.
 * @param Skeleton & ent - the model skeleton.
 * @param Skeleton & anim - the animation skeleton.
 */
void Frame::regenButtons(Skeleton & ent, Skeleton & anim){
    
    wxArrayString modelJoints;
    wxArrayString animJoints;
    
    for(int i = 0; i < ent.size(); i++)
        modelJoints.push_back(ent[i].getName());
    
    wxString none = "none";
    
    animJoints.Add(none);
    
    for(int i = 0; i < anim.size(); i++)
        animJoints.Add(anim[i].getName());
    
    w->Scroll(0, 0);
    
    //add buttons
    for(int i = 0; i < ent.size(); i++){
        int pos = 0;
        
        labels.push_back(new wxStaticText(w, wxID_ANY, modelJoints[i]));
        is->Add(labels[i], 1, wxEXPAND | wxALL, 5);
        
        for(int j = 0; j < animJoints.size(); j++)
            if( strcasecmp(modelJoints[i].c_str(), animJoints[j].c_str()) == 0)
                pos = j;
        
        
        choices.push_back(new wxChoice(w, wxID_ANY, wxDefaultPosition, wxDefaultSize, animJoints));
        choices[i]->SetSelection(pos);
        is->Add(choices[i], 1, wxEXPAND | wxALL, 1);
        
        radio.push_back(new wxCheckBox(w, wxID_ANY, wxT("Root"), wxDefaultPosition, wxDefaultSize, wxRB_SINGLE));
        is->Add(radio[i], 1, wxALL, 1);
    }
    
    
    applyButton = new wxButton(w, ID_Retarget, wxT("Apply"));
    is->Add(applyButton, 1, wxEXPAND | wxALL, 1);
    is->Layout();

}

/*
 * resetRetargetButtons
 * if a new 'To' or 'From' model is loaded this function makes the necessary changes to the retargeting panel.
 * @param Skeleton & ent - the model skeleton.
 * @param Skeleton & anim - the animation skeleton.
 */
void Frame::resetRetargetButtons(Skeleton & ent, Skeleton & anim){

    applyButton->Destroy();
    
    for(int i = 0; i < choices.size(); i++){
         choices[i]->Destroy();
    }
    
    for(int i = 0; i < labels.size(); i++){
        labels[i]->Destroy();
        radio[i]->Destroy();
    }
    
    labels.clear();
    labels.shrink_to_fit();
    choices.clear();
    choices.shrink_to_fit();
    radio.clear();
    radio.shrink_to_fit();
    
    regenButtons(ent, anim);
}

/*
 * sendWarning
 * show waring dialog when an exception is thrown
 * @param Skeleton & ent - the model skeleton.
 * @param Skeleton & anim - the animation skeleton.
 */
void Frame::sendWarning(string message){

    wxMessageDialog *dial = new wxMessageDialog(NULL, message, wxT("Error"), wxOK | wxICON_ERROR);
    
    dial->ShowModal();
}

/*
 * OnPlayFrom
 * response to the "play from" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnPlayFrom(wxCommandEvent& event){
    parent->setPlayFrom();
}

/*
 * OnPlayTo
 * response to the "play To" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnPlayTo(wxCommandEvent& event){
    parent->setPlayTo();
}

/*
 * OnBindFrom
 * response to the "Bind from" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnBindFrom(wxCommandEvent& event){
    parent->setBindFrom();
}

/*
 * OnBindTo
 * response to the "Bind To" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnBindTo(wxCommandEvent& event){
    parent->setBindTo();
}

/*
 * OnHideFrom
 * response to the "hide from" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnHideFrom(wxCommandEvent& event){
    parent->setHideFrom();
}

/*
 * OnHideTo
 * response to the "hide to" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnHideTo(wxCommandEvent& event){
    parent->setHideTo();
}

/*
 * OnToMesh
 * response to the "load to mesh" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnToMesh(wxCommandEvent& event){

    std::map<wxString, wxString> filePaths;
    
    loadDialog * load = new loadDialog(this, false, "Retarget To:");
    
    wxString texpath(""), meshpath("");
    
    if (load->ShowModal() == wxID_OK){
        filePaths = load->getPaths();
    }else
        return;
    
    if(filePaths.count("mesh"))
        meshpath = filePaths.find("mesh")->second;
    
    if(filePaths.count("texture"))
        texpath = filePaths.find("texture")->second;
    
    parent->setToMesh(meshpath, texpath);
    
}

/*
 * OnRetarget
 * response to the "retarget" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnRetarget(wxCommandEvent& event)
{
    userMap->clear();
    
    vector<int> roots;
    
    for(int i = 0; i < radio.size(); i++){
        if(radio[i]->GetValue())
            roots.push_back(i);
    }
    
    for(int i = 0; i < labels.size(); i++){
        cout << labels[i]->GetLabel() << ": " << choices[i]->GetString(choices[i]->GetCurrentSelection()) << endl;
        
        if(choices[i]->GetCurrentSelection() > 0)
            userMap->insert(std::pair<int, int>(i, choices[i]->GetCurrentSelection()-1));
    }
    
    for(const std::pair<int,int>& x: *userMap) {
        std::cout << x.first << " : " << x.second << '\n';
    }
    
    parent->setRetarget(roots);
    
    cout << "retarget pressed" << endl;
}

/*
 * OnOpenMesh
 * response to the "load from mesh" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnOpenMesh(wxCommandEvent& event)
{
    std::map<wxString, wxString> filePaths;
    
    loadDialog * load = new loadDialog(this, true, "Retarget From:");
    
    wxString texpath(""), meshpath(""), animpath("");
    
    if (load->ShowModal() == wxID_OK){
        filePaths = load->getPaths();
    }else
        return;
    
    if(filePaths.count("mesh"))
        meshpath = filePaths.find("mesh")->second;
    
    
    if(filePaths.count("texture"))
        texpath = filePaths.find("texture")->second;
    
    if(filePaths.count("anim")){
        animpath = filePaths.find("anim")->second;
        cout << animpath  << endl;
    }
    
    parent->setFromMesh(meshpath, texpath);
    parent->setFromAnim(animpath);
    
}

/*
 * OnClose
 * response to the "close" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnClose(wxCloseEvent& event){
    Hide();
}

/*
 * OnViewRetarget
 * response to the "view retarget" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnViewRetarget(wxCommandEvent& event){
    Show();
}

/*
 * OnExit
 * response to the "close" button being pressed
 * @param wxCommandEvent& event - button press event.
 */
void Frame::OnExit(wxCommandEvent& event)
{
    parent->close();
    Destroy();
}

/*
 * Exit
 * close the application
 */
void Frame::Exit(){
    wxCommandEvent command;
    OnExit(command);
}







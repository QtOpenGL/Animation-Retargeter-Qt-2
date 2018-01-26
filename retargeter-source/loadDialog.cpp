//
//  loadDialog.cpp
//  WxSDL3
//
//  Created by Alex Handby on 13/08/2015.
//  Copyright (c) 2015 Alex Handby. All rights reserved.
//

#include "loadDialog.h"

using std::cout;
using std::endl;

loadDialog::loadDialog(wxWindow * parent, bool anim, const wxString & title): getAnim(anim), wxDialog(parent, -1, title, wxDefaultPosition, wxSize(350, 350))
{

    
    wxPanel *panel = new wxPanel(this, -1);
    
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBox *st = new wxStaticBox(panel, -1, wxT("Select Animation, Model and Texture"),
                                      wxPoint(5, 5), wxSize(340, 330));
    
    wxStaticText *meshlabel = new wxStaticText(panel, -1, wxT("Model: "), wxPoint(15, 30), wxDefaultSize);
    wxTextCtrl *meshpath = new wxTextCtrl(panel, -1, wxT("enter file path..."), wxPoint(95, 30));
    wxButton *meshButton = new wxButton(panel, -1, wxT("Find"), wxPoint(230, 26), wxDefaultSize);
    mpath = meshpath;

    
    wxStaticText *texLabel = new wxStaticText(panel, -1, wxT("Texture: "), wxPoint(15, 60), wxDefaultSize);
    wxTextCtrl *texpath = new wxTextCtrl(panel, -1, wxT("enter file path..."), wxPoint(95, 60));
    tpath = texpath;
    wxButton *texButton = new wxButton(panel, -1, wxT("Find"), wxPoint(230, 56), wxDefaultSize);

    
    wxStaticText *animLabel = new wxStaticText(panel, -1, wxT("Animation: "), wxPoint(15, 90), wxDefaultSize);
    wxTextCtrl *animpath = new wxTextCtrl(panel, -1, wxT("enter file path..."), wxPoint(95, 90));
    apath = animpath;
    wxButton *animButton = new wxButton(panel, -1, wxT("Find"), wxPoint(230, 86), wxDefaultSize);
        
    if(!anim){
        animLabel->Hide();
        animpath->Hide();
        animButton->Hide();
    }
    
    
    wxButton *okButton = new wxButton(this, -1, wxT("Ok"),
                                      wxDefaultPosition, wxSize(70, 30));
    wxButton *closeButton = new wxButton(this, -1, wxT("Close"),
                                         wxDefaultPosition, wxSize(70, 30));
    
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &loadDialog::onOpenAnim, this, animButton->GetId());
    
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &loadDialog::onOpenTexture,
         this, texButton->GetId());
    
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &loadDialog::onOpenMesh,
         this, meshButton->GetId());
    
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &loadDialog::onCancel,
         this, closeButton->GetId());
    
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &loadDialog::onOk,
         this, okButton->GetId());
    
    
    hbox->Add(okButton, 1);
    hbox->Add(closeButton, 1, wxLEFT, 5);
    
    vbox->Add(panel, 1);
    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
    
    SetSizer(vbox);
    
    Centre();
    
}

void loadDialog::onCancel(wxCommandEvent& WXUNUSED(pEvent))    {
    EndModal(wxID_CANCEL);
    Destroy();
}

void loadDialog::onOk(wxCommandEvent& WXUNUSED(pEvent))    {
    
    wxString test = "enter file path...";
    
    wxString tval = tpath->GetValue();
    
    wxString aval = apath->GetValue();
    
    wxString mval = mpath->GetValue();
    
    
    cout << "aval: " << aval << endl;
    
    if(!tval.IsSameAs(test)){
        filepaths.insert(std::pair<wxString, wxString>("texture", tval));
    }
    if(!aval.IsSameAs(test)){
        filepaths.insert(std::pair<wxString, wxString>("anim", aval));
    }
    if(!mval.IsSameAs(test)){
        filepaths.insert(std::pair<wxString, wxString>("mesh", mval));
    }
    
    EndModal(wxID_OK);
    Destroy();
}

void loadDialog::onOpenTexture(wxCommandEvent& WXUNUSED(pEvent)){

    wxString fileName;
    
    wxFileDialog *openFileDialog = new wxFileDialog(this, _("Open Traga file"), "", "", "Truevision Traga files (*.tga)|*.tga", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    
    if (openFileDialog->ShowModal() == wxID_OK){
        fileName = openFileDialog->GetPath();
        tpath->SetValue(fileName);
    }
    
}

void loadDialog::onOpenAnim(wxCommandEvent& WXUNUSED(pEvent)){
    
    wxString fileName;
    
    wxFileDialog * openFileDialog = new wxFileDialog(this, _("Open md5Anim file"), "", "", "md5Anim files (*.md5Anim)|*.md5Anim", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    
    if (openFileDialog->ShowModal() == wxID_OK){
        fileName = openFileDialog->GetPath();
         apath->SetValue(fileName);
    }
    
}

void loadDialog::onOpenMesh(wxCommandEvent& WXUNUSED(pEvent)){
    
    wxString fileName;
    
    wxFileDialog * openFileDialog = new wxFileDialog(this, _("Open md5Mesh file"), "", "", "md5Mesh files (*.md5Mesh)|*.md5Mesh", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    
    if (openFileDialog->ShowModal() == wxID_OK){
        fileName = openFileDialog->GetPath();
        mpath->SetValue(fileName);
    }
}

map<wxString, wxString> loadDialog::getPaths(){
    return filepaths;
}

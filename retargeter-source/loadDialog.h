//
//  loadDialog.h
//  WxSDL3
//
//  Created by Alex Handby on 13/08/2015.
//  Copyright (c) 2015 Alex Handby. All rights reserved.
//

#ifndef __WxSDL3__loadDialog__
#define __WxSDL3__loadDialog__

#include <iostream>

#include <wx/wx.h>
#include <wx/choice.h>
#include <vector>
#include <map>
#include <memory>
#include "entity.h"

using std::vector;
using std::map;
using std::shared_ptr;

class loadDialog : public wxDialog{
private:
    map<wxString, wxString> filepaths;
    wxTextCtrl * apath;
    wxTextCtrl * tpath;
    wxTextCtrl * mpath;
    bool getAnim;

    void onCancel(wxCommandEvent& pEvent);
    void onOk(wxCommandEvent& pEvent);
    void onOpenTexture(wxCommandEvent& pEvent);
    void onOpenMesh(wxCommandEvent& pEvent);
    void onOpenAnim(wxCommandEvent& pEvent);
    
public:
    loadDialog(wxWindow * parent, bool anim, const wxString & title);
    map<wxString, wxString> getPaths();
        
};

#endif /* defined(__WxSDL3__loadDialog__) */

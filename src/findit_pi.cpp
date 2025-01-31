/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Findit Plugin
 * Author:   David Register
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */


#include <wx/wxprec.h>

#ifndef  WX_PRECOMP
#include <wx/wx.h>
#include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/aui/aui.h>
#include <wx/fileconf.h>

#include "findit_pi.h"
#include "wx/jsonval.h"
#include "wx/jsonreader.h"
#include "gui.h"
#include "icons.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new findit_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}


findit_pi::findit_pi(void *ppimgr)
   :opencpn_plugin_118(ppimgr)
{
  // Create the PlugIn icons
    initialize_images();

 // From shipdriver to read panel icon file
    wxFileName fn;

    auto path = GetPluginDataDir("findit_pi");
    fn.SetPath(path);
    fn.AppendDir("data");
    fn.SetFullName("findit_panel.png");

    path = fn.GetFullPath();

    wxInitAllImageHandlers();

    wxLogDebug(wxString("Using icon path: ") + path);
    if (!wxImage::CanRead(path)) {
       wxLogDebug("Initiating image handlers.");
       wxInitAllImageHandlers();
    }
    wxImage panelIcon(path);
    if (panelIcon.IsOk())
       m_panelBitmap = wxBitmap(panelIcon);
    else
       wxLogWarning("Findit panel icon has NOT been loaded");
       m_bFINDITShowIcon = false;
 // End of from Shipdriver
}

findit_pi::~findit_pi()
{
}
//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

int findit_pi::Init(void)
{
    AddLocaleCatalog( _T("opencpn-findit_pi") );

    m_pFindItWindow = NULL;

    //    Get a pointer to the opencpn configuration object
    m_pconfig = GetOCPNConfigObject();

    //    And load the configuration items
     LoadConfig();

    //    Get a pointer to the opencpn display canvas, to use as a parent for windows created
    m_parent_window = GetOCPNCanvasWindow();

      //    This PlugIn needs a toolbar icon, so request its insertion if enabled locally
    //         Findit had the code below which did not use SVG
    //         Create the Context Menu Items
    //         In order to avoid an ASSERT on msw debug builds,
    //         we need to create a dummy menu to act as a surrogate parent of the created MenuItems
    //         The Items will be re-parented when added to the real context menu

wxMenu dummy_menu;
   m_bFINDITShowIcon = true;
   if(m_bFINDITShowIcon)

#ifdef PLUGIN_USE_SVG
         // This PlugIn needs a toolbar icon, so request SVG insertion
		 //extern "C"  DECL_EXP
		 // int InsertPlugInToolSVG(wxString label, wxString SVGfile, wxString SVGfileRollover, wxString SVGfileToggled,

      m_leftclick_tool_id = InsertPlugInToolSVG( "Findit" , _svg_findit, _svg_findit_rollover, _svg_findit_toggled, wxITEM_CHECK,
	               _("Findit"),  _T( "" ), NULL,
				   FINDIT_TOOL_POSITION, 0, this);

#else
 
	 m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_findit, _img_findit, wxITEM_CHECK,
	                _("FindIt"), _T(""), NULL,
					FINDIT_TOOL_POSITION, 0, this);

#endif
    return (
               WANTS_TOOLBAR_CALLBACK    |
               WANTS_PREFERENCES         |
               WANTS_PLUGIN_MESSAGING
           );
}

bool findit_pi::DeInit(void)
{
    if(m_pFindItWindow)
    {
        m_pFindItWindow->Destroy();
        m_pFindItWindow = NULL;
    }
    return true;
}

void findit_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
    if(message_id == _T("LOGBOOK_READY_FOR_REQUESTS"))
    {
        this->isLogbookReady = (message_body == _T("TRUE"))?true:false;

        if(m_pFindItWindow)
            m_pFindItWindow->setLogbookColumns(isLogbookReady);
    }
    else if(message_id == _T("LOGBOOK_WINDOW_SHOWN"))
    {
        isLogbookWindowShown = true;
    }
    else if(message_id == _T("LOGBOOK_WINDOW_HIDDEN"))
    {
        isLogbookWindowShown = false;
    }
}

int findit_pi::GetAPIVersionMajor()
{
    return OCPN_API_VERSION_MAJOR;
}

int findit_pi::GetAPIVersionMinor()
{
    return OCPN_API_VERSION_MINOR;
}

int findit_pi::GetPlugInVersionMajor()
{
    return PLUGIN_VERSION_MAJOR;
}

int findit_pi::GetPlugInVersionMinor()
{
    return PLUGIN_VERSION_MINOR;
}

int findit_pi::GetPlugInVersionPatch()
{
    return PLUGIN_VERSION_PATCH;
}

int findit_pi::GetPlugInVersionPost()
{
    return PLUGIN_VERSION_TWEAK;
}


wxString findit_pi::GetCommonName()
{
   return _T(PLUGIN_COMMON_NAME);
}

wxString findit_pi::GetShortDescription()
{
     return _(PLUGIN_SHORT_DESCRIPTION);
}

wxString findit_pi::GetLongDescription()
{
    return _(PLUGIN_LONG_DESCRIPTION);
}

//wxBitmap *findit_pi::GetPlugInBitmap()
//{
//    return _img_findit_pi;//
//}

// Shipdriver uses the climatology_panel.png file to make the bitmap.

wxBitmap* findit_pi::GetPlugInBitmap() { return &m_panelBitmap; }

// End of shipdriver process

void findit_pi::SetColorScheme(PI_ColorScheme cs)
{
    if (NULL == m_pFindItWindow)
        return;
    DimeWindow(m_pFindItWindow);
}

void findit_pi::OnToolbarToolCallback(int id)
{
    SendPluginMessage(_T("LOGBOOK_IS_READY_FOR_REQUEST"), wxEmptyString);
    if(NULL == m_pFindItWindow)
        m_pFindItWindow = new MainDialog(this->m_parent_window,this);
    else
    {
        if(m_pFindItWindow->IsIconized())
            m_pFindItWindow->Iconize(false);
    }

    SetColorScheme(PI_ColorScheme());

    m_pFindItWindow->Show();
    m_pFindItWindow->SetFocus();
}

void findit_pi::SetDefaults(void)
{
    // If the config somehow says NOT to show the icon, override it so the user gets good feedback
    if(!m_bFINDITShowIcon)
    {
        m_bFINDITShowIcon = true;

        m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_findit, _img_findit, wxITEM_NORMAL,
                                                _("FindIt"), _T(""), NULL, FINDIT_TOOL_POSITION, 0, this);

    }
}

void findit_pi::UpdateAuiStatus(void)
{

}

void findit_pi::ShowPreferencesDialog( wxWindow* parent )
{
    int buyNotemp = buyNo;
    int toBuyZerotemp = toBuyZero;
    int lastRowDefaulttemp = lastRowDefault;

    OptionsDialog* dlg = new OptionsDialog(parent,this);

    wxColour cl;
    GetGlobalColor(_T("DILG1"), &cl);
    dlg->SetBackgroundColour(cl);

//	dlg->m_checkBoxShowLogbook->SetValue(m_bLOGShowIcon);

    if(dlg->ShowModal() == wxID_OK)
    {
        buyNo = dlg->m_radioBox11->GetSelection();
        toBuyZero = dlg->m_radioBox1->GetSelection();
        lastRowDefault = dlg->m_radioBox5->GetSelection();

        if((buyNo != buyNotemp) || (toBuyZero != toBuyZerotemp) || (lastRowDefault != lastRowDefaulttemp))
            if(m_pFindItWindow)
                m_pFindItWindow->reloadData();

        //    Show Icon changed value?
        if(m_bFINDITShowIcon != dlg->m_checkBoxFindItIcon->GetValue())
        {
            m_bFINDITShowIcon = dlg->m_checkBoxFindItIcon->GetValue();

            if(m_bFINDITShowIcon)
                m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_findit, _img_findit, wxITEM_NORMAL,
                                                        _("FindIt"), _T(""), NULL, FINDIT_TOOL_POSITION, 0, this);
            else
                RemovePlugInTool(m_leftclick_tool_id);
        }
        SaveConfig();
    }
    else
    {
        if(buyNo != buyNotemp || toBuyZero != toBuyZerotemp || lastRowDefault != lastRowDefaulttemp)
            if(m_pFindItWindow)
                m_pFindItWindow->reloadData();
    }
    delete dlg;
}

void findit_pi::SaveConfig()
{
    if(m_pconfig)
    {
        m_pconfig->SetPath ( _T ( "/PlugIns/FindIt" ) );
        m_pconfig->Write ( _T( "ShowFindItIcon" ), m_bFINDITShowIcon );
        m_pconfig->Write ( _T( "buyNo" ), buyNo );
        m_pconfig->Write ( _T( "toBuyZero" ), toBuyZero );
        m_pconfig->Write ( _T( "lastRowDefault" ), lastRowDefault );
    }
}

void findit_pi::LoadConfig()
{
    if(m_pconfig)
    {
        m_pconfig->SetPath ( _T( "/PlugIns/FindIt" ) );
        m_pconfig->Read ( _T( "ShowFindItIcon" ),  &m_bFINDITShowIcon, 1 );
        m_pconfig->Read ( _T( "buyNo" ),  &buyNo, 0 );
        m_pconfig->Read ( _T( "toBuyZero" ),  &toBuyZero, 0 );
        m_pconfig->Read ( _T( "lastRowDefault" ), &lastRowDefault, 0 );
    }
}
//////////////////////OptionsDialog ////////////////////
////////////////////////////////////////////////////////
void OptionsDialog::OnInitDialog(wxInitDialogEvent& event)
{
    this->m_checkBoxFindItIcon->SetValue(parent->m_bFINDITShowIcon);
    this->m_radioBox1->SetSelection(parent->toBuyZero);
    this->m_radioBox11->SetSelection(parent->buyNo);
    this->m_radioBox5->SetSelection(parent->lastRowDefault);
}



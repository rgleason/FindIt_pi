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
#define FINDIT_TOOL_POSITION -1

#ifndef _FINDITPI_H_
#define _FINDITPI_H_

#include <wx/wxprec.h>

#ifndef  WX_PRECOMP
#include <wx/wx.h>
#endif //precompiled headers


#include <wx/glcanvas.h>


#include "version.h"

#include "ocpn_plugin.h"
#include "findit.h"

class MainDialog;
class findit_pi;
//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------


class findit_pi : public opencpn_plugin_118
{
public:

    findit_pi(void *ppimgr);

    ~findit_pi();

//    The required PlugIn Methods
    int Init(void);
    bool DeInit(void);

    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
    int GetPlugInVersionPatch();
    int GetPlugInVersionPost();
    wxBitmap* GetPlugInBitmap();

    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();

//    The optional method overrides
    void OnToolbarToolCallback(int id);
    void SetColorScheme(PI_ColorScheme cs);
    void SetPluginMessage(wxString &message_id, wxString &message_body);
    void ShowPreferencesDialog( wxWindow* parent );
    void UpdateAuiStatus(void);

    void SetDefaults(void);

    bool isLogbookReady;
    bool isLogbookWindowShown;

    int		buyNo;
    int		toBuyZero;
    int		lastRowDefault;
    bool		m_bFINDITShowIcon;

// From Shipdriver to show listing panel image
    wxBitmap m_panelBitmap;


private:
    wxWindow         *m_parent_window;

    MainDialog       *m_pFindItWindow;
    wxAuiManager     *m_AUImgr;
    wxFileConfig     *m_pconfig;
    int               m_show_id;
    int               m_hide_id;

    int				m_leftclick_tool_id;

    void				SaveConfig();
    void				LoadConfig();
};



#endif




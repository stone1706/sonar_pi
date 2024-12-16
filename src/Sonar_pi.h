/******************************************************************************
 * $Id:  $
 *
 * Project:  OpenCPN
 * Purpose:  Radar Plugin
 * Author:   Johan van der Sman
 *
 ***************************************************************************
 *   Copyright (C) 2015 - 2020 Johan van der Sman                          *
 *   johan.sman@gmail.com                                                  *
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

#ifndef _AISRADARPI_H_
#define _AISRADARPI_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif // precompiled headers

#include "version.h"

#include "ocpn_plugin.h"
#include "Sonarview.h"

#include <wx/display.h>

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define AISVIEW_TOOL_POSITION -1 // Request default positioning of toolbar tool

class sonar_pi : public opencpn_plugin_117
{
public:
    sonar_pi(void *ppimgr);
    ~sonar_pi();

    // The required PlugIn Methods
    int Init(void) override;
    bool DeInit(void) override;

    int GetAPIVersionMajor() override;
    int GetAPIVersionMinor() override;
    int GetPlugInVersionMajor() override;
    int GetPlugInVersionMinor() override;
    int GetPlugInVersionPatch() override;
    int GetPlugInVersionPost() override;
    wxBitmap *GetPlugInBitmap() override;
    wxString GetCommonName() override;
    wxString GetShortDescription() override;
    wxString GetLongDescription() override;
    wxBitmap m_panelBitmap;
    const char *GetPlugInVersionPre() override { return ""; }
    const char *GetPlugInVersionBuild() override { return ""; }
    // FIXME: Add real build info such as git hash and/or build number

    // from shipdriver for panel icon
    void SetDefaults(void) override;
    int GetToolbarToolCount(void) override;
    void ShowPreferencesDialog(wxWindow *parent) override;
    void OnToolbarToolCallback(int id) override;
    void SetAISSentence(wxString &sentence) override;
    void SetPositionFix(PlugIn_Position_Fix &pfix) override;
    void SetPluginMessage(wxString &message_id, wxString &message_body) override;
    void SetColorScheme(PI_ColorScheme cs) override;

    // Other public methods
    void SetSonarFrameX(int x) { m_sonar_frame_x = x; }
    void SetSonarFrameY(int x) { m_sonar_frame_y = x; }
    void SetSonarFrameSizeX(int x) { m_sonar_frame_sx = x; }
    void SetSonarFrameSizeY(int x) { m_sonar_frame_sy = x; }
    void SetSonarNorthUp(bool x) { m_sonar_north_up = x; }
    void SetSonarRange(int x) { m_sonar_range = x; }
    bool GetSonarNorthUp(void) { return m_sonar_north_up; }
    int GetSonarRange(void) { return m_sonar_range; }
    double GetCog(void) { return m_cog; }
    double GetSog(void) { return m_sog; }
    int GetSats(void) { return m_sats; }
    wxFileConfig *GetConfig(void) { return m_pconfig; }
    ArrayOfPlugIn_AIS_Targets *GetSonarTargets();
    void OnSonarFrameClose();

    // // 声明静态事件表和事件表条目
    void ToggleSonarFrame(); // 新方法
    void OnKeyPress(wxKeyEvent &event);

private:
    bool LoadConfig(void);
    bool SaveConfig(void);

private:
    wxFileConfig *m_pconfig;
    wxWindow *m_parent_window;
    SonarFrame *m_pSonarFrame;
    ArrayOfPlugIn_AIS_Targets *SonarTargets;
    int m_display_width, m_display_height;
    int m_leftclick_tool_id;
    int m_sonar_frame_x, m_sonar_frame_y;
    int m_sonar_frame_sx, m_sonar_frame_sy;
    int m_sonar_range;
    double m_lat;
    double m_lon;
    double m_cog;
    double m_sog;
    int m_sats;
    bool m_sonar_show_icon;
    bool m_sonar_use_sonar;
    bool m_sonar_north_up;
    wxCheckBox *m_pSonarShowIcon;
    wxCheckBox *m_pSonarUseSonar;
    // wxBitmap          m_panelBitmap;
};

#endif

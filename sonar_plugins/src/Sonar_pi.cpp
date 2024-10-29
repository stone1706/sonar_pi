/******************************************************************************
 * $Id:  $
 *
 * Project:  OpenCPN
 * Purpose:  Sonar Plugin
 * Author:   Johan van der Sman
 *
 ***************************************************************************
 * Frontend2  Author: Jon Gough  Version: 1.0.132
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
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // precompiled headers

#include <wx/fileconf.h>
#include "Sonar_pi.h"

#include "icons.h"
#include <wx/event.h>

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin *create_pi(void *ppimgr)
{
    return new sonar_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin *p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//    Sonar PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------
#include "icons.h"

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

void sonar_pi::OnKeyPress(wxKeyEvent &event)
{
    // wxMessageBox("Hello, World!", "Canvas MessageBox", wxOK | wxICON_INFORMATION);
    // WXK_NUMPAD3
    if (event.GetKeyCode() == '1' && event.ControlDown())
    {
        try
        {
            ToggleSonarFrame(); // 调用打开/关闭 SonarFrame 方法
            // m_parent_window->SetFocus();
        }
        catch (const std::exception &e)
        {
            ;
        }
    }
    event.Skip(); // 继续处理其他事件
}

sonar_pi::sonar_pi(void *ppimgr)
    : opencpn_plugin_117(ppimgr),
      m_pconfig(0),
      m_parent_window(0),
      m_pSonarFrame(0),
      SonarTargets(0),
      m_display_width(0),
      m_display_height(0),
      m_leftclick_tool_id(0),
      m_sonar_frame_x(0),
      m_sonar_frame_y(0),
      m_sonar_frame_sx(0),
      m_sonar_frame_sy(0),
      m_sonar_range(0),
      m_lat(0.0),
      m_lon(0.0),
      m_cog(0.0),
      m_sog(0.0),
      m_sats(0),
      m_sonar_show_icon(true),
      m_sonar_use_sonar(true),
      m_sonar_north_up(false),
      m_pSonarShowIcon(0),
      m_pSonarUseSonar(0)
{

    initialize_images();

    // Create the PlugIn icons  -from shipdriver
    // loads png file for the listing panel icon
    wxFileName fn;
    auto path = GetPluginDataDir("Sonar_pi");
    fn.SetPath(path);
    fn.AppendDir("data");
    fn.SetFullName("sonar_panel.png");

    path = fn.GetFullPath();

    wxInitAllImageHandlers();

    wxLogDebug(wxString("Using icon path: ") + path);
    if (!wxImage::CanRead(path))
    {
        wxLogDebug("Initiating image handlers.");
        wxInitAllImageHandlers();
    }
    wxImage panelIcon(path);
    if (panelIcon.IsOk())
        m_panelBitmap = wxBitmap(panelIcon);
    else
        wxLogWarning("Sonar Panel icon has NOT been loaded");
}

sonar_pi::~sonar_pi()
{
    if (SonarTargets)
    {
        WX_CLEAR_ARRAY(*SonarTargets);
        delete SonarTargets;
        SonarTargets = 0;
    }
}

int sonar_pi::Init(void)
{
    AddLocaleCatalog(_T("opencpn-sonar_pi"));
    m_sonar_frame_x = m_sonar_frame_y = 0;
    m_sonar_frame_sx = m_sonar_frame_sy = 200;
    m_pSonarFrame = 0;
    m_lat = m_lon = 0.;
    m_cog = m_sog = 0.;
    m_sats = 0;
    ::wxDisplaySize(&m_display_width, &m_display_height);
    m_pconfig = GetOCPNConfigObject();
    LoadConfig();
    if (SonarTargets)
    { // Init may be called more than once, check for cleanup
        WX_CLEAR_ARRAY(*SonarTargets);
        delete SonarTargets;
    }
    m_parent_window = GetOCPNCanvasWindow();
    m_parent_window->Bind(wxEVT_KEY_DOWN, &sonar_pi::OnKeyPress, this);

    if (m_sonar_show_icon)
    {

        // FOR SVG ICONS  - CMakeLists.txt line 72  PLUGIN_USE_SVG=ON

#ifdef PLUGIN_USE_SVG
        m_leftclick_tool_id = InsertPlugInToolSVG(_T( "Sonar" ),
                                                  _svg_sonar, _svg_sonar_toggled, _svg_sonar_toggled,
                                                  wxITEM_CHECK, _("Sonar"), _T( "" ), NULL, AISVIEW_TOOL_POSITION, 0, this);
#else
        m_leftclick_tool_id = InsertPlugInTool(_T(""), _img_sonar_pi, _img_sonar_pi, wxITEM_CHECK,
                                               _("SonarView"), _T(""), NULL,
                                               AISVIEW_TOOL_POSITION, 0, this);
#endif
    }

    SonarTargets = GetAISTargetArray();
    return (WANTS_TOOLBAR_CALLBACK | INSTALLS_TOOLBAR_TOOL |
            WANTS_CONFIG | WANTS_AIS_SENTENCES |
            WANTS_NMEA_EVENTS | WANTS_PLUGIN_MESSAGING | USES_AUI_MANAGER);
}
// Removed WANTS_PREFERENCES | before Sonar

bool sonar_pi::DeInit(void)
{
    if (m_pSonarFrame)
    {
        m_pSonarFrame->Close();
    }
    return true;
}

int sonar_pi::GetAPIVersionMajor()
{
    return OCPN_API_VERSION_MAJOR;
}

int sonar_pi::GetAPIVersionMinor()
{
    return OCPN_API_VERSION_MINOR;
}

int sonar_pi::GetPlugInVersionMajor()
{
    return PLUGIN_VERSION_MAJOR;
}

int sonar_pi::GetPlugInVersionMinor()
{
    return PLUGIN_VERSION_MINOR;
}

int sonar_pi::GetPlugInVersionPatch()
{
    return PLUGIN_VERSION_PATCH;
}

int sonar_pi::GetPlugInVersionPost()
{
    return PLUGIN_VERSION_TWEAK;
}

// Shipdriver uses the climatology_panel.png file to make the bitmap.
wxBitmap *sonar_pi::GetPlugInBitmap() { return &m_panelBitmap; }
// End of shipdriver process

wxString sonar_pi::GetCommonName()
{
    return _T(PLUGIN_COMMON_NAME);
}

wxString sonar_pi::GetShortDescription()
{
    return _(PLUGIN_SHORT_DESCRIPTION);
}

wxString sonar_pi::GetLongDescription()
{
    return _(PLUGIN_LONG_DESCRIPTION);
}

void sonar_pi::SetDefaults(void)
{
}

int sonar_pi::GetToolbarToolCount(void)
{
    return 1;
}

void sonar_pi::ShowPreferencesDialog(wxWindow *parent)
{
    // 创建一个新的对话框，parent参数指定了对话框的父窗口
    wxDialog *dialog = new wxDialog(parent, wxID_ANY, _("Sonar view Preferences"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    int border_size = 4;

    wxColor cl;
    // 获取一个全局颜色，并将其设置为对话框的背景颜色
    GetGlobalColor(_T("DILG1"), &cl);
    dialog->SetBackgroundColour(cl);

    // 创建一个垂直的盒子布局管理器
    wxBoxSizer *PanelSizer = new wxBoxSizer(wxVERTICAL);
    // 将布局管理器设置到对话框中
    dialog->SetSizer(PanelSizer);

    //  Sonar toolbox icon checkbox
    // 创建一个静态框，用于包含与Sonar相关的选项
    wxStaticBox *SonarBox = new wxStaticBox(dialog, wxID_ANY, _("Sonar"));
    // 创建一个与静态框关联的布局管理器
    wxStaticBoxSizer *SonarBoxSizer = new wxStaticBoxSizer(SonarBox, wxVERTICAL);
    // 将静态框布局管理器添加到对话框的主布局管理器中
    PanelSizer->Add(SonarBoxSizer, 0, wxGROW | wxALL, border_size);

    // 创建一个复选框，用于控制是否显示Sonar图标
    m_pSonarShowIcon = new wxCheckBox(dialog, -1, _("Show Sonar icon:"), wxDefaultPosition, wxSize(-1, -1), 0);
    // 将复选框添加到静态框布局管理器中
    SonarBoxSizer->Add(m_pSonarShowIcon, 1, wxALIGN_LEFT | wxALL, border_size);
    // 设置复选框的值
    m_pSonarShowIcon->SetValue(m_sonar_show_icon);

    // 创建另一个复选框，用于控制是否使用Sonar作为雷达源
    m_pSonarUseSonar = new wxCheckBox(dialog, -1, _("Use Sonar as radar source:"), wxDefaultPosition, wxSize(-1, -1), 0);
    // 将复选框添加到静态框布局管理器中
    SonarBoxSizer->Add(m_pSonarUseSonar, 2, wxALIGN_LEFT | wxALL, border_size);
    // 设置复选框的值
    m_pSonarUseSonar->SetValue(m_sonar_use_sonar);

    // 创建一个标准对话框按钮布局管理器，包含OK和Cancel按钮
    wxStdDialogButtonSizer *DialogButtonSizer = dialog->CreateStdDialogButtonSizer(wxOK | wxCANCEL);
    // 将按钮布局管理器添加到对话框的主布局管理器中
    PanelSizer->Add(DialogButtonSizer, 0, wxALIGN_RIGHT | wxALL, 5);
    // 使对话框的尺寸适应其内容
    dialog->Fit();
    // 显示对话框，并等待用户响应
    if (dialog->ShowModal() == wxID_OK)
    {
        //    Show Icon changed value?
        // 检查是否更改了“显示图标”的值
        if (m_sonar_show_icon != m_pSonarShowIcon->GetValue())
        {
            // 更新成员变量以反映新值
            m_sonar_show_icon = m_pSonarShowIcon->GetValue();
            // 根据复选框的值，添加或移除工具栏图标
            if (m_sonar_show_icon)
            {
#ifdef PLUGIN_USE_SVG
                m_leftclick_tool_id = InsertPlugInToolSVG(_T( "Sonar" ),
                                                          _svg_sonar, _svg_sonar_rollover, _svg_sonar_toggled,
                                                          wxITEM_CHECK, _("Sonar"), _T( "" ), NULL, AISVIEW_TOOL_POSITION, 0, this);
#else
                m_leftclick_tool_id = InsertPlugInTool(_T(""), _img_sonar_pi, _img_sonar_pi, wxITEM_CHECK,
                                                       _("SonarView"), _T(""), NULL,
                                                       AISVIEW_TOOL_POSITION, 0, this);
#endif
            }
            else
            {
                RemovePlugInTool(m_leftclick_tool_id);
            }
        }
        m_sonar_use_sonar = m_pSonarUseSonar->GetValue();
        SaveConfig();
    }

    delete dialog;
}

void sonar_pi::ToggleSonarFrame()
{
    ::wxBell();
    if (!m_pSonarFrame)
    {
        m_pSonarFrame = new SonarFrame();
        m_pSonarFrame->Create(m_parent_window,
                              this,
                              -1,
                              wxString::Format(_T("Sonar View %d.%d"),
                                               PLUGIN_VERSION_MAJOR,
                                               PLUGIN_VERSION_MINOR),
                              wxPoint(m_sonar_frame_x, m_sonar_frame_y),
                              wxSize(m_sonar_frame_sx, m_sonar_frame_sy));
        m_pSonarFrame->Show();
    }
    else
    {
        m_pSonarFrame->Close(true);
    }
}

void sonar_pi::OnToolbarToolCallback(int id)
{
    ToggleSonarFrame();
}

void sonar_pi::SetAISSentence(wxString &sentence)
{
    // Ignore the Sonar message itself. It is merely used as an event
    // to retrieve the Sonar targets from the mainprogram
    // The targets are already updated to reflect the current message
    // So we re-use that information
    if (m_sonar_use_sonar)
    {
        GetSonarTargets();
    }
    if (m_pSonarFrame)
    {
        m_pSonarFrame->Refresh();
    }
}

void sonar_pi::SetPositionFix(PlugIn_Position_Fix &pfix)
{
    // Check if our position changed
    // If so, update view otherwise not,
    if (m_lat != pfix.Lat || m_lon != pfix.Lon || m_cog != pfix.Cog || m_sog != pfix.Sog)
    {
        m_lat = pfix.Lat;
        m_lon = pfix.Lon;
        if (pfix.Cog >= 0.0)
        {
            m_cog = pfix.Cog;
        }
        if (pfix.Sog >= 0.0)
        {
            m_sog = pfix.Sog;
        }
        m_sats = pfix.nSats;
        if (m_pSonarFrame)
        {
            m_pSonarFrame->Refresh();
        }
    }
}

void sonar_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
}

void sonar_pi::OnSonarFrameClose()
{
    m_pSonarFrame = 0;
    SaveConfig();
}

bool sonar_pi::ShowMoored(void)
{
    bool Result = true;
    m_pconfig->SetPath(_T( "/Settings/Sonar" ));
    m_pconfig->Read(_T( "bShowMooredTargets" ), &Result, 1);
    return Result;
}

double sonar_pi::GetMooredSpeed(void)
{
    double Result = 0.;
    m_pconfig->SetPath(_T( "/Settings/Sonar" ));
    m_pconfig->Read(_T( "MooredTargetMaxSpeedKnots" ), &Result, 0.0);
    return Result;
}

bool sonar_pi::ShowCogArrows(void)
{
    bool Result = true;
    m_pconfig->SetPath(_T( "/Settings/Sonar" ));
    m_pconfig->Read(_T("bShowCOGArrows"), &Result, 1);
    return Result;
}

int sonar_pi::GetCogArrowMinutes(void)
{
    int Result = 6;
    m_pconfig->SetPath(_T( "/Settings/Sonar" ));
    m_pconfig->Read(_T("CogArrowMinutes"), &Result, 6);
    return Result;
}

void sonar_pi::SetColorScheme(PI_ColorScheme cs)
{
    // Colours changed, pass the event on to the radarframe
    if (m_pSonarFrame)
    {
        m_pSonarFrame->SetColourScheme(cs);
        DimeWindow(m_pSonarFrame);
    }
}

ArrayOfPlugIn_AIS_Targets *sonar_pi::GetSonarTargets()
{
    if (SonarTargets)
    {
        WX_CLEAR_ARRAY(*SonarTargets);
        delete SonarTargets;
    }
    SonarTargets = GetAISTargetArray();
    return SonarTargets;
}

bool sonar_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;
    if (pConf)
    {
        pConf->SetPath(_T( "/Plugins/Sonar" ));
        pConf->Read(_T( "ShowRADARIcon" ), &m_sonar_show_icon, 1);
        pConf->Read(_T( "UseSonarRadar" ), &m_sonar_use_sonar, 1);
        pConf->Read(_T( "NorthUp" ), &m_sonar_north_up, 0);
        m_sonar_frame_sx = pConf->Read(_T ( "RADARDialogSizeX" ), 300L);
        m_sonar_frame_sy = pConf->Read(_T ( "RADARDialogSizeY" ), 300L);
        m_sonar_frame_x = pConf->Read(_T ( "RADARDialogPosX" ), 50L);
        m_sonar_frame_y = pConf->Read(_T ( "RADARDialogPosY" ), 170L);
        m_sonar_range = pConf->Read(_T ( "RADARRange" ), 4L);
        return true;
    }
    else
    {
        return false;
    }
}

bool sonar_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;
    if (pConf)
    {
        pConf->SetPath(_T ( "/Plugins/Sonar" ));
        pConf->Write(_T ( "ShowRADARIcon" ), m_sonar_show_icon);
        pConf->Write(_T ( "UseSonarRadar" ), m_sonar_use_sonar);
        pConf->Write(_T ( "NorthUp" ), m_sonar_north_up);
        pConf->Write(_T ( "RADARDialogSizeX" ), m_sonar_frame_sx);
        pConf->Write(_T ( "RADARDialogSizeY" ), m_sonar_frame_sy);
        pConf->Write(_T ( "RADARDialogPosX" ), m_sonar_frame_x);
        pConf->Write(_T ( "RADARDialogPosY" ), m_sonar_frame_y);
        pConf->Write(_T ( "RADARRange" ), m_sonar_range);
        return true;
    }
    else
    {
        return false;
    }
}

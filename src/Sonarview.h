/******************************************************************************
 * $Id:  $
 *
 * Project:  OpenCPN
 * Purpose:  Sonar view Plugin
 * Author:   Johan van der Sman
 *
 ***************************************************************************
 *   Copyright (C) 2015 Johan van der Sman                                 *
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

#ifndef _AISVIEW_H_
#define _AISVIEW_H_

#include "wx/wxprec.h"
#include "wx/dcbuffer.h"
#include "ocpn_plugin.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif // precompiled headers

#define ID_OK 10001
#define MIN_RADIUS 150
#define TEXT_MARGIN 5
#define SPACER_MARGIN 5
#ifdef WIN32
#define MyFit(a) Fit(a)
#else
#define MyFit(a) FitInside(a)
#endif

class sonar_pi;

class ViewState
{
public:
    ViewState(const wxPoint &p, const wxSize &s) : Pos(p), Size(s) {};
    wxPoint GetPos() { return Pos; }
    wxSize GetWindowSize()
    {
#ifdef WIN32
        return wxDefaultSize;
#else
        return Size;
#endif
    }
    wxSize GetCanvasSize()
    {
#ifdef WIN32
        return Size;
#else
        return wxDefaultSize;
#endif
    }
    wxSize GetSize() { return Size; }
    void SetPos(const wxPoint &p) { Pos = p; }
    void SetWindowSize(const wxSize &s)
    {
#ifndef WIN32
        Size = s;
#endif
    }
    void SetCanvasSize(const wxSize &s)
    {
#ifdef WIN32
        Size = s;
#endif
    }

private:
    wxPoint Pos;
    wxSize Size;
};

//----------------------------------------------------------------------------------------------------------
//    AIS View Specification
//----------------------------------------------------------------------------------------------------------
class SonarFrame : public wxDialog
{
    DECLARE_CLASS(SonarFrame)
    DECLARE_EVENT_TABLE()

public:
    SonarFrame();
    ~SonarFrame();
    void Init();
    void Config(int min_radius);
    bool Create(wxWindow *parent, sonar_pi *ppi, wxWindowID id = wxID_ANY,
                const wxString &caption = _("Sonar Display"),
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize);
    void OnRestorationButtonClick(wxCommandEvent &event);
    void SetColourScheme(PI_ColorScheme cs);
    void OnLeftMouse(const wxPoint &curpos);
    void OnLeftMouseMove(const wxPoint &curpos);
    void OnLeftMouseRelease(const wxPoint &curpos);
    void OnMouseScroll(const int rotation);
    void paintEvent(wxPaintEvent &event);
    void resize_img_data(wxBitmap *img_data, float size);

    sonar_pi *pPlugIn;
    wxPoint img_position;
    float img_data_size;

private:
    void OnClose(wxCloseEvent &event);
    void OnNorthUp(wxCommandEvent &event);
    void OnTimer(wxTimerEvent &event);
    void OnMove(wxMoveEvent &event);
    void OnSize(wxSizeEvent &event);
    void render(wxDC &dc);
    void renderRange(wxDC &dc, wxPoint &center, wxSize &size, int radius);

    //    Data
    wxWindow *pParent;

    wxTimer *m_Timer;
    wxPanel *m_pCanvas;
    wxCheckBox *m_pNorthUp;
    wxColor m_BgColour;
    double m_Ebl;
    int m_Range;
    ViewState *m_pViewState;
    wxBitmap *img_data;

    bool m_isDragging;
};

#endif

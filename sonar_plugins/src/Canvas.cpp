/******************************************************************************
 * $Id:   $
 *
 * Project:  OpenCPN
 * Purpose:  Clickable canvas to draw upon
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
 *
 */
//
// wxWidgets sends the mouse movement and mouse clicks to the windows objects
// In this case the panel object that acts as drawing canvas
// This helper class receives the events and informs the radar view object that owns
// the canvas
//

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif // precompiled headers

#include "Canvas.h"

BEGIN_EVENT_TABLE(Canvas, wxPanel)
EVT_MOTION(Canvas::mouseMoved)
EVT_LEFT_DOWN(Canvas::mouseDown)
EVT_LEFT_UP(Canvas::mouseReleased)
EVT_MOUSEWHEEL(Canvas::mouseScroll)
EVT_PAINT(Canvas::paintEvent)
EVT_KEY_DOWN(Canvas::OnKeyDown) // 绑定按键事件
END_EVENT_TABLE()

// 重写键盘事件处理方法
void Canvas::OnKeyDown(wxKeyEvent &event)
{
    try
    {
        if (event.GetKeyCode() == '1' && event.ControlDown())
        {

            pv->pPlugIn->ToggleSonarFrame(); // 调用打开/关闭 SonarFrame 方法
        }
        // WXK_LEFT,
        // WXK_RIGHT,
        if (event.GetKeyCode() == WXK_UP)
        {
            pv->img_data_size += 0.1;
            pv->Refresh();
            this->SetFocus();
        }
        if (event.GetKeyCode() == WXK_DOWN)
        {
            pv->img_data_size -= 0.1;
            pv->Refresh();
            this->SetFocus();
        }
    }
    catch (const std::exception &e)
    {
        ;
    }
    event.Skip(); // 继续处理其他事件

    // wxMessageBox("Hello, World!", "Canvas MessageBox", wxOK | wxICON_INFORMATION);
}

Canvas::Canvas(wxWindow *parent, SonarFrame *view,
               wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxPanel(parent, id, pos, size),
      pv(view),
      MouseDown(false),
      Parent(view)
{
}

void Canvas::mouseMoved(wxMouseEvent &event)
{
    if (MouseDown)
    {
        if (pv)
        {
            // pv->OnLeftMouse(event.GetPosition());
            pv->OnLeftMouseMove(event.GetPosition());
        }
    }
    event.Skip();
}

void Canvas::mouseDown(wxMouseEvent &event)
{
    MouseDown = true;
    if (pv)
    {
        pv->OnLeftMouse(event.GetPosition());
    }
    event.Skip();
}

void Canvas::mouseReleased(wxMouseEvent &event)
{
    MouseDown = false;
    if (pv)
    {
        pv->OnLeftMouseRelease(event.GetPosition());
    }
    event.Skip();
}

void Canvas::mouseScroll(wxMouseEvent &event)
{
    if (pv)
    {
        pv->OnMouseScroll(event.GetWheelRotation());
    }
    event.Skip();
}

void Canvas::paintEvent(wxPaintEvent &event)
{
    Parent->paintEvent(event);
}

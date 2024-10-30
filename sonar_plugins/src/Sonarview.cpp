/******************************************************************************
 * $Id:   $
 *
 * Project:  OpenCPN
 * Purpose:  SonarView Object
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
#include "wx/wx.h"
#include <wx/debug.h>
#include <wx/fileconf.h>
#include <math.h>
#include "Sonar_pi.h"
#include "Canvas.h"

enum Ids
{
    cbNorthUpId = 10002,
    tmRefreshId,
    plCanvasId,
};

static const int RESTART = -1;

//---------------------------------------------------------------------------------------
//          Sonar Dialog Implementation
//---------------------------------------------------------------------------------------
IMPLEMENT_CLASS(SonarFrame, wxDialog)

BEGIN_EVENT_TABLE(SonarFrame, wxDialog)

EVT_CLOSE(SonarFrame::OnClose)
EVT_MOVE(SonarFrame::OnMove)
EVT_SIZE(SonarFrame::OnSize)
EVT_CHECKBOX(cbNorthUpId, SonarFrame::OnNorthUp)
EVT_TIMER(tmRefreshId, SonarFrame::OnTimer)

END_EVENT_TABLE()

SonarFrame::SonarFrame()
    : pParent(0),
      pPlugIn(0),
      m_Timer(0),
      m_pCanvas(0),
      m_pNorthUp(0),
      m_BgColour(),
      m_Ebl(0.0),
      m_Range(0),
      m_pViewState(0)
{
    Init();
}

SonarFrame::~SonarFrame()
{
}

void SonarFrame::Init()
{
    m_isDragging = false;
    img_position.x = 0;
    img_position.y = 0;
    img_data_size = 1;
    GetGlobalColor(_T("DILG1"), &m_BgColour);
    SetBackgroundColour(m_BgColour);

    wxFileName fn;
    auto path = GetPluginDataDir("Sonar_pi");
    fn.SetPath(path);
    fn.AppendDir("data");
    fn.SetFullName("test.png");
    path = fn.GetFullPath();
    img_data = new wxBitmap(path, wxBITMAP_TYPE_PNG);
}

bool SonarFrame::Create(wxWindow *parent, sonar_pi *ppi, wxWindowID id,
                        const wxString &caption,
                        const wxPoint &pos, const wxSize &size)
{
    pParent = parent;
    pPlugIn = ppi;
    long wstyle = (wxSYSTEM_MENU |
                   wxRESIZE_BORDER |
                   wxMAXIMIZE_BOX |
                   wxCLOSE_BOX |
                   wxCAPTION |
                   wxCLIP_CHILDREN);

    // long wstyle = wxDEFAULT_FRAME_STYLE;
    m_pViewState = new ViewState(pos, size);
    if (!wxDialog::Create(parent, id, caption, pos, m_pViewState->GetWindowSize(), wstyle))
    {
        return false;
    }
    this->CenterOnScreen(); // 先居中
    this->Move(pos);        // 然后移动到指定位置

    // Add panel to contents of frame
    wxPanel *panel = new wxPanel(this, plCanvasId);
    panel->SetAutoLayout(true);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(vbox);

    // Add Canvas panel to draw upon
    // Use the stored size provided by the pi
    // Create a square box based on the smallest side
    wxBoxSizer *canvas = new wxBoxSizer(wxHORIZONTAL);
    m_pCanvas = new Canvas(panel, this, wxID_ANY, pos, m_pViewState->GetCanvasSize());
    m_pCanvas->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    wxBoxSizer *cbox = new wxBoxSizer(wxVERTICAL);
    cbox->FitInside(m_pCanvas);
    canvas->Add(m_pCanvas, 1, wxEXPAND);
    vbox->Add(canvas, 1, wxALL | wxEXPAND, 5);

    // Add controls
    // 创建一个静态框
    wxStaticBox *sb = new wxStaticBox(panel, wxID_ANY, _("Options"));
    wxStaticBoxSizer *controls = new wxStaticBoxSizer(sb, wxHORIZONTAL);
    // 创建一个按钮
    wxButton *button = new wxButton(panel, wxID_ANY, _("restoration"));
    // 将按钮添加到静态框的布局中
    controls->Add(button, 0, wxALL | wxCENTER, 5);
    button->Bind(wxEVT_BUTTON, &SonarFrame::OnRestorationButtonClick, this);
    // 将静态框的布局添加到主垂直布局中

    m_pNorthUp = new wxCheckBox(panel, cbNorthUpId, _("North Up"));
    m_pNorthUp->SetValue(pPlugIn->GetSonarNorthUp());
    controls->Add(m_pNorthUp, 0, wxLEFT, 10);
    // 将静态框的布局添加到主垂直布局中
    vbox->Add(controls, 0, wxEXPAND | wxALL, 5);

    // Add timer
    m_Timer = new wxTimer(this, tmRefreshId);
    m_Timer->Start(2000);

    vbox->MyFit(this);

    return true;
}

void SonarFrame::OnRestorationButtonClick(wxCommandEvent &event)
{
    // 在这里处理按钮点击事件
    img_position.x = 0;
    img_position.y = 0;
    img_data_size = 1;
}

void SonarFrame::SetColourScheme(PI_ColorScheme cs)
{
    GetGlobalColor(_T("DILG1"), &m_BgColour);
    SetBackgroundColour(m_BgColour);
    this->Refresh();
}

void SonarFrame::OnClose(wxCloseEvent &event)
{
    // Stop timer if still running
    m_Timer->Stop();
    delete m_Timer;

    // Save window size
    pPlugIn->SetSonarFrameX(m_pViewState->GetPos().x);
    pPlugIn->SetSonarFrameY(m_pViewState->GetPos().y);
    pPlugIn->SetSonarFrameSizeX(m_pViewState->GetSize().GetWidth());
    pPlugIn->SetSonarFrameSizeY(m_pViewState->GetSize().GetHeight());

    // Cleanup
    RequestRefresh(pParent);
    Destroy();
    pPlugIn->OnSonarFrameClose();
}

void SonarFrame::OnNorthUp(wxCommandEvent &event)
{
    pPlugIn->SetSonarNorthUp(m_pNorthUp->GetValue());
    if (m_pNorthUp->GetValue())
    {
        m_Ebl += pPlugIn->GetCog();
    }
    else
    {
        m_Ebl -= pPlugIn->GetCog();
    }
    this->Refresh();
}

void SonarFrame::OnTimer(wxTimerEvent &event)
{
    this->Refresh();
}

static wxPoint m_lastMousePos;
void SonarFrame::OnLeftMouseRelease(const wxPoint &curpos)
{
    // 停止拖动
    m_isDragging = false;
}

void SonarFrame::OnLeftMouse(const wxPoint &curpos)
{
    m_lastMousePos = curpos;
    if (!m_isDragging)
    {
        // 开始拖动
        m_isDragging = true;

        // 请求窗口重绘以更新显示
        this->Refresh();
    }
}

void SonarFrame::OnLeftMouseMove(const wxPoint &curpos)
{
    if (m_isDragging)
    {
        // 计算移动的距离
        int deltaX = curpos.x - m_lastMousePos.x;
        int deltaY = curpos.y - m_lastMousePos.y;
        // 更新图像位置
        img_position.x += deltaX;
        img_position.y += deltaY;

        // 更新最后鼠标位置
        m_lastMousePos = curpos;

        // 请求窗口重绘以更新显示
        this->Refresh();
    }
}

void SonarFrame::OnMove(wxMoveEvent &event)
{
    wxPoint p = event.GetPosition();

    // Save window position
    m_pViewState->SetPos(wxPoint(p.x, p.y));
    event.Skip();
}

void SonarFrame::OnMouseScroll(const int rotation)
{
    if (rotation > 0)
    {
        img_data_size += 0.1;
    }
    if (rotation < 0)
    {
        img_data_size -= 0.1;
    }

    this->Refresh();
}

void SonarFrame::OnSize(wxSizeEvent &event)
{
    event.Skip();
    if (m_pCanvas)
    {
        wxClientDC dc(m_pCanvas);
        wxSize cs = m_pCanvas->GetClientSize();
        m_pViewState->SetCanvasSize(cs);
        m_pViewState->SetWindowSize(GetSize());
        render(dc);
    }
}

void SonarFrame::paintEvent(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(m_pCanvas);
    render(dc);
    event.Skip();
}

void SonarFrame::render(wxDC &dc)
{
    static int img_num = 1;
    // 启动或重启一个定时器，RESTART是定时器的时间间隔常量。
    m_Timer->Start(RESTART);
    // Calculate the size based on paint area size, if smaller then the minimum
    // then the default minimum size applies
    // 获取画布（Canvas）的尺寸，并计算绘制区域的宽度和高度。
    // 如果计算得到的尺寸小于最小尺寸（MIN_RADIUS*2），则使用最小尺寸。
    int width = std::max(m_pCanvas->GetSize().GetWidth(), (MIN_RADIUS) * 2);
    int height = std::max(m_pCanvas->GetSize().GetHeight(), (MIN_RADIUS) * 2);
    // 定义一个wxSize对象，表示绘制区域的尺寸。
    wxSize size(width, height);
    // 计算绘制区域的中心点坐标。
    wxPoint center(width / 2, height / 2);
    // 计算绘制区域的半径，取宽度和高度中较小值的一半和最小半径中的较大值。
    int radius = std::max((std::min(width, height) / 2), MIN_RADIUS);

    //    m_pCanvas->SetBackgroundColour (m_BgColour);
    delete img_data;
    wxFileName fn;
    auto path = GetPluginDataDir("Sonar_pi");
    fn.SetPath(path);
    fn.AppendDir("data");
    if (img_num == 1)
    {
        img_num = 2;
        fn.SetFullName("test1.png");
    }
    else if (img_num == 2)
    {
        img_num = 3;
        fn.SetFullName("test2.png");
    }
    else if (img_num == 3)
    {
        img_num = 1;
        fn.SetFullName("test3.png");
    }
    path = fn.GetFullPath();
    img_data = new wxBitmap(path, wxBITMAP_TYPE_PNG);
    resize_img_data(img_data, img_data_size);
    renderRange(dc, center, size, radius);
}

void SonarFrame::renderRange(wxDC &dc, wxPoint &center, wxSize &size, int radius)
{
    // Draw the circles
    dc.SetBackground(wxBrush(m_BgColour));
    dc.Clear();

    dc.DrawBitmap(*img_data, img_position.x, img_position.y, true);
}

void SonarFrame::resize_img_data(wxBitmap *img_data, float size)
{
    // 将wxBitmap转换为wxImage
    wxImage img = (*img_data).ConvertToImage();
    static int Width = int(img.GetWidth());
    static int Height = int(img.GetHeight());
    // 设置新的大小，这里假设你想要将图像放大到原来的两倍
    // 你可以根据需要调整这些值来放大或缩小图像
    int newWidth = int(Width * size);
    int newHeight = int(Height * size);

    // 调整图像大小
    img.Rescale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);

    // 将调整大小后的wxImage转换回wxBitmap
    *img_data = wxBitmap(img);
}
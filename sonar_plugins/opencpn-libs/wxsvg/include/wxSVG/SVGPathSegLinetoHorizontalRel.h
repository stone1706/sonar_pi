//////////////////////////////////////////////////////////////////////////////
// Name:        SVGPathSegLinetoHorizontalRel.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_PATH_SEG_LINETO_HORIZONTAL_REL_H
#define WX_SVG_PATH_SEG_LINETO_HORIZONTAL_REL_H

#include "SVGPathSeg.h"

class wxSVGPathSegLinetoHorizontalRel : public wxSVGPathSeg {
protected:
  double m_x;

public:
  inline double GetX() const { return m_x; }
  inline void SetX(double n) { m_x = n; }

public:
  wxSVGPathSegLinetoHorizontalRel()
      : wxSVGPathSeg(wxPATHSEG_LINETO_HORIZONTAL_REL), m_x(0) {}
  virtual ~wxSVGPathSegLinetoHorizontalRel() {}
};

#endif  // WX_SVG_PATH_SEG_LINETO_HORIZONTAL_REL_H
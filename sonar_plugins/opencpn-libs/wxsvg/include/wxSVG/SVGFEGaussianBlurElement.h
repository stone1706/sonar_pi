//////////////////////////////////////////////////////////////////////////////
// Name:        SVGFEGaussianBlurElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_FE_GAUSSIAN_BLUR_ELEMENT_H
#define WX_SVG_FE_GAUSSIAN_BLUR_ELEMENT_H

#include "SVGElement.h"
#include "SVGFilterPrimitiveStandardAttributes.h"
#include "SVGAnimatedString.h"
#include "SVGAnimatedNumber.h"
#include "String_wxsvg.h"
#include "Element.h"
#include "SVGAnimatedType.h"

class wxSVGFEGaussianBlurElement
    : public wxSVGElement,
      public wxSVGFilterPrimitiveStandardAttributes {
protected:
  wxSVGAnimatedString m_in1;
  wxSVGAnimatedNumber m_stdDeviationX;
  wxSVGAnimatedNumber m_stdDeviationY;
  bool HasCustomAttribute(const wxString& name) const;
  wxString GetCustomAttribute(const wxString& name) const;
  bool SetCustomAttribute(const wxString& name, const wxString& value);
  wxSvgXmlAttrHash GetCustomAttributes() const;
  bool SetCustomAnimatedValue(const wxString& name,
                              const wxSVGAnimatedType& value);

public:
  inline const wxSVGAnimatedString& GetIn1() const { return m_in1; }
  inline void SetIn1(const wxSVGAnimatedString& n) { m_in1 = n; }
  inline void SetIn1(const wxString& n) { m_in1.SetBaseVal(n); }

  inline const wxSVGAnimatedNumber& GetStdDeviationX() const {
    return m_stdDeviationX;
  }
  inline void SetStdDeviationX(const wxSVGAnimatedNumber& n) {
    m_stdDeviationX = n;
  }
  inline void SetStdDeviationX(float n) { m_stdDeviationX.SetBaseVal(n); }

  inline const wxSVGAnimatedNumber& GetStdDeviationY() const {
    return m_stdDeviationY;
  }
  inline void SetStdDeviationY(const wxSVGAnimatedNumber& n) {
    m_stdDeviationY = n;
  }
  inline void SetStdDeviationY(float n) { m_stdDeviationY.SetBaseVal(n); }

public:
  wxSVGFEGaussianBlurElement(wxString tagName = wxT("feGaussianBlur"))
      : wxSVGElement(tagName) {}
  virtual ~wxSVGFEGaussianBlurElement() {}
  wxSvgXmlNode* CloneNode(bool deep = true) {
    return new wxSVGFEGaussianBlurElement(*this);
  }
  virtual void SetStdDeviation(double stdDeviationX, double stdDeviationY);
  bool HasAttribute(const wxString& name) const;
  wxString GetAttribute(const wxString& name) const;
  bool SetAttribute(const wxString& name, const wxString& value);
  wxSvgXmlAttrHash GetAttributes() const;
  bool SetAnimatedValue(const wxString& name, const wxSVGAnimatedType& value);
  virtual wxSVGDTD GetDtd() const { return wxSVG_FEGAUSSIANBLUR_ELEMENT; }
};

#endif  // WX_SVG_FE_GAUSSIAN_BLUR_ELEMENT_H
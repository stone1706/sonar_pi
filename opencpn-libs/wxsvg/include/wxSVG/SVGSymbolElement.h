//////////////////////////////////////////////////////////////////////////////
// Name:        SVGSymbolElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_SYMBOL_ELEMENT_H
#define WX_SVG_SYMBOL_ELEMENT_H

#include "SVGElement.h"
#include "SVGLangSpace.h"
#include "SVGExternalResourcesRequired.h"
#include "SVGStylable.h"
#include "SVGFitToViewBox.h"
#include "EventTarget.h"
#include "String_wxsvg.h"
#include "Element.h"
#include "SVGAnimatedType.h"

class wxSVGSymbolElement : public wxSVGElement,
                           public wxSVGLangSpace,
                           public wxSVGExternalResourcesRequired,
                           public wxSVGStylable,
                           public wxSVGFitToViewBox,
                           public wxEventTarget {
public:
  wxSVGSymbolElement(wxString tagName = wxT("symbol"))
      : wxSVGElement(tagName) {}
  virtual ~wxSVGSymbolElement() {}
  wxSvgXmlNode* CloneNode(bool deep = true) {
    return new wxSVGSymbolElement(*this);
  }
  bool HasAttribute(const wxString& name) const;
  wxString GetAttribute(const wxString& name) const;
  bool SetAttribute(const wxString& name, const wxString& value);
  wxSvgXmlAttrHash GetAttributes() const;
  bool SetAnimatedValue(const wxString& name, const wxSVGAnimatedType& value);
  virtual wxSVGDTD GetDtd() const { return wxSVG_SYMBOL_ELEMENT; }
};

#endif  // WX_SVG_SYMBOL_ELEMENT_H
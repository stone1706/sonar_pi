//////////////////////////////////////////////////////////////////////////////
// Name:        SVGMatrix.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_MATRIX_H
#define WX_SVG_MATRIX_H

class wxSVGMatrix {
protected:
  double m_a;
  double m_b;
  double m_c;
  double m_d;
  double m_e;
  double m_f;

public:
  inline double GetA() const { return m_a; }
  inline void SetA(double n) { m_a = n; }

  inline double GetB() const { return m_b; }
  inline void SetB(double n) { m_b = n; }

  inline double GetC() const { return m_c; }
  inline void SetC(double n) { m_c = n; }

  inline double GetD() const { return m_d; }
  inline void SetD(double n) { m_d = n; }

  inline double GetE() const { return m_e; }
  inline void SetE(double n) { m_e = n; }

  inline double GetF() const { return m_f; }
  inline void SetF(double n) { m_f = n; }

public:
  wxSVGMatrix() : m_a(1), m_b(0), m_c(0), m_d(1), m_e(0), m_f(0) {}
  wxSVGMatrix(double a, double b, double c, double d, double e, double f)
      : m_a(a), m_b(b), m_c(c), m_d(d), m_e(e), m_f(f) {}
  virtual ~wxSVGMatrix() {}
  virtual wxSVGMatrix Multiply(const wxSVGMatrix& secondMatrix) const;
  virtual wxSVGMatrix Inverse() const;
  virtual wxSVGMatrix Translate(double x, double y) const;
  virtual wxSVGMatrix Scale(double scaleFactor) const;
  virtual wxSVGMatrix ScaleNonUniform(double scaleFactorX,
                                      double scaleFactorY) const;
  virtual wxSVGMatrix Rotate(double angle) const;
  virtual wxSVGMatrix RotateFromVector(double x, double y) const;
  virtual wxSVGMatrix FlipX() const;
  virtual wxSVGMatrix FlipY() const;
  virtual wxSVGMatrix SkewX(double angle) const;
  virtual wxSVGMatrix SkewY(double angle) const;
};

#endif  // WX_SVG_MATRIX_H
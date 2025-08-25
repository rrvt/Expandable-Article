// ExpandableExmplView.h : interface of the ExpandableExmplView class


#pragma once
#include "CScrView.h"
//#include "ExamplesDef.h"
#include "ReportNtpd.h"
#ifdef Examples
#include "StoreRpt.h"
#endif


class ExpandableExmplDoc;


class ExpandableExmplView : public CScrView {

#ifdef Examples

StoreRpt   dspStore;
StoreRpt   prtStore;

#endif

CMenu      menu;
CMenu      sub;

protected: // create from serialization only

  ExpandableExmplView() noexcept;

  DECLARE_DYNCREATE(ExpandableExmplView)

public:

  virtual           ~ExpandableExmplView() { }

  virtual void       initNoteOrietn();
  virtual void       saveNoteOrietn();
  virtual void       initRptOrietn();
  virtual void       saveRptOrietn();
  virtual PrtrOrient getOrientation() {return prtNote.prtrOrietn;}

  virtual BOOL       PreCreateWindow(CREATESTRUCT& cs);
  virtual void       OnInitialUpdate();

  virtual void       onDisplayOutput();
  virtual void       displayHeader(DevStream& dev);
  virtual void       displayFooter(DevStream& dev);

  virtual void       onPreparePrinting(CPrintInfo* info);
  virtual void       onBeginPrinting();
  virtual void       printHeader(DevStream& dev, int pageNo);
  virtual void       printFooter(DevStream& dev, int pageNo);
  virtual void       OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

  ExpandableExmplDoc*    GetDocument() const;

#ifdef Examples
  StoreRpt&          storeRpt()  {return dspStore;}
#endif

public:

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

public:

  DECLARE_MESSAGE_MAP()

  afx_msg void onOptions();
  afx_msg void onRptOrietn();

  afx_msg void OnSetFocus(CWnd* pOldWnd);

  afx_msg void OnLButtonDown(  UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

  afx_msg void OnContextMenu(  CWnd* ,      CPoint point);
  afx_msg void onCopy();
  afx_msg void onPup1();
  afx_msg void onPup2();
  };



#ifndef _DEBUG  // debug version in ExpandableExmplView.cpp
inline ExpandableExmplDoc* ExpandableExmplView::GetDocument() const
                                           {return reinterpret_cast<ExpandableExmplDoc*>(m_pDocument);}
#endif


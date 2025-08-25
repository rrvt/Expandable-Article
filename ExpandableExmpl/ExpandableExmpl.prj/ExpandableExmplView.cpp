// ExpandableExmplView.cpp : implementation of the ExpandableExmplView class


#include "pch.h"
#include "ExpandableExmplView.h"
#include "ExpandableExmpl.h"
#include "ExpandableExmplDoc.h"
#include "ClipLine.h"
#include "IniFileEx.h"
#include "ReportNtpd.h"
#include "OptionsDlg.h"
#include "Resource.h"
#include "ResourceData.h"
#include "RptOrientDlgTwo.h"
//#include "Store.h"


static TCchar* StrOrietnKey = _T("Store");


IMPLEMENT_DYNCREATE(ExpandableExmplView, CScrView)

BEGIN_MESSAGE_MAP(ExpandableExmplView, CScrView)
  ON_COMMAND(ID_Options,     &onOptions)
  ON_COMMAND(ID_Orientation, &onRptOrietn)

  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONDBLCLK()

  ON_WM_CONTEXTMENU()                     // Right Mouse Popup Menu
  ON_COMMAND(ID_CopySel, &onCopy)
  ON_COMMAND(ID_Pup1,    &onPup1)
  ON_COMMAND(ID_Pup2,    &onPup2)

END_MESSAGE_MAP()


#ifdef Examples
ExpandableExmplView::ExpandableExmplView() noexcept : dspStore(dMgr.getNotePad()),
                                              prtStore(pMgr.getNotePad()) {
#else
ExpandableExmplView::ExpandableExmplView() noexcept {
#endif
ResourceData res;
String       pn;
  if (res.getProductName(pn)) prtNote.setTitle(pn);

  sub.LoadMenu(IDR_RtMouseMenu);
  menu.CreatePopupMenu();
  menu.AppendMenu(MF_POPUP, (UINT_PTR) sub.GetSafeHmenu(), _T(""));        //

  sub.Detach();
  }


BOOL ExpandableExmplView::PreCreateWindow(CREATESTRUCT& cs) {return CScrView::PreCreateWindow(cs);}


void ExpandableExmplView::OnInitialUpdate() {CScrView::OnInitialUpdate();}


void ExpandableExmplView::initNoteOrietn() {
  dspNote.prtrOrietn = prtNote.prtrOrietn =
                           (PrtrOrient) iniFile.readInt(RptOrietnSect, NoteOrietnKey, PortOrient);
  }


void ExpandableExmplView::saveNoteOrietn()
                                 {iniFile.write(RptOrietnSect, NoteOrietnKey, prtNote.prtrOrietn);}


void ExpandableExmplView::onOptions() {
OptionsDlg dlg;

  if (printer.name.isEmpty()) printer.load(0);

  if (dlg.DoModal() == IDOK) pMgr.setFontScale(printer.scale);
  }


void ExpandableExmplView::onRptOrietn() {
RptOrietnDlg dlg;

  dlg.lbl01 = _T("Store:");
  dlg.ntpd = printer.toStg(prtNote.prtrOrietn);
#ifdef Examples
  dlg.rpt1 = printer.toStg(prtStore.prtrOrietn);
#endif

  if (dlg.DoModal() == IDOK) {
    prtNote.prtrOrietn  = printer.toOrient(dlg.ntpd);
#ifdef Examples
    prtStore.prtrOrietn = printer.toOrient(dlg.rpt1);
#endif
    saveRptOrietn();
    }
  }


void ExpandableExmplView::initRptOrietn()
#ifdef Examples
  {prtStore.prtrOrietn = (PrtrOrient) iniFile.readInt(RptOrietnSect, StrOrietnKey, PortOrient);}
#else
  { }
#endif


void ExpandableExmplView::saveRptOrietn() {
  saveNoteOrietn();

#ifdef Examples
  iniFile.write(RptOrietnSect, StrOrietnKey,  (int) prtStore.prtrOrietn);
#endif
  }


void ExpandableExmplView::onDisplayOutput() {

  switch(doc()->dataSrc()) {
    case NotePadSrc : dspNote.display(*this); break;
#ifdef Examples
    case StoreSrc   : dspStore.display(*this); break;
#endif
    }
  }


void ExpandableExmplView::displayHeader(DevStream& dev) {
  switch(doc()->dataSrc()) {
    case NotePadSrc   : dspNote.dspHeader(dev);   break;
#ifdef Examples
    case StoreSrc     : dspStore.dspHeader(dev);  break;
#endif
    }
  }


void ExpandableExmplView::displayFooter(DevStream& dev) {
  switch(doc()->dataSrc()) {
    case NotePadSrc   : dspNote.dspFooter(dev);   break;
#ifdef Examples
    case StoreSrc     : dspStore.dspFooter(dev);  break;
#endif
    }
  }



void ExpandableExmplView::onPreparePrinting(CPrintInfo* info) {

  switch(doc()->dataSrc()) {
    case NotePadSrc : prtNote.onPreparePrinting(info);     break;
#ifdef Examples
    case StoreSrc   : prtStore.onPreparePrinting(info);    break;
#endif
    }
  }


// Perpare output (i.e. report) then start the output with the call to SCrView

void ExpandableExmplView::onBeginPrinting() {

  switch(doc()->dataSrc()) {
    case NotePadSrc : prtNote.onBeginPrinting(*this);  break;
#ifdef Examples
    case StoreSrc   : prtStore.onBeginPrinting(*this); break;
#endif
    }
  }


void ExpandableExmplView::printHeader(DevStream& dev, int pageNo) {
  switch(doc()->dataSrc()) {
    case NotePadSrc: prtNote.prtHeader(dev, pageNo);   break;
#ifdef Examples
    case StoreSrc  : dspStore.prtHeader(dev, pageNo);  break;
#endif
    }
  }


// The footer is injected into the printed output, so the output goes directly to the device.
// The output streaming functions are very similar to NotePad's streaming functions so it should
// not be a great hardship to construct a footer.

void ExpandableExmplView::printFooter(DevStream& dev, int pageNo) {
  switch(doc()->dataSrc()) {
    case NotePadSrc : prtNote.prtFooter(dev, pageNo);  break;
#ifdef Examples
    case StoreSrc   : prtStore.prtFooter(dev, pageNo); break;
#endif
    }
  }



void ExpandableExmplView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {

  switch(doc()->dataSrc()) {
    case NotePadSrc : break;
    case StoreSrc   : break;
    }
  }


void ExpandableExmplView::OnSetFocus(CWnd* pOldWnd) {

  CScrView::OnSetFocus(pOldWnd);

  switch(doc()->dataSrc()) {
    case NotePadSrc : break;
    case StoreSrc   : break;
    }
  }


void ExpandableExmplView::OnLButtonDown(UINT nFlags, CPoint point)
                   {clipLine.set(point);   invalidate();   CScrView::OnLButtonDown(nFlags, point);}


void ExpandableExmplView::OnLButtonDblClk(UINT nFlags, CPoint point) {
  clipLine.set(point);   RedrawWindow();   clipLine.load();

  CScrView::OnLButtonDblClk(nFlags, point);
  }


void ExpandableExmplView::OnContextMenu(CWnd* /*pWnd*/, CPoint point) {
CRect  rect;
CMenu* popup;
CWnd*  pWndPopupOwner = this;

  if (point.x == -1 && point.y == -1)
        {GetClientRect(rect);  ClientToScreen(rect);  point = rect.TopLeft();  point.Offset(5, 5);}

  popup = menu.GetSubMenu(0);   if (!popup) return;

  while (pWndPopupOwner->GetStyle() & WS_CHILD) pWndPopupOwner = pWndPopupOwner->GetParent();

  popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
  }


void ExpandableExmplView::onCopy() {clipLine.load();  invalidate();}


void ExpandableExmplView::onPup1() {  }


void ExpandableExmplView::onPup2() {  }


// ExpandableExmplView diagnostics

#ifdef _DEBUG
void ExpandableExmplView::AssertValid() const          {CScrollView::AssertValid();}
void ExpandableExmplView::Dump(CDumpContext& dc) const {CScrollView::Dump(dc);}
                                                                    // non-debug version is inline
ExpandableExmplDoc* ExpandableExmplView::GetDocument() const {              // non-debug version is inline
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ExpandableExmplDoc)));
  return (ExpandableExmplDoc*)m_pDocument;
  }
#endif //_DEBUG




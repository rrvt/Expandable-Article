// MainFrm.cpp : implementation of the MainFrame class


#include "pch.h"
#include "MainFrame.h"
#include "AboutDlg.h"


// MainFrame

IMPLEMENT_DYNCREATE(MainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWndEx)
  ON_WM_CREATE()
  ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, &OnResetToolBar)
  ON_WM_SYSCOMMAND()

  ON_WM_MOVE()
  ON_WM_SIZE()
END_MESSAGE_MAP()


MainFrame::MainFrame() noexcept : statusBar(2), isInitialized(false) { }

MainFrame::~MainFrame() {winPos.~WinPos();}


BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs) {

  cs.style &= ~FWS_ADDTOTITLE;  cs.lpszName = _T("Expandable Example");
                                                               // Sets the default title left part
  return CFrameWndEx::PreCreateWindow(cs);
  }


int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
CRect winRect;

  if (CFrameWndEx::OnCreate(lpCreateStruct) == -1) return -1;

  if (!menuBar.Create(this)) {TRACE0("Failed to create menubar\n"); return -1;}

  CMFCPopupMenu::SetForceMenuFocus(FALSE);

  if (!toolBar.create(this, IDR_MAINFRAME)) {TRACE0("Failed to create toolbar\n"); return -1;}

  addAboutToSysMenu(IDD_AboutBox, IDS_AboutBox);

  GetWindowRect(&winRect);   winPos.initialPos(this, winRect);

  if (!statusBar.create(this)) {TRACE0("Failed to create status bar\n"); return -1;}

  DockPane(&menuBar);   DockPane(&toolBar);

  CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
                                                                  // Affects look of toolbar, etc.
  isInitialized = true;   return 0;
  }


void MainFrame::OnSysCommand(UINT nID, LPARAM lParam) {

  if ((nID & 0xFFF0) == sysAboutID) {AboutDlg aboutDlg; aboutDlg.DoModal(); return;}

  CMainFrm::OnSysCommand(nID, lParam);
  }


void MainFrame::OnMove(int x, int y)
    {CRect winRect;   GetWindowRect(&winRect);   winPos.set(winRect);   CFrameWndEx::OnMove(x, y);}


void MainFrame::OnSize(UINT nType, int cx, int cy) {
CRect r;

  GetWindowRect(&r);

  if (!isInitialized) {winPos.setInvBdrs(r, cx, cy);   return;}

  winPos.set(cx, cy);   CFrameWndEx::OnSize(nType, cx, cy);

  statusBar.setup();
  }


// MainFrame message handlers

LRESULT MainFrame::OnResetToolBar(WPARAM wParam, LPARAM lParam) {setupToolBar();  return 0;}


void MainFrame::setupToolBar() {
CRect winRect;   GetWindowRect(&winRect);   toolBar.set(winRect);

  toolBar.addMenu(   ID_Menu,  IDR_TBMenu,  _T("Expandable Version"));
  toolBar.addMenu(   ID_Menu1, IDR_TBMenu1, _T("Operation on Expandable"));
  toolBar.addMenu(   ID_TBSaveMenu, IDR_TBSaveMenu,  3);


#ifdef Examples
  toolBar.addButton( ID_Button, _T("Load Combo"));
  toolBar.addEditBox(ID_EditBox, 20);
  toolBar.addCBx(    ID_CboBx);
#endif
  }


// MainFrame diagnostics

#ifdef _DEBUG
void MainFrame::AssertValid() const          {CFrameWndEx::AssertValid();}
void MainFrame::Dump(CDumpContext& dc) const {CFrameWndEx::Dump(dc);}
#endif //_DEBUG




////--------------------

#if 1
#else
  setupStatusBar();
  statusBar.SetIndicators(indicators, noElements(indicators));
  w = statusBar.GetPaneWidth(0) + statusBar.GetPaneWidth(1);

  statusBar.SetPaneWidth(0, w/2);   statusBar.SetPaneWidth(1, w/2);
#endif
#if 1
#else
  setupStatusBar();

  statusBar.SetIndicators(indicators, noElements(indicators));

  w = statusBar.GetPaneWidth(0) + statusBar.GetPaneWidth(1);

  statusBar.SetPaneWidth(0, w/2);   statusBar.SetPaneWidth(1, w/2);
#endif
#if 0
bool StatusBar::setParts() {
CRect rect;
int   parts[2];

  GetClientRect(&rect);   parts[0] = rect.right/2;  parts[1] =  -1;

  return SetParts(noElements(parts), parts);
  }
#endif

#if 0
void MainFrame::setupStatusBar() {
CRect rect;
int   percents[noElements(indicators)] = {41, 41, 6, 6, 6};
int   i;

  GetClientRect(&rect);

  statusBar.SetIndicators(indicators, noElements(indicators));

  for (i = 0; i < noElements(indicators); i++) {
    statusBar.SetPaneWidth(i, rect.right * percents[i] / 100);
    }
  }
#endif


// MainFrame.h : interface of the MainFrame class


#pragma once
#include "CMainFrm.h"
#include "CMFStatusBar.h"
#include "MyToolBar.h"
#include "WinPos.h"


class MainFrame : public CMainFrm {

CMFCMenuBar   menuBar;
MyToolBar     toolBar;
CMFStatusBar  statusBar;

bool          isInitialized;

protected:                                          // create from serialization only

  MainFrame() noexcept;

  DECLARE_DYNCREATE(MainFrame)

public:                                             // Overrides

  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

  virtual     ~MainFrame();

  MyToolBar&   getToolBar() {return toolBar;}

  void         setStatus(TCchar* txt)        {setStatus(0, txt);}
  void         setStatus(int i, TCchar* txt) {statusBar.setText(i, txt);}

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:                                          // Generated message map functions

  DECLARE_MESSAGE_MAP()

  afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg LRESULT OnResetToolBar(WPARAM wParam, LPARAM lParam);

private:

  void            setupToolBar();
//  void            setupStatusBar();

public:

  afx_msg void    OnSysCommand(UINT nID, LPARAM lParam);

  afx_msg void    OnMove(int x, int y);
  afx_msg void    OnSize(UINT nType, int cx, int cy);
  };



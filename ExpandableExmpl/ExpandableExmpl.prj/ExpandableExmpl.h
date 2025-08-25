// ExpandableExmpl.h : main header file for the ExpandableExmpl application

#pragma once
#include "CApp.h"
#include "MainFrame.h"


class ExpandableExmplDoc;
class ExpandableExmplView;


class ExpandableExmpl : public CApp {

String roamPath;
String appPath;

public:

               ExpandableExmpl() noexcept : CApp(this) { }
              ~ExpandableExmpl() { }

  virtual BOOL InitInstance();
  virtual int  ExitInstance();

          ExpandableExmplDoc*  doc()         {return (ExpandableExmplDoc*)  CApp::getDoc();}
          ExpandableExmplView* view()        {return (ExpandableExmplView*) CApp::getView();}
          MainFrame*           mainFrm()     {return (MainFrame*)           m_pMainWnd;}
          String&              roamingPath() {return roamPath;}
          String&              thisAppPath() {return appPath;}

  DECLARE_MESSAGE_MAP()

  afx_msg void onAppAbout();
  afx_msg void onHelp();
  };


extern ExpandableExmpl theApp;

inline void                 invalidate()                  {theApp.invalidate();}
inline ExpandableExmplDoc*  doc()                         {return theApp.doc();}
inline ExpandableExmplView* view()                        {return theApp.view();}
inline MainFrame*           mainFrm()                     {return theApp.mainFrm();}
inline MyToolBar&           getToolBar()                  {return mainFrm()->getToolBar();}
inline void                 setStatus(TCchar* txt)        {mainFrm()->setStatus(txt);}
inline void                 setStatus(int i, TCchar* txt) {mainFrm()->setStatus(i, txt);}

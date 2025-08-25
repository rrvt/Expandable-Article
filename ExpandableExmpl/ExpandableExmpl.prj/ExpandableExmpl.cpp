// ExpandableExmpl.cpp : Defines the class behaviors for the application.


#include "pch.h"
#include "ExpandableExmpl.h"
#include "AboutDlg.h"
#include "ExpandableExmplDoc.h"
#include "ExpandableExmplView.h"
#include "FileName.h"
#include "IniFileEx.h"
#include "NotePad.h"
#include "Resource.h"


ExpandableExmpl theApp;                       // The one and only ExpandableExmpl object
IniFileEx       iniFile(theApp);


// ExpandableExmpl

BEGIN_MESSAGE_MAP(ExpandableExmpl, CWinAppEx)
  ON_COMMAND(ID_Help,      &onHelp)
  ON_COMMAND(ID_App_About, &onAppAbout)
END_MESSAGE_MAP()


// ExpandableExmpl initialization

BOOL ExpandableExmpl::InitInstance() {

  CWinAppEx::InitInstance();

  iniFile.setAppDataPath(m_pszHelpFilePath);

  roamPath = getPath(iniFile.getAppDataPath(m_pszHelpFilePath));
  appPath  = getPath(m_pszHelpFilePath);

  notePad.clear();

  SetRegistryKey(appID);

  LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  // serve as the connection between documents, frame windows and views

  CSingleDocTemplate* pDocTemplate;

  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(ExpandableExmplDoc),
    RUNTIME_CLASS(MainFrame),       // main SDI frame window
    RUNTIME_CLASS(ExpandableExmplView));

  if (!pDocTemplate) return FALSE;

  AddDocTemplate(pDocTemplate);

  // Parse command line for standard shell commands, DDE, file open

  CCommandLineInfo cmdInfo;   ParseCommandLine(cmdInfo);

  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.

  if (!ProcessShellCommand(cmdInfo)) return FALSE;

  setAppName(_T("Expandable Example")); setTitle(_T("Main"));

  view()->setFont(_T("Arial"), 12.0);

  m_pMainWnd->ShowWindow(SW_SHOW);   m_pMainWnd->UpdateWindow();   return TRUE;
  }



int ExpandableExmpl::ExitInstance() {

  return CApp::ExitInstance();
  }


void ExpandableExmpl::onHelp() {
String topic = m_pszHelpFilePath; topic += _T(">Introduction");

  ::HtmlHelp(m_pMainWnd->m_hWnd, topic,  HH_DISPLAY_TOC, 0);
  }


void ExpandableExmpl::onAppAbout() {AboutDlg aboutDlg; aboutDlg.DoModal();}


// My ToolBar


#pragma once
//#include "ExamplesDef.h"
#ifdef Examples
#include "TBButton.h"
#include "TBCboBx.h"
#include "TBEditBox.h"
#endif
#include "TBMenu.h"
#include "ToolBarBase.h"

struct CbxItem;


class MyToolBar : public ToolBarBase {

#ifdef Examples
TBButton  button;
TBEditBox editBox;
TBCboBx   cboBx;
#endif

TBMenu    menu;
TBMenu    menu1;
TBMenu    saveMenu;

public:

  MyToolBar();
 ~MyToolBar() { }

#ifdef Examples

  bool    addButton( uint id, TCchar* caption);
  bool    addEditBox(uint id, int     noChars);
  CString getText(uint id);


  bool    addCBx(    uint id);
  bool    addCBx(    uint id, int     idr, TCchar* caption);
  bool    addCbxItem(uint id, TCchar* txt, int data = 0);
  bool    addCBx(    uint id, CbxItem cbxItem[], int n, TCchar* caption);
  bool    addCbxItemSorted(uint id, TCchar* txt, int data = 0);
  void    setCaption(uint id, TCchar* caption);
  void    setWidth(  uint id);
  void    setHeight( uint id);
  bool    getCurSel( uint id, String& s, void*& data);

#endif

  bool    addMenu(uint id, int idr, int     index);         // uses icon at index on toolbar
  bool    addMenu(uint id, int idr, TCchar* caption);
  };




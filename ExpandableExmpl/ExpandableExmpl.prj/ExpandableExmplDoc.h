// ExpandableExmplDoc.h : interface of the ExpandableExmplDoc class


#pragma once
#include "CDoc.h"
#include "MainFrame.h"
#include "Store.h"
#include "StoreP.h"


enum DataSource {NotePadSrc, StoreSrc, StrRptSrc, FontSrc};
enum ExpCh      {NilChoice,  ExpChoice, ExpPChoice};


class ExpandableExmplDoc : public CDoc {

PathDlgDsc  pathDlgDsc;

DataSource  dataSource;

ExpCh       expCh;
Store       store;
StoreP      storeP;

protected: // create from serialization only

  ExpandableExmplDoc() noexcept;
  DECLARE_DYNCREATE(ExpandableExmplDoc)

public:

  DataSource dataSrc() {return dataSource;}
  void       display(DataSource ds = NotePadSrc);

  virtual void serialize(Archive& ar);

// Implementation
public:
  virtual ~ExpandableExmplDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

private:

  void copyDataStore();
  void copyDataStoreP();

  void addFile(TCchar* pth);
  void addFileP(TCchar* pth);

  void moveStore();
  void moveStoreP();

  void copyDatum();
  void copyDatumP();

  void storeDatum();
  void storeDatumP();

  void appendAllctDtm();
  void appendAllctDtmP();

  void appendCpyDtm();
  void appendCpyDtmP();

  void insSrtAllctDtm();
  void insSrtAllctDtmP();

  void insSrtCpyDtm();
  void insSrtCpyDtmP();

  void insAllctDtmAtIP();

  void insCpyDtmAtI();
  void insCpyDtmAtIP();

  void getRefAtI();
  void getRefAtIP();

  void pushPop();
  void pushPopP();

  void delEntry();
  void delEntryP();

  void findEntry();
  void findEntryP();

  void binarySearch();
  void binarySearchP();

  void wordsExmpl(Words& words);
  void datumExmpl(int key, Datum &dtm);

  void setTestTitle(TCchar* status, TCchar* testTtl);

  void displayStr();
  void display(TCchar* title, StorePData& d);
  void display(TCchar* title, StoreData&  d);
  void checkPtr(Words* p);

  void saveFile(TCchar* title, TCchar* suffix, TCchar* fileType);

  void testLine(int n);
  void wholePage();

// Generated message map functions

protected:

  DECLARE_MESSAGE_MAP()

public:

  afx_msg void onPickFile();

  afx_msg void onSaveFile();
  afx_msg void onSaveStrRpt();
  afx_msg void onSaveNotePad();

  afx_msg void onEditCopy();

  afx_msg void onSetExp();
  afx_msg void onSetExpP();

  afx_msg void onClearData();
  afx_msg void onLoadFile();
  afx_msg void onLoadFileSorted();
  afx_msg void onCopyData();
  afx_msg void onAddFile();
  afx_msg void onMoveData();
  afx_msg void onCopyDatum();
  afx_msg void onStoreDatum();
  afx_msg void onAllctDeAllct();

  afx_msg void onAddAllctDtm();
  afx_msg void onAddCpyDtm();
  afx_msg void onInsSrtAllctDtm();
  afx_msg void onInsCpyDtm();
  afx_msg void onInsAllctDtmAtI();
  afx_msg void onInsCpyDtmAtI();
  afx_msg void onGetRefAtI();
  afx_msg void onPushPop();
  afx_msg void onDelEntry();

  afx_msg void onFindEntry();
  afx_msg void onBinarySearch();

































  };



////--------------------
#if 0
//#include "ExamplesDef.h"

#ifdef Examples
  afx_msg void OnTest();
  afx_msg void displayDataStore();
  afx_msg void myButton();

  afx_msg void OnComboBoxChng();

  afx_msg void OnTBEditBox();
  afx_msg void myButton1();
  afx_msg void OnTestEditBoxes();
#endif

#endif

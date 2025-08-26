 // ExpandableExmplDoc.cpp : implementation of the ExpandableExmplDoc class

/*
  Key         key;        // Key used in Datum for searchs and insert sort
  Datum       dtm;
  Datum*      p;
  ExpandableP a;
  ExpandableP b;
  ExpandableP c;


  Check Description
  ----- -----------
        Constructor
        Destructor

    X   a.clear();          removes all nodes from array and clears all pointers
        n = b.end();        return number of used cells in array

    X   b  = a;             Copy operator, data remains in a, copied to b
    X   b += c;             Append c to b, data remains in c, appended to b
    X   b -= a;             Move a to b, a is cleared of data

        dtm  = b[i];        Copy data from array at index, i, to dtm, data stays in b;
        b[j] = dtm;         Copy dtm node data to arrat at index, i, data stays in node dtm

        b += p;             p points to an allocated node and is added to the end of the array
        b += dtm;           dtm is a reference to a node, a node is allocated, the data copied into
                            the new node and it is added to the end of the array
    X   p = b.nextData();   Allocate a node, added it to the end of the array and return a
                            reference to it.

        b = p;              p points to an allocated node.  It is inserted in the array according
                            to the boolean expressions in the Datum class
    X   b = dtm;            dtm is a reference to a node, a node is allocated and the data copied
                            into the new node.  It is inserted in the array according to the
                            boolean expressions in the Datum class

        b(i, p);            Insert p, an allocated node, into b at index i, moving other entries
                            out of the way
        b(i, dtm);          dtm is a reference to a node, a node is allocated and the data copied
                            into the new node.  Insert new node into b at index i, moving other
                            entries out of the way
        Datum& node =       Return a reference to the node at index i allocating a new node and
            b.getData(i);   adding it the array at index i.

        b.push(p);          p points to an allocated node and is added to the end of the array
        b.push(dtm);        dtm is a reference to a node, a new node is allocated, the data copied
                            into the new node and it is added to the end of the array
        p = b.pop();        The end node is removed from the array and a pointer is returned.
                            This module is not responsible for deallocating the node.

        b.del(i);           Delete node and deallocate it.  Adjust b to fill the gap.
        b.del(p);           Delete node with address p. Adjust b to fill the gap.

        p = b.find(key);    Returns a pointer to the first node that matches the key from a linear
                            search
        p = b.bSearch(key); returns a pointer to a node that matches the key using a binary search.
                            the array must be sorted on the key.

        p = b.allocate();   Allocates a node are returns a pointer to it.  No change to b
        b.deallocate(p);    Deallocates the node pointed to by p.  No change to b
*/


#include "pch.h"
#include "ExpandableExmplDoc.h"
#include "ExpandableExmpl.h"
#include "ExpandableExmplView.h"
#include "ClipLine.h"
#include "filename.h"
#include "IniFileEx.h"
#include "MessageBox.h"
#include "NotePad.h"
#include "Printer.h"
#include "Resource.h"


static const TCchar* Section    = _T("Global");
static const TCchar* PathKey    = _T("Path");
static const TCchar* NilVersion = _T("Expandable Version not chosen!");

// ExpandableExmplDoc

IMPLEMENT_DYNCREATE(ExpandableExmplDoc, CDoc)

BEGIN_MESSAGE_MAP(ExpandableExmplDoc, CDoc)
  ON_COMMAND(      ID_PickFile,           &onPickFile)

  ON_COMMAND(      ID_Menu,               &onSetExp)
  ON_COMMAND(      ID_SetExp,             &onSetExp)
  ON_COMMAND(      ID_SetExpP,            &onSetExpP)

  ON_COMMAND(      ID_Menu1,              &onLoadFile)
  ON_COMMAND(      ID_ClearData,          &onClearData)
  ON_COMMAND(      ID_LoadFile,           &onLoadFile)
  ON_COMMAND(      ID_LoadFileSorted,     &onLoadFileSorted)
  ON_COMMAND(      ID_CopyData,           &onCopyData)
  ON_COMMAND(      ID_AddFile,            &onAddFile)
  ON_COMMAND(      ID_MoveData,           &onMoveData)
  ON_COMMAND(      ID_CopyDatum,          &onCopyDatum)
  ON_COMMAND(      ID_StoreDatum,         &onStoreDatum)
  ON_COMMAND(      ID_AllctDeAllct,       &onAllctDeAllct)

  ON_COMMAND(      ID_AddAllctDtm,        &onAddAllctDtm)
  ON_COMMAND(      ID_AddCpyDtm,          &onAddCpyDtm)
  ON_COMMAND(      ID_InsSrtAllctDtm,     &onInsSrtAllctDtm)
  ON_COMMAND(      ID_InsCpyDtm,          &onInsCpyDtm)
  ON_COMMAND(      ID_InsAllctDtmAtI,     &onInsAllctDtmAtI)
  ON_COMMAND(      ID_InsCpyDtmAtI,       &onInsCpyDtmAtI)
  ON_COMMAND(      ID_GetRefAtI,          &onGetRefAtI)
  ON_COMMAND(      ID_PushPop,            &onPushPop)
  ON_COMMAND(      ID_DelEntry,           &onDelEntry)
  ON_COMMAND(      ID_FindEntry,          &onFindEntry)
  ON_COMMAND(      ID_BinarySearch,       &onBinarySearch)
  ON_COMMAND(      ID_Iterators,          &onIterators)
  ON_COMMAND(      ID_Sort,               &onSort)

  ON_COMMAND(      ID_TBSaveMenu,         &onSaveFile)
  ON_COMMAND(      ID_SaveFile,           &onSaveFile)
  ON_COMMAND(      ID_SaveStrRpt,         &onSaveStrRpt)
  ON_COMMAND(      ID_SaveNotePad,        &onSaveNotePad)

  ON_COMMAND(      ID_EDIT_COPY,          &onEditCopy)

END_MESSAGE_MAP()


// ExpandableExmplDoc construction/destruction

ExpandableExmplDoc::ExpandableExmplDoc() noexcept : dataSource(NotePadSrc), expCh(NilChoice) {
  iniFile.read(Section, PathKey, path);
  }

ExpandableExmplDoc::~ExpandableExmplDoc() { }


void ExpandableExmplDoc::onPickFile() {

  notePad.clear();   dataSource = StoreSrc;

  pathDlgDsc(_T("Load Example"), pathDlgDsc.name, _T("txt"), _T("*.txt"));

  if (!setOpenPath(pathDlgDsc)) return;

  theApp.setTitle(path);   iniFile.write(Section, PathKey, path);

  display();
  }


void ExpandableExmplDoc::onSetExp() {expCh = ExpChoice;  setStatus(_T("Expandable"));}
void ExpandableExmplDoc::onSetExpP(){expCh = ExpPChoice; setStatus(_T("ExpandableP"));}


void ExpandableExmplDoc::onClearData() {

  notePad.clear();

  setTestTitle(_T("Clear"), _T("Clear Expandable Store"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);   break;
    case ExpChoice  : store.clear();    store.probe(_T("After Clear"));    break;
    case ExpPChoice : storeP.clear();   storeP.probe(_T("After Clear"));   break;
    }

  displayStr();
  }


void ExpandableExmplDoc::onLoadFile() {

  notePad.clear();

  if (path.isEmpty()) onPickFile();

  setTestTitle(_T("Load File"), _T("Load File"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);   break;
    case ExpChoice  : store.load(path);         break;
    case ExpPChoice : storeP.load(path);        break;
    }

  displayStr();
  }


void ExpandableExmplDoc::onLoadFileSorted() {

  notePad.clear();

  if (path.isEmpty()) onPickFile();

  setTestTitle(_T("Load Sorted"), _T("Load File Sorted"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);   break;
    case ExpChoice  : store.loadSorted(path);   break;
    case ExpPChoice : storeP.loadSorted(path);  break;
    }

  displayStr();
  }


// test b = a;

void ExpandableExmplDoc::onCopyData() {

  notePad.clear();

  setTestTitle(_T("Copy Data"), _T("Test b = a;"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : copyDataStore();         break;
    case ExpPChoice : copyDataStoreP();        break;
    }
  }


void ExpandableExmplDoc::copyDataStore() {
StoreData a;
StoreData b;

  store.copyData(a);    a.probe(_T("After copy"));      // Copy whatever is in store to a

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  b = a;                                                // Test of b = a;

  a.probe(_T("a after b = a"));
  b.probe(_T("b after b = a"));   notePad << nCrlf;

  display(_T("b data structure"), b);
  }


void ExpandableExmplDoc::copyDataStoreP() {
StorePData a;
StorePData b;

  storeP.copyData(a);    a.probe(_T("After copy"));     // Copy whatever is in store to a

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  b = a;                                                // Test of b = a;

  a.probe(_T("a after b = a"));
  b.probe(_T("b after b = a"));   notePad << nCrlf;

  display(_T("b data structure"), b);
  }


void ExpandableExmplDoc::onAddFile() {
String     pth;
PathDlgDsc dlgDsc(_T("Add File"), pth, _T("txt"), _T("*.txt"));

  notePad.clear();

  if (!getOpenDlg(dlgDsc, pth)) return;

  setTestTitle(_T("Add File"), _T("Add File"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : addFile(pth);      break;
    case ExpPChoice : addFileP(pth);     break;
    }

  display();
  }


void ExpandableExmplDoc::addFile(TCchar* pth) {
StoreData a;
StoreData b;
Random rand(521);

  store.copyData(a);   a.probe(_T("After Copy"));
  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  store.load(pth, b, rand);
  display(_T("b data structure"), b);   notePad << nCrlf << nCrlf;

  a += b;                         // Test add one array

  a.probe(_T("After b added to a"));  display(_T("a after adding b"), a);
  notePad << nCrlf << nCrlf;

  b.probe(_T("b is unchanged"));   display(_T("b is unchanged after add operation"), b);
  }


void ExpandableExmplDoc::addFileP(TCchar* pth) {
StorePData a;
StorePData b;
Random rand(521);

  storeP.copyData(a);   a.probe(_T("After Copy"));
  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  storeP.load(pth, b);
  display(_T("b data structure"), b);   notePad << nCrlf << nCrlf;

  a += b;                         // Test add one array

  a.probe(_T("After b added to a"));  display(_T("a after adding b"), a);
  notePad << nCrlf << nCrlf;

  b.probe(_T("b is unchanged"));   display(_T("b is unchanged after add operation"), b);
  }


void ExpandableExmplDoc::onMoveData() {

  notePad.clear();

  setTestTitle(_T("Move Data"), _T("Move Data"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : moveStore();             break;
    case ExpPChoice : moveStoreP();            break;
    }
  }


void ExpandableExmplDoc::moveStore() {
StoreData a;
StoreData b;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  b -= a;                                   // Test b -= a;

  a.probe(_T("a after b -= a"));
  b.probe(_T("b after b -= a"));   notePad << nCrlf;

  display(_T("b data structure"), b);
  }


void ExpandableExmplDoc::moveStoreP() {
StorePData a;
StorePData b;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  b -= a;                                   // Test b -= a;

  a.probe(_T("a after b -= a"));
  b.probe(_T("b after b -= a"));   notePad << nCrlf;

  display(_T("b data structure"), b);
  }


// Test dtm=b[i];

void ExpandableExmplDoc::onCopyDatum() {

  notePad.clear();

  setTestTitle(_T("dtm = b[i]"), _T("Copy Datum (dtm = b[i])"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : copyDatum();             break;
    case ExpPChoice : copyDatumP();            break;
    }
  }


void ExpandableExmplDoc::copyDatum() {
StoreData a;
Datum     dtm;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  dtm = a[(a.end() / 2) - 1];                 // Test by retrieving middle element of array

  notePad << _T("datum copied from array a:  ") << nCrlf;
  dtm.display();   notePad << nCrlf;

  a.probe(_T("After copy"));
  display(_T("a data structure after getting item from array"), a);
  }


void ExpandableExmplDoc::copyDatumP() {
StorePData a;
Words      words;
int        i;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  i     = (a.end() / 2) - 1;
  words = a[i];                 // Test by retrieving middle element of array

  notePad << _T("datum copied from array a:  ") << nCrlf;
  words.display(i);   notePad << nCrlf;

  a.probe(_T("After copy"));
  display(_T("a data structure after getting item from array"), a);
  }



void ExpandableExmplDoc::onStoreDatum() {

  notePad.clear();

  setTestTitle(_T("b[i] = dtm"), _T("Copy Datum (dtm = b[i])"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : storeDatum();             break;
    case ExpPChoice : storeDatumP();            break;
    }
  }


void ExpandableExmplDoc::storeDatum() {
StoreData a;
Datum     dtm;
int       i;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  datumExmpl(12345, dtm);

  dtm.display(_T("Dtm before"));   notePad << nCrlf;

  i = (a.end() / 2) - 1;

  a[i] = dtm;                 // Test by assigning middle element of array to dtm content

  a.probe(_T("After copy"));

  display(_T("After Test a[i] = dtm"), a);   notePad << nCrlf << nCrlf;

  dtm.display(_T("dtm after"));
  }


void ExpandableExmplDoc::storeDatumP() {
StorePData a;
Words      words;
int        i;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  wordsExmpl(words);   words.display(_T("Words before"));   notePad << nCrlf;

  i = (a.end() / 2) - 1;

  a[i] = words;                 // Test by retrieving middle element of array

  a.probe(_T("After copy"));

  display(_T("After Test a[i] = dtm"), a);   notePad << nCrlf << nCrlf;

  notePad << _T("Content of words after Test") << nCrlf;   words.display(i);
  }


void ExpandableExmplDoc::onAllctDeAllct() {
StorePData a;
Words*     p;

  if (expCh != ExpPChoice) return;

  notePad.clear();

  setTestTitle(_T("allocate/deallocate p;"),
                                         _T("Append Datum (p = a.allocate())   a.deallocate(p);"));

  p = a.allocate();                           // Test allocation of a Datum

  if (!p) {notePad << _T("Allocate failed") << nCrlf; return;}

  wordsExmpl(*p);   p->display(_T("Words after"));

  a.deallocate(p);                            // Test deallocation of a Datum

  checkPtr(p);

  display();
  }


void ExpandableExmplDoc::onAddAllctDtm() {

  notePad.clear();

  setTestTitle(_T("b += p"), _T("Append Datum (b += p)"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : appendAllctDtm();             break;
    case ExpPChoice : appendAllctDtmP();            break;
    }

  display();
  }


void ExpandableExmplDoc::appendAllctDtm() {
StoreData a;
Datum     dtm;
Datum*    p = &dtm;
Datum*    q;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  datumExmpl(99999, *p);   p->display(_T("Datum before"));   notePad << nCrlf;

  q = a += p;                 // Test appending node to a, p is attached to array DO Not Deallocate

  a.probe(_T("After a += p"));

  display(_T("After Test a += p"), a);   notePad << nCrlf << nCrlf;

  p->display(_T("*p after"));
  notePad << _T("And q points to array entry") << nCrlf;
  q->display(_T("q after"));   notePad << nCrlf;
  }


void ExpandableExmplDoc::appendAllctDtmP() {
StorePData a;
Words*     p = a.allocate();
Words*     q;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  wordsExmpl(*p);

  p->display(_T("Words before"));   notePad << nCrlf;

  q = a += p;                 // Test appending node to a, p is attached to array DO Not Deallocate

  a.probe(_T("After a += p"));

  display(_T("After Test a += p"), a);   notePad << nCrlf << nCrlf;

  notePad << _T("Node p is attached to a, but p still points to it") << nCrlf;
  p->display(_T("*p after"));
  notePad << _T("And q points to it also") << nCrlf;
  q->display(_T("q after"));   notePad << nCrlf;
  }



void ExpandableExmplDoc::onAddCpyDtm() {

  notePad.clear();

  setTestTitle(_T("a = dtm"), _T("Append Datum (a = dtm)"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : appendCpyDtm();             break;
    case ExpPChoice : appendCpyDtmP();            break;
    }

  display();
  }


void ExpandableExmplDoc::appendCpyDtm() {
StoreData a;
Datum     dtm;
Datum*    q;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  datumExmpl(99999, dtm);   dtm.display(_T("Datum before"));   notePad << nCrlf;

  q = a += dtm;                 // Test appending node to a, p is attached to array DO Not Deallocate

  a.probe(_T("After a += dtm"));

  display(_T("After Test a += dtm"), a);   notePad << nCrlf << nCrlf;

  dtm.display(_T("dtm after"));
  notePad << _T("And q points to array entry") << nCrlf;
  q->display(_T("q after"));   notePad << nCrlf;
  }


void ExpandableExmplDoc::appendCpyDtmP() {
StorePData a;
Words      words;
Words*     q;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  wordsExmpl(words);

  words.display(_T("Words before"));   notePad << nCrlf;

  q = a += words;                 // Test appending a copy of words to a

  a.probe(_T("After a += dtm"));

  display(_T("After Test a += dtm"), a);   notePad << nCrlf << nCrlf;

  words.display(_T("Words after"));
  q->display(_T("q after"));
  }


// Test a = p;  Insert into sort array

void ExpandableExmplDoc::onInsSrtAllctDtm() {

  notePad.clear();

  setTestTitle(_T("a = p"), _T("Add Sorted Datum (a = p)"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : insSrtAllctDtm();        break;
    case ExpPChoice : insSrtAllctDtmP();       break;
    }

  display();
  }


void ExpandableExmplDoc::insSrtAllctDtm() {
StoreData a;
Datum     dtm;
Datum*    p = &dtm;
Datum*    q;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  datumExmpl(55555, *p);   p->display(_T("Datum before"));   notePad << nCrlf;

  q = a = p;                  // Test appending node to a, p is attached to array DO Not Deallocate

  a.probe(_T("After a = p"));

  display(_T("After Test a = p"), a);   notePad << nCrlf << nCrlf;

  p->display(_T("*p after"));
  notePad << _T("And q points to array entry") << nCrlf;
  q->display(_T("q after"));   notePad << nCrlf;
  }




void ExpandableExmplDoc::insSrtAllctDtmP() {
StorePData a;
Words*     words = a.allocate();
Words*     q;
String     s;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  wordsExmpl(*words);

  words->display(_T("Words before"));   notePad << nCrlf;

  q = a = words;                  // Test insert a copy of words into sorted array a

  a.probe(_T("After a + p"));

  display(_T("After Test a = p"), a);   notePad << nCrlf << nCrlf;

  words->display(_T("Words after"));
  q->display(_T("q after"));
  }


void ExpandableExmplDoc::onInsCpyDtm() {

  notePad.clear();

  setTestTitle(_T("b = dtm"), _T("Add sorted Datum (b = dtm)"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : insSrtCpyDtm();        break;
    case ExpPChoice : insSrtCpyDtmP();       break;
    }

  display();
  }


void ExpandableExmplDoc::insSrtCpyDtm() {
StoreData a;
Datum     dtm;
Datum*    q;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  datumExmpl(55555, dtm);   dtm.display(_T("Datum before"));   notePad << nCrlf;

  q = a = dtm;                // Test appending node to a, p is attached to array DO Not Deallocate

  a.probe(_T("After a = dtm"));

  display(_T("After Test a = dtm"), a);   notePad << nCrlf << nCrlf;

  dtm.display(_T("dtm after"));
  notePad << _T("And q points to array entry") << nCrlf;
  q->display(_T("q after"));   notePad << nCrlf;
  }


void ExpandableExmplDoc::insSrtCpyDtmP() {
StorePData a;
Words      words;
Words*     q;
String     s;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  wordsExmpl(words);

  words.display(_T("Words before"));   notePad << nCrlf;

  q = a = words;                  // Test insert a copy of words into sorted array a

  a.probe(_T("After a + dtm"));

  display(_T("After Test a = dtm"), a);   notePad << nCrlf << nCrlf;

  if (q != &words) notePad << _T("q != &words") << nCrlf;

  words.display(_T("Words after"));
  q->display(_T("q after"));
  }


// Test a(i,p);

void ExpandableExmplDoc::onInsAllctDtmAtI() {

  notePad.clear();

  setTestTitle(_T("a(i, p)"), _T("Insert at i, a(i, p)"));

  switch (expCh) {
    case NilChoice  :                          break;
    case ExpChoice  : messageBox(NilVersion);  break;
    case ExpPChoice : insAllctDtmAtIP();       break;
    }

  display();
  }


void ExpandableExmplDoc::insAllctDtmAtIP() {
StorePData a;
Words*     words = a.allocate();

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  wordsExmpl(*words);

  words->display(_T("Words before"));   notePad << nCrlf;

  if (a(7, words)) notePad << _T("Insertion Succeeded") << nCrlf;
  else             notePad << _T("Insertion Failed")    << nCrlf;
                                                    // Test insert a allocated node at index 7
  a.probe(_T("After a(7, p))"));

  display(_T("After Test a(7, p)"), a);   notePad << nCrlf << nCrlf;

  words->display(_T("Words after"));
  }


// Test a(i, dtm)

void ExpandableExmplDoc::onInsCpyDtmAtI() {

  notePad.clear();

  setTestTitle(_T("a(i, dtm)"), _T("Insert at i a(i, dtm)"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : insCpyDtmAtI();        break;
    case ExpPChoice : insCpyDtmAtIP();       break;
    }

  display();
  }


void ExpandableExmplDoc::insCpyDtmAtI() {
StoreData a;
Datum     dtm;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  datumExmpl(55555, dtm);   dtm.display(_T("Datum before"));   notePad << nCrlf;

  if (a(7, dtm)) notePad << _T("Insertion Succeeded") << nCrlf;
  else           notePad << _T("Insertion Failed")    << nCrlf;
                                                    // Test insert a allocated node at index 7
  a.probe(_T("After a = dtm"));

  display(_T("After Test a(7, dtm)"), a);   notePad << nCrlf << nCrlf;

  dtm.display(_T("dtm after"));
  }


void ExpandableExmplDoc::insCpyDtmAtIP() {
StorePData a;
Words      words;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  wordsExmpl(words);

  words.display(_T("Words before"));   notePad << nCrlf;

  if (a(7, words)) notePad << _T("Insertion Succeeded") << nCrlf;
  else             notePad << _T("Insertion Failed")    << nCrlf;
                                                    // Test insert a allocated node at index 7
  a.probe(_T("After a(7, p))"));

  display(_T("After Test a(7, p)"), a);   notePad << nCrlf << nCrlf;

  words.display(_T("Words after"));
  }


void ExpandableExmplDoc::onGetRefAtI() {

  notePad.clear();

  setTestTitle(_T("ref = a.getData(i)"), _T("Reference = a.getData(i) and a.getData(99)"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : getRefAtI();        break;
    case ExpPChoice : getRefAtIP();       break;
    }

  display();
  }


void ExpandableExmplDoc::getRefAtI() {
StoreData a;
Datum     dtm;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf;

  Datum& ref = a.getData(7);      // Test get reference to Datum at a[7]

  ref.display(_T("Ref after"));

  a.probe(_T("After ref = a.getData(7)"));
  display(_T("After Test ref = a.getData(7)"), a);   notePad << nCrlf << nCrlf;

  datumExmpl(55555, dtm);   dtm.display(_T("Datum before"));   notePad << nCrlf;

  Datum& ref2 = a.getData(10);   ref2 = dtm;

  ref2.display(_T("Ref2 after"));
  a.probe(_T("After store at ref2"));
  display(_T("After store at ref2"), a);
  }


void ExpandableExmplDoc::getRefAtIP() {
StorePData a;
Words      words;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  Words& ref = a.getData(7);    // Test get reference to Datum at a[7]

  a.probe(_T("After a.getData(7);"));

  ref.display(_T("Ref after"));  notePad << nCrlf;

  display(_T("After Test a.getData(7);"), a);   notePad << nCrlf << nCrlf;

  wordsExmpl(words);

  Words& ref2 = a.getData(99);   ref2 = words;

  display(_T("After Test a.getData(a.end());"), a);
  }


void ExpandableExmplDoc::onPushPop() {

  notePad.clear();

  setTestTitle(_T("push/pop"), _T("Push/Pop Test"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : pushPop();               break;
    case ExpPChoice : pushPopP();              break;
    }

  display();
  }


void ExpandableExmplDoc::pushPop() {
StoreData a;
Datum     dtm;
Datum     dtm2;
Datum     dtm3;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  datumExmpl(11111, dtm);
  datumExmpl(22222, dtm2);

  dtm.display(_T("Datum before"));
  dtm2.display(_T("Datum 2 before"));    notePad << nCrlf;

  a.push(dtm);                      // Test push dtm to a
  a.push(dtm2);                     // Test push the line

  a.probe(_T("After two pushes"));

  display(_T("After two pushes"), a);   notePad << nCrlf << nCrlf;

  dtm.display(_T("Datum after"));
  dtm2.display(_T("Datum 2 after"));

  a.pop(dtm3);                      // Test Pop

  dtm3.display(_T("pop result"));

  a.pop(dtm3);

  dtm3.display(_T("Pop two"));

  notePad << nCrlf << nCrlf;

  display(_T("After two pops"), a);
  }


void ExpandableExmplDoc::pushPopP() {
StorePData a;
Words*     p = a.allocate();
Words      words;
Words*     q;
String     line = _T("Now is the time for all good people.");

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  wordsExmpl(*p);
  words.load(line);

  words.display(_T("Words before"));
     p->display(_T("*p before"));    notePad << nCrlf;

  a.push(words);                 // Test push words to a
  a.push(p);                     // Test push the line

  a.probe(_T("After a.push(words);"));

  display(_T("After Test two pushes"), a);   notePad << nCrlf << nCrlf;

  words.display(_T("Words after"));
     p->display(_T("*p after"));

  q = a.pop();                    // Test Pop

  q->display(_T("pop result"));   a.deallocate(q);

  q = a.pop();

  q->display(_T("Pop two"));   a.deallocate(q);

  notePad << nCrlf << nCrlf;

  display(_T("After two pops"), a);
  }


void ExpandableExmplDoc::onDelEntry() {

  notePad.clear();

  setTestTitle(_T("Delete Entry"), _T("Delete Entry"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : delEntry();              break;
    case ExpPChoice : delEntryP();             break;
    }

  display();
  }


void ExpandableExmplDoc::delEntry() {
StoreData a;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  if (a.del(7)) notePad << _T("Index Deletion Succeeded") << nCrlf;   // Test Del at index
  else          notePad << _T("Index Deletion Failed")    << nCrlf;

  notePad << nCrlf << nCrlf;

  a.probe(_T("After a.del(7)"));

  display(_T("After Test a.del(7)"), a);
  }


void ExpandableExmplDoc::delEntryP() {
StorePData a;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  if (a.del(7)) notePad << _T("Index Deletion Succeeded") << nCrlf;   // Test Del at index
  else          notePad << _T("Index Deletion Failed")    << nCrlf;

  notePad << nCrlf << nCrlf;

  a.probe(_T("After a.del(7)"));

  display(_T("After Test a.del(7)"), a);   notePad << nCrlf << nCrlf;

  Words& ref = a.getData(11);
  if (a.del(&ref)) notePad << _T("Ptr Deletion Succeeded") << nCrlf;  // Test Del for ptr
  else             notePad << _T("Ptr Deletion Failed")    << nCrlf;
  notePad << nCrlf << nCrlf;

  a.probe(_T("After a.del(p)"));
  display(_T("After Test a.del(p)"), a);   notePad << nCrlf << nCrlf;
  }


void ExpandableExmplDoc::onFindEntry() {

  notePad.clear();

  setTestTitle(_T("Find Entry"), _T("Find Entry"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : findEntry();             break;
    case ExpPChoice : findEntryP();            break;
    }

  display();
  }



void ExpandableExmplDoc::findEntry() {
StoreData a;
Datum*    q;
int       key;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  key = 12345;   q = a.find(key);                 // Failed search test
  if (q) {q->display(_T("Ugly"));}
  else    notePad << _T("Find 'ugly' failed") << nCrlf;
  notePad << nCrlf;

  a.probe(_T("After a.find(key);"));
  display(_T("After Test a.find(key);"), a);   notePad << nCrlf << nCrlf;

  Datum& ref = a.getData(7);    // Test get reference to Datum at a[7]

  q = a.find(ref.key);          // find test success?

  if (q) q->display(_T("Entry found"));
  else   notePad << _T("Find failed") << nCrlf;
  notePad << nCrlf;

  a.probe(_T("After a.find(key);"));
  display(_T("After Test a.find(key);"), a);
  }


void ExpandableExmplDoc::findEntryP() {
StorePData a;
Words*     q;
String     key;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  key = _T("ugly");   q = a.find(key);                 // Failed search test
  if (q) {q->display(_T("Ugly"));}
  else    notePad << _T("Find 'ugly' failed") << nCrlf;
  notePad << nCrlf;

  a.probe(_T("After a.find(key);"));
  display(_T("After Test a.find(key);"), a);   notePad << nCrlf << nCrlf;

  key = _T("ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR,");
  q = a.find(key); // find test success?

  if (q) q->display(_T("Entry found"));
  else   notePad << _T("Find failed") << nCrlf;
  notePad << nCrlf;

  a.probe(_T("After a.find(key);"));
  display(_T("After Test a.find(key);"), a);   notePad << nCrlf << nCrlf;
  }


void ExpandableExmplDoc::onBinarySearch() {

  notePad.clear();

  setTestTitle(_T("Binary Search"), _T("Binary Search"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : binarySearch();          break;
    case ExpPChoice : binarySearchP();         break;
    }

  display();
  }


void ExpandableExmplDoc::binarySearch() {
StoreData a;
Datum*    q;
int       key;

  store.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  key = 12345;   q = a.bSearch(key);                 // Failed bSearch test

  if (q) q->display(_T("Ugly found"));
  else   notePad << _T("Find 'ugly' failed") << nCrlf;
  notePad << nCrlf;

  a.probe(_T("After a.bSearch(key);"));
  display(_T("After Test a.bSearch(key);"), a);   notePad << nCrlf << nCrlf;

  Datum& ref = a.getData(7);    // Test get reference to Datum at a[7]

  q = a.bSearch(ref.key);

  if (q) q->display(_T("Entry found"));
  else   notePad << _T("Search for ") << ref.key << _T("Failed.") << nCrlf;
  notePad << nCrlf;

  a.probe(_T("After a.bSearch(key);"));
  display(_T("After Test a.bSearch(key);"), a);
  }


void ExpandableExmplDoc::binarySearchP() {
StorePData a;
Words*     q;
String     key;

  storeP.copyData(a);   a.probe(_T("After Copy"));

  display(_T("a data structure"), a);   notePad << nCrlf << nCrlf;

  key = _T("ugly");   q = a.bSearch(key);                 // Failed bSearch test
  if (q) q->display(_T("Ugly found"));
  else   notePad << _T("Find 'ugly' failed") << nCrlf;
  notePad << nCrlf;

  a.probe(_T("After a.find(key);"));
  display(_T("After Test a.find(key);"), a);   notePad << nCrlf << nCrlf;

  key = _T("ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR,");   q = a.bSearch(key);
  if (q) q->display(_T("Entry found"));
  else    notePad << _T("Search failed") << nCrlf;
  notePad << nCrlf;

  a.probe(_T("After a.find(key);"));
  display(_T("After Test a.find(key);"), a);
  }



void ExpandableExmplDoc::onIterators() {

  notePad.clear();

  setTestTitle(_T("Binary Search"), _T("Binary Search"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : iterators();             break;
    case ExpPChoice : iteratorsP();            break;
    }

  display();
  }



void ExpandableExmplDoc::iterators() {
StoreIter iter(store);           // Define iterator over data in StoreP
Datum*    q;
int       i = 0;

  notePad << _T("data currently in store") << nCrlf;
  displayStr();   notePad << nCrlf;

  notePad << _T("Items found when iteration on storeP data") << nCrlf;

  for (q = iter(); q; q = iter++) q->display();
  }


// Test iterator

void ExpandableExmplDoc::iteratorsP() {
StorePIter iter(storeP);           // Define iterator over data in StoreP
Words*     q;
int        i = 0;

  notePad << _T("data currently in storeP") << nCrlf;
  displayStr();   notePad << nCrlf;

  notePad << _T("Items found when iteration on storeP data") << nCrlf;

  for (q = iter(); q; q = iter++) q->display(i++);
  }


// Test:  qsort(&data[0], &data[data.end()-1]);

void ExpandableExmplDoc::onSort() {

  notePad.clear();

  setTestTitle(_T("Binary Search"), _T("Binary Search"));

  switch (expCh) {
    case NilChoice  : messageBox(NilVersion);  break;
    case ExpChoice  : sort();          break;
    case ExpPChoice : sortP();         break;
    }

  display();
  }


void ExpandableExmplDoc::sort() {
StoreData a;

  store.copyData(a);
  display(_T("data currently in a"), a);   notePad << nCrlf;

  qsort(&a[0], &a[a.end()-1]);

  display(_T("a after sorting"), a);   notePad << nCrlf;
  }


void ExpandableExmplDoc::sortP() {
StorePData a;

  storeP.copyData(a);
  display(_T("data currently in a"), a);   notePad << nCrlf;

  qsort(&a[0], &a[a.end()-1]);

  display(_T("a after sorting"), a);   notePad << nCrlf;
  }



void ExpandableExmplDoc::datumExmpl(int key, Datum &dtm) {
  dtm.key  = key;
  dtm.line = _T("One for the money, two for the show, three to make ready, and four to go");
  }


void ExpandableExmplDoc::wordsExmpl(Words& words) {
  words.zero  = _T("One for the money, ");
  words.one   = _T("two for the show, ");
  words.two   = _T("three to make ready, ");
  words.three = _T("and four to go");
  }


void ExpandableExmplDoc::setTestTitle(TCchar* status, TCchar* testTtl)
                                    {setStatus(1, status);   notePad << testTtl << nCrlf << nCrlf;}


void ExpandableExmplDoc::displayStr() {

  notePad << nCrlf;

  switch (expCh) {
    case NilChoice  : break;
    case ExpChoice  : store.display(); break;
    case ExpPChoice : storeP.display(); break;
    }

  display();
  }


void ExpandableExmplDoc::display(TCchar* title, StorePData& d)
                                    {notePad << title << nCrlf;   storeP.display(d);  display();}


void ExpandableExmplDoc::display(TCchar* title, StoreData&  d)
                                    {notePad << title << nCrlf;   store.display(d);   display();}


void ExpandableExmplDoc::checkPtr(Words* p) {
  if (p) notePad << _T("Deallocate failed");
  else   notePad << _T("Deallocate Succeeded");
  notePad << nCrlf;
  }


void ExpandableExmplDoc::onEditCopy() {clipLine.load();}


void ExpandableExmplDoc::onSaveFile()
          {dataSource = StoreSrc; saveFile(_T("Save File"), _T(""), _T("txt")); display(StoreSrc);}


void ExpandableExmplDoc::onSaveStrRpt() {

  dataSource = StrRptSrc;   if (setSaveAsPath(pathDlgDsc)) OnSaveDocument(path);

  display(StoreSrc);
  }


void ExpandableExmplDoc::onSaveNotePad() {

  dataSource = NotePadSrc;   if (setSaveAsPath(pathDlgDsc)) OnSaveDocument(path);

  display(StoreSrc);
  }


void ExpandableExmplDoc::display(DataSource ds) {dataSource = ds; invalidate();}


void ExpandableExmplDoc::saveFile(TCchar* title, TCchar* suffix, TCchar* fileType) {
String fileName = path;
int    pos      = fileName.find_last_of(_T('\\'));
String ext      = _T("*."); ext += fileType;
String ttl      = title;    ttl += _T(" Output");

  fileName = fileName.substr(pos+1);   pos = fileName.find_first_of(_T('.'));
  fileName = fileName.substr(0, pos);  fileName += suffix;

  pathDlgDsc(ttl, fileName, fileType, ext);

  if (setSaveAsPath(pathDlgDsc)) OnSaveDocument(path);
  }


// UglyDoc serialization

void ExpandableExmplDoc::serialize(Archive& ar) {

  if (ar.isStoring())
    switch(dataSource) {
      case NotePadSrc : ar << notePad; return;
      default         : return;
      }

  else
    switch(dataSource) {
      case FontSrc  :
      default       : return;
      }
  }


// ExpandableExmplDoc diagnostics

#ifdef _DEBUG
void ExpandableExmplDoc::AssertValid() const {         CDocument::AssertValid();}
void ExpandableExmplDoc::Dump(CDumpContext& dc) const {CDocument::Dump(dc);}
#endif //_DEBUG




////////---------------------------

#if 0
#ifdef Examples

static CbxItem cbxText[] = {{_T("Zeta"),     1},
                            {_T("Beta"),     2},
                            {_T("Alpha"),    3},
                            {_T("Omega"),    4},
                            {_T("Phi"),      5},
                            {_T("Mu"),       6},
                            {_T("Xi"),       7},
                            {_T("Omicron"),  8},
                            {_T("Pi"),       9},
                            {_T("Rho"),     10},
                            {_T("Sigma"),   11},
                            {_T("Nu"),      12},
                            {_T("Kappa"),   13},
                            {_T("Iota"),    14},
                            {_T("This is a reallyt long Greek"), 15}
                            };
static TCchar* CbxCaption = _T("Greeks");


void ExpandableExmplDoc::myButton() {
MyToolBar& toolBar = getToolBar();
int        i;
int        n;

  for (i = 0, n = noElements(cbxText); i < n; i++) {
    CbxItem& item = cbxText[i];

    toolBar.addCbxItemSorted(ID_CboBx, item.txt, item.data);
    }
  toolBar.setCaption(ID_CboBx, CbxCaption);
  toolBar.setWidth(ID_CboBx);

  notePad << _T("Loaded ") << CbxCaption << _T(" into ComboBx") << nCrlf;  display();
  }


void ExpandableExmplDoc::OnComboBoxChng() {
MyToolBar& toolBar = getToolBar();
String     s;
void*      x;

  if (toolBar.getCurSel(ID_CboBx, s, x))
                         notePad << _T("On Change, Item = ") << s << _T(", Data = ") << (int)x << nCrlf;
  display();
  }



void ExpandableExmplDoc::OnTBEditBox() {
MyToolBar& toolBar = getToolBar();
CString    cs      = toolBar.getText(ID_EditBox);
String     s       = cs;

  if (!s.isEmpty()) notePad << s << nCrlf;

  display();
  }


void ExpandableExmplDoc::myButton1() {
MyToolBar& toolBar = getToolBar();
CString    cs      = toolBar.getText(ID_EditBox);
String     s       = cs;

  if (!s.isEmpty()) notePad << s << nCrlf;

  display();
  }


void ExpandableExmplDoc::OnTestEditBoxes() {display();}


// ExpandableExmplDoc commands


void ExpandableExmplDoc::OnTest() {
int n;

  notePad.clear();  theApp.setTitle(_T("My Test"));

  notePad << nFFace(_T("Windsor BT")) << nFSize(14.5) << nBold;

  notePad << _T("Hello World") << nCrlf;

  notePad << nFont << nFont << nFont;

  n = printer.orient == LandOrient ? 10 : 8;

  notePad << nFFace(_T("Courier New")) << nFSize(12.0);   testLine(n);   notePad << nFont << nFont;

  testLine(n);   display();
  }


void ExpandableExmplDoc::testLine(int n) {
int i;

  for (i = 0; i < n; i++) {

    if (!i) notePad << _T('|'); else notePad << i;

    notePad << _T("123456789");
    }

  notePad << _T('|') << nCrlf;
  }


void ExpandableExmplDoc::wholePage() {
int    i;
int    j;
String s;

  notePad << nSetRTab(4) << nSetTab(6) << nSetTab(8) << nSetRTab(12) << nSetTab(14);

  for (i = 0; i < 100; i++) {
    s.format(_T("%3i:"), i);

    notePad << nTab << s << nTab << _T("X") << nTab << _T("Y") << nTab << i << nTab;

    for (j = i+1; j < i+68; j++)
                              {s.format(_T("%i"), j);   s = s.substr(s.length()-1);  notePad << s;}
    notePad << nCrlf;
    }
  }


void ExpandableExmplDoc::displayDataStore() {display(StoreSrc);}

#endif
#ifdef Examples
#include "Store.h"
#endif

#ifdef Examples
  ON_COMMAND(      ID_Button,      &myButton)
  ON_CBN_KILLFOCUS(ID_EditBox,     &OnTBEditBox)
  ON_COMMAND(      ID_EditBox,     &OnTBEditBox)

  ON_CBN_SELCHANGE(ID_CboBx,       &OnComboBoxChng)
  ON_COMMAND(      ID_CboBx,       &OnComboBoxChng)
#endif

#ifdef Examples
  ON_COMMAND(      ID_Test,        &OnTest)
  ON_COMMAND(      ID_SelDataStr,  &displayDataStore)
#endif
#ifdef Examples
      case StoreSrc   : store.store(ar); return;
      case StrRptSrc  : view()->storeRpt().txtOut( ar, 1.35); return;
#endif

#ifdef Examples
      case StoreSrc : store.load(ar); return;
#endif
#if 1
#else
  if (p) notePad << _T("Deallocate failed")    << nCrlf;
  else   notePad << _T("Deallocate Succeeded") << nCrlf;
#endif

#endif

// ExpExample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "StdHdr.h"
#include "Store.h"
#include "StoreP.h"
#include "WinSDKVer.h"


static TCchar* filePath = _T("..\\ExampleData\\DataFile.txt");

enum Commands {Display, Sort, Append, LoadSorted, IterDisplay, BSearch, LinearSrch,
               LoadStoreP, LoadSortedP, BSearchP, LinearSrchP};

static String commands[] = {_T("Display"),
                            _T("Sort"),
                            _T("Append"),

                            _T("LoadSorted"),
                            _T("IterDisplay"),
                            _T("BSearch"),
                            _T("LinearSrch"),

                            _T("LoadStoreP"),
                            _T("LoadSortedP"),
                            _T("BSearchP"),
                            _T("LinearSrchP")
                             };


static void linearSrch();
static void linearSrch(ulong key);
static void binarySrch();
static void binarySrch(ulong key);
static void display(Datum* dtm);

static void linearSrchP();
static void linearSrchP(TCchar* key);
static void binarySrchP();
static void binarySrchP(TCchar* key);
static void display(Words* words);
static void help();


int main(int argc, char* argv[]) {
int  i;
int  j;
char ch;

  if (argc < 2) {help(); return 0;}

  for (i = 1; i < argc; i++) {

    if (i > 1) wcout << endl << endl;
    wcout << "Test: " << argv[i];
    if (i > 1) {wcout  << "  --  Hit Enter Key "; cin.get(ch);}
    else        wcout << endl;

    wcout << endl;

    ToUniCode uni(argv[i]);

    for (j = 0; j < noElements(commands); j++) if (commands[j] == uni()) break;

//Display, Sort, appendDatum, LoadAlt2, IterDisplay, LoadStoreP, LoadSortedP,
//                BSearch, LinearSrch


    switch (j) {
      case Display    :
      case IterDisplay:
      case Sort       : store.load(filePath);        break;
      case Append     : store.appendDatum(filePath); break;
      case BSearch    :
      case LinearSrch :
      case LoadSorted : store.loadSorted(filePath);  break;

      case LoadStoreP : storeP.load(filePath);       break;
      case LoadSortedP:
      case BSearchP   :
      case LinearSrchP: storeP.loadSorted(filePath); break;
      default         : help();                      break;
      }

    switch (j) {
      case Display    : store.display();  break;
      case Sort       : store.sort();
      case Append     :
      case LoadSorted : store.display();  break;
      case IterDisplay: store.display2(); break;
      case LinearSrch : linearSrch();     break;
      case BSearch    : binarySrch();     break;

      case LoadStoreP :
      case LoadSortedP: storeP.display(); break;
      case BSearchP   : binarySrchP();    break;
      case LinearSrchP: linearSrchP();    break;
      default         : help();           break;
      }
    }
  }


void linearSrch() {
StoreIter iter(store);
Datum*    dtm;

  wcout << endl << "Linear Search of Store Object" << endl << endl;

  for (dtm = iter(StoreIter::Rev); dtm; dtm = iter--) linearSrch(dtm->key);

  linearSrch(123); linearSrch(100000);
  }


void linearSrch(ulong key) {
Datum* dtm = store.find(key);

  wcout << "Key: \"" << key << "\"";

  if (dtm) {wcout << " -- found: ";   display(dtm);}
  else      wcout << " -- not found";

  wcout << endl;
  }


void binarySrch() {
StoreIter iter(store);
Datum*    dtm;

  wcout << endl << "Binary Search of Store Object" << endl << endl;

  for (dtm = iter(StoreIter::Rev); dtm; dtm = iter--) binarySrch(dtm->key);

  binarySrch(123); binarySrch(100000);
  }


void binarySrch(ulong key) {
Datum* dtm = store.bSearch(key);

  wcout << "Key: \"" << key << "\"";

  if (dtm) {wcout << " -- found: ";   display(dtm);}
  else      wcout << " -- not found";

  wcout << endl;
  }


void display(Datum* dtm) {
  wcout << dtm->key;
  if (!dtm->line.isEmpty())   wcout << " " << dtm->line;
  }



void linearSrchP() {
StorePIter iter(storeP);
Words*     words;

  wcout << "Linear Search of StoreP Object" << endl;

  for (words = iter(); words; words = iter++) linearSrchP(words->zero);
  }


void linearSrchP(TCchar* key) {
Words* words = storeP.find(key);

  wcout << "Key: \"" << key << "\"";

  if (words) {wcout << " -- found: ";   display(words);}
  else        wcout << " -- not found";

  wcout << endl;
  }


void binarySrchP() {
StorePIter iter(storeP);
Words*     words;

  wcout << "Binary Search of StoreP Object" << endl;

  for (words = iter(); words; words = iter++) binarySrchP(words->zero);
  }


void binarySrchP(TCchar* key) {
Words* words = storeP.bSearch(key);

  wcout << "Key \"" << key << "\"";

  if (words) {wcout << " -- found: ";   display(words);}
  else        wcout << " -- not found";

  wcout << endl;
  }


void display(Words* words) {
  wcout << words->zero;
  if (!words->one.isEmpty())   wcout << ", " << words->one;
  if (!words->two.isEmpty())   wcout << ", " << words->two;
  if (!words->three.isEmpty()) wcout << ", " << words->three;
  if (!words->rest.isEmpty())  wcout << ", " << words->rest;
  }


void help() {
int i;

  wcout << "Please supply one or more of the following commands:" << endl;

  for (i = 0; i < noElements(commands); i++) {
    wcout << commands[i] << endl;
    }
  }


// ExpandableP Store Example


#include "pch.h"
#include "StoreP.h"
#include "FileIO.h"
#include "filename.h"
#include "NotePad.h"

#include "MessageBox.h"


void StoreP::addFile(TCchar* path) {
StorePData addData;
String     name = getMainName(path);
String     title;

  data.probe(_T("Data Loaded"));

  load(path, addData);   data += addData;

  title = name + _T(" Loaded");   addData.probe(title);

  title = name + _T(" data Added");   data.probe(title);
  addData.probe(_T("AD after add"));
  }



// Load the expandable array...

void StoreP::load(TCchar* filePath, StorePData& d, bool sortIt) {
String path = filePath;
String name = getMainName(path);
String title;
FileIO fil;
String line;

title = name + _T(" Constructed");   d.probe(title);

  d.clear();   sorted = sortIt;

title = name + _T(" Cleared");   d.probe(title);

  if (fil.open(path, FileIO::Read)) {
    notePad << path << " opened" << nCrlf;

    while (fil.read(line)) {
      if (sorted) {Words words;   words.load(line);   data = words;}
      else        {Words& words = d.nextData();  words.load(line);}
      }                                                        // nextData at the end of the array
    }

  fil.close();

  title = name + _T(" Loaded");   d.probe(title);
  }


void Words::load(String& line) {
String s = line;

  s.trim();
  if (!nextWord(s, zero))  return;
  if (!nextWord(s, one))   return;
  if (!nextWord(s, two))   return;
  if (!nextWord(s, three)) return;
  rest = s;
  }


bool Words::nextWord(String& s, String& word) {
int pos = s.find(_T(' '));

  if (pos > 0) {word = s.substr(0, pos);  s = s.substr(pos);  s.trim();  return true;}

  if (s.length()) {word = s; s.clear(); return true;}

  return false;
  }


void StoreP::display(StorePData& d) {
int n = d.end();
int i;

  for (i = 0; i < n; i++) {Words* p = d[i];   if (p) p->display(i);}
  }


void Words::display(int i) {

  notePad << nClrTabs << nSetRTab(4) << nSetTab(6);

  notePad << nTab << i << nTab;   dispFlds();   notePad << nCrlf;
  }


void Words::display(TCchar* prefix)
                                {notePad << prefix << _T(":  ");   dispFlds();   notePad << nCrlf;}


void Words::dispFlds() {
                               if (zero.isEmpty())  return;
  notePad << zero;             if (one.isEmpty())   return;
  notePad << _T(' ') << one;   if (two.isEmpty())   return;
  notePad << _T(' ') << two;   if (three.isEmpty()) return;
  notePad << _T(' ') << three; if (rest.isEmpty())  return;
  notePad << _T(' ') << rest;
  }


void Words::copy(Words& wrds) {
  zero  = wrds.zero;
  one   = wrds.one;
  two   = wrds.two;
  three = wrds.three;
  rest  = wrds.rest;
  }



////------------

#if 0
Words* StoreP::bSearch(TCchar* key) {
int    n    = data.end();
int    beg  = 0;
int    end  = n;
int    last = -1;
int    i;
Words* words;

  for (i = (beg+end)/2; i < n && i != last; last = i, i = (beg+end)/2) {

    words = data[i].p;

    if (key == words->zero) return words;
    if (key >  words->zero) {beg = i; continue;}
    if (key <  words->zero) {end = i; continue;}
    }

  return 0;
  }
#endif
#if 0
void Words::copy(Words& wrds) {
  zero  = wrds.zero;
  one   = wrds.one;
  two   = wrds.two;
  three = wrds.three;
  rest  = wrds.rest;
  }
#endif

//StoreP storeP;
#if 0
void StoreP::display() {
StorePIter iter(*this);
Words*     words;
String     s;

  for (words = iter(); words; words = iter++)
#if 1
    display(words, iter.index());
#else
  {
    s.format(_T("%3i: "), iter.index());

    notePad << s.str();   words->display();   notePad << nCrlf;
    }
#endif
  }
#endif
#if 0
void StoreP::loadSorted(TCchar* filePath) {
#if 1

  sorted = true;   load(filePath, data);

#else
String path = filePath;
FileIO fil;
String line;
int    i = 0;

data.probe(_T("Constructed"));

  data.clear();   sorted = true;

data.probe(_T("Cleared"));

  if (fil.open(path, FileIO::Read)) {
    notePad << filePath << " opened" << nCrlf;

    while (fil.read(line)) {
      Words words;  words.load(line);   data = words;
      }
    }

  fil.close();
data.probe(_T("Loaded"));
#endif
  }
#endif
#if 0
String     path = filePath;
FileIO     fil;
String     line;
StorePData addData;

data.probe(_T("Loaded"));
addData.probe(_T("AD construct"));

  if (fil.open(path, FileIO::Read)) {
    notePad << filePath << " opened" << nCrlf;

    while (fil.read(line)) {

      Words words;   words.load(line);

      if (sorted) addData  = words;
      else          addData += words;
      }
    }

  fil.close();

addData.probe(_T("AD Loaded"));
#endif


// ExpandableP Store Example


#include "StdHdr.h"
#include "StoreP.h"
#include "FileIO.h"


StoreP storeP;


void Words::copy(Words& wrds) {
  zero  = wrds.zero;
  one   = wrds.one;
  two   = wrds.two;
  three = wrds.three;
  rest  = wrds.rest;
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




void StoreP::load(TCchar* filePath) {
String path = filePath;
FileIO fil;
String line;
int    i = 0;

  data.clear();

  if (fil.open(path, FileIO::Read)) {
    wcout << filePath << " opened" << endl;

    while (fil.read(line)) {
      Words& words = data.nextData();  words.load(line);
      }
    }

  fil.close();
  }


void StoreP::loadSorted(TCchar* filePath) {
String path = filePath;
FileIO fil;
String line;
int    i = 0;

  data.clear();

  if (fil.open(path, FileIO::Read)) {
    wcout << filePath << " opened" << endl;

    while (fil.read(line)) {
      Words words;  words.load(line);   data = words;
      }
    }

  fil.close();
  }




void StoreP::display() {
StorePIter iter(*this);
Words*     words;
String     s;

  for (words = iter(); words; words = iter++) {
    s.format(_T("%3i: "), iter.index());

    wcout << s.str();   words->display();   wcout << endl;
    }
  }



void Words::display() {
                            if (zero.isEmpty())  return;
  wcout << zero;            if (one.isEmpty())   return;
  wcout  << _T(' ') << one; if (two.isEmpty())   return;
  wcout<< _T(' ') << two;   if (three.isEmpty()) return;
  wcout<< _T(' ') << three; if (rest.isEmpty())  return;
  wcout<< _T(' ') << rest;
  }



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


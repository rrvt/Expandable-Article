// ExpandableP Store Example


#pragma once
#include "ExpandableP.h"
#include "FileIO.h"
#include "IterT.h"


class Words {
public:

String zero;
String one;
String two;
String three;
String rest;

  Words() { }
  Words(Words& wrds) {copy(wrds);}
 ~Words() { }

  Words& operator= (Words& wrds) {copy(wrds); return *this;}

  void load(String& line);

  void display();

  // Required for Insertion Sort, i.e. data = dtm;
  bool operator >= (Words& w) {return zero >= w.zero;}
  bool operator == (Words& w) {return zero == w.zero;}

  // Required for Binary Search
  bool operator== (TCchar* key) {return zero == key;}
  bool operator<  (TCchar* key) {return zero <  key;}
  bool operator>  (TCchar* key) {return zero >  key;}

private:

  void copy(Words& wrds);

  bool nextWord(String& s, String& word);
  };


class StoreP;
typedef IterT<StoreP, Words> StorePIter;
typedef DatumPtrT<Words> WordsP;


class StoreP {

ExpandableP<Words, WordsP, 2> data;

public:

  StoreP() { }
 ~StoreP() { }

  void load(TCchar* filePath);
  void loadSorted(TCchar* filePath);

  void display();

  Words* bSearch(TCchar* key) {return data.bSearch(key);}
  Words* find(   TCchar* key) {return data.find(key);}

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  Words* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}       // note: data[i].p

  int   nData()      {return data.end();}                       // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename StorePIter;
  };


extern StoreP storeP;

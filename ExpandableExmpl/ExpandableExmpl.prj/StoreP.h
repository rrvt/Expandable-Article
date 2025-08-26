// ExpandableP Store Example


#pragma once
#include "ExpandableP.h"
#include "FileIO.h"
#include "IterT.h"
#include "qsort.h"


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

  Words& operator= (Words* w) {copy(*w);   return *this;}
  Words& operator= (Words& w) {copy(w);    return *this;}

  void load(String& line);

  void display(int i);
  void display(TCchar* prefix);

  // Required for Insertion Sort, i.e. data = dtm;
  bool operator >= (Words& w) {return zero >= w.zero;}
  bool operator == (Words& w) {return zero == w.zero;}

  // Required for Qsort
  bool operator>  (Words& w) {return zero >  w.zero;}
  bool operator<= (Words& w) {return zero <= w.zero;}

  // Required for Binary Search
  bool operator== (TCchar* key) {return zero == key;}
  bool operator<  (TCchar* key) {return zero <  key;}
  bool operator>  (TCchar* key) {return zero >  key;}

private:

  void copy(Words& wrds);                                     // Not required for expandableP

  bool nextWord(String& s, String& word);
  void dispFlds();
  };


class StoreP;
typedef DatumPtrT<Words, String>              WordsP;         // Define pointer structure
typedef IterT<StoreP, Words>                  StorePIter;     // Define Iterator over array
typedef ExpandableP<Words, String, WordsP, 2> StorePData;     // Define array structure


class StoreP {

bool       sorted;
StorePData data;

public:

  StoreP() : sorted(false) { }
 ~StoreP() { }

  void   clear() {sorted = false;   data.clear();}
  bool   isSorted() {return sorted;}

  void   load(TCchar* filePath) {load(filePath, data);}
                                                        // load linear, set mode

  void   loadSorted(TCchar* filePath) {load(filePath, data, true);}
                                                        // load sorted, set mode
  void   addFile(TCchar* filePath);                     // Add file linear or sorted mode

  void   load(TCchar* path, StorePData& d, bool sortIt = false);  // Load an expandable array...

  void   copyData(StorePData& dst) {dst  = data;}       // Copy data from "this" to dst
  void   setData(StorePData& src)  {data = src;}        // copy data from src to "this"

  Words* bSearch(String& key) {return data.bSearch(key);}
  Words* find(   String& key) {return data.find(key);}

  void   sort() {qsort(&data[0], &data[data.end()-1]);}

  void   display() {display(data);}
  void   display(StorePData& d);

  int   nData()      {return data.end();}               // returns number of data items in array

  void  probe(TCchar* title) {data.probe(title);}

private:

  void display(Words* words, int i);

  // returns either a pointer to data (or datum) at index i in array or zero

  Words* datum(int i) {return 0 <= i && i < nData() ? data[i] : 0;}

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename StorePIter;
  };





//////////----------------

//extern StoreP storeP;

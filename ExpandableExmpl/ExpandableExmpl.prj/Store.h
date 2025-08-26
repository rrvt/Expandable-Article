// Store Example


#pragma once
#include "Expandable.h"
#include "IterT.h"
#include "qsort.h"

class Random;


class Datum {
public:

ulong  key;                         // A 5 digit key created to be unique
String line;                        // Line from the file

  Datum() : key(0) { }
  Datum(Datum& dtm) {copy(dtm);}
 ~Datum() {clear();}

  void clear()   {key = 0; line.clear();}
  bool isEmpty() {return !key;}

  void display();
  void display(TCchar* prefix);

  Datum& operator=(Datum& dtm) {copy(dtm); return *this;}

  // Required for Insertion Sort, i.e. data = dtm;
  bool operator>= (Datum& dtm) {return key >= dtm.key;}
  bool operator== (Datum& dtm) {return key == dtm.key;}

  // Required for Qsort
  bool operator>  (Datum& dtm) {return key >  dtm.key;}
  bool operator<= (Datum& dtm) {return key <= dtm.key;}

  // Required for Binary Search
  bool     operator== (ulong key) {return this->key == key;}
  bool     operator<  (ulong key) {return this->key <  key;}
  bool     operator>  (ulong key) {return this->key >  key;}

private:

  void copy(Datum& dtm) {key = dtm.key;  line = dtm.line;}
  };


class Store;
typedef IterT<Store, Datum> StoreIter;

typedef Expandable<Datum, 2> StoreData;


class Store {

bool      sorted;
StoreData data;

public:

  Store() : sorted(false) { }
 ~Store() { }

  void   clear() {sorted = false; data.clear();}  // Clear Store
  bool   isSorted() {return sorted;}

  void   load(TCchar* filePath);                    // load linear, set mode
  void   loadSorted(TCchar* filePath);              // load sorted, set mode
  void   load(TCchar* path, StoreData& d, Random& rand, bool sortIt = false);
                                                    // Load an expandable array...
  void   addFile(TCchar* filePath);                 // Add file linear or sorted mode

  void   copyData(StoreData& dst) {dst  = data;}    // Copy data from "this" to dst

  void   getData(StoreData& d) {d -= data;}         // Move data from "this" to d
  void   setData(StoreData& d) {data -= d;}         // Move data from d to "this"

  void   sort() {qsort(&data[0], &data[data.end()-1]);}

  Datum* bSearch(int key) {return data.bSearch(key);}
  Datum* find(   int key) {return data.find(key);}

  void   display() {display(data);}
  void   display(StoreData& d);

  int   nData()      {return data.end();}         // returns number of data items in array

  void  probe(TCchar* title) {data.probe(title);}

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  Datum* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  ulong nextRandom(Random& rand, int i);

  friend typename StoreIter;
  };




///--------------

//extern Store store;

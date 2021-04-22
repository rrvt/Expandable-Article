// Store Example


#include "StdHdr.h"
#include "Store.h"
#include "FileIO.h"
#include "Random.h"


Store store;


void Store::load(TCchar* filePath) {
FileIO fil;
int    i;
String line;
Random rand(213);

  path = filePath;   data.clear();

  if (fil.open(path, FileIO::Read)) {
    wcout << filePath << " opened" << endl;

    for (i = 0; fil.read(line); i++) {
      Datum& dtm = data[i];
      ulong  r   = ulong(rand.next() * 1000.0);             // construct a random number

      dtm.key = r * 100 + i; dtm.line = line.trimRight();   // Shift the random number and make unique
      }
    }

  fil.close();
  }


void Store::appendDatum(TCchar* filePath) {
FileIO fil;
String line;
int    i = 0;
Random rand(213);

  path = filePath;   data.clear();

  if (fil.open(path, FileIO::Read)) {
    wcout << filePath << " opened" << endl;

    while (fil.read(line)) {
      Datum dtm;
      ulong r = ulong(rand.next() * 1000.0);

      dtm.key = r * 100 + i++;   dtm.line = line.trimRight();   data += dtm;
      }
    }

  fil.close();
  }


void Store::loadSorted(TCchar* filePath) {
FileIO fil;
String line;
int    i = 0;
Random rand(213);

  path = filePath;   data.clear();

  if (fil.open(path, FileIO::Read)) {
    wcout << filePath << " opened" << endl;

    while (fil.read(line)) {
      Datum dtm;
      ulong r = ulong(rand.next() * 1000.0);

      dtm.key = r * 100 + i++; dtm.line = line.trimRight();   data = dtm;
      }
    }

  fil.close();
  }


void Store::display() {
int    n = data.end();
int    i;
String s;

  for (i = 0; i < n; i++) {
    s.format(_T("%5i: "), data[i].key);

    wcout << s.str() << data[i].line.str() << endl;
    }
  }


void Store::display2() {
StoreIter iter(*this);
Datum*    dtm;
String    s;

  for (dtm = iter(); dtm; dtm = iter++) {
    s.format(_T("%3i/%5i: "), iter.index(),  dtm->key);

    wcout << s.str() << dtm->line.str();
    }
  }


// Store Example


#include "pch.h"
#include "Store.h"
#include "FileIO.h"
#include "filename.h"
#include "NotePad.h"
#include "Random.h"



void Store::load(TCchar* filePath) {Random rand(213);   load(filePath, data, rand);}



void Store::loadSorted(TCchar* filePath) {Random rand(213);   load(filePath, data, rand, true);}


void Store::addFile(TCchar* filePath) {
String path = filePath;
FileIO fil;
String line;
int    i = 0;
Random rand(521);

  if (fil.open(path, FileIO::Read)) {
    notePad << filePath << " opened" << nCrlf;

    while (fil.read(line)) {
      Datum dtm;

      dtm.key  = nextRandom(rand, i++);
      dtm.line = line.trimRight();

      if (sorted) data =  dtm;
      else          data += dtm;
      }
    }

  fil.close();
  }


// Load the expandable array...

void Store::load(TCchar* filePath, StoreData& d, Random& rand, bool sortIt) {
String path = filePath;
String name = getMainName(path);
String title;
FileIO fil;
int    i = 0;
String line;

  d.clear();   sorted = sortIt;

  if (fil.open(path, FileIO::Read)) {
    notePad << filePath << " opened" << nCrlf;;

    while (fil.read(line)) {
      Datum dtm;
      dtm.key  = nextRandom(rand, i++);
      dtm.line = line.trimRight();

      if (sorted) d  = dtm;
      else        d += dtm;
      }
    }

  fil.close();

  title = name + _T(" Loaded");   d.probe(title);
  }


// construct an non-zero unique random number

ulong Store::nextRandom(Random& rand, int i) {
ulong  r = ulong(rand.next() * 997.0) + 7;

  return r * 100 + i;
  }



void Store::display(StoreData& d) {
int    n = d.end();
int    i;

  notePad << nClrTabs << nSetRTab(6) << nSetTab(8);

  for (i = 0; i < n; i++) d[i].display();
  }


static String Blanks = _T("        ");

void Datum::display(TCchar* prefix) {
int n = 12 - _tcslen(prefix);

  notePad << prefix;   if (n > 0) notePad << Blanks.substr(0, n);

  display();
  }


void Datum::display() {notePad << nTab << key << _T(' ') << nTab << line << nCrlf;}



/////-----------------

#if 0
//Store store;
#if 1
#else
      ulong r = ulong(rand.next() * 1000.0);

      dtm.key = r * 100 + i++;
#endif

#if 1
#else                                       // Index into array
      ulong  r   = ulong(rand.next() * 1000.0);             // construct a random number

      dtm.key = r * 100 + i;
#endif

#if 1
#else
String path = filePath;
String name = getMainName(path);
String title;
FileIO fil;
String line;
int    i = 0;
Random rand(213);

  data.clear();   sorted = true;

  if (fil.open(path, FileIO::Read)) {
    notePad << filePath << " opened" << nCrlf;;

    while (fil.read(line)) {
      Datum dtm;
      dtm.key  = nextRandom(rand, i++);
      dtm.line = line.trimRight();

      data = dtm;
      }
    }

  fil.close();

  title = name + _T(" Loaded");   data.probe(title);
#endif
#if 1
#else
String path = filePath;
String name = getMainName(path);
String title;
FileIO fil;
int    i;
String line;
Random rand(213);

  data.clear();   sorted = false;

  if (fil.open(path, FileIO::Read)) {
    notePad << filePath << " opened" << nCrlf;

    for (i = 0; fil.read(line); i++) {
      Datum& dtm = data[i];
      dtm.key  = nextRandom(rand, i);
      dtm.line = line.trimRight();   // Make unique random number
      }
    }

  fil.close();

  title = name + _T(" Loaded");   data.probe(title);
#endif
      #if 1
#else
  if (fil.open(path, FileIO::Read)) {
    notePad << path << " opened" << nCrlf;

    while (fil.read(line)) {
      if (sorted) {Words words;   words.load(line);   data = words;}
      else          {Words& words = d.nextData();  words.load(line);}
      }                                                         // nextData at the end of the array

    }
  fil.close();

  title = name + _T(" Loaded");   d.probe(title);
#endif

#endif

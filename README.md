# Expandable(P) Example

Expandable(P) are templates for storing records in a program.  The templates model the behavior on
a single dimension array, a vector, i.e. a[i].  The difference between c++ vectors and
Expandable(P) is that a c++ vector is fixed in size at compile time and an Expandable(P) vector is
not.  The size of an Expandable(P) is directly related to the data that is stored in it.
Unfortunately just increasing the size incrementally would slow everything down.  So the size is
determined to be a power of two larger than the index of the last element stored in the vector.  In
other words it gets big really fast...

The data kept in an Expandable vector is moved every time the size increases.  The Expandable
version stores an object in its vector.  Since the object could be really big moving the data might
seem expensive.  Reducing the number of times the move operation occurs led to the doubling size
algorithm.  One is also allowed to specify the beginning size.

ExpandableP reduces the move overhead by only storing a pointer to an object stored in the vector
at a specific index.  Then when the move occurs only the pointer is moved, not the object.

There is another advantage of ExpandableP over Expandable.  A reference or pointer to an object
in an Expandable vector is only valid until the next expansion of the array.  A reference or
pointer to an object in an ExpandableP vector is to an object that is never moved (only a pointer
to it is moved).  Thus the reference or pointer is valid until the object is removed from
ExpandableP vector.

## Expandable

Expandable produces a vector of objects.  The first step is to create an object, then create a
vector of them:

  #include "Expanable.h"

  class Datum {
    ...
    Datum& operator=(Datum& dtm) {copy(dtm); return *this;}     // required for assignment
    ...
    };

  typedef Expandable<Datum, 5> Vector;
  Vector a;
  Vector b;
  Datum  dtm;
  Datum* p;

Now the data is defined what can be done with it?  Here are some examples of things:

  a.clear();                    // Vector a is cleared (i.e. all objects are destructed)

  n = a.end();                  // The number of occupied elements in the vector

  a[0] = dtm;   a[1] = p;       // p should contain a pointer to a Datum somewhere or zero

  dtm        = a[1];
  Datum& ref = a[1];            // Remember this reference is good only until next expansion...
  p          = &a[0]            // Ditto above

  a         += dtm;   a += p;   // append object to the end of the vector (i.e. one beyond the
                                // last occupied element of the vector)
  Datum& ref = a.nextData();    // To avoid a copy needed in "+=" nextData returns a reference
                                // that can be used to store data directly in the vector.
  Datum& ref = a.getData(i);    // Returns a reference to the ith element of the vector that
                                // can be used to store directly in the vector.  It can only
                                // increase size of the vector by one element.
  bool rslt = a(i, dtm);        // Insert dtm into vector at i moving existing objects out of the
                                // way.  Fails if i is not within elements in the existing vector.
  a.push(dtm);                  // Push object on end of vector
  a.pop(dtm);                   // Pop object from end of vector (removing object from the vector)
  a.del(i);                     // Delete object (i.e. destruct it) and remove it from the vector
                                // and adjust the vector to fill in the empty spot

In addition to the above operations the vector should be searchable and sortable.  Adding the
following boolean operations to support operations on the vector:

  Note below KeyType is either a class, struct, typedef or c/c++ type that provides all 6 boolean
  operations.

  class Datum {
  KeyType key;
    ...
    Datum& operator=(Datum& dtm) {copy(dtm); return *this;}

    // Required for Insertion Sort, i.e. data = dtm;
    bool operator>= (Datum& dtm) {return key >= dtm.key;}
    bool operator== (Datum& dtm) {return key == dtm.key;}

    // Required for Qsort
    bool operator>  (Datum& dtm) {return key >  dtm.key;}
    bool operator<= (Datum& dtm) {return key <= dtm.key;}

    // Required for Binary Search
    bool     operator== (KeyType key) {return this->key == key;}
    bool     operator<  (KeyType key) {return this->key <  key;}
    bool     operator>  (KeyType key) {return this->key >  key;}
    };

  a = dtm;   a = p;             // Insertion sort object into the vector, vector is sorted on key
  p = a.find(key);              // Return a pointer to an object is based on a linear search for
                                // key
  p = a.bSearcy(key);           // Return a pointer to an object is based on a binary search of
                                // the sorted vector (won't work if vector not sorted on key)

The following operations supplied with Expandable are on the whole vector.

  b = a;                        // Copy the vector a to the vector b, the original contents of b
                                // are deleted (i.e. destructed).  The vector a is unaffected.
  b += a;                       // Append the vector a to the vector b.  The vector a is
                                // unaffected.
  b -= a;                       // Move the contents of vector a to vector b.  The original
                                // contents of be are deleted (i.e. destructed).

## ExpandableP

ExpandableP is a bit harder to create because it needs a pointer class to do a lot of the work.
Fortunately, the pointer class is a template also.  Here is a declaration of an ExpandableP vector:

  typedef DatumPtrT<  Datum, KeyType>            DatumP;         // Define pointer structure
  typedef ExpandableP<Datum, KeyType, DatumP, 2> VectorPData;    // Define array structure

  VectorPData a;
  VectorPData b;
  Datum       dtm;
  Datum*      p;

The operations on an Expandable vector appear in the ExpandableP vector with some subtle
differences.  One of those differences is that we are dealing with allocated and deallocated
objects.  When an operation accepts a pointer it is assumed by the operation that the object has
been allocated from the heap and just moves the pointer into the vector.  At that point the vector
owns the object and it must not be deallocated outside of the vector.  Here is an example:

  a += dtm;                     // Allocate an object and copy the data from dtm in the new object.
                                // The new object is appended to the vector
  a += p;                       // The object pointer, p, is appended to the vector a.

There is another version of delete object due to the notion that sometimes a pointer is all that is
available to specify the object to be deleted:

  a.del(p);                     // Find object with address p, delete (and deallocate it also) and
                                // adjust the vector to fill in the empty spot.

The Datum class for ExpanadbleP needs an additional assignment operator due to the use of pointers:

  class Datum {
    ...
    Datum& operator= (Datum* dtm) {copy(*dtm);   return *this;}
    Datum& operator= (Datum& dtm) {copy(dtm);    return *this;}
    ...
    };

Due to the need for allocated objects, two functions have been added to allocate/deallocate
objects.

  p = a.allocate();             // Allocates an object, execute the constructor and return a
                                // pointer to the object.
  a.deallocate(p);              // execute the destructor and "free" the object

## Sorting an Expandable(P)

There is also a template for a sorting algorithm that combines qsort and insert sort.  It works on
standard c/c++ vectors and it also works exactly the same way on an Executable(P) vector.  Here is
an example of using qsort:

  qsort(a[0], a[a.end()-1]);    // This works for both Expandable templates

## Iterator

An Expandable(P) is a container for data.  However, I've found that its best use is to bury it in
a class as a private object.  But doing that makes access to the data in the Expandable object a
bit more difficult.  The IterT (i.e. Iterator Template) provides a simple way to increment through
the elements of an Explanable object whether one is implementing from functions inside or outside
the class containing the Expandable object.

The Iterator template is used to declare an object which the actual iterator.  Unfortunately it
cannot reach inside a private Expanable Object to get the information needed, so some helper
functions are needed.  They should be hidden also, so the best solution is to make the Iterator
type a friend of the class holding the Expandable object.  Here is the pattern used to implement
an Iterator:

  #include "IterT.h"

  class Store;
  typedef IterT<Store, Datum> StoreIter;


  class Store {
  Vector data;

  public:

    ...

  private:

    int   nData()      {return data.end();}         // returns number of data items in array

    // returns either a pointer to data (or datum) at index i in array or zero

    Datum* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

    void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

    friend typename StoreIter;
    };

So when a Store object is declared an Iterator on the data within the object is possible both
within the object and external to the object.  Here is an example:

  Store store;

  bool memble(TCchar* key) {
  StoreIter iter(store);                // when within a Store object use ... iter(*this);
  Datum*    dtm;

    for (dtm = iter(); dtm; dtm = iter++) {

      // dtm is a pointer to the objects stored in the data and when the last one is exceeded
      // dtm becomes zero which the for loop terminates it (zero is the universal false in c/c++).
      }

Look at the IterT.h templates to see the details of the incrementing from zero to n-1, decrementing
from n-1 to 0, and removal of the node a specific point in the incrementing/decrementing.

## Purpose of App

The purpose of this app is to demonstrate the operations and attributes of both Expandable(P)
templates.  Store implements an Expandable object and StoreP implements an ExpandableP object.

There two flavors of commands, setup for demonstrations and execution of a demonstration.  There
are two setup commands:
  - Select a file to be used later
  - Select the template version (Expandable or ExpandableP) to be used later

The commands are organized in a drop down list in the order that they should be used.  Note,
that some features need to be used to just load the Expandable(P) objects in store and storeP.
To get the most out of this App one should compile it in debug mode and both study the code and
step through it.  I've compiled it with some serious optimization so stepping doesn't always show
all the code.  Here is a list of the demonstrations:
  - clear the Expandable(P) object
  - Load the Expandable(P) object from a file (picked earlier), uses a.nextData()
  - Copy one Expandable(P) object to another
  - Append one Expandable(P) object to another
  - Move one Expandable(P) object to another
  - Copy a Datum object from Expandable(P) object with an array reference dtm = a[i]
  - Copy a Datum object to an Expandable(P) object with an array reference a[i] = dtm
  - Allocate and deallocate a Datum object for an ExpandableP object: p = a.allocate(), ...
  - Append a Datum object pointed to by p to an Expandable(P) object:  a += p;
  - Append a Datum object to an Expandable(P) object:  a += dtm;
  - Insert a Datum object pointed to by p to an Expandable(P) object:  a = p; governed by
    conditionals in the Datum object (usually an insertion sort)
  - Insert a Datum object to an Expandable(P) object:  a += dtm; governed by
    conditionals in the Datum object (usually an insertion sort)
  - Insert a Datum object pointed to by p to an Expandable(P) object:  a(i, p); at index i
    moving the other objects out of the way
  - Insert a Datum object to an Expandable(P) object:  a(i, dtm); at index i
    moving the other objects out of the way
  - Get a Datum reference at index i:  ref = a.getData(i);
  - Push two Datum objects onto an Expandable(P) object and pop them off
  - Delete an object from an Expandable(P) object and fill in the empty slot:  a.del(i) and
    for ExpandableP delete an object defined by its address a.del(p);
  - Find a Datum object based on a key, where the Datum defines what a key is.
  - find a Datum object using a binary search on a sorted vector
  - Demo an iterator in both stores
  - Demo a qsort

## Getting Started

This version was created with Visual Studion 2022 (VS22).  It was compiled with the following
properties:
  - Windows SDK Version: 10.0.xxxxx (latest)
  - Platfor Toolset: visual Studio 2022 (latest)
  - MFC: Use MFC in a Shared DLL
  - Character Set:  Use Unicode Character Set
  - Precompiled Header:  Not Using Precompiled Headers
  - Linker/Additional Dependencies:  Htmlhelp.lib

The HTML Help Workshop (HHW), Version 4.74.8702.0 (the last one produced) was used to prepare the
help file (WixApp.chm).  It is copied into the Release directory.  I used Dreamweaver (DW) to do
most of the content production of the help files that the HTML Help Workshop produces (i.e. HHW is
used to produce the pages/files needed and DW is used to fill in the content).

Wix is used to create the &lt;AppName&gt;.msi file.  WixApp (one of my applications, see git) was
used to produce the product.wxs file.

Note:  In order to avoid hickups during the compilation/link process make sure the VS22
dependencies are set so the projects are produced in the proper order.

## Prerequisites

Windows 11 or above.  visual Studio 2022 or above.  The WiX Toolset must be installed in Visual
Studio.  The "HTML Help Workshop" (google it) must be installed on the PC.

All the code is contained in the Projects:

  - ExpandableExmpl -- contains the code to execute the Expandable code
  - Library         -- contains the Expandable and iterator templates and other useful modules

## Author

Robert R. Van Tuyl

Contact:  romeo romeo victor tango alpha tango sam whisky delta echo dot charlie oscar mike

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details



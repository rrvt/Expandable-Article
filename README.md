# Expandable Example

Expandable is a template that is just a method for storing records in a program.
It is certainly more expensive than other methods but it is simple and accomodates records
containing any data.  Once the data is loaded a binary search on a sorted key is possible along with
linear searche as part of the template.  A simple iterator may be implemented for a class that contains
an Expandable vector.

A better description resides in the Word and pdf file "ExpandableVector".  While I did write an article
for CodeProject this version of the article may be better.

The console example program is a simple console program with a few examples of the use of expandable.
It is intended to be used in the Visual Studio Debugger so that the code can be examined.
Here is the list of command lines that can be selected and followed in the code:

  - Display -- Load a file using an array reference (i.e. data[i] = record) and display the contents
    using an array reference (i.e. cout << data[i] etc.) in the order the lines appear in the file
  - Sort -- Load the file then use qsort to sort the array
  - Append -- Load a file using the append operator (i.e. data += record) and display the result.
  - LoadSorted -- Load the file using the insertion sort operator (i.e. data = record;).  To make it
    interesting a random number is used as the key.
  - IterDisplay -- Load a file and then use an iterator to display the file.
  - BSearch -- Load a file storing it with a random key.  Use an iterator to touch every record and
    then use a binary search to find the record and display it or an error.
  - LinearSrch -- load a file and then perform a linear search (i.e. find(key)) on each line in the file

The following examples use an ExpandableP template.

  - LoadStoreP -- Get a heap node at the end of the data array and then put the data into the node.
  - LoadSortedP -- Fill a local copy of the record with the data then perform an insertion sort of
    the record into the data (after copying the data from the local record to a heap record).
  - BSearchP -- After the data is sorted perform a binary search on the key
  - LinearSrchP -- perform a linear search for each record in the vector.

### Prerequisites

Compiled with Visual Studio 2017 +/- and the ability to
debug to view the code in action.

All the code is contained in the Projects:

  - ExpandableExample -- contains the code to execute the Expandable code
  - Library -- contains the Expandable and iterator templates and other useful modules

## Getting Started

This version was created with Visual Studion 2022.  It was compiled with the following properties:
  o Windows SDK Version: Latest
  o MFC: Use MFC in a Shared DLL
  o Character Set:  Unicode

## Updates

### Update 6/27/23

Library Updates.

### Update 4/30/22

Moved the example to Win11 and Visual Studio 2022.  Rewrote the article to be more like a tutorial, one
little topic at a time.

## Authors

Robert R. Van Tuyl, romeo romeo victor tango alpha tango sam whisky delta echo dot charlie oscar mike

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details





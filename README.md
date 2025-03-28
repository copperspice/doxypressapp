## DoxyPressApp

### Introduction

DoxyPress is a modern C++ application for generating documentation from annotated sources and API documentation.
DoxyPressApp is a GUI application used to configure the JSON project file and run DoxyPress.

DoxyPress and DoxyPressApp were initially derived from Doxygen and DoxyWizard.


###### Major Enhancements

Parsing

* User selection for full parsing of C and C++ with clang in lieu of the legacy lex parser
* Clang parser allows complicated modern C++ to be parsed accurately, especially as C++ is updated
* Added a new parser for makefiles*

Classes and Containers

* DoxyPress links with the CopperSpice libraries
* Removed all obsolete classes and containers
* Refactored to use value based containers instead of pointer based containers
* For improved memory management variadic templates and shared pointers are used
* Source code reformatted for improved readability
* Modified source code to use modern C++ functionality, features, and syntax

Output

* Project file format enhanced to use a JSON format for improved functionality
* Command line option to convert Doxygen project files to the DoxyPress format
* Enhanced layout file to include new sections
* Improved html output for correctness


### Binary Files

DoxyPressApp prebuilt binaries are available for all supported platforms.

https://github.com/copperspice/doxypress/releases/
https://download.copperspice.com/doxypress/binary/

CopperSpice prebuilt binaries are available for all supported platforms.

https://github.com/copperspice/copperspice/releases/
https://download.copperspice.com/copperspice/binary/


### System Requirements

Building DoxyPressApp requires a C++20 compiler and a C++20 standard library. You will also need a current copy
of the CopperSpice libraries. DoxyPressApp links with CsCore, CsGui, CsNetwork, and CsXml.

CMake build files are provided with the source distribution to build this application.


### Documentation

Full documentation for DoxyPress is available on the website or from our download page.


|URL      |Description|
|---------|-----------|
|https://www.copperspice.com/docs/doxypress/index.html| Online
|https://download.copperspice.com/doxypress/documentation| Download (tar and zip formats)|


For documentation on how to build the DoxyPress application refer to the following page:

https://www.copperspice.com/docs/doxypress/build-doxypress.html


### Presentations

Our YouTube channel contains over 75 videos about C++, programming fundamentals, Unicode/Strings, multithreading,
graphics, CopperSpice, DoxyPress, and other software development topics.

https://www.youtube.com/copperspice

Links to additional videos can be found on our website.

https://www.copperspice.com/presentations.html


### Authors / Contributors

* **Ansel Sermersheim**
* **Barbara Geller**
* **Andrew Marlow**
* **Adam Mensel**
* **Tim van Deurzen**


### License

This program is released under the GPL 2 license. For more information refer to the LICENSE file provided with this
project.


### References

 * Website:  https://www.copperspice.com
 * Twitter:  https://twitter.com/copperspice_cpp
 * Email:    info@copperspice.com

<!-- -->
 * Github:   https://github.com/copperspice

<!-- -->
 * Forum:    https://forum.copperspice.com
 * Journal:  https://journal.copperspice.com

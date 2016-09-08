# About
n-body-sim simulates how gravity affects bodies with mass in three dimensional space. This program reads in an XML file of initial conditions of a system and displays on a screen, in real-time, the positions of bodies in the system.

# Building
Run _make_ to build the software. Run _./n-body-sim_ to run the simulation. Run _make clean_ to remove all binary and executable files generated in the build process.

# Dependencies
This program is a C++ program and it depends on the [SDL library](https://www.libsdl.org/), which must be downloaded separately, and the rapidxml library, which has been included in this project.

# Support
n-body-sim works only on Mac OS X and has been tested on version 10.11.6. All other operating systems are not expected to be able to compile this project without changes to the makefile.

# License 
n-body-sim is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

n-body-sim is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with n-body-sim.  If not, see <http://www.gnu.org/licenses/>.

## rapidxml
The rapidxml library included on this software and is licensed under both the Boost Software License and the MIT License, where users may choose which license to use in their project. n-body-sim shall use rapidxml under the MIT License. 

# Structure
- Doxyfile is a doxygen input file and is used to auto-generate documentation for this project
- headers/ contains the specifications to the programs classes
- inputs/ contains input xml files that the program can parse to bring in scenarios
- LICENSE.txt contains a copy of the GPLv3 text
- logo.svg logo for the project
- Makefile is the makefile for the project used to convert the source files into an executable
- rapidxml/ contains the rapidxml library used to parse xml files and used under the MIT license
- src/ contains the implementation of the programs classes
- tests/ contains unit tests for the project

# Coding Standards
Consistancy helps build maintainable code, even if you don't agree with all the decisions. If you contribute to this project, please follow these guidelines:
- Always use tabs for indentation, never use spaces. If your editor does not render them the way you want, consider different editors.
- Never use global variables outside of classes. Pass pointers to functions and methods instead.
- Never use macros or _\#define_, these are global and their scope cannot be controled with namespaces. Use functions and const variables instead.
- Always call out namespaces explicitly. Never use _using namespace_. _using namespace_ can cause name collisions when libraries are updated.
- Always document classes, methods, functions, structures, enumerations and variables with javadoc comments in the specification (header file). This will allow the code to be automatically documented with Doxygen.

To get this project to work under Windows do the following:

1. Download the Windows Visual C++ development libraries from: http://libsdl.org/download-2.0.php
2. Make a folder called C:\vs_dev_lib
3. Place the contents of the downloaded zip file into C:\vs_dev_lib
4. Copy C:\vs-dev-lib\SDL2-2.0.5\lib\x86\SDL2.dll to C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin
5. Copy C:\vs-dev-lib\SDL2-2.0.5\lib\x86\SDL2.lib to C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib
5. Copy C:\vs-dev-lib\SDL2-2.0.5\lib\x86\SDL2main.lib to C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib
5. Copy C:\vs-dev-lib\SDL2-2.0.5\lib\x86\SDL2test.lib to C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib
6. Run git submodule init
7. Run git submodule update
8. Download the zip file of Boost at https://dl.bintray.com/boostorg/release/1.64.0/source/
9. Unpack the zip file and place the files into C:\Program Files (x86)\boost\boost_1_64_0
10. In the command line run: chdir C:\Program Files (x86)\boost\boost_1_64_0
11. bootstrap
12. .\b2
13. Wait for the build process to complete (it will take a while). 
14. When it's done you'll be able to compile and run the program with visual studio.
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
8. Download http://cs.du.edu/~mitchell/pthreads_compiled.zip
9. Create the folder C:\vs-dev-lib\pthread
10. Put the files pthread.h, sched.h, sempahore.h into C:\vs-dev-lib\pthread
11. Place pthreadVC2.dll into C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin
12. Place pthreadVC2.lib into C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib
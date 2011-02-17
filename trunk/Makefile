# LPATH tells the linker where to find libraries
LPATH = -Llib -lcartwheel -L/misc/load/linux_x86_64_opteron 

# IPATH tells the compiler where to look for include files.
IPATH = -Iinclude -I/misc/include 

# CWLIBS are the libraries necessary for Cartwheel-3d
CWLIBS = -lode 

# GLLIBS are the GLUT and OpenGL libraries needed by the linker.
GLLIBS = -lglut -lGLU -lGL -lGLEW

# XLIBS are the X libraries needed by the linker because GLUT and OpenGL
#       call X routines.
# XLIBS = -lXi -lXmu -lX11 -lXext

# MISCLIBS are miscellaneous libs that are needed by the linker.
# -lm denotes the math library.
MISCLIBS = -lm -lgsl -lgslcblas

# Used to have LPATH in front
LIBS = $(CWLIBS) $(GLLIBS) $(MISCLIBS)

CPP_FILES := $(wildcard src/**/*.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)

# compiler
CC = g++
# compiler flags:
# -g	tells the compiler to produce symbolic information that a debugger
#	(like gdb) needs.
# -Wall	tells the compiler to print warnings about pretty much everything.
# -w    Hide all of the warnings because I don't like seeing them
CFLAGS = -g -w -fPIC #-DdDOUBLE

all : library mainControl 

library : $(OBJ_FILES)
	$(CC) -shared -o lib/libcartwheel.so $(OBJ_FILES) 

# The default way to convert .c files into .o files.
%.o : %.cpp		
	$(CC) $(CFLAGS) $(IPATH) -o $@ -c $< 

clean :
	$(RM) src/*/*.o lib/libcartwheel.so
	
mainControl : library 
	$(CC) $(CFLAGS) $(IPATH) -o src/mainControl.o -c src/mainControl.cpp
	$(CC) $(CFLAGS) -o bin/mainControl src/mainControl.o $(LPATH) $(LIBS)

# OSTYPE is not set by default on Mac OS X.  Should end up being "darwin"
ifndef OSTYPE
  OSTYPE = $(shell uname -s|awk '{print tolower($$0)}')
  #export OSTYPE
endif

# Linux (default)

# Linking information
FRAMEWORKS = 
GLLIBS = -lGLEW -lGL -lGLU -lglut
MISCLIBS = -lm -lgsl -lgslcblas -lode
LPATH = 

# IPATH tells the compiler where to look for include files.
IPATH = -Iinclude 

# CWLPATH tells the linker where to find libraries for building executables
CWLPATH = -Llib -lcartwheel

# CFLAGS contains flags for the compiler
CFLAGS = -g -w -fPIC

# OS X
ifeq "$(OSTYPE)" "darwin"
	FRAMEWORKS = -framework Carbon -framework OpenGL -framework GLUT
	GLLIBS = -lglew
	IPATH = -Iinclude -I/opt/local/include/
	LPATH = -L/opt/local/lib
	CFLAGS = -g -w -fPIC -DdDOUBLE
endif

# Used to have LPATH in front
LIBS = $(GLLIBS) $(MISCLIBS)

CPP_FILES := $(wildcard src/**/*.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)

# compiler
CC = g++
# compiler flags:
# -g	tells the compiler to produce symbolic information that a debugger
#	(like gdb) needs.
# -Wall	tells the compiler to print warnings about pretty much everything.
# -w    Hide all of the warnings because I don't like seeing them


all : library interactControl interface tomsLogger

library : $(OBJ_FILES)
	$(CC) -shared $(FRAMEWORKS) $(LPATH) $(LIBS) -o lib/libcartwheel.so $(OBJ_FILES) 

# The default way to convert .c files into .o files.
%.o : %.cpp		
	$(CC) $(CFLAGS) $(IPATH) -o $@ -c $< 

clean :
	$(RM) src/*.o src/*/*.o lib/libcartwheel.so

mainControl : library 
	$(CC) $(CFLAGS) $(IPATH) -o src/mainControl.o -c src/mainControl.cpp
	$(CC) $(CFLAGS) $(FRAMEWORKS) -o bin/mainControl src/mainControl.o $(CWLPATH) $(LPATH) $(LIBS)

interactControl : library 
	$(CC) $(CFLAGS) $(IPATH) -o src/interactControl.o -c src/interactControl.cpp
	$(CC) $(CFLAGS) $(FRAMEWORKS) -o bin/interactControl src/interactControl.o $(CWLPATH) $(LPATH)  $(LIBS)
	
interface : library 
	$(CC) $(CFLAGS) $(IPATH) -o src/interface.o -c src/interface.cpp
	$(CC) $(CFLAGS) $(FRAMEWORKS) -o bin/interface src/interface.o $(CWLPATH) $(LPATH) $(LIBS)



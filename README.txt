To build the shared library and example executable, run:

make

To run the example executable:

LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH ./bin/mainControl 

(Partial List of) Dependencies:

libgl, libglut
libode, libode-dev
libgsl, libgsl-dev
libglew, libglew-dev

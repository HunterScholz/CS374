# Hunter Scholz's Grayscaler

## Building
Command line:

* `make` to build. An executable called `grayscaler` will be produced.

## Files

* `grayscaler.c`: The main code to run the program
* `ppm.c`: Portable PixMap library for helper functions
* `ppm.h`: Compiler for the ppm library
* `goat.png`: The test image that will be grayscaled
* `graygoat.png`: the output image that is grayscaled

## Data

_[Description of the main data used in the program. Just list the major
data structures—not every variable.]_

There is a Parameters struct that is passed into each thread. It holds 
all the parameters that the run() method will need. It also hols a ppm 
struct, which contains the data and dimensions of the image.

An Outputs struct is created within each thread, holding the average RGB
values of each image slice. This struct is returned back to the main thread.

## Functions

_[This is a tree of functions and their short descriptions]_

* `main()`
  * `usage()` Called in case the arguements are incorrect
  * `run()` Function called by each thread, converts a slice into grayscale
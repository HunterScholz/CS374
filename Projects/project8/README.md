# Hunter Scholz's Reservation Project 7

## Building
Command line:

* `make` to build. An executable called `reservations` will be produced.

## Files

* `reservations.c`: The main code to handle seat reservations
* `.gitignore`: Keeps build files off GitHub

## Data

_[Description of the main data used in the program. Just list the major
data structures—not every variable.]_

The *seat_taken array holds a list of all the seats, denoted by a 0 if they are free
or a 1 if they are taken. The seat_taken_count keeps track of how many seats in 
seat_taken are 1s (taken). This value is verified by the process to check if the program
is working correctly.

## Functions

_[This is a tree of functions and their short descriptions]_

* `main()`
  * `seat_broker()` Run by each thread
    * `reserve_seat()` Marks a seat in seat_taken as taken if its free
      * `lock()`
      * `unlock()`
    * `free_seat()` Marks a seat in seat_taken as free if its taken
      * `lock()`
      * `unlock()`
    * `verify_seat_count()` Checks if the seat_taken_count is accurate to the 1s in seat_taken
      * `lock()`
      * `unlock()`

## Notes

* seat_taken_count must start as 0
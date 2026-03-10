# Project 9 - Producers & Consumers


## Building

Command line:

* `make` to build. Two executables called `pc` and `eventbuf` will be produced.

## Files

* `pc.c`: The main code that creates the producer and consumer threads
* `eventbuf.c`: Code for the eventbuf struct, used in `pc.c`
* `eventbuf.h`: Macros for `eventbuf.c`

## Data

This project mainly uses a Event Buffer, a custom data structure that is a linked 
list and acts as a FIFO queue. It The event buffer holds all incoming events created
by the producer threads, and removes events that are taken by the consumer threads.

## Functions

* `main()`
  * `usage()` Clarifies command line input
  * `eventbuf_create()` Macro that creates a new event buffer
  * `sem_temp_open()` Helper function to create a semaphore
    * `sem_open()`
    * `sem_unlink()`
    * `sem_close()`
  * `pthread_create()`
    * `producer_run()` Creates events for the event buffer
      * `eventbuf_add()`
    * `consumer_run()` Gets events from the event buffer
      * `eventbuf_empty()` Boolean that checks if the event buffer is empty
      * `eventbuf_get()`
  * `pthread_join()`
  * `eventbuf_free()` Frees the memory allocated for the event buffer
  * `sem_close()`

## Notes

* The last output lines should be all the consumers exitting.
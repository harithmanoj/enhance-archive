[//]: # (\mainpage Enhance Main Page)

## ENHANCE ##

Enhance is a collection of multiple `C++` libraries that cater to many needs.

It is released under [GPL License](https://github.com/harithmanoj/Enhance/blob/master/License.md)

Source code is Hosted in [GitHub](https://github.com/harithmanoj/Enhance)

Stable Releases can be found [here](https://github.com/harithmanoj/Enhance/releases)

Code Documentation is Hosted by GitHub Pages [here](https://harithmanoj.github.io/Enhance/index.html)

Including but not limited to :

* Diagnostics.
* Time and related tracking and syncronisation.
* Error Tracking.
* Version info to be used inside programs.
* Concurrent execution.

The Library requires `C++ standard 17` to compile.

_______________________________________________________________________________
## Framework
_______________________________________________________________________________

Framework is a library that defines The version of Enhance and class to store
version info.

Exists in `namespace enh`.

### Headers :

`framework.enh.h`

### The Library 

* Class to be used for storing version information.

* Version of the Enhance library

_______________________________________________________________________________
## Diagnose
_______________________________________________________________________________

Diagnose is a diagnostics library to log to file during sequential code 
execution.

Exists in `namespace debug`, but use Macros for logging easily.

See `logger.enh.h` documentation for usage.

### Headers :

`logger.enh.h`

### The Library 

* Functions that log information to a file unique to each thread
* 5 optimisation levels


_______________________________________________________________________________
## Error Handling
_______________________________________________________________________________

Common is a library that contains functions and classes for various error
 handling uses.

Exists in `namespace enh`.

### Headers :

`common.enh.h`

### The Library 

* Class to be used for base class for inheriting error management functionality.

* Enumeration to provide 3 possible outcomes (good, error, blocked due to 
previous error) for functions.

* `constexpr` constants signum and inclusive ratio, which rounds up in integer 
division.

_______________________________________________________________________________
## Concurrent
_______________________________________________________________________________

Library for ease of concurrent programming.

Exists in `namespace enh`.

### Headers :

`queued_process.enh.h`

### The Library 

* Class that executes a function by passing messages pushed to a queue.
_______________________________________________________________________________
## Time
_______________________________________________________________________________

Library for time tracking, synchronisation etc. 

Exists in `namespace enh`.

### Header:

`timer.enh.h`

`counter.enh.h`

### The Library 

* Tracking time in a sec : min : hr : day manner(representation).

* Tracking time elapsed and providing clients to the class periodical signals.

* Block execution of a thread for a period of time accurately.

## HOW TO INSTALL 

* Download all required headers and source files, and add them to dependancy 
project.

* Make sure to compile in `C++17`.

### Modules

* %Framework - `framework.enh.h`

* %Common - `common.enh.h`

* %Debug - `logger.enh.h, logger.cpp`

* %Queued_Process - `queued_process.enh.h`

* %Counter - `counter.enh.h`

* %Timer - `timer.enh.h`

### Depandencies

| Header / dependencies | Framework |  Common | Debug | Queued_Process  | Counter | Timer |
| --------------------- |  :----:   |  :---:  | :---: |      :---:      |  :---:  | :---: |
| Framework             | O         | X       | X     | X               | X       | X     |
| Common                | Y         | O       | Y     | X               | X       | X     |
| Debug                 | Y         | X       | O     | X               | X       | X     |
| Queued_Process        | Y         | Y       | Y     | O               | X       | X     |
| Counter               | Y         | X       | X     | X               | O       | X     |
| Timer                 | Y         | X       | Y     | X               | X       | O     |


## Contribution


Code contribution is welcome. Refer [Contributing.md](https://github.com/harithmanoj/Enhance/blob/master/Contributing.md) for ways to contribute.



## AUTHOR

Harith Manoj, <harithpub@gmail.com>


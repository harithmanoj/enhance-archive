[//]: # (\mainpage Enhance Main Page)

## ENHANCE ##

Enhance is a collection of multiple `C++` libraries that cater to many needs.

Including but not limited to :

* Diagnostics.
* Time and related tracking and syncronisation.
* Error Tracking.
* Version info to be used inside programs.
* Concurrent execution.

The Library requires `C++ standard 17` to compile.

The Library is released under [GPL License](https://github.com/harithmanoj/Enhance/blob/master/License.md)

Source code is Hosted in [GitHub](https://github.com/harithmanoj/Enhance)

Stable Releases can be found [here](https://github.com/harithmanoj/Enhance/releases)

Code Documentation is Hosted by GitHub Pages [here](https://harithmanoj.github.io/Enhance/index.html)

The branch master contains last stable release code. For latest features, bugfixes etc, see develop branch.

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
## General
_______________________________________________________________________________

General is a library of functions for general use.

Exists in `namespace enh`.

### Headers :

`general.enh.h`

### The Library 

* Check if bits are high in a variable (also constexpr).
* Check if value is within bounds (also constexpr).
* Signum function and inclusive_ration (also constexpr).

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

Error Handling Library is a library that contains functions and classes for 
various error handling uses.

Exists in `namespace enh`.

### Headers :

`error_base.enh.h`

### The Library 

* Class to be used for base class for inheriting error management functionality.

* Enumeration to provide 3 possible outcomes (good, error, blocked due to 
previous error) for functions.

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

* %General - `general.enh.h`

* %Error_Base - `error_base.enh.h`

* %Debug - `logger.enh.h, logger.cpp`

* %Queued_Process - `queued_process.enh.h`

* %Counter - `counter.enh.h`

* %Timer - `timer.enh.h`

### Dependencies

Here the coulumn means depends on.

| Header                | Framework |  General  |  Error_Base | Debug | Queued_Process  | Counter | Timer |
|         :----:        |  :----:   |  :----:   |   :---:     | :---: |      :---:      |  :---:  | :---: |
| Framework             | O         | X         | X           | X     | X               | X       | X     |
| General               | Y         | O         | X           | X     | X               | X       | X     |
| Error_Base            | Y         | Y         | O           | Y     | X               | X       | X     |
| Debug                 | Y         | X         | X           | O     | X               | X       | X     |
| Queued_Process        | Y         | Y         | Y           | Y     | O               | X       | X     |
| Counter               | Y         | X         | X           | X     | X               | O       | X     |
| Timer                 | Y         | X         | X           | Y     | X               | X       | O     |

Row Debug, coloumn Framework is Y, this means Debug module headers includes headers in Framework module.

O means same module, X means no dependency.

## Contribution


Code contribution is welcome. Refer [Contributing.md](https://github.com/harithmanoj/Enhance/blob/master/Contributing.md) for ways to contribute.



## AUTHOR

Harith Manoj, <harithpub@gmail.com>


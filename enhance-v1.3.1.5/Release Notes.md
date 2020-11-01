
[//]: # (\page Release Notes)

Enhance Release Notes
===============================================================================

## Version 1.3

### Version 1.3.1.5.fin

* Added class `confined_base` for a value that is within an interval.
* Added class `NumericSystem` for a value that is within [0,upper_limit).
* Added types for date, month, wkday, yrday, second, minute, hour.
* Made private data members of date, time_stamp, and DateTime class a 
confined type.
* Added function for multiple additions to date, time_stamp, DateTime.

### Version 1.3.0.4.fin

* Modified `queued_process` to stop thread on destructor call.
* Added functions for safe and force join.
* Modified `timer` to stop thread on destructor call.

* Made classes `date`, `time_stamp` and `DateTime` literal types.
* Added comparison functions for `date`, `time_stamp` and `DateTime`.

## Version 1.2

### Version 1.2.0.3.fin

* Added class date
* Added class time_stamp
* Added class DateTime

* Added functions signExtend, signExtendValue, getordinalIndicator

## Version 1.1

### Version 1.1.0.2.fin

* Renamed `common.enh.h` -> `error_base.enh.h`

* Added Library General.
	* moved	`CheckField`, `signum`, `inclusive_ratio` from (now) 
	`error_base.enh.h` to `general.enh.h`
	* Added `signum_fn`, `incl_ratio`, `isConfined`, made `signum` and 
	`inclusive_ratio` deprecated.

* Removed class queued_process inheritance of error_base class.

* Timer class bugFix : start_timer returned true all time, no it returns 
false if timer is already running.

## Version 1.0

### Version 1.0.0.1.fin

* All functionalities :- 

	* class `enh::counter`
	* class `enh::error_base`
	* class `queued_process`
	* class `version_info`
	* enumeration tristate
	* Diagnostics Macro


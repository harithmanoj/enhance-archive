/** ***************************************************************************
	\file logger.enh.h

	\brief The file for logging functionality

	Created 26 April 2020
		
	This file is part of Enhance.

	Copyright 2020 Harith Manoj <harithpub@gmail.com>

	Enhance is free software : you can redistribute it and /or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	Enhance is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with Enhance. If not, see < https://www.gnu.org/licenses/>.



	<h3> How To Use</h3>
	- Include file `logger.enh.h`

	- Make sure to add `logger.cpp` to compilation

	- Use the macros `O#_LOG_LINE, O#_LOG_DESC, O#_LOG_VAL where # in [1,5]` in
	code at specific logging points.
		- `O#_LOG_LINE where # in [1,5]` takes no arguments and logs the 
	completion of that line in that function.
		- `O#_LOG_DESC where # in [1,5]` takes one argument that is a string
	and logs the value of the string and current line and function. 
		- `O#_LOG_VAL where # in [1,5]` takes one argument. The argument must 
	be a type accepted by stream insertion operator. The evaluated value and 
	literal text converted form of passed expresion is logged.

	- When compiling, define `ENH_OPTIMISATION` above the include directive as
	a value between 0 and 5.

	- Higher the optimisation the lower the number of logging, that is for 
	optimisation 0, all logging O5, O4, O3, O2, O1 is active but for 
	optimisation 5 none is active.

	- Define `ENH_CLEAR_OP__` to use no logging (automatic if `_DEBUG` is not
	defined).

	- Define `IGNORE_ENHANCE_DIAGNOSTICS` to remove all logging calls from 
	enhance library classes and functions.

	- Use `ERROR_FLAG_LOG` to log errors managed by `common::error_base` 
	class template, Pass the object with which error_base methods can be 
	called.

	- Use `REPLACE` for expressions to be evaluated only during debug.

	- Use `REPLACE_AS` for expression with different values during debug and
	release.

	<h4> Examples </h4>
	
	Debug active :

	\include{lineno} logger_dbg_ex.cpp

	Debug not active :

	\include{lineno} logger_ndbg_ex.cpp


******************************************************************************/

#ifndef LOGGER_ENH_H

#define LOGGER_ENH_H				logger.enh.h

#include <filesystem>
#include <thread>
#include <string>
#include <sstream>
#include <iomanip>



#ifdef _DEBUG
#ifndef ENH_DEBUG_CONTROL
#define ENH_DEBUG_CONTROL				true
#endif
#ifndef ENH_OPTIMISATION
#define ENH_OPTIMISATION				0
#endif
#endif

#ifdef ENH_CLEAR_OP__
#ifdef ENH_DEBUG_CONTROL 
#undef ENH_DEBUG_CONTROL
#endif

#ifdef ENH_OPTIMISATION
#undef ENH_OPTIMISATION
#endif

#define ENH_OPTIMISATION				10

#endif


#if defined(ENH_DEBUG_CONTROL) && (ENH_OPTIMISATION < 5) && !defined(IGNORE_ENHANCE_DIAGNOSTICS)
#define ERROR_BASE_LOG
#endif

#if  defined(ENH_DEBUG_CONTROL) && (ENH_OPTIMISATION < 5)
/**
	\brief The namespace for diagnostics
*/
namespace debug
{


	/**
		\brief The file to get which file to log into.

		<h3>Return</h3>
		The path of file.

	*/
	std::filesystem::path getFile(
		std::thread::id id /**< : <i>in</i> : The calling thread id.*/,
		std::string function /**< : <i>in</i> : Logging function name.*/
	);


	/**
		\brief Logs a string to a file indicated byt current thread.

		The function which logs the string should be passed to function.
	*/
	void Log(
		std::string lg /**< : <i>in</i> : The string to log.*/,
		std::string function /**< : <i>in</i> : Logging function name.*/
	);

	/**
		\brief Logs calling line, function and file to a file indicated by 
		current thread.
	*/
	void Log(
		std::string file /**< : <i>in</i> : The file in which logging code is 
						 present.*/,
		std::string function/**< : <i>in</i> : Logging function name.*/,
		unsigned long line /**< : <i>in</i> : The line of file in which 
						   logging code is present.*/
	);

	/**
		\brief Logs calling line, function, file and a description to a file 
		indicated by current thread.
	*/
	void Log(
		std::string file /**< : <i>in</i> : The file in which logging code is 
						 present.*/,
		std::string function /**< : <i>in</i> : Logging function name.*/,
		unsigned long line /**< : <i>in</i> : The line of file in which 
						   logging code is present.*/,
		std::string descr /**< : <i>in</i> : The string to log.*/
	);

	/**
		\brief Logs calling line, function, file and a value to a file
		indicated by current thread.

		The type should be accepted by the stream insertion operator.
	*/
	template<class T>
	void Log(
		std::string file /**< : <i>in</i> : The file in which logging code is 
						 present.*/,
		std::string function /**< : <i>in</i> : Logging function name.*/,
		unsigned long line /**< : <i>in</i> : The line of file in which 
						   logging code is present.*/,
		std::string var /**< : <i>in</i> : The name of variable.*/,
		const T& val /**< : <i>in</i> : The value of the variable.*/
	)
	{
		std::ostringstream out;
		out << std::setw(80) << file << " : " << std::setw(6) << line << "   " << std::setw(15) << function << "  " << var << " = " << val;
		Log(out.str(),function);
	}

}
#endif


#ifdef ENH_DEBUG_CONTROL

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined.
*/
#define REPLACE(x)			x

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined.
*/
#define REPLACE_AS(x,y)		x


/**
	\brief The non-diagnostic version of REPLACE(x).
*/
#define NDBG_REPLACE(x)	

/**
	\brief The non-diagnostic version of REPLACE_AS(x,y).
*/
#define NDBG_REPLACE_AS(x,y)		y

#else

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined.
*/
#define REPLACE(x)		

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined.
*/
#define REPLACE_AS(x,y)		y


/**
	\brief The non-diagnostic version of REPLACE(x).
*/
#define NDBG_REPLACE(x)	

/**
	\brief The non-diagnostic version of REPLACE_AS(x,y).
*/
#define NDBG_REPLACE_AS(x,y)		y

#endif


#if ENH_OPTIMISATION < 5

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	 or if ENH_OPTIMISATION is greater than 5.
*/
#define O5_REPLACE(x)			REPLACE(x)

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if ENH_OPTIMISATION is greater than 5.
*/
#define O5_REPLACE_AS(x,y)		REPLACE_AS(x,y)

#else

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	 or if ENH_OPTIMISATION is greater than 5.
*/
#define O5_REPLACE(x)			NDBG_REPLACE(x)

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if ENH_OPTIMISATION is greater than 5.
*/
#define O5_REPLACE_AS(x,y)		NDBG_REPLACE_AS(x,y)

#endif


#if ENH_OPTIMISATION < 4

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	 or if ENH_OPTIMISATION is greater than 4.
*/
#define O4_REPLACE(x)			REPLACE(x)

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if ENH_OPTIMISATION is greater than 4.
*/
#define O4_REPLACE_AS(x,y)		REPLACE_AS(x,y)

#else

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	 or if ENH_OPTIMISATION is greater than 4.
*/
#define O4_REPLACE(x)			NDBG_REPLACE(x)

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if ENH_OPTIMISATION is greater than 4.
*/
#define O4_REPLACE_AS(x,y)		NDBG_REPLACE_AS(x,y)

#endif


#if ENH_OPTIMISATION < 3

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	 or if ENH_OPTIMISATION is greater than 3.
*/
#define O3_REPLACE(x)			REPLACE(x)

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if ENH_OPTIMISATION is greater than 3.
*/
#define O3_REPLACE_AS(x,y)		REPLACE_AS(x,y)

#else

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	 or if ENH_OPTIMISATION is greater than 3.
*/
#define O3_REPLACE(x)			NDBG_REPLACE(x)

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if ENH_OPTIMISATION is greater than 3.
*/
#define O3_REPLACE_AS(x,y)		NDBG_REPLACE_AS(x,y)

#endif



#if ENH_OPTIMISATION < 2

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	 or if ENH_OPTIMISATION is greater than 2.
*/
#define O2_REPLACE(x)			REPLACE(x)

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if ENH_OPTIMISATION is greater than 2.
*/
#define O2_REPLACE_AS(x,y)		REPLACE_AS(x,y)

#else

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	 or if ENH_OPTIMISATION is greater than 2.
*/
#define O2_REPLACE(x)			NDBG_REPLACE(x)

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if ENH_OPTIMISATION is greater than 2.
*/
#define O2_REPLACE_AS(x,y)		NDBG_REPLACE_AS(x,y)

#endif


#if ENH_OPTIMISATION < 1

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	 or if ENH_OPTIMISATION is greater than 1.
*/
#define O1_REPLACE(x)			REPLACE(x)

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if ENH_OPTIMISATION is greater than 1.
*/
#define O1_REPLACE_AS(x,y)		REPLACE_AS(x,y)

#else

/**
	\brief The Macro to execute a statement only in debug mode.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	 or if ENH_OPTIMISATION is greater than 1.
*/
#define O1_REPLACE(x)			NDBG_REPLACE(x)

/**
	\brief The Macro to execute a statement in debug mode and another if not.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if ENH_OPTIMISATION is greater than 1.
*/
#define O1_REPLACE_AS(x,y)		NDBG_REPLACE_AS(x,y)

#endif





#if !defined(IGNORE_ENHANCE_DIAGNOSTICS) && defined(ENH_DEBUG_CONTROL)

/**
	\brief The Macro to execute a statement only in debug mode, library call.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined.
*/
#define LIB_REPLACE(x)			x

/**
	\brief The Macro to execute a statement in debug mode and another if not, 
	library call.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if 
	ENH_CLEAR_OP__ is defined or if IGNORE_ENHANCE_DIAGNOSTICS is defined.
*/
#define LIB_REPLACE_AS(x,y)		x

#else

/**
	\brief The Macro to execute a statement only in debug mode, library call.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined.
*/
#define LIB_REPLACE(x)		

/**
	\brief The Macro to execute a statement in debug mode and another if not, library call.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined.
*/
#define LIB_REPLACE_AS(x,y)		y


/**
	\brief The non-diagnostic version of LIB_REPLACE(x).
*/
#define NDBG_LIB_REPLACE(x)	

/**
	\brief The non-diagnostic version of LIB_REPLACE_AS(x,y).
*/
#define NDBG_LIB_REPLACE_AS(x,y)		y
#endif

/**
	\brief The Macro to execute a statement only in debug mode, library call.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined or if ENH_OPTIMISATION is less
	than 4.
*/
#define O5_LIB_REPLACE(x)		O5_REPLACE(LIB_REPLACE(x))

/**
	\brief The Macro to execute a statement only in debug mode, library call.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined or if ENH_OPTIMISATION is less
	than 3.
*/
#define O4_LIB_REPLACE(x)		O4_REPLACE(LIB_REPLACE(x))

/**
	\brief The Macro to execute a statement only in debug mode, library call.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined or if ENH_OPTIMISATION is less
	than 2.
*/
#define O3_LIB_REPLACE(x)		O3_REPLACE(LIB_REPLACE(x))

/**
	\brief The Macro to execute a statement only in debug mode, library call.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined or if ENH_OPTIMISATION is less
	than 1.
*/
#define O2_LIB_REPLACE(x)		O2_REPLACE(LIB_REPLACE(x))

/**
	\brief The Macro to execute a statement only in debug mode, library call.

	Evaluates to the macro argument passed if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined or if ENH_OPTIMISATION is less
	than 0.
*/
#define O1_LIB_REPLACE(x)		O1_REPLACE(LIB_REPLACE(x))


/**
	\brief The Macro to execute a statement in debug mode and another if not,
	library call.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if IGNORE_ENHANCE_DIAGNOSTICS is defined
	or if ENH_OPTIMISATION is less
	than 4.
*/
#define O5_LIB_REPLACE_AS(x,y)		O5_REPLACE(LIB_REPLACE_AS(x,y))

/**
	\brief The Macro to execute a statement in debug mode and another if not,
	library call.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if IGNORE_ENHANCE_DIAGNOSTICS is defined
	or if ENH_OPTIMISATION is less
	than 3.
*/
#define O4_LIB_REPLACE_AS(x,y)		O4_REPLACE(LIB_REPLACE_AS(x,y))

/**
	\brief The Macro to execute a statement in debug mode and another if not,
	library call.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if IGNORE_ENHANCE_DIAGNOSTICS is defined
	 or if ENH_OPTIMISATION is less
	than 2.
*/
#define O3_LIB_REPLACE_AS(x,y)		O3_REPLACE(LIB_REPLACE_AS(x,y))

/**
	\brief The Macro to execute a statement in debug mode and another if not,
	library call.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if IGNORE_ENHANCE_DIAGNOSTICS is defined
	or if ENH_OPTIMISATION is less
	than 1.
*/
#define O2_LIB_REPLACE_AS(x,y)		O2_REPLACE(LIB_REPLACE_AS(x,y))

/**
	\brief The Macro to execute a statement in debug mode and another if not,
	library call.

	Evaluates to the first macro argument passed if DEBUG is defined.\n\n
	Evaluates to seconds macro argument if DEBUG is not defined or if
	ENH_CLEAR_OP__ is defined or if IGNORE_ENHANCE_DIAGNOSTICS is defined
	or if ENH_OPTIMISATION is less
	than 0.
*/
#define O1_LIB_REPLACE_AS(x,y)		O1_REPLACE(LIB_REPLACE_AS(x,y))



/**
	\brief The info that is commonly passed to all logging functions.
*/
#define INFO_FOR_LOG		__FILE__,__func__,__LINE__

/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined.
*/
#define LOG_LINE REPLACE(debug::Log(INFO_FOR_LOG))

/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined.
*/
#define LOG_DESC(x) REPLACE(debug::Log(INFO_FOR_LOG,x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,#x,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined.
*/
#define LOG_VAL(x) REPLACE(debug::Log(INFO_FOR_LOG,#x,x))

/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined 
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined.
*/
#define LIB_LOG_LINE LIB_REPLACE(debug::Log(INFO_FOR_LOG))

/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined 
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined.
*/
#define LIB_LOG_DESC(x) LIB_REPLACE(debug::Log(INFO_FOR_LOG,x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,#x,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined 
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined.
*/
#define LIB_LOG_VAL(x) LIB_REPLACE(debug::Log(INFO_FOR_LOG,#x,x))


/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 4.
*/
#define O5_LOG_LINE		O5_REPLACE(LOG_LINE)

/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 4.
*/
#define O5_LOG_DESC(x)	O5_REPLACE(LOG_DESC(x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG, #x, x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 4.
*/
#define O5_LOG_VAL(x)	O5_REPLACE(LOG_VAL(x))

/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 3.
*/
#define O4_LOG_LINE		O4_REPLACE(LOG_LINE)

/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 3.
*/
#define O4_LOG_DESC(x)	O4_REPLACE(LOG_DESC(x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG, #x, x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 3.
*/
#define O4_LOG_VAL(x)	O4_REPLACE(LOG_VAL(x))

/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 2.
*/
#define O3_LOG_LINE		O3_REPLACE(LOG_LINE)

/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 2.
*/
#define O3_LOG_DESC(x)	O3_REPLACE(LOG_DESC(x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG, #x, x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 2.
*/
#define O3_LOG_VAL(x)	O3_REPLACE(LOG_VAL(x))

/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 1.
*/
#define O2_LOG_LINE		O2_REPLACE(LOG_LINE)

/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 1.
*/
#define O2_LOG_DESC(x)	O2_REPLACE(LOG_DESC(x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG, #x, x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 1.
*/
#define O2_LOG_VAL(x)	O2_REPLACE(LOG_VAL(x))

/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 0.
*/
#define O1_LOG_LINE		O1_REPLACE(LOG_LINE)

/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 0.
*/
#define O1_LOG_DESC(x)	O1_REPLACE(LOG_DESC(x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG, #x, x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if ENH_OPTIMISATION is greater than 0.
*/
#define O1_LOG_VAL(x)	O1_REPLACE(LOG_VAL(x))

/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is 
	greater than 4.
*/
#define O5_LIB_LOG_LINE		O5_LIB_REPLACE(LIB_LOG_LINE)

/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is 
	greater than 4.
*/
#define O5_LIB_LOG_DESC(x)	O5_LIB_REPLACE(LIB_LOG_DESC(x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,#x,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is 
	greater than 4.
*/
#define O5_LIB_LOG_VAL(x)	O5_LIB_REPLACE(LIB_LOG_VAL(x))

/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 3.
*/
#define O4_LIB_LOG_LINE		O4_LIB_REPLACE(LIB_LOG_LINE)


/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 3.
*/
#define O4_LIB_LOG_DESC(x)	O4_LIB_REPLACE(LIB_LOG_DESC(x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,#x,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 3.
*/
#define O4_LIB_LOG_VAL(x)	O4_LIB_REPLACE(LIB_LOG_VAL(x))

/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 2.
*/
#define O3_LIB_LOG_LINE		O3_LIB_REPLACE(LIB_LOG_LINE)

/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 2.
*/
#define O3_LIB_LOG_DESC(x)	O3_LIB_REPLACE(LIB_LOG_DESC(x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,#x,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 2.
*/
#define O3_LIB_LOG_VAL(x)	O3_LIB_REPLACE(LIB_LOG_VAL(x))

/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 1.
*/
#define O2_LIB_LOG_LINE		O2_LIB_REPLACE(LIB_LOG_LINE)

/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 1.
*/
#define O2_LIB_LOG_DESC(x)	O2_LIB_REPLACE(LIB_LOG_DESC(x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,#x,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 1.
*/
#define O2_LIB_LOG_VAL(x)	O2_LIB_REPLACE(LIB_LOG_VAL(x))

/**
	\brief The Macro to log line completion in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 0.
*/
#define O1_LIB_LOG_LINE		O1_LIB_REPLACE(LIB_LOG_LINE)

/**
	\brief The Macro to log a string in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 0.
*/
#define O1_LIB_LOG_DESC(x)	O1_LIB_REPLACE(LIB_LOG_DESC(x))

/**
	\brief The Macro to log a variable state in debug mode.

	Evaluates to debug::Log(INFO_FOR_LOG,#x,x) if DEBUG is defined.\n\n
	Evaluates to blank if DEBUG is not defined or if ENH_CLEAR_OP__ is defined
	or if IGNORE_ENHANCE_DIAGNOSTICS is defined	or if ENH_OPTIMISATION is
	greater than 0.
*/
#define O1_LIB_LOG_VAL(x)	O1_LIB_REPLACE(LIB_LOG_VAL(x))
#endif // !LOGGER_ENH_H



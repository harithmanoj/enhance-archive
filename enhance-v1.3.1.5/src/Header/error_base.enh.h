/** ***************************************************************************
	\file error_base.enh.h
	
	\brief The file to declare Error Handling classes and functions
 
	Created 24 March 2020	
		
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

******************************************************************************/



#ifndef ERROR_BASE_ENH_H

#define ERROR_BASE_ENH_H				error_base.enh.h

#include <atomic>
#include "general.enh.h"
#include "logger.enh.h"


/*
	\brief The namespace for all the functions and classes of this library
*/
namespace enh
{
	
		
	/**
		\brief A byte long type that takes 3 values, true (1), error(0),
		previous_error(-1).

		<h3> Examples</h3>

		\include{lineno} tristate_ex.cpp
	*/
	enum class tristate : char
	{
		GOOD = 1 /**< : <i>1</i> : No error flag set. */,
		ERROR = 0 /**< : <i>0</i> : Error flag set now. */,
		PREV_ERR = -1 /**< : <i>-1</i> : Error flag was set by some
					  previous function. */
	};

	/**
		\brief checks if input is not safe (not GOOD).

		<h3>Return</h3>
		Returns false if e is tristate::GOOD and true
		else.\n
	*/
	inline bool operator !(
		tristate e /**< : <i>in</i> : The tristate to check.*/
		) noexcept
	{
		return (e != tristate::GOOD);
	}

	/**
		\brief The class for inheriting error tracking Functionality.


		hasErrorHandlers        = false;\n\n\n

		Value 0x00 is reserved for Safe.\n
		Value 0x01 is reserved for unknown errors.\n
		Value 0x02 is reserved for invalid argument errors.\n\n

		<h3>Template Argument</h3>
		The underlying type to hold error information. (must be integral).

		<h3>Example</h3>

		\include{lineno} error_base_ex.cpp
	*/
	template<class type>
	class error_base
	{
	public:

		static_assert(std::is_integral_v<type>, "error_base should have integral error type");
		
		/**
			\brief The alias to hold constants for error reporting.
		*/
		using error = type;

		/**
			\brief <i>0x00</i> : No error after last clear. 
		*/
		static constexpr error	SAFE = 0x00;

		/**
			\brief <i>0x01</i> : Unknown Error. 
		*/
		static constexpr error UNKNOWN = 0x01;

		/**
			\brief <i>0x02</i> : Invalid argument. 
		*/
		static constexpr error INVALID_ARG = 0x02;
	protected:

		/**
			\brief The error if any occured.
		*/
		std::atomic<error> flag;

	public:



		/**
			\brief default constructor, constructs it as SAFE no error.
		*/
		inline error_base()
		{
			flag = SAFE;
		}

		/**
			\brief The function to clear error flag.

		*/
		inline void clear() noexcept
		{
			flag = SAFE;
		}

		/**
			\brief The function to get error flag.
			
			<h3> Return </h3>
			The error flag.\n
		*/
		inline error getError() const noexcept
		{
			return flag;
		}

		/**
			\brief The function to check if certian error(s) are present.

			<h3> Return</h3>
			Returns true if passed error exists in tag.

		*/
		inline bool checkFlag(
			error check_flag /**< : <i>in</i> : flag to check if raised.*/
		) const noexcept
		{
			return checkField(flag.load(), check_flag);
		}

		/**
			\brief The function to check if no error flag(s) are set.

			<h3>Return</h3>
			true if no error flags are set.\n

		*/
		inline bool isSafe() const noexcept
		{
			return (flag.load() == SAFE);
		}
	protected:
		
		/**
			\brief The function to add the error given as argument to the
			error flag of class.

			For Setting flag while simultaneously return error state.\n\n

			<h3>Return</h3>
			Returns value tristate::ERROR.\n

		*/
		inline tristate setFlag(
			error set /**< : <i>in</i> : flag to be added.*/
		) noexcept
		{
			flag |= set;
			return tristate::ERROR;
		}

		/**
			\brief The function to clear certian fields from error set.

			For clearing certian flags after it has been resolved.\n\n

			<h3>Return</h3>
			Returns value tristate::ERROR if flag is
			not set, tristate::GOOD else.\n

		*/
		inline tristate clearFlag(
			error bitClear /**< : <i>in</i> : flag to be removed.*/
		) noexcept
		{
			if (!checkField(flag.load(), bitClear))
				return tristate::ERROR;
			flag ^= bitClear;
			return tristate::GOOD;
		}

#ifdef ERROR_BASE_LOG


		/*
			\brief The name of derived class.

			<h3>Return</h3>
			The name of the class.\n
		*/
		virtual std::string derived_class() const noexcept
		{
			return "error_base";
		}

		/*
			\brief All error flag(s) set.

			<h3>Return</h3>
			All error flag(s) set.\n
		*/
		virtual std::string error_string() const
		{
			std::string ret = "";
			if (isSafe())
				return "SAFE";
			if (checkFlag(UNKNOWN))
				ret = "UNKNOWN";
			if (checkFlag(INVALID_ARG))
				ret += " + INVALID_ARG";
			
			return ret;
		}
		public:
		
		/**
			\brief Logs value of flag at calling line, function and file to a 
			file indicated by current thread.
		*/
		void Log(
			std::string file /**< : <i>in</i> : The file in which logging 
							 code is present.*/,
			std::string function/**< : <i>in</i> : Logging function name.*/,
			unsigned long line /**< : <i>in</i> : The line of file in which
							   logging code is present.*/,
			std::string variable /**< : <i>in</i> : The variable name of 
								 error holder.*/
		) const noexcept
		{
			debug::Log(file, function, line, derived_class() + " " + variable 
				+ " flag : " + error_string());
		}
#endif
	};

	

}

/**
	\brief Log error flags within enhance library.
	x must be a type that inherits publically from error_base or an object of 
	error_base.
*/
#define LIB_ERROR_FLAG_LOG(x)		O2_LIB_REPLACE(x.Log(INFO_FOR_LOG,#x))

/**
	\brief Log error flags.
	x must be a type that inherits publically from error_base or an object of
	error_base.
*/
#define ERROR_FLAG_LOG(x)			O2_REPLACE(x.Log(INFO_FOR_LOG,#x))

/**
	\brief Log error flags within enhance library using pointer (in class 
	logging using this pointer).
	x must be a type that inherits publically from error_base or an object of
	error_base.
*/
#define LIB_ERROR_FLAG_LOGP(x)		O2_LIB_REPLACE(x->Log(INFO_FOR_LOG,#x))

/**
	\brief Log error flags using pointer (in class logging using this pointer).
	x must be a type that inherits publically from error_base or an object of
	error_base.
*/
#define ERROR_FLAG_LOGP(x)			O2_REPLACE(x->Log(INFO_FOR_LOG,#x))

#endif
/** ***************************************************************************
	\file general.enh.h

	\brief The file to declare general purpose classes and functions

	Created 11 May 2020

	This file is part of Enhance.

	Copyright 2020 Harith Manoj <harithpub@gmail.com>

	Enhance is free software : you can redistribute it and /or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Enhance is distributed base the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Enhance. If not, see < https://www.gnu.org/licenses/>.

******************************************************************************/



#ifndef GENERAL_ENH_H

#define GENERAL_ENH_H				general.enh.h

#include <atomic>
#include <type_traits>
#include <string>


namespace enh
{
	/**
		\brief Check if all the bits high in parameter 'toCheckFor' are high in
		parameter 'base'.\n

		<h3>Return</h3>
		Returns true if all bits high in 'toCheckFor' are high in 'base'.\n

		<h3>Template</h3>
		-#  <code>enumT</code> : The enumeration type (preffered) that is to
		be compared.\n

	*/
	template<class enumT>
	inline constexpr bool checkField(
		enumT base /**< : <i>base</i> : The toCheckFor to check.*/,
		enumT toCheckFor /**< : <i>base</i> : The fields to check for.*/
	) noexcept
	{
		return ((base | toCheckFor) == base);
	}

	/**
		\brief [[deprecated]] The signum function, value is 0 if val is 0, 1 if val > 0,
		-1 if  val < 0.

		<b> DEPRECATED </b> : Use constexpr function signum_fn.
	*/
	template<long long val>
	constexpr short signum = (val > 0) ? 1 : -1;

	template<>
	constexpr short signum<0> = 0;

	/**
		\brief [[deprecated]] The rounded up value of the ratio num / denom.

		<b> DEPRECATED </b> : Use constexpr function incl_ratio.
	*/
	template<unsigned long long num, unsigned long long denom>
	constexpr unsigned long long inclusive_ratio = num / denom +
		signum<num %denom>;


	/**
		\brief The mathematical signum function to extract sign.

		<h3>Template</h3>
		<code>class arithmetic</code> : The arithmetic type of the argument.

		<h3>Return</h3>
		The sign : -1 if argument is negative, +1 if argument is postive, 0 
		if argument is 0
	*/
	template<class arithmetic>
	constexpr short signum_fn(arithmetic arg)
	{
		static_assert(std::is_arithmetic_v<arithmetic>, "signum function takes an arithmetic type");
		if (arg > 0)
			return 1;
		else if (arg < 0)
			return -1;
		else
			return 0;
	}

	/**
		\brief The inclusive ratio (ratio rounded up while deviding).

		<h3>Template</h3>
		<code>class integral</code> : The integral type of the argument.

		<h3>Return</h3>
		The rounded up value, returns num / denom rounded up.

		eg : incl_ratio(25,3) == 9
	*/
	template<class integral>
	constexpr integral incl_ratio(
		integral num /*< : <i>base</i> : The numerator of fraction.*/,
		integral denom /*< : <i>base</i> : The denominator of fraction.*/
	)
	{
		static_assert(std::is_integral_v<integral>, "inclusive ratio is for integral types");
		return (num / denom) + signum_fn(num % denom);
	}

	/**
		\brief Checks if the value is within bounds.

		<h3>Template</h3>
		<code>class type</code> : Any type that can be compared using <, >, ==.

		<h3>Return</h3>
		Returns if unChecked is within interval :

		<table>
			<tr>
				<th> lInclusive </th>
				<th> uInclusive </th>
				<th> Interval </th>
			</tr>
			<tr>
				<th> false </th>
				<th> false </th>
				<th> Non-inclusive (lBounds, uBounds) </th>
			</tr>
			<tr>
				<th> false </th>
				<th> true </th>
				<th> Upper Bound inclusive (lBounds, uBounds] </th>
			</tr>
			<tr>
				<th> true </th>
				<th> false </th>
				<th> Lower Bound inclusive [lBounds, uBounds) </th>
			</tr>
			<tr>
				<th> true </th>
				<th> true </th>
				<th> Inclusive [lBounds, uBounds] </th>
			</tr>
		</table>

	*/
	template<class type>
	constexpr bool isConfined(
		type unChecked /*< : <i>in</i> : The value to check.*/,
		type lBounds /*< : <i>in</i> : The Lower bound of the interval.*/,
		type uBounds /*< : <i>in</i> : The Upper bound of the interval.*/,
		bool lInclusive = false /*< : <i>in</i> : inclusive lower bound?.*/,
		bool uInclusive = false /*< : <i>in</i> : inclusive upper bound?.*/
	)
	{
		bool lCheck(false), uCheck(false);

		if (unChecked < uBounds)
			uCheck = true;
		else if ((unChecked == uBounds) && uInclusive)
			uCheck = true;

		if (unChecked > lBounds)
			lCheck = true;
		else if ((unChecked == lBounds) && lInclusive)
			lCheck = true;

		return lCheck && uCheck;
	}


	/**
		\brief Prepend 0's to the value according to the number passed.

		<code>signExtend("25",4)</code> returns <code>"0025"</code>
	*/
	inline std::string signExtend(
		std::string value /**< : <i>in</i> : The value to sign extend.*/,
		unsigned length /**< : <i>in</i> : The minimum number of digits.*/
		)
	{
		if (value.size() < length)
			return std::string(length - value.size(), '0') + value;
		else
			return value;
	}

	/**
		\brief Prepend 0's to the value according to the number passed.

		<code>signExtend(25,4)</code> returns <code>"0025"</code>
	*/
	template<class arithmetic>
	inline std::string signExtendValue(
		arithmetic value /**< : <i>in</i> : The value to sign extend.*/,
		unsigned length /**< : <i>in</i> : The minimum number of digits.*/
	)
	{
		static_assert(std::is_arithmetic_v<arithmetic>, "Type should be "
			"arithmetic type.");
		return signExtend(std::to_string(value), length);
	}

	/**
		\brief The Ordinal for the value.

		<h3>Template</h3>
		<code>class integral</code> : The integral type of the argument.

		<h3>Return</h3>
		The ordinal indicator used for this value.

		eg : \n
		getOrdinalIndicator(25) == "th"\n
		getOrdinalIndicator(21) == "st"
				
	*/
	template<class integral>
	constexpr inline std::string_view getOrdinalIndicator(integral value)
	{
		static_assert(std::is_integral_v<integral>, "Ordinal Indicator is \
										for integral types");
		if ((value / 10) % 10 == 1)
			return "th";
		if (value % 10 == 1)
			return "st";
		else if (value % 10 == 2)
			return "nd";
		else if (value % 10 == 3)
			return "rd";
		else
			return "th";
	}

}

#endif
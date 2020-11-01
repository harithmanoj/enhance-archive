/** ***************************************************************************
	\file numeral_system.enh.h

	\brief The file to declare class for a custom numeral system

	Created 16 May 2020

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

#ifndef NUMERAL_SYSTEM_ENH_H

#define NUMERAL_SYSTEM_ENH_H					numeral_system.enh.h

#include "confined.enh.h"

namespace enh
{
	/**
		\brief The class for a value that stays between 0 and upper.

		<h3>template</h3>
		-# <code>integral</code> : The type of value.\n
		-# <code>integral upper</code> : The upper limit for the value.\n
	*/
	template<class integral, integral upper>
	class NumericSystem : public confined_base<integral>
	{
	public:
		
		/**
			\brief The upper limit for values of this type.
		*/
		static constexpr confined_base<integral>::value_type limit = upper;

		/**
			\brief The constructor the class initialises value to 0.
		*/
		constexpr inline NumericSystem() noexcept
			: confined_base<integral>::confined_base(
				[&](long long a) {return a < limit; },
				[&](long long a) { return a >= 0; },
				[&]() {return limit - 1; },
				[&]() {return 0; },
				0)
		{}

		/**
			\brief The constructor initiliases with the value of parameter.

			<h3>Exception</h3>
			Throws <code>std::invalid_argument</code> if value is greater 
			than or equal to upper.
		*/
		constexpr inline NumericSystem(
			confined_base<integral>::value_type val
		) : confined_base<integral>::confined_base(
				[&](long long a) {return a < limit; },
				[&](long long a) { return a >= 0; },
				[&]() {return limit - 1; },
				[&]() {return 0; },
				val)
		{}
	};

	/**
		\brief The namespace for all aliases for 
		different upper limited systems.
	*/
	namespace numeric
	{
		/**
			\brief Type with upper limit at 10.
		*/
		using dec = NumericSystem<unsigned short, 10U>;
		
		/**
			\brief Type with upper limit at 100.
		*/
		using cent = NumericSystem<unsigned short, 100U>;
		
		/**
			\brief Type with upper limit at 1000.
		*/
		using kilo = NumericSystem<unsigned short, 1000U>;
		
		/**
			\brief Type with upper limit at 1000000.
		*/
		using mill = NumericSystem<unsigned long, 1000000UL>;

		/**
			\brief Type with upper limit at 1000000000.
		*/
		using bill = NumericSystem<unsigned long, 1000000000ULL>;
	}

}


#endif
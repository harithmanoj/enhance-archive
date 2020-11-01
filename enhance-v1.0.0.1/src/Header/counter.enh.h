/** ***************************************************************************
	\file counter.enh.h

	\brief The file to declare counter class

	Created 29 March 2020	
		
	This file is part of Enhance.

	Copyright 2020 Harith Manoj <harithpub@gmail.com>

		<p>
	Enhance is free software : you can redistribute it and /or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	</p>
	<p>
	Enhance is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.
	</p>
	<p>
	You should have received a copy of the GNU General Public License
	along with Enhance. If not, see < https://www.gnu.org/licenses/>.
	</p>
******************************************************************************/

#ifndef COUNTER_ENH_H

#define COUNTER_ENH_H			counter.enh.h

#include "framework.enh.h"
#include <atomic>
#include <string>
#include <exception>
#include <ostream>
#include <utility>


namespace enh
{
	
	/**
		\brief The class for counter functionality.

		hasErrorHandlers        = false;\n


		Make sure to compile `counter.cpp` also.

		<h3>Example</h3>

		\include{lineno} counter_ex.cpp

	*/
	class counter
	{
		/**
			\brief Stores the seconds state of counter.

			value < 60.\n\n
		*/
		std::atomic<unsigned> seconds;

		/**
			\brief Stores the minutes state of counter.

			value < 60.\n\n
		*/
		std::atomic<unsigned> minutes;

		/**
			\brief Stores the hours state of counter.

			value < 24.\n\n
		*/
		std::atomic<unsigned> hours;

		/**
			\brief Stores the days state of counter.
		*/
		std::atomic<unsigned long long> days;

	public:

		/**
			\brief The destructor for the class.
		*/
		inline ~counter() {}

		/**
			\brief The constructor of the class sets all to 0.
		*/
		inline counter() noexcept : seconds(0), minutes(0), hours(0), days(0)
		{}

		/**
			\brief default copy constructor.
		*/
		inline counter(const counter& c) noexcept
		{
			seconds = c.seconds.load();
			minutes = c.minutes.load();
			hours = c.hours.load();
			days = c.days.load();

		}

		/**
			\brief default move constructor.
		*/
		inline counter(counter&& c) noexcept
		{
			seconds = c.seconds.load();
			minutes = c.minutes.load();
			hours = c.hours.load();
			days = c.days.load();
		}

		/**
			\brief constructs using these values.

			Setting of the state is done by ignoring the excess (sec < 60,
			min < 60, hr < 24).\n\n
		*/
		inline counter(
			unsigned sec /**< : <i>in</i> : The seconds to be set.*/,
			unsigned min /**< : <i>in</i> : The minutes to be set.*/,
			unsigned hr /**< : <i>in</i> : The hours to be set.*/,
			unsigned long long dy /**< : <i>in</i> : The days to be set.*/
		) noexcept
		{
			set(sec, min, hr, dy);
		}

		/**
			\brief default copy assignment.
		*/
		inline counter& operator = (const counter& c) noexcept
		{
			seconds = c.seconds.load();
			minutes = c.minutes.load();
			hours = c.hours.load();
			days = c.days.load();
			return *this;
		}

		/**
			\brief default copy assignment.
		*/
		inline counter& operator = (counter&& c) noexcept
		{
			seconds = c.seconds.load();
			minutes = c.minutes.load();
			hours = c.hours.load();
			days = c.days.load();
			return *this;
		}

		/**
			\brief Resets state to 0s 0m 0hr 0day.


			<h3>Return</h3>
			Reference to current state.\n

		*/
		inline counter& reset() noexcept
		{
			seconds = 0;
			minutes = 0;
			hours = 0;
			days = 0;
			return *this;
		}

		/**
			\brief Resets state to 0s 0m 0hr 0day.


			<h3>Return</h3>
			Reference to current state.\n

		*/
		inline counter& operator () () noexcept
		{
			return reset();
		}

		/**
			\brief Functon sets the value of state to sec number of seconds.
			
			Reset is called.
			If sec is more than 60, It is converted to sec, min, hr, day format
			and overwritten over old values.\n\n
		*/
		void set_seconds(
			unsigned long long sec /**< : <i>in</i> : The seconds to be set.*/
		) noexcept
		{
			reset();
			seconds = sec;
			if (sec >= 60)
			{
				add_minutes((sec / 60));
				seconds = sec % 60;
			}
		}

		/**
			\brief Functon adds sec seconds to the value of state.

			If sec + seconds is more than 60, value is added to minutes,
			hours, days	also.\n\n
		*/
		void add_seconds(
			unsigned long long sec /**< : <i>in</i> : The seconds to be added.*/
		) noexcept
		{
			if ((sec + seconds.load()) >= 60)
			{
				add_minutes((sec + seconds.load()) / 60);
			}

			seconds = (seconds.load() + sec) % 60;
		}

		/**
			\brief Functon sets the value of state to min number of minutes.

			Reset is called.
			If min is more than 60, It is converted to min, hr, day format
			and overwritten over old values.\n\n
		*/
		void set_minutes(
			unsigned long long min/**< : <i>in</i> : The minutes to be set.*/
		) noexcept
		{
			reset();
			minutes = min;
			if (min >= 60)
			{
				add_hours((min / 60));
				minutes = min % 60;
			}

		}

		/**
			\brief Functon adds min minutes to the value of state.

			If min + minutes is more than 60, value is added to
			hours, days	also.\n\n
		*/
		void add_minutes(
			unsigned long long min /**< : <i>in</i> : The minutes to be added.*/
		) noexcept
		{
			if ((min + minutes.load()) >= 60)
				add_hours((min + minutes.load()) / 60);

			minutes = (minutes.load() + min) % 60;
		}

		/**
			\brief Functon sets the value of state to hr number of hours.

			Reset is called.
			If hr is more than 60, It is converted to hr, day format
			and overwritten over old values.\n\n
		*/
		void set_hours(
			unsigned long long hr/**< : <i>in</i> : The hours to be set.*/
		) noexcept
		{
			reset();
			hours = hr;
			if (hr >= 24)
			{
				add_days((hr / 24));
				hours = hr % 24;
			}
		}

		/**
			\brief Functon adds hr hours to the value of state.

			If hr + hours is more than 24, value is added to days also.\n\n

		*/
		void add_hours(
			unsigned long long hr/**< : <i>in</i> : The hours to be added.*/
		) noexcept
		{
			if ((hr + hours.load()) >= 24)
				add_days((hr + hours.load()) / 24);

			hours = ((hours.load() + hr) % 24);
		}

		/**
			\brief Functon sets the value of state to dy number of days.
		*/
		void set_days(
			unsigned long long dy/**< : <i>in</i> : The days to be set.*/
		) noexcept
		{
			reset();
			days = dy;
		}

		/**
			\brief Functon adds dy days to the value of state.

		*/
		void add_days(
			unsigned long long dy/**< : <i>in</i> : The days to be added.*/
		) noexcept
		{
			days += dy;
		}

		/**
			\brief Returns the seconds part of the state.

			<h3>Return</h3>
			The number of seconds.\n

		*/
		inline unsigned get_seconds() const noexcept { return seconds.load();}

		/**
			\brief Returns the minutes part of the state.

			<h3>Return</h3>
			The number of minutes.\n

		*/
		inline unsigned get_minutes() const noexcept { return minutes.load();}

		/**
			\brief Returns the hours part of the state.

			<h3>Return</h3>
			The number of hours.\n

		*/
		inline unsigned get_hours() const noexcept { return hours.load();}

		/**
			\brief Returns the days part of the state.


			<h3>Return</h3>
			The number of days.\n

		*/
		inline unsigned long long get_days() const noexcept
		{ return days.load(); }

		/**
			\brief Returns the total hours elapsed.

			<h3>Return</h3>
			The total hours elapsed.\n

		*/
		unsigned long long get_total_hours() const noexcept
		{
			unsigned long long tot = (unsigned long long)hours.load() +
				(days.load() * 24ULL);
			if ((minutes >= 45) || ((minutes >= 44) && (seconds >= 45)))
				tot += 1;
			return tot;
		}


		/**
			\brief Functon sets the value of state to state input.

			Setting of the state is done by ignoring the excess (sec < 60,
			min < 60, hr < 24).\n\n
		*/
		inline void set(
			unsigned sec /**< : <i>in</i> : The seconds to be set.*/,
			unsigned min /**< : <i>in</i> : The minutes to be set.*/,
			unsigned hr /**< : <i>in</i> : The hours to be set.*/,
			unsigned long long dy /**< : <i>in</i> : The days to be set.*/
		) noexcept
		{
			seconds = sec % 60;
			minutes = min % 60;
			hours = hr % 24;
			days = dy;
		}

		/**
			\brief Functon adds inputs to the state.
		*/
		inline void add(
			unsigned sec /**< : <i>in</i> : The seconds to be added.*/,
			unsigned min /**< : <i>in</i> : The minutes to be added.*/,
			unsigned hr /**< : <i>in</i> : The hours to be added.*/,
			unsigned long long dy /**< : <i>in</i> : The days to be added.*/
		)
		{
			add_seconds(sec);
			add_minutes(min);
			add_hours(hr);
			add_days(dy);
		}

		/**
			\brief Returns the information as a string.

			<h3>Return</h3>
			The info formatted as a string.
		*/
		std::string get_string() const
		{
			return std::move(std::to_string(get_seconds()) + "s : "
				+ std::to_string(get_minutes()) + "min : "
				+ std::to_string(get_hours()) + "hr : "
				+ std::to_string(get_days()) + "days ; "
				+ std::to_string(get_total_hours()) + " total hours");
		}


		/**
			\brief get size of seconds storage type
		*/
		static constexpr unsigned get_seconds_size()
		{
			return sizeof(unsigned);
		}

		/**
			\brief get size of minutes storage type
		*/
		static constexpr unsigned get_minutes_size()
		{
			return sizeof(unsigned);
		}


		/**
			\brief get size of hours storage type
		*/
		static constexpr unsigned get_hours_size()
		{
			return sizeof(unsigned);
		}

		/**
			\brief get size of days storage type
		*/
		static constexpr unsigned get_days_size()
		{
			return sizeof(unsigned long long);
		}


		/**
			\brief get size of raw data stream representing this.
		*/
		static constexpr unsigned get_raw_size()
		{
			return get_seconds_size() + get_minutes_size() + get_hours_size() + get_days_size();
		}

		/**
			\brief read raw data from argument to each field.
		*/
		void read_raw(
			const char* raw /**< : <i>in</i> : The raw data stream.*/,
			unsigned size /**< : <i>in</i> : The length of the raw stream. 
						  (should be get_raw_size()).*/
		)
		{
			if (size != get_raw_size())
				throw std::invalid_argument("raw stream should be " 
					+ std::to_string(get_raw_size()) + " bytes long");
			seconds.store(*reinterpret_cast<const unsigned*>(raw));
			raw = &raw[get_seconds_size()];
			minutes.store(*reinterpret_cast<const unsigned*>(raw));
			raw = &raw[get_minutes_size()];
			hours.store(*reinterpret_cast<const unsigned*>(raw));
			raw = &raw[get_hours_size()];
			days.store(*reinterpret_cast<const unsigned*>(raw));
		}

		/**
			\brief get string holding raw data stream representation of the objects state.
		*/
		std::string get_raw() const
		{
			auto sec = get_seconds();
			auto min = get_minutes();
			auto hr = get_hours();
			auto dy = get_days();
			return std::move(
				std::string(reinterpret_cast<char*>(&sec), sizeof(sec))
				+ std::string(reinterpret_cast<char*>(&min), sizeof(min))
				+ std::string(reinterpret_cast<char*>(&hr), sizeof(hr))
				+ std::string(reinterpret_cast<char*>(&dy), sizeof(dy))
			);
		}


		/**
			\brief Checks if current object is at lower time count than
			parameter.

			<h3> Return </h3>
			Returns true if current state is less than
			parameter.
		*/
		bool operator < (
			counter a /**< : <i>in</i> : The argument to check against.*/
			)  const noexcept
		{
			if (days.load() < a.get_days())
				return true;
			if (days.load() > a.get_days())
				return false;
			if (hours.load() < a.get_hours())
				return true;
			if (hours.load() > a.get_hours())
				return false;
			if (minutes.load() < a.get_minutes())
				return true;
			if (minutes.load() > a.get_minutes())
				return false;
			if (seconds.load() < a.get_seconds())
				return true;
			if (seconds.load() > a.get_seconds())
				return false;
			return false;
		}

		/**
			\brief Checks if current object is at greater time count than 
			parameter.

			<h3> Return </h3>
			Returns true if current state is greater than 
			parameter.
		*/
		bool operator > (
			counter a /**< : <i>in</i> : The argument to check against.*/
			)  const noexcept
		{
			if (days.load() > a.get_days())
				return true;
			if (days.load() < a.get_days())
				return false;
			if (hours.load() > a.get_hours())
				return true;
			if (hours.load() < a.get_hours())
				return false;
			if (minutes.load() > a.get_minutes())
				return true;
			if (minutes.load() < a.get_minutes())
				return false;
			if (seconds.load() > a.get_seconds())
				return true;
			if (seconds.load() < a.get_seconds())
				return false;
			return false;
		}

		/**
			\brief Checks if current object is at equal time count than parameter.

			<h3> Return </h3>
			Returns true if current state is equal to parameter.
		*/
		bool operator == (
			counter a /**< : <i>in</i> : The argument to check against.*/
			)  const noexcept
		{
			if (days.load() == a.get_days())
				if (hours.load() == a.get_hours())
					if (minutes.load() == a.get_minutes())
						if (seconds.load() == a.get_seconds())
							return true;
			return false;
		}
		

		/**
			\brief Checks if current object is at lower or equal time count 
			than parameter.

			<h3> Return </h3>
			Returns true if current state is less than or
			 equal parameter.
		*/
		bool operator <= (
			counter a /**< : <i>in</i> : The argument to check against.*/
			)  const noexcept
		{
			if (days.load() < a.get_days())
				return true;
			if (days.load() > a.get_days())
				return false;
			if (hours.load() < a.get_hours())
				return true;
			if (hours.load() > a.get_hours())
				return false;
			if (minutes.load() < a.get_minutes())
				return true;
			if (minutes.load() > a.get_minutes())
				return false;
			if (seconds.load() < a.get_seconds())
				return true;
			if (seconds.load() > a.get_seconds())
				return false;
			return true;
		}
		

		/**
			\brief Checks if current object is at greater or equal time count
			than parameter.

			<h3> Return </h3>
			Returns true if current state is more than or
			 equal parameter.
		*/
		bool operator >= (
			counter a /**< : <i>in</i> : The argument to check against.*/
			)  const noexcept
		{
			if (days.load() > a.get_days())
				return true;
			if (days.load() < a.get_days())
				return false;
			if (hours.load() > a.get_hours())
				return true;
			if (hours.load() < a.get_hours())
				return false;
			if (minutes.load() > a.get_minutes())
				return true;
			if (minutes.load() < a.get_minutes())
				return false;
			if (seconds.load() > a.get_seconds())
				return true;
			if (seconds.load() < a.get_seconds())
				return false;
			return true;
		}

	};

	/**
		\brief returns the greater value of the two 

	    <h3>Return</h3>
		The greater value.\n

	*/
	inline counter max(
		const counter a /**< : <i>in</i> : first parameter.*/,
		const counter b /**< : <i>in</i> : seconds parameter.*/
	) noexcept
	{
		if (a < b)
			return b;
		else
			return a;
	}

	/**
		\brief returns the lesser value of the two

		<h3>Return</h3>
		The lesser value.\n

	*/
	inline counter min(
		const counter a /**< : <i>in</i> : first parameter.*/,
		const counter b /**< : <i>in</i> : seconds parameter.*/
	) noexcept
	{
		if (a < b)
			return a;
		else
			return b;
	}


	/**
		\brief Stream insertion operator overloaded for the class counter.

		<h3>Return</h3>
		The reference to stream passed.\n
	*/
	inline std::ostream& operator << (
		std::ostream& out /**< : <i>inout</i> : The stream which to insert.*/,
		counter ct /**< : <i>in</i> : The cunter object to insert.*/
		) noexcept
	{
		out << ct.get_string();
		return out;
	}

}

#endif // !COUNTER_TICK_H

/** ***************************************************************************
	\file time_stamp.enh.h

	\brief File for time class.

	Created 13 May 2020

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

#ifndef TIME_STAMP_ENH_H

#define TIME_STAMP_ENH_H				time_stamp.enh.h

#include "date.enh.h"

namespace enh
{
	namespace dt_type
	{
		using sec_t = enh::NumericSystem<unsigned short, 60>;
		using min_t = enh::NumericSystem<unsigned short, 60>;
		using hr_t = enh::NumericSystem<unsigned short, 24>;
	}

	/**
		\brief Class for time manipulation.

		<h3> Examples </h3>

		\include{lineno} time_stamp_ex.cpp
	*/
	class time_stamp
	{
		/**
			\brief The seconds part of time [0,60].
		*/
		dt_type::sec_t seconds;
		
		/**
			\brief The minutes part of time [0,59].
		*/
		dt_type::min_t minutes;

		/**
			\brief The hours part of time [0,23].
		*/
		dt_type::hr_t hours;

	public:

		/**
			\brief Sets the time to the time indicated by arguments.

			<h3>Exception</h3>
			Throws <code>std::invalid_argument</code> if sec, min, hr is
			not within bounds. [0,60], [0,59], [0,23] respectively.
		*/
		constexpr inline void setTime(
			unsigned short sec /**< : <i>in</i> : The seconds field [0,59].*/,
			unsigned short min /**< : <i>in</i> : The minutes field [0,59].*/,
			unsigned short hr  /**< : <i>in</i> : The hours field [0,59].*/
		)
		{
			seconds.set(sec);
			minutes.set(min);
			hours.set(hr);
		}

		/**
			\brief Sets the time to the time indicated by argument.
		*/
		inline void setTime(
			time_t timeStamp /**< : <i>in</i> : The time to set.*/
		)
		{
			tm tm_str;
			enh::localtime(&tm_str, &timeStamp);
			if (tm_str.tm_sec <= 59)
				setTime(tm_str.tm_sec, tm_str.tm_min, tm_str.tm_hour);
			else
				setTime(59, tm_str.tm_min, tm_str.tm_hour);
		}

		/**
			\brief Sets the time to the current time.
		*/
		inline void setTime()
		{
			setTime(std::time(nullptr));
		}

		/**
			\brief Sets the time to the time indicated by arguments.

			<h3>Exception</h3>
			Throws <code>std::invalid_argument</code> if sec, min, hr is
			not within bounds. [0,60], [0,59], [0,23] respectively.
		*/
		constexpr inline time_stamp(
			unsigned short sec /**< : <i>in</i> : The seconds field [0,60].*/,
			unsigned short min /**< : <i>in</i> : The minutes field [0,59].*/,
			unsigned short hr  /**< : <i>in</i> : The hours field [0,59].*/
		) : seconds(sec), minutes(min), hours(hr) {}

		/**
			\brief Sets the time to the time indicated by argument.
		*/
		inline time_stamp(
			time_t timeStamp /**< : <i>in</i> : The time to set.*/
		)
		{
			setTime(timeStamp);
		}

		/**
			\brief Sets the time to the current time.
		*/
		inline time_stamp()
		{
			setTime();
		}

		/**
			\brief Adds to the hour part of time held.

			<h3>Return</h3>
			Returns the number of days passed (hr/24).

		*/
		constexpr inline unsigned long long addHours(
			unsigned long long hr /**< : <i>in</i> : The hours to add.*/
		) noexcept
		{
			return hours.add(hr);
		}

		/**
			\brief Adds to the minute part of time held (also hour).

			<h3>Return</h3>
			Returns the number of days passed (min/(24*60)).

		*/
		constexpr inline unsigned long long addMinutes(
			unsigned long long min /**< : <i>in</i> : The minutes to add.*/
		) noexcept
		{
			return addHours(minutes.add(min));
		}

		/**
			\brief Adds to the second part of time held (also hour).

			<h3>Return</h3>
			Returns the number of days passed (sec/(24*60*60)).

		*/
		constexpr inline unsigned long long addSeconds(
			unsigned long long sec  /**< : <i>in</i> : The seconds to add.*/
		) noexcept
		{
			return addMinutes(seconds.add(sec));
		}

		/**
			\brief Reduce the hour part of time held.

			<h3>Return</h3>
			Returns the number of days reduced (hr/24).

		*/
		constexpr inline unsigned long long subHours(
			unsigned long long hr /**< : <i>in</i> : The hours to reduce.*/
		) noexcept
		{
			return hours.sub(hr);
		}

		/**
			\brief Reduce the minute part of time held (also hour).

			<h3>Return</h3>
			Returns the number of days passed (min/(24*60)).

		*/
		constexpr inline unsigned long long subMinutes(
			unsigned long long min /**< : <i>in</i> : The minutes to reduce.*/
		) noexcept
		{
			return subHours(minutes.sub(min));
		}

		/**
			\brief Reduces the second part of time held (also hour).

			<h3>Return</h3>
			Returns the number of days passed (sec/(24*60*60)).

		*/
		constexpr inline unsigned long long subSeconds(
			unsigned long long sec /**< : <i>in</i> : The seconds to reduce.*/
		) noexcept
		{
			return subMinutes(seconds.sub(sec));
		}

		/**
			\brief Get Seconds field.
		*/
		constexpr inline unsigned short getSeconds() const noexcept { return seconds.get(); }

		/**
			\brief Get Minutes field.
		*/
		constexpr inline unsigned short getMinutes() const noexcept { return minutes.get(); }

		/**
			\brief Get Hours field.
		*/
		constexpr inline unsigned short getHours() const noexcept { return hours.get(); }

		/**
			\brief Get The time as a string in default format.

			hour:min:sec\n\n

			<h3>Overload</h3>
			-# <code>inline std::string getStringTime(std::string format)
			const;</code>\n
		*/
		inline std::string getStringTime() const
		{
			return signExtendValue(hours.get(), 2) + " : "
				+ signExtendValue(minutes.get(), 2) + " : "
				+ signExtendValue(seconds.get(), 2);
		}

		/**
			\brief Get The time as a string in custom format.

			Pass Argument as a string containing any of the following
			components.

			<b>sec</b> : The seconds.\n
			<b>min</b> : The minutes.\n
			<b>hour</b> : The hours.\n

			Example : <code>"secs minm hourhr"</code> : <code>"25s 25m 10hr"
				</code>\n

			<h3>Overload</h3>
			-# <code>inline std::string getStringTime() const;</code>\n
		*/
		inline std::string getStringTime(
			std::string format /**< : <i>in</i> : The format of date.*/
		) const
		{
			std::size_t psec, pmin, phour;
			psec = format.find("sec");
			if (psec != std::string::npos)
				format.replace(psec, 3, signExtendValue(seconds.get(), 2));
			
			pmin = format.find("min");
			if (pmin != std::string::npos)
				format.replace(pmin, 3, signExtendValue(minutes.get(), 2));
			
			phour = format.find("hour");
			if (phour != std::string::npos)
				format.replace(phour, 4, signExtendValue(hours.get(), 2));

			return format;
		}

		/**
			\brief Checks if argument is equal to this object.

			<h3>Return</h3>
			Returns true if hours, minutes and seconds of argument is equal to
			current object.
		*/
		constexpr inline bool isEqualTo(
			const time_stamp &dt /**< : <i>in</i> : The time_stamp to compare with.*/
		) const noexcept
		{
			return (hours == dt.hours) && (minutes == dt.minutes) && (seconds == dt.seconds);
		}

		/**
			\brief Checks if argument is not equal to this object.

			<h3>Return</h3>
			Returns true if hours, minutes and seconds of argument is not equal to
			current object.
		*/
		constexpr inline bool isNotEqualTo(
			const time_stamp &dt /**< : <i>in</i> : The time_stamp to compare with.*/
		) const noexcept
		{
			return !isEqualTo(dt);
		}

		/**
			\brief Checks if current time_stamp is lesser than argument.

			<h3>Return</h3>
			Returns true if current time_stamp is lesser than argument.
		*/
		constexpr inline bool isLesserThan(
			const time_stamp &dt /**< : <i>in</i> : The time_stamp to compare with.*/
		) const noexcept
		{
			if (hours < dt.hours)
				return true;
			else if (hours > dt.hours)
				return false;
			else
			{
				if (minutes < dt.minutes)
					return true;
				else if (minutes > dt.minutes)
					return false;
				else
				{
					if (seconds < dt.seconds)
						return true;
					else if (seconds > dt.seconds)
						return false;
					else
						return false;

				}
			}
		}

		/**
			\brief Checks if current time_stamp is lesser than or equal to argument.

			<h3>Return</h3>
			Returns true if current time_stamp is lesser than or equal to argument.
		*/
		constexpr inline bool isLesserThanEq(
			const time_stamp &dt /**< : <i>in</i> : The time_stamp to compare with.*/
		) const noexcept
		{
			if (hours < dt.hours)
				return true;
			else if (hours > dt.hours)
				return false;
			else
			{
				if (minutes < dt.minutes)
					return true;
				else if (minutes > dt.minutes)
					return false;
				else
				{
					if (seconds < dt.seconds)
						return true;
					else if (seconds > dt.seconds)
						return false;
					else
						return true;

				}
			}
		}

		/**
			\brief Checks if current time_stamp is greater than argument.

			<h3>Return</h3>
			Returns true if current time_stamp is greater than argument.
		*/
		constexpr inline bool isGreaterThan(
			const time_stamp &dt /**< : <i>in</i> : The time_stamp to compare with.*/
		) const noexcept
		{
			return !isLesserThanEq(dt);
		}

		/**
			\brief Checks if current time_stamp is greater than or equal to argument.

			<h3>Return</h3>
			Returns true if current time_stamp is greater than or equal to argument.
		*/
		constexpr inline bool isGreaterThanEq(
			const time_stamp &dt /**< : <i>in</i> : The time_stamp to compare with.*/
		) const noexcept
		{
			return !isLesserThan(dt);
		}
	};

	/**
		\brief Checks if lhs is equal to rhs.
	*/
	constexpr inline bool operator == (
		const time_stamp &lhs /**< : <i>in</i> : The left hand side of the
						expression.*/,
		const time_stamp &rhs /**< : <i>in</i> : The right hand side of the
						expression.*/
		) noexcept
	{
		return lhs.isEqualTo(rhs);
	}

	/**
		\brief Checks if lhs is not equal to rhs.
	*/
	constexpr inline bool operator != (
		const time_stamp &lhs /**< : <i>in</i> : The left hand side of the
						expression.*/,
		const time_stamp &rhs /**< : <i>in</i> : The right hand side of the
						expression.*/
		) noexcept
	{
		return lhs.isNotEqualTo(rhs);
	}

	/**
		\brief Checks if lhs is greater than rhs.
	*/
	constexpr inline bool operator > (
		const time_stamp &lhs /**< : <i>in</i> : The left hand side of the
						expression.*/,
		const time_stamp &rhs /**< : <i>in</i> : The right hand side of the
						expression.*/
		) noexcept
	{
		return rhs.isLesserThan(lhs);
	}

	/**
		\brief Checks if lhs is greater than or equal to rhs.
	*/
	constexpr inline bool operator >= (
		const time_stamp &lhs /**< : <i>in</i> : The left hand side of the
						expression.*/,
		const time_stamp &rhs /**< : <i>in</i> : The right hand side of the
						expression.*/
		) noexcept
	{
		return rhs.isLesserThanEq(lhs);
	}

	/**
		\brief Checks if lhs is lesser than rhs.
	*/
	constexpr inline bool operator < (
		const time_stamp &lhs /**< : <i>in</i> : The left hand side of the
						expression.*/,
		const time_stamp &rhs /**< : <i>in</i> : The right hand side of the
						expression.*/
		) noexcept
	{
		return lhs.isLesserThan(rhs);
	}

	/**
		\brief Checks if lhs is lesser than or equal to rhs.
	*/
	constexpr inline bool operator <= (
		const time_stamp &lhs /**< : <i>in</i> : The left hand side of the
						expression.*/,
		const time_stamp &rhs /**< : <i>in</i> : The right hand side of the
						expression.*/
		) noexcept
	{
		return lhs.isLesserThanEq(rhs);
	}
}

#endif
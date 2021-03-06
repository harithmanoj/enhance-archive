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
		unsigned short seconds;
		
		/**
			\brief The minutes part of time [0,59].
		*/
		unsigned short minutes;

		/**
			\brief The hours part of time [0,23].
		*/
		unsigned short hours;

	public:

		/**
			\brief Sets the time to the time indicated by arguments.

			<h3>Exception</h3>
			Throws <code>std::invalid_argument</code> if sec, min, hr is
			not within bounds. [0,60], [0,59], [0,23] respectively.
		*/
		inline void setTime(
			unsigned short sec /**< : <i>in</i> : The seconds field [0,60].*/,
			unsigned short min /**< : <i>in</i> : The minutes field [0,59].*/,
			unsigned short hr  /**< : <i>in</i> : The hours field [0,59].*/
		)
		{
			if (!isConfined<unsigned short>(sec, 0, 60, true, true))
				throw std::invalid_argument("Seconds should be in range"
					" [0,60]");
			if (!isConfined<unsigned short>(min, 0, 59, true, true))
				throw std::invalid_argument("Minutes should be in range"
					" [0,59]");
			if (!isConfined<unsigned short>(hr, 0, 23, true, true))
				throw std::invalid_argument("Hours should be in range"
					" [0,23]");

			seconds = sec;
			minutes = min;
			hours = hr;
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
			setTime(tm_str.tm_sec, tm_str.tm_min, tm_str.tm_hour);
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
		inline time_stamp(
			unsigned short sec /**< : <i>in</i> : The seconds field [0,60].*/,
			unsigned short min /**< : <i>in</i> : The minutes field [0,59].*/,
			unsigned short hr  /**< : <i>in</i> : The hours field [0,59].*/
		)
		{
			setTime(sec, min, hr);
		}

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
			\brief Adds One Hour to the time stamp,
			if Hour is 23, resets to 0.\n

			<h3>Return</h3>
			Returns true if Hour is reset to 0.
		*/
		inline bool addHour()
		{
			if (hours >= 23)
			{
				hours = 0;
				return true;
			}
			else
				++hours;
			return false;
		}

		/**
			\brief Adds One Minutes to the time stamp,
			if Minutes is 59, resets to 0, Adds One Hour.\n

			<h3>Return</h3>
			Returns true if Hour is reset to 0.
		*/
		inline bool addMinutes()
		{
			if (minutes >= 59)
			{
				minutes = 0;
				return addHour();
			}
			else
				++minutes;
			return false;
		}

		/**
			\brief Adds One Seconds to the time stamp,
			if Seconds is 59, resets to 0, Adds One Minutes.\n

			<h3>Return</h3>
			Returns true if Hour is reset to 0.
		*/
		inline bool addSeconds()
		{
			if (seconds >= 59)
			{
				seconds = 0;
				return addMinutes();
			}
			else
				++seconds;
			return false;
		}

		/**
			\brief Get Seconds field.
		*/
		inline unsigned short getSeconds() const noexcept { return seconds; }

		/**
			\brief Get Minutes field.
		*/
		inline unsigned short getMinutes() const noexcept { return minutes; }

		/**
			\brief Get Hours field.
		*/
		inline unsigned short getHours() const noexcept { return hours; }

		/**
			\brief Get The time as a string in default format.

			hour:min:sec\n\n

			<h3>Overload</h3>
			-# <code>inline std::string getStringTime(std::string format)
			const;</code>\n
		*/
		inline std::string getStringTime() const
		{
			return signExtendValue(hours, 2) + " : "
				+ signExtendValue(minutes, 2) + " : "
				+ signExtendValue(seconds, 2);
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
				format.replace(psec, 3, signExtendValue(seconds, 2));
			
			pmin = format.find("min");
			if (pmin != std::string::npos)
				format.replace(pmin, 3, signExtendValue(minutes, 2));
			
			phour = format.find("hour");
			if (phour != std::string::npos)
				format.replace(phour, 4, signExtendValue(hours, 2));

			return format;
		}
	};
}

#endif
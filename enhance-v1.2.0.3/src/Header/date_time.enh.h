/** ***************************************************************************
	\file date_time.enh.h

	\brief File for combined date time class.

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

#ifndef DATE_TIME_ENH_H

#define DATE_TIME_ENH_H					date_time.enh.h


#include "time_stamp.enh.h"

namespace enh
{
	/**
		\brief Class for date and time joint manipulation.

		<h3> Examples </h3>

		\include{lineno} DateTime_ex.cpp
	*/
	class DateTime : public date, public time_stamp
	{
	
	public:
		
		/**
			\brief Sets the time and date to the time and date indicated by 
			argument.
		*/
		inline void set(
			unsigned short dy /**< : <i>in</i> : The day of the month 
							  [1,month_limit].*/,
			unsigned short mnth /**< : <i>in</i> : The number of months after
								January [0,11].*/,
			long yr /**< : <i>in</i> : The Year.*/,
			unsigned short week /**< : <i>in</i> : The day of the week after
								Sunday [0,6].*/,
			unsigned short ydy /**< : <i>in</i> : The number of day after 01 January
						 of that year [1,year_limit).*/,
			unsigned short sec /**< : <i>in</i> : The seconds field [0,60].*/,
			unsigned short min /**< : <i>in</i> : The minutes field [0,59].*/,
			unsigned short hr /**< : <i>in</i> : The hours field [0,59].*/
		)
		{
			setDate(dy, mnth, yr, week, ydy);
			setTime(sec, min, hr);
		}

		/**
			\brief Sets the time and date to the time and date indicated by
			argument.
		*/
		inline void set(
			time_t stamp /**< : <i>in</i> : The time stamp which 
							 contains the date and time.*/
		)
		{
			tm temp;
			localtime(&temp, &stamp);
			set(temp.tm_mday, temp.tm_mon, temp.tm_year + 1900, temp.tm_wday,
				temp.tm_yday, temp.tm_sec, temp.tm_min, temp.tm_hour);
		}

		/**
			\brief Sets the time and date to the current time and date.
		*/
		inline void set()
		{
			set(std::time(nullptr));
		}

		/**
			\brief Sets the time and date to the time and date indicated by
			argument.
		*/
		inline DateTime(
			unsigned short dy /**< : <i>in</i> : The day of the month
							  [1,month_limit].*/,
			unsigned short mnth /**< : <i>in</i> : The number of months after
								January [0,11].*/,
			long yr /**< : <i>in</i> : The Year.*/,
			unsigned short week /**< : <i>in</i> : The day of the week after
								Sunday [0,6].*/,
			unsigned short ydy /**< : <i>in</i> : The number of day after 01 January
						 of that year [1,year_limit).*/,
			unsigned short sec /**< : <i>in</i> : The seconds field [0,60].*/,
			unsigned short min /**< : <i>in</i> : The minutes field [0,59].*/,
			unsigned short hr /**< : <i>in</i> : The hours field [0,59].*/
		) : date(dy, mnth, yr, week, ydy), time_stamp(sec, min, hr) {}

		/**
			\brief Sets the time and date to the time and date indicated by
			argument.
		*/
		inline DateTime(
			time_t stamp /**< : <i>in</i> : The time stamp which
							 contains the date and time.*/
		) : date(stamp), time_stamp(stamp) {}

		/**
			\brief Sets the time and date to the current time and date.
		*/
		inline DateTime() : date(), time_stamp() {}

		/**
			\brief Get The date and time as a string in default format.

			hour:min:sec ; Day, ddth Month yyyy\n\n

			<h3>Overload</h3>
			-# <code>inline std::string getStringDateTime(std::string format)
			const;</code>\n
		*/
		inline std::string getStringDateTime()
		{
			return getStringTime() + " ; " + getStringDate();
		}


		/**
			\brief Get The date and time as a string in custom format.

			Pass Argument as a string containing any of the following
			components.

			<b>Day</b> : Name of day (Sunday) OR <b>shDay</b> : shortened
			name (Sun).\n
			<b>dd</b> : The date (20) OR <b>ddth</b> : date with
			superscript (21st).\n
			<b>Month</b> : The name of Month (January) OR <b>mm</b> : The
			numerical month (01 for January) OR <b>shMonth</b> : The name of
			month shortened (Jan).\n
			<b>yyyy</b> : Year (2020).\n
			<b>sec</b> : The seconds.\n
			<b>min</b> : The minutes.\n
			<b>hour</b> : The hours.\n

			<h3>Overload</h3>
			-# <code>inline std::string getStringDateTime() const;</code>\n
		*/
		inline std::string getStringDateTime(
			std::string format /**< : <i>in</i> : The format of date.*/
		)
		{
			return getStringDate(getStringTime(format));
		}

		/**
			\brief Adds One Seconds to the time stamp,
			if Seconds is 59, resets to 0, Adds One Minutes.\n

			<h3>Return</h3>
			Returns true if Hour is reset to 0.
		*/
		bool addSeconds()
		{
			auto t = time_stamp::addSeconds();
			if (t)
				add_day();
			return t;
		}

		/**
			\brief Adds One Minutes to the time stamp,
			if Minutes is 59, resets to 0, Adds One Hour.\n

			<h3>Return</h3>
			Returns true if Hour is reset to 0.
		*/
		bool addMinutes()
		{
			auto t = time_stamp::addMinutes();
			if (t)
				add_day();
			return t;
		}

		/**
			\brief Adds One Hour to the time stamp,
			if Hour is 23, resets to 0.\n

			<h3>Return</h3>
			Returns true if Hour is reset to 0.
		*/
		bool addHour()
		{
			auto t = time_stamp::addHour();
			if (t)
				add_day();
			return t;
		}
	};

}

#endif
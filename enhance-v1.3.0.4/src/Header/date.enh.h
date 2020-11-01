/** ***************************************************************************
	\file date.enh.h

	\brief File for date class.

	Created 12 May 2020

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

#ifndef DATE_ENH_H

#define DATE_ENH_H							date.enh.h

#include "general.enh.h"
#include <ctime>
#include <exception>
#include <stdexcept>



namespace enh
{
	/**
			\brief wrapper over unsafe localtime function.

			The Microsoft version localtime_s is called. Please Edit if not working.
		*/
	inline void localtime(
		tm *str_tm /**< : <i>in</i> : The pointer to tm structure to 
				   assign time values.*/,
		time_t *arith_tm /**< : <i>in</i> : The pointer to tm structure to 
				   assign time values.*/
	)
	{
		localtime_s(str_tm, arith_tm);
	}

	/**
			\brief The maximum date for that month.
	*/
	inline constexpr unsigned short month_limit(
		unsigned short mnth /**< : <i>in</i> : The month count.*/,
		long yr /**< : <i>in</i> : The year count.*/
	) noexcept
	{
		switch (mnth)
		{

		case 0:
			return 31;

		case 1:
		{
			if ((yr % 4) == 0)
				return 29;
			else
				return 28;
		}

		case 2:
			return 31;

		case 3:
			return 30;

		case 4:
			return 31;

		case 5:
			return 30;

		case 6:
			return 31;

		case 7:
			return 31;

		case 8:
			return 30;

		case 9:
			return 31;

		case 10:
			return 30;

		case 11:
			return 31;

		default:
			return 165;
			break;
		}
	}

	/**
		\brief The maximum date for that year.
	*/
	inline constexpr unsigned year_limit(
		long yr /**< : <i>in</i> : The year count.*/
	) noexcept
	{
		if (yr % 4 == 0)
			return 366;
		else
			return 365;
	}

	/**
		\brief The week day after day_count number of days from week.
	*/
	inline constexpr unsigned short week_day_increments(
		unsigned short week /*< : <i>in</i> : The current week day.*/,
		unsigned long day_count /*< : <i>in</i> : The number of days to
								add.*/
	) noexcept
	{
		unsigned long long tmp = day_count;
		tmp += week;
		return tmp % 7;
	}

	/**
		\brief Class to represent date.

		<h3>Example</h3>
		
		\include{lineno} date_ex.cpp
	*/
	class date
	{
		/**
			\brief The day of the month. [1,31]
		*/
		unsigned short day;

		/**
			\brief The month since january. [0,11]
		*/
		unsigned short month;

		/**
			\brief The Year.

			AD is positive.
			BC is negative.
		*/
		long year;

		/**
			\brief Days after Sunday. [0,6]
		*/
		unsigned short wkday;

		/**
			\brief Days after 01 January . [0,365]
		*/
		unsigned yrday;

		/**
			\brief Adds One Month to current date.
		*/
		constexpr inline void add_month()
		{
			if (month == 11)
			{
				month = 0;
				++year;
			}
			else
				++month;
		}

	public:

		/**
			\brief Add one day to date.
		*/
		constexpr inline void add_day()
		{
			++day;
			auto limit = month_limit(month, year);
			if (day == limit)
			{
				day = 0;
				add_month();
			}
			if (wkday == 6)
				wkday = 0;
			else
				++wkday;
		}

		/**
			\brief Sets the date to the date indicated by arguments.

			<h3>Exception</h3>
			Throws <code>std::invalid_argument</code> if dy, mnth, week, ydy is 
			not within bounds. [0,month_limit], [0,11], [0,6], [0,year_limit)
			respectively.
		*/
		constexpr inline void setDate(
			unsigned short dy /**< : <i>in</i> : The day of the month 
							  [1,month_limit].*/,
			unsigned short mnth /**< : <i>in</i> : The number of months after
								January [0,11].*/,
			long yr /**< : <i>in</i> : The Year.*/,
			unsigned short week /**< : <i>in</i> : The day of the week after
								Sunday [0,6].*/,
			unsigned ydy /**< : <i>in</i> : The number of day after 01 January
						 of that year [1,year_limit).*/
		)
		{
			if (!isConfined<unsigned short>(week, 0, 7, true, false))
				throw std::invalid_argument("Week day should be in range [0,6]");
			if (!isConfined<unsigned short>(mnth, 0, 12, true, false))
				throw std::invalid_argument("Month should be in range [0,11]");
			if (!isConfined<unsigned short>(dy, 1, month_limit(mnth, yr), true, true))
				throw std::invalid_argument("day should be within the monthly "
						"maximum (28,29,30 or 31 according to month).");
			if (!isConfined(ydy, 0u, year_limit(yr), true, false))
				throw std::invalid_argument("year day should be less than "
					"that for that year (365,366).");
			day = dy;
			month = mnth;
			year = yr;
			wkday = week;
			yrday = ydy;
		}

		/**
			\brief Sets the date to the date indicated by argument.
		*/
		inline void setDate(
			time_t timeStamp /**< : <i>in</i> : The time stamp which 
							 contains the date.*/
		)
		{
			tm temp;
			enh::localtime(&temp, &timeStamp);
			setDate(temp.tm_mday, temp.tm_mon, temp.tm_year + 1900, temp.tm_wday, temp.tm_yday);
		}

		/**
			\brief Sets the date to the date current date.
		*/
		inline void setDate()
		{
			setDate(std::time(nullptr));
		}

		/**
			\brief Sets the date to the date indicated by arguments.

			<h3>Exception</h3>
			Throws <code>std::invalid_argument</code> if dy, mnth, week, ydy is
			not within bounds. [0,month_limit], [0,11], [0,6], [0,year_limit)
			respectively.
		*/
		constexpr inline date(
			unsigned short dy /**< : <i>in</i> : The day of the month
							  [1,month_limit].*/,
			unsigned short mnth /**< : <i>in</i> : The number of months after
								January [0,11].*/,
			long yr /**< : <i>in</i> : The Year.*/,
			unsigned short week /**< : <i>in</i> : The day of the week after
								Sunday [0,6].*/,
			unsigned ydy /**< : <i>in</i> : The number of day after 01 January
						 of that year [1,year_limit).*/
		) : day(dy), month(mnth), year(yr), wkday(week), yrday(ydy)
		{
			if (!isConfined<unsigned short>(week, 0, 7, true, false))
				throw std::invalid_argument("Week day should be in range [0,6]");
			if (!isConfined<unsigned short>(mnth, 0, 12, true, false))
				throw std::invalid_argument("Month should be in range [0,11]");
			if (!isConfined<unsigned short>(dy, 1, month_limit(mnth, yr), true, true))
				throw std::invalid_argument("day should be within the monthly "
					"maximum (28,29,30 or 31 according to month).");
			if (!isConfined(ydy, 0u, year_limit(yr), true, false))
				throw std::invalid_argument("year day should be less than "
					"that for that year (365,366).");
		}

		/**
			\brief Sets the date to the date indicated by argument.
		*/
		inline date(
			time_t timeStamp /**< : <i>in</i> : The time stamp which
							 contains the date.*/
		)
		{
			setDate(timeStamp);
		}

		/**
			\brief Sets the date to the date current date.
		*/
		inline date()
		{
			setDate();
		}

		/**
			\brief The day of this month.
		*/
		constexpr inline unsigned short getDayOfMonth() const noexcept { return day; }

		/**
			\brief The number of months after January of this year.
		*/
		constexpr inline unsigned short getMonth() const noexcept { return month; }

		/**
			\brief The name of the month.
		*/
		inline std::string getMonthString() const
		{
			switch (month)
			{
			case 0: return "January";
			case 1: return "February";
			case 2: return "March";
			case 3: return "April";
			case 4: return "May";
			case 5: return "June";
			case 6: return "July";
			case 7: return "August";
			case 8: return "September";
			case 9: return "October";
			case 10: return "November";
			case 11: return "December";
			default: return "Error";
			}
		}

		/**
			\brief The name of the month (shortened).
		*/
		inline std::string getShortMonthString() const
		{
			switch (month)
			{
			case 0: return "Jan";
			case 1: return "Feb";
			case 2: return "Mar";
			case 3: return "Apr";
			case 4: return "May";
			case 5: return "Jun";
			case 6: return "Jul";
			case 7: return "Aug";
			case 8: return "Sep";
			case 9: return "Oct";
			case 10: return "Nov";
			case 11: return "Dec";
			default: return "Error";
			}
		}

		/**
			\brief The Year.
		*/
		constexpr inline long getYear() const noexcept { return year; }

		/**
			\brief The number of days after last Sunday.
		*/
		constexpr inline unsigned short getDayOfWeek() const noexcept { return wkday; }


		/**
			\brief The number of days after 1st of January this year.
		*/
		constexpr inline unsigned getDayOfYear() const noexcept { return yrday; }

		/**
			\brief The name of the day.
		*/
		inline std::string getDayOfWeekString() const
		{
			switch (wkday)
			{
			case 0: return "Sunday";
			case 1: return "Monday";
			case 2: return "Tuesday";
			case 3: return "Wednesday";
			case 4: return "Thursday";
			case 5: return "Friday";
			case 6: return "Saturday";
			default: return "Error";
			}
		}

		/**
			\brief The name of the day (shortened).
		*/
		inline std::string getShortDayOfWeekString() const
		{
			switch (wkday)
			{
			case 0: return "Sun";
			case 1: return "Mon";
			case 2: return "Tue";
			case 3: return "Wed";
			case 4: return "Thu";
			case 5: return "Fri";
			case 6: return "Sat";
			default: return "Error";
			}
		}

		/**
			\brief The superscript to be added after the month.

			eg st : 21st, rd : 23rd, nd : 22nd, th : 24th.
		*/
		inline std::string getDaySuperScript() const
		{
			return getOrdinalIndicator(day);
		}

		/**
			\brief Get The date as a string.

			Format : Day, ddth Month yyyy\n
			eg : Tuesday, 12th May 2020

			<h3>Overload</h3>
			-# <code>inline std::string getStringDate(std::string format)
			const;</code>\n
		*/
		inline std::string getStringDate() const
		{
			return getDayOfWeekString() + ", " + std::to_string(day) +
				getDaySuperScript() + " " + getMonthString() + " " +
				std::to_string(year);
		}

		/**
			\brief Get The date as a string in custom format.

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

			<h3>Overload</h3>
			-# <code>inline std::string getStringDate() const;</code>\n
		*/
		inline std::string getStringDate(
			std::string format /**< : <i>in</i> : The format of date.*/
		) const
		{
			std::size_t pDay, pshDay, pdd, pddth, pMonth, pmm, pshMonth, 
				pyyyy;
			pshDay = format.find("shDay");
			if (pshDay != std::string::npos)
				format.replace(pshDay, 5, getShortDayOfWeekString());
			else
			{
				pDay = format.find("Day");
				if (pDay != std::string::npos)
					format.replace(pDay, 3, getDayOfWeekString());
			}

			pddth = format.find("ddth");
			if (pddth != std::string::npos)
				format.replace(pddth, 4, signExtendValue(day,2) + 
					getDaySuperScript());
			else
			{
				pdd = format.find("dd");
				if (pdd != std::string::npos)
					format.replace(pdd, 2, signExtendValue(day, 2));
			}

			pshMonth = format.find("shMonth");
			if (pshMonth != std::string::npos)
				format.replace(pshMonth, 7, getShortMonthString());
			else
			{
				pMonth = format.find("Month");
				if (pMonth != std::string::npos)
					format.replace(pMonth, 5, getMonthString());
				else
				{
					pmm = format.find("mm");
					if (pmm != std::string::npos)
						format.replace(pmm, 2, signExtendValue(month 
							+ 1, 2));
				}
			}

			pyyyy = format.find("yyyy");
			if (pyyyy != std::string::npos)
				format.replace(pyyyy, 4, signExtendValue(year, 4));
			return format;
		}

		/**
			\brief Checks if argument is equal to this object.

			<h3>Return</h3>
			Returns true if year, month and day of argument is equal to 
			current object.
		*/
		constexpr inline bool isEqualTo(
			const date &dt /**< : <i>in</i> : The date to compare with.*/
		) const noexcept
		{
			return (year == dt.year) && (month == dt.month) && (day == dt.day);
		}

		/**
			\brief Checks if argument is not equal to this object.

			<h3>Return</h3>
			Returns true if year, month and day of argument is not equal to
			current object.
		*/
		constexpr inline bool isNotEqualTo(
			const date &dt /**< : <i>in</i> : The date to compare with.*/
		) const noexcept
		{
			return !isEqualTo(dt);
		}

		/**
			\brief Checks if current date is lesser than argument.

			<h3>Return</h3>
			Returns true if current date is lesser than argument.
		*/
		constexpr inline bool isLesserThan(
			const date &dt /**< : <i>in</i> : The date to compare with.*/
		) const noexcept
		{
			if (year < dt.year)
				return true;
			else if (year > dt.year)
				return false;
			else
			{
				if (month < dt.month)
					return true;
				else if (month > dt.month)
					return false;
				else
				{
					if (day < dt.day)
						return true;
					else if (day > dt.day)
						return false;
					else
						return false;

				}
			}
		}

		/**
			\brief Checks if current date is lesser than or equal to argument.

			<h3>Return</h3>
			Returns true if current date is lesser than or equal to argument.
		*/
		constexpr inline bool isLesserThanEq(
			const date &dt /**< : <i>in</i> : The date to compare with.*/
		) const noexcept
		{
			if (year < dt.year)
				return true;
			else if (year > dt.year)
				return false;
			else
			{
				if (month < dt.month)
					return true;
				else if (month > dt.month)
					return false;
				else
				{
					if (day < dt.day)
						return true;
					else if (day > dt.day)
						return false;
					else
						return true;

				}
			}
		}

		/**
			\brief Checks if current date is greater than argument.

			<h3>Return</h3>
			Returns true if current date is greater than argument.
		*/
		constexpr inline bool isGreaterThan(
			const date &dt /**< : <i>in</i> : The date to compare with.*/
		) const noexcept
		{
			return !isLesserThanEq(dt);
		}

		/**
			\brief Checks if current date is greater than or equal to argument.

			<h3>Return</h3>
			Returns true if current date is greater than or equal to argument.
		*/
		constexpr inline bool isGreaterThanEq(
			const date &dt /**< : <i>in</i> : The date to compare with.*/
		) const noexcept
		{
			return !isLesserThan(dt);
		}
	};

	/**
		\brief Checks if lhs is equal to rhs.
	*/
	constexpr inline bool operator == (
		const date &lhs /**< : <i>in</i> : The left hand side of the 
						expression.*/,
		const date &rhs /**< : <i>in</i> : The right hand side of the 
						expression.*/
	) noexcept
	{
		return lhs.isEqualTo(rhs);
	}

	/**
		\brief Checks if lhs is not equal to rhs.
	*/
	constexpr inline bool operator != (
		const date &lhs /**< : <i>in</i> : The left hand side of the
						expression.*/,
		const date &rhs /**< : <i>in</i> : The right hand side of the
						expression.*/
	) noexcept
	{
		return lhs.isNotEqualTo(rhs);
	}

	/**
		\brief Checks if lhs is greater than rhs.
	*/
	constexpr inline bool operator > (
		const date &lhs /**< : <i>in</i> : The left hand side of the
						expression.*/,
		const date &rhs /**< : <i>in</i> : The right hand side of the
						expression.*/
	) noexcept
	{
		return rhs.isLesserThan(lhs);
	}

	/**
		\brief Checks if lhs is greater than or equal to rhs.
	*/
	constexpr inline bool operator >= (
		const date &lhs /**< : <i>in</i> : The left hand side of the
						expression.*/,
		const date &rhs /**< : <i>in</i> : The right hand side of the
						expression.*/
	) noexcept
	{
		return rhs.isLesserThanEq(lhs);
	}

	/**
		\brief Checks if lhs is lesser than rhs.
	*/
	constexpr inline bool operator < (
		const date &lhs /**< : <i>in</i> : The left hand side of the
						expression.*/,
		const date &rhs /**< : <i>in</i> : The right hand side of the
						expression.*/
	) noexcept
	{
		return lhs.isLesserThan(rhs);
	}

	/**
		\brief Checks if lhs is lesser than or equal to rhs.
	*/
	constexpr inline bool operator <= (
		const date &lhs /**< : <i>in</i> : The left hand side of the
						expression.*/,
		const date &rhs /**< : <i>in</i> : The right hand side of the
						expression.*/
	) noexcept
	{
		return lhs.isLesserThanEq(rhs);
	}
}


#endif
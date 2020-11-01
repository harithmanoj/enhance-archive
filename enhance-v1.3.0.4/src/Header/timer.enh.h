/** ***************************************************************************
	\file timer.enh.h

	\brief File to declare functions and classes related to timer 
	functionality.

	Created 29 March 2020
		
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

#ifndef TIMER_ENH_H

#define TIMER_ENH_H						timer.enh.h

#include "logger.enh.h"

#include <chrono>
#include <type_traits>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

namespace enh
{
	/**
		\brief Alias for the standard high resolution clock for easy access.
	*/
	using high_res = std::chrono::high_resolution_clock;

	/**
		\brief Alias for easily declaring complex time stamp type.
	*/
	using time_pt = decltype(high_res::now());



	/**
		\brief Template type to assertian whether a type can be used to instanciate
		the class @ref enh::timer.

		<h3>Template Parameter</h3>
		-#  <code>T</code> : The type to check.

		<h3>Values </h3>
		<code>true</code> for <code>std::chrono::milliseconds, std::chrono::seconds,
		std::chrono::minutes, std::chrono::hours.</code>\n
		false for all else.\n
	*/
	template<class T>
	constexpr bool isGoodTimer_v = false; // evaluates to false unless explicitely specified.

	//explicitely specifies that milliseconds is fine.
	template<>
	constexpr bool isGoodTimer_v<std::chrono::milliseconds> = true;

	//explicitely specifies that seconds is fine.
	template<>
	constexpr bool isGoodTimer_v<std::chrono::seconds> = true;

	//explicitely specifies that minutes is fine.
	template<>
	constexpr bool isGoodTimer_v<std::chrono::minutes> = true;

	//explicitely specifies that hours is fine.
	template<>
	constexpr bool isGoodTimer_v<std::chrono::hours> = true;

	/**
		\brief Template type to assertian whether a type can be used to instanciate
		the template of class @ref enh::timer.

		<h3>Template Parameter</h3>
		-#  <code>T</code> : The type to check.

		<h3>Values </h3>
		<code>true</code> for <code>std::chrono::milliseconds, std::chrono::seconds,
		std::chrono::minutes, std::chrono::hours, std:chrono::microseconds,
		 std:chrono::nanoseconds.</code>\n
		false for all else.\n


		<b>Note : </b> Not all of these type can be used to make an object of that class.
	*/
	template<class T>
	constexpr bool isGoodTimerType_v = isGoodTimer_v<T>;

	template<>
	constexpr bool isGoodTimerType_v<std::chrono::nanoseconds> = true;

	template<>
	constexpr bool isGoodTimerType_v<std::chrono::microseconds> = true;


	/**
		\brief The class to create a timer that notifies all clients periodically.

		The class will start a timer on instanciation and starts notifying 
		all client threads, periodically.
		

		The period is determined by the template arguments.

		The period cannot be less than 5 if unit is milliseconds for accuracy.

		The class cannot be instanciated using any other type than 
		milliseconds, seconds, minutes and hours.

		But the template can be instanciated with nanoseconds and microseconds,
		but object cannot be created.


		hasErrorHandlers        = false;\n
		
		<h3>template</h3>
		-#  <code>unsigned _per</code> : The period of time between each notification.\n
		-#  <code>unit</code> : The unit of time of period.\n

		<h3>Example</h3>

		\include{lineno} timer_ex.cpp

	*/
	template<unsigned _per = 50U, class time_unit = std::chrono::milliseconds>
	class timer
	{
	
	public:

		/**
			\brief The period of timer.
		*/
		static constexpr unsigned period = _per;

		/**
			\brief The unit of measurement.
		*/
		using unit = time_unit;

	private:


		//fails if not time unit.
		static_assert(isGoodTimerType_v<unit>, "unit type must be time type");
		//fails if period < 5ms
		static_assert(!(std::is_same_v<std::chrono::milliseconds, unit> && (period < 5)),
			"Precision cannot be achieved lower than 5ms");


		/**
			\brief The variable that is initialized at the begining of program
			to the time at that point. Approximately the program begining time.
		*/
		static time_pt program_start;

		/**
			\brief The variable that is initialized at the begining of timer
			thread start, the begining time point of the timer.
		*/
		time_pt timer_start;

		/**
			\brief The time at which next notification is to be sent.

			Value is @ref timer_start + period*@ref elapsed_cycles.
		*/
		time_pt timer_next;


		/**
			\brief The mutex to hold ownership over @ref elapsed_cycles and
			the notifier.
		*/
		std::mutex mtxTimer;

		/**
			\brief The condition_variable to notify all client threads about 
			the end of that period.
		*/
		std::condition_variable cvTimer;

		/**
			\brief The variable to signal the end of the timer loop, set by 
			any control thread and read by the timer thread to stop execution
			and return.
		*/
		std::atomic<bool> stopTimer;

		/**
			\brief The variable that tracks the cycles elapsed since timer
			start.

			The product of this and period gives time elapsed.
		*/
		unsigned long long elapsed_cycles;


		/**
			\brief the thread handle to the thread running the timer function.
		*/
		std::thread timerThread;

		/**
			\brief true if timer thread is running.
		*/
		bool isTimerActive;



		/**
			\brief Blocks execution till the time point described in 
			timer_next.

			And then notifies the waiting threads.
		*/
		inline void single_period() noexcept
		{
			std::this_thread::sleep_until(timer_next);
			{
				std::lock_guard<std::mutex> lock(mtxTimer);
				++elapsed_cycles;
				timer_next += unit(period);
			}
			cvTimer.notify_all();
		}


		/**
			\brief keeps on executing single_period until the time when 
			stopTimer is set.
		*/
		void loop() noexcept
		{
			clear_stop();
			elapsed_cycles = 0;
			timer_start = high_res::now();
			timer_next = timer_start + unit(period);
			while (!stopTimer.load())
			{
				single_period();
			}
		}

	public:

		/**
			\brief The constructor of the class.


			The timer constructor also invokes enh::timer::start_timer.

			Constructor fails assert if it is not a time type > ms.
		*/
		inline timer() noexcept
		{
			static_assert(isGoodTimer_v<unit>, "unit type must be std::chrono::milliseconds, seconds or hours");
			isTimerActive = false;
			clear_stop();
			elapsed_cycles = 0;
			start_timer();
		}
		

		/**
			\brief destructor of the class.

			invokes enh::timer::force_join, waits till the timer Thread joins.
		*/
		inline ~timer() noexcept
		{
			force_join();
		}

		/**
			\brief The Functions blocks execution till the number of cycles 
			elapsed is greater than or equal to the expected value.

			Invokes enh::timer::start_timer if timer is not active.

			<h3>Return</h3>
			The difference between elapsed 
			cycles and expected (the "overshoot").\n
		*/
		inline unsigned long long wait(
			unsigned long long expected /**< : <i>in</i> : The expected cycle 
										count to wait till.*/
		) noexcept
		{
			if (!isTimerActive)
				start_timer();
			std::unique_lock<std::mutex> lock(mtxTimer);
			cvTimer.wait(lock,
				[expected, this]() {
					return elapsed_cycles >= expected;
				});
			return (elapsed_cycles - expected);
		}

		/**
			\brief The Functions blocks execution till the number of cycles
			elapsed is greater than or equal to the current elapsed cycles 
			+ 1.

			<h3>Return</h3>
			The difference between elapsed
			cycles and expected (the "overshoot").\n
		*/
		inline unsigned long long wait() noexcept
		{
			return wait(elapsed_cycles + 1);
		}

		/**
			\brief The function blocks for a certian cycles unless the
			condition becomes false.

			The functions waits for a cycle then checks the condition passed 
			through cond, and if it returns false, the function exits.

			<h3>Return</h3>
			-1 if the condition fails. The overshoot 
			if it doesnt.


		*/
		inline long long wait_for(
			unsigned mult_count /**< : <i>in</i> : The amount of cycles to 
								wait.*/,
			std::function<bool()> condition /**< : <i>in</i> : The condition
											to exit immediately.*/
		) noexcept
		{
			unsigned long long expected = elapsed_cycles + mult_count;
			while (elapsed_cycles < expected)
			{
				if (!condition())
					return -1;
				wait();
			}
			return elapsed_cycles - expected;
		}
		
		/**
			\brief sets stopTimer to true.
		*/
		inline void stop() noexcept { stopTimer = true; }

		/**
			\brief sets stopTimer to false.
		*/
		inline void clear_stop() noexcept { stopTimer = false; }

		/**
			\brief returns the start @ref time_pt of the program.
		*/
		inline static time_pt program_start_point() noexcept 
		{ return program_start; }

		/**
			\brief Returns the duration elapsed from program start in the type 
			passed through the template.
		*/
		inline static unit program_elapsed() noexcept
		{
			return std::chrono::duration_cast<unit>(high_res::now()
				- program_start);
		}

		/**
			\brief Returns the number of cycles elapsed from timer start.
		*/
		inline unsigned long long elapsed() noexcept { return elapsed_cycles; }

		/**
			\brief blocks function execution for mult_count number of cycles.

			Returns the overshoot from the expected value.
		*/
		inline unsigned long long wait_for(
			unsigned long mult_count /**< : <i>in</i> : The cycles to wait for.*/
		)noexcept
		{
			return wait(elapsed_cycles + mult_count);
		}

		/**
			\brief Checks if timer is running.

			<h3>Return</h3>
			If timer is running it returns true.
			Else it returns false;
		*/
		inline bool isTimerCounting()
		{
			if (isTimerActive)
			{
				// isTimer Active is true, thread has finished but thread has 
				// not been joined.
				if (timerThread.joinable())
				{
					timerThread.join();
					isTimerActive = false;
					return false;
				}
				else
					return true;
			}
			else
				return false;
		}

		/**
			\brief The function to start timer.

			The function clears the stopTimer flag, sets elapsed_cycles to 
			0 and then allocates a thread for the timer.

			<h3>Return</h3>
			Returns false if timer is already running.
		*/
		inline bool start_timer() noexcept
		{
			if (isTimerCounting())
				return false;
			O3_LIB_LOG_LINE;
			clear_stop();
			elapsed_cycles = 0;
			timerThread = std::thread(&timer<period, unit>::loop, this);
			isTimerActive = true;
			return true;
		}

		/**
			\brief overloaded operator !, returns true if the timer Thread 
			is not allocated.
		*/
		inline bool operator !() noexcept
		{
			return !isTimerCounting();
		}

		/**
			\brief Waits till timerThread finishes execution.

			The function blocks till the timer Thread joins.
			
			If the timer Thread is empty, it returns immediately.
		*/
		inline void join() noexcept
		{
			if (isTimerCounting())
			{
				timerThread.join();
				isTimerActive = false;
			}
		}

		/**
			\brief sets stopTimer flag to true, then waits for thread to 
			join via enh::timer::join.
		*/
		inline void force_join() noexcept
		{
			stop();
			join();
			return ;
		}
	};

	template<unsigned a, class b>
	time_pt timer<a,b>::program_start = high_res::now();

	/**
		\brief The enh::timer class with unit <code>std::chrono::
		nanoseconds</code> 
		
		
		The alias of a timer class template instanciated with 
		<code>std::chrono::nanoseconds</code> to measure time elapsed in 
		nanoseconds from program start.

		<b>NOTE : </b> An object of this type cannot be created, it would 
		fail an assert in the constructor.
	*/
	using nanos = timer<50, std::chrono::nanoseconds>;

	/**
		\brief The enh::timer class with unit <code>std::chrono::
		microseconds</code>


		The alias of a timer class template instanciated with
		<code>std::chrono::microseconds</code> to measure time elapsed in
		microseconds from program start.

		<b>NOTE : </b> An object of this type cannot be created, it would
		fail an assert in the constructor.
	*/
	using micros = timer<50, std::chrono::microseconds>;

	/**
		\brief The enh::timer class with unit <code>std::chrono::
		milliseconds</code>


		The alias of a timer class template instanciated with
		<code>std::chrono::milliseconds</code>.

		<b>NOTE : </b> An instance of this template cannot be created with 
		period less than 5, it would fail an assert during template 
		instanciation.

		<h3>Template</h3>
		<code>unsigned period</code> : The period of one cycle in the timer, 
		cannot be less than 5, default value 50.
	*/
	template<unsigned period = 50U>
	using millis = timer<period, std::chrono::milliseconds>;

	/**
		\brief The enh::timer class with unit <code>std::chrono::
		seconds</code>


		The alias of a timer class template instanciated with
		<code>std::chrono::seconds</code>.

		<h3>Template</h3>
		<code>unsigned period</code> : The period of one cycle in the timer,
		default value 50.
	*/
	template<unsigned period = 50U>
	using seconds = timer<period, std::chrono::seconds>;

	/**
		\brief The enh::timer class with unit <code>std::chrono::
		minutes</code>


		The alias of a timer class template instanciated with
		<code>std::chrono::minutes</code>.

		<h3>Template</h3>
		<code>unsigned period</code> : The period of one cycle in the timer,
		default value 50.
	*/
	template<unsigned period = 50U>
	using minutes = timer<period, std::chrono::minutes>;

	/**
		\brief The enh::timer class with unit <code>std::chrono::
		hours</code>


		The alias of a timer class template instanciated with
		<code>std::chrono::hours</code>.

		<h3>Template</h3>
		<code>unsigned period</code> : The period of one cycle in the timer,
		default value 50.
	*/
	template<unsigned period = 50U>
	using hours = timer<period, std::chrono::hours>;

}


#endif // !TIMER_TICK_H

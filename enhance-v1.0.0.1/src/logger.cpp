/** ***************************************************************************
	\file logger.cpp

	\brief The file to define logger functions

	Created 27 April 2020
		
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


#include "header/logger.enh.h"

#if  defined(ENH_DEBUG_CONTROL) && (ENH_OPTIMISATION < 5)
#include <map>
#include <fstream>
#include <exception>


// setup will indicate if no file existed previously
std::string Register(bool& setup, std::string function, std::thread::id id = std::this_thread::get_id())
{
	static std::map<std::thread::id, std::string> call_info; // To store thread id based log-file
	std::string ret;
	setup = false;
	try {
		ret = call_info.at(id);
	}
	catch (std::out_of_range)
	{
		call_info.emplace(id, function);
		ret = function;
		setup = true;
	}
	return ret;
}

//write to path
void write(std::string buff, std::filesystem::path file)
{
	std::ofstream out(file, std::ios::app | std::ios::out);
	out << buff << "\n";
}

std::filesystem::path debug::getFile(std::thread::id id, std::string function)
{
	auto file = std::filesystem::path("");
	std::ostringstream out;
	bool setup = false;
	out << id << "_thread_fn_" << Register(setup, function, id) << ".log";
	file = out.str();
	if (setup)
	{
		std::ostringstream buff;
		buff << "Thread id : " << id << "\n\t\tthread first logging function " << function;
		write(buff.str(), file);
	}
	return file;
}

void debug::Log(std::string lg, std::string function)
{
	write(lg, getFile(std::this_thread::get_id(), function));
}

void debug::Log(std::string file, std::string function, unsigned long line)
{
	std::ostringstream out;
	out << std::setw(80) << file << " : " << std::setw(6) << line << "   " << std::setw(15) << function;
	Log(out.str(), function);
}

void debug::Log(std::string file, std::string function, unsigned long line, std::string descr)
{
	std::ostringstream out;
	out << std::setw(80) << file << " : " << std::setw(6) << line << "   " << std::setw(15) << function << " ::   " << descr;
	Log(out.str(), function);
}

#endif
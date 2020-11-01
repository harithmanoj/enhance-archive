/** ***************************************************************************
	\file framework.enh.h

	\brief The file to declare version and the version info class

	Created 10 April 2020
		
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


#ifndef FRAMEWORK_ENH_H

#define FRAMEWORK_ENH_H						framework.enh.h

/**
	\brief Convert version numbers to a format that can be taken as argument by 
	`enh::version_info` (beta release).
*/
#define VERSION_INFO_BETA(mj,mn,re,bl)		mj,mn,re,bl,enh::rel_type::BETA,\
											#mj "." #mn "." #re "." #bl ".beta"

/**
	\brief Convert version numbers to a format that can be taken as argument by
	`enh::version_info` (final release).
*/
#define VERSION_INFO_FIN(mj,mn,re,bl)		mj,mn,re,bl,enh::rel_type::RELEASE\
											,#mj "." #mn "." #re "." #bl ".fin"

#include <string>

/**
	\brief The namespace for all the functions and classes of this library.
*/
namespace enh
{
	/**
		\brief Enumeration to define the release type.
	*/
	enum class rel_type : unsigned {
		BETA = 0,			/**< \brief <i>0</i> : Beta external release.*/
		RELEASE = 1			/**< \brief <i>1</i> : Final external release.*/
	};


	/**
		\brief The class to store version information.

		hasErrorHandlers        = false;\n

		<h3> Usage </h3>

		Declare a constexpr object of this class and give the version as
		parameter, where the last parameter.

		For easy declaration, use macros `VERSION_INFO_BETA` and 
		`VERSION_INFO_FIN` and pass version as arguments.

		Use different getters to use those values in banners and about
		sections of your program.

		<h3>Example</h3>

		\include{lineno} version_info_ex.cpp

	*/
	class version_info
	{
		/**
			\brief The major version.
		*/
		unsigned major;
		
		/**
			\brief The minor version.
		*/
		unsigned minor;
		
		/**
			\brief The revision count.
		*/
		unsigned revision;
		
		/**
			\brief The build number.
		*/
		unsigned long build;

		/**
			\brief The release type.
		*/
		rel_type type;

		/**
			\brief The string representation of the version.
		*/
		std::string_view string_rep;
	public:


		/**
			\brief The constructor initialises to 0.0.0.0.beta


			<h3>Overloads</h3>
			-# <code>inline version_info(unsigned, unsigned, unsigned, unsigned,
			 enh::rel_type) noexcept;</code>\n
			-# <code>inline version_info(unsigned, unsigned, unsigned, unsigned,
			enh::rel_type, std::string_view) noexcept;</code>\n
		*/
		inline version_info() noexcept :major(0), minor(0), revision(0),
			build(0), type(rel_type::BETA) { }

		/**
			\brief The constructor initialises to argument passed (no default 
			string).

			<h3>Overloads</h3>
			-# <code>inline version_info() noexcept;</code>\n
			-# <code>inline version_info(unsigned, unsigned, unsigned,
			unsigned, enh::rel_type, std::string_view) noexcept;</code>\n
		*/
		constexpr inline version_info(
			unsigned mj /**< : <i>in</i> : major version.*/,
			unsigned mn /**< : <i>in</i> : minor version.*/,
			unsigned rv /**< : <i>in</i> : revision number.*/,
			unsigned long bld /**< : <i>in</i> : build number.*/,
			rel_type tp /**< : <i>in</i> : The release type.*/
		) noexcept : major(mj), minor(mn), revision(rv), build(bld), 
			type(tp), string_rep("") { }

		/**
			\brief The constructor initialises to argument passed with compile
			-time string format of version.

			<h3>Overloads</h3>
			-# <code>inline version_info(unsigned, unsigned, unsigned,
			unsigned, enh::rel_type) noexcept;</code>\n
			-# <code>inline version_info() noexcept;</code>\n
		*/
		constexpr inline version_info(
			unsigned mj /**< : <i>in</i> : major version.*/,
			unsigned mn /**< : <i>in</i> : minor version.*/,
			unsigned rv /**< : <i>in</i> : revision number.*/,
			unsigned long bld /**< : <i>in</i> : build number.*/,
			rel_type tp /**< : <i>in</i> : The release type.*/,
			std::string_view rep  /**< : <i>in</i> : The version as a string.*/
		) noexcept : major(mj), minor(mn), revision(rv), build(bld),
			type(tp), string_rep(rep) { }

		/**
			\brief The get major version.

			<h3>Return</h3>
			Major version.\n
		*/
		inline constexpr unsigned get_major() const noexcept { return major; }

		/**
			\brief The get minor version.

			<h3>Return</h3>
			Minor version.\n
		*/
		inline constexpr unsigned get_minor() const noexcept { return minor; }

		/**
			\brief The get revision number.

			<h3>Return</h3>
			Revision number.\n
		*/
		inline constexpr unsigned get_revision() const noexcept 
		{ return revision; }

		/**
			\brief The get build number.

			<h3>Return</h3>
			Build number.\n
		*/
		inline constexpr unsigned long get_build() const noexcept 
		{ return build; }

		/**
			\brief The get release type.

			<h3>Return</h3>
			Release type.\n
		*/
		inline constexpr rel_type get_type() const noexcept { return type; }

		/*
			\brief The function to get version as a string.

			<h3>Return</h3>
			The version as a string.\n
		*/
		inline std::string to_string() const
		{
			if (!string_rep.empty())
				return string_rep.data();
			std::string ret = std::to_string(major) + "." + std::to_string(minor) + "."
				+ std::to_string(revision) + "." + std::to_string(build)
				+ ".";
			switch (type)
			{
			case rel_type::BETA:
				ret += "beta";
				break;
			case rel_type::RELEASE:
				ret += "fin";
				break;
			default:
				break;
			}
			return ret;
		}

		/**
			\brief Function returns string format of version.

			Works compile time too.

			Only returns if it is set during construction.
		*/
		inline constexpr std::string_view string() const noexcept
		{
			return string_rep;
		}
	};

	/**
		\brief The version of enhance library.
	*/
	constexpr version_info enhance_version = version_info(VERSION_INFO_FIN(1,1,0,2));
}


#undef  MAJOR_ENH				
#undef  MINOR_ENH				
#undef  REVISION_ENH			
#undef  RELEASE_ENH				
#undef  RELEASE_ENH_ST			
#undef  VERSION_DT_ENH			
#undef  TEXT_VER_ENH			
#undef  VERSION_ENH				

#endif
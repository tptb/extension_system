/**
	@file
	@copyright
		Copyright Bernd Amend and Michael Adam 2014
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)
*/
#pragma once

#include <extension_system/Extension.hpp>

class Interface1
{
public:
	virtual void test1() = 0;
	virtual ~Interface1() {}
};
EXTENSION_SYSTEM_INTERFACE(Interface1)

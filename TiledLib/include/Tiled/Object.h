//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "PropertyBag.h"
#include <pugixml/pugixml.hpp>
#include <string>
#include <memory>


namespace KAOS { namespace Tiled
{

	class Object
	{
	public:

		virtual ~Object() = default;

		virtual std::unique_ptr<Object> Clone() const;

		virtual bool Parse(const pugi::xml_node& objectNode);


		std::string GetName() const;
		std::string GetType() const;
		int GetXPos() const;
		int GetYPos() const;

		virtual std::optional<PropertyBag::value_type> QueryProperty(const std::string& name) const;

	private:

		std::string		m_Name;
		std::string		m_Type;
		int				m_XPos = 0;
		int				m_YPos = 0;
		PropertyBag		m_PropertyBag;
	};

}}




//	Copyright (c) 2018 Chet Simpson
//	
//	Permission is hereby granted, free of charge, to any person
//	obtaining a copy of this software and associated documentation
//	files (the "Software"), to deal in the Software without
//	restriction, including without limitation the rights to use,
//	copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the
//	Software is furnished to do so, subject to the following
//	conditions:
//	
//	The above copyright notice and this permission notice shall be
//	included in all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//	OTHER DEALINGS IN THE SOFTWARE.

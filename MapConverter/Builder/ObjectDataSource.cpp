//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Builder/ObjectDataSource.h"


namespace Builder
{

	ObjectDataSource::ObjectDataSource(
		const KAOS::Tiled::Object& object,
		unsigned long long typeId,
		std::shared_ptr<map_type> map,
		std::shared_ptr<tilesetcache_type> tilesetCache,
		std::shared_ptr<const descriptors_type> descriptors)
		:
		DataSource(move(map), move(tilesetCache), move(descriptors)),
		m_Object(object),
		m_TypeId(typeId)
	{}


	std::optional<KAOS::Tiled::PropertyBag::value_type> ObjectDataSource::QueryProperty(const std::string& name) const
	{
		using value_type = KAOS::Tiled::PropertyBag::value_type;

		if (name == "$(Id)")
		{
			return value_type(name, value_type::int_type(m_TypeId));
		}
		else if (name == "X")
		{
			return value_type(name, value_type::int_type(m_Object.GetXPos()));
		}
		else if (name == "Y")
		{
			return value_type(name, value_type::int_type(m_Object.GetYPos()));
		}

		return m_Object.QueryProperty(name);
	}

}




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

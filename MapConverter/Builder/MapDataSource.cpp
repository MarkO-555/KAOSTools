//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Builder/MapDataSource.h"


namespace Builder
{

	MapDataSource::MapDataSource(
		const std::shared_ptr<map_type> map,
		std::shared_ptr<tilesetcache_type> tilesetCache,
		std::shared_ptr<const descriptors_type> descriptors)
		:
		DataSource(move(map), move(tilesetCache), move(descriptors))
	{
		if (!m_Map)
		{
			throw std::invalid_argument("Map cannot be null");
		}
	}


	std::optional<KAOS::Tiled::PropertyBag::value_type> MapDataSource::QueryProperty(const std::string& name) const
	{
		using value_type = KAOS::Tiled::PropertyBag::value_type;

		if (name == "Width")
		{
			return value_type(name, value_type::int_type(m_Map->GetDimensions().GetWidth()));
		}
		else if (name == "Height")
		{
			return value_type(name, value_type::int_type(m_Map->GetDimensions().GetHeight()));
		}
		else if (name == "$(IsCompressed)")
		{
			return value_type(name, value_type::bool_type(false));
		}

		return m_Map->QueryProperty(name);
	}



	std::shared_ptr<MapDataSource::objectdescriptor_type> MapDataSource::QueryObjectDescriptor(const std::string& /*name*/) const
	{
		return nullptr;
	}


	std::shared_ptr<MapDataSource::layer_type> MapDataSource::QueryLayer(const std::string& name) const
	{
		return m_Map->QueryObjectLayer(name);
	}


	std::shared_ptr<MapDataSource::objectlayer_type> MapDataSource::QueryObjectLayer(const std::string& name) const
	{
		return m_Map->QueryObjectLayer(name);
	}

	std::shared_ptr<MapDataSource::tilesetlayer_type> MapDataSource::QueryTilesetLayer(const std::string& name) const
	{
		return m_Map->QueryTilesetLayer(name);
	}

	std::shared_ptr<MapDataSource::tilesetlayer_type> MapDataSource::QueryTilesetLayer(uint64_t index) const
	{
		return m_Map->QueryTilesetLayer(index);
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

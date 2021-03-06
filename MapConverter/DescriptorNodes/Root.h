//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "DescriptorNodes/CompositeNode.h"
#include "DescriptorNodes/Objects.h"
#include "DescriptorNodes/TileDescriptor.h"
#include "DescriptorNodes/MapFile.h"


namespace DescriptorNodes
{

	class Root : public CompositeNode
	{
	public:

		bool Parse(const pugi::xml_node& node) override;

		virtual std::shared_ptr<Objects> QueryObjectsDescriptor(const std::string& name) const;
		virtual std::shared_ptr<TileDescriptor> QueryTileDescriptor(const std::string& name) const;
		virtual std::shared_ptr<MapFile> QueryMapFileDescriptor(const std::string& name) const;


	protected:

		const factory_type& GetExemplars() const override;
		bool ParseRequiresSymbol() const override;

		using objects_type = std::map<std::string, std::shared_ptr<Objects>>;
		using tiledescriptors_type = std::map<std::string, std::shared_ptr<TileDescriptor>>;
		using mapfiledescriptors_type = std::map<std::string, std::shared_ptr<MapFile>>;

		objects_type			m_ObjectCollections;
		tiledescriptors_type	m_TileDescriptors;
		mapfiledescriptors_type m_MapFileDescriptors;
	};

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

//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "Builder/DefinitionBuilder.h"
#include "Builder/DataBuilder.h"
#include "Builder/DataSource.h"
#include <Tiled/Tile.h>
#include <pugixml/pugixml.hpp>


class DescriptorNode
{
public:

	using databuilder_type = Builder::DataBuilder;
	using definitionbuilder_type = Builder::DefinitionBuilder;
	using datasource_type = Builder::DataSource;
	using name_type = std::string;

	virtual ~DescriptorNode() = default;


	virtual name_type GetSymbol() const;

	virtual bool Parse(const pugi::xml_node& node);

	virtual bool CompileDefinition(definitionbuilder_type& builder) const = 0;
	virtual bool CompileSubInstance(databuilder_type& builder, const datasource_type& dataSource) const;
	virtual bool CompileInstance(databuilder_type& builder, const datasource_type& dataSource) const = 0;


protected:

	virtual bool ParseRequiresSymbol() const;
	virtual std::string GetSymbolAttributeName() const;

	template<class ValueType_>
	void AddInteger(databuilder_type& builder, const KAOS::Tiled::NamedProperty& property) const
	{
		if (!property.IsIntegerType())
		{
			throw std::runtime_error("Cannot convert value.");
		}

		builder.EmitValue(std::string(), KAOS::Common::NativeProperty(ValueType_(property.ToInteger())), GetSymbol());
	}

	void AddValue(
		databuilder_type& builder,
		KAOS::Common::NativeProperty::typeid_type outputType,
		const KAOS::Tiled::NamedProperty& property) const;


protected:

	name_type			m_Symbol;
};




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

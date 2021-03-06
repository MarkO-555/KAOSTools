//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/TileDescriptor.h"
#include "DescriptorNodes/TypedValue.h"
#include "DescriptorNodes/PackedValue.h"


namespace DescriptorNodes
{

	const TileDescriptor::factory_type& TileDescriptor::GetExemplars() const
	{
		static const factory_type exemplars
		{
			{ "Byte", std::make_unique<ByteProperty> },
			{ "Word", std::make_unique<WordProperty> },
			{ "Quad", std::make_unique<QuadProperty> },
			{ "PackedByte", std::make_unique<PackedByteProperty> },
			{ "PackedWord", std::make_unique<PackedWordProperty> },
			{ "PackedQuad", std::make_unique<PackedQuadProperty> }
		};

		return exemplars;
	}




	bool TileDescriptor::CompileDefinition(definitionbuilder_type& builder) const
	{
		builder.Begin(GetSymbol());
		if (!CompositeNode::CompileDefinition(builder))
		{
			return false;
		}
		builder.End();

		return true;
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

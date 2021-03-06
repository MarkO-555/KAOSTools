//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Builder/SimpleDataBuilder.h"


namespace Builder
{

	SimpleDataBuilder::SimpleDataBuilder(std::ostream& output)
		: m_Generator(output)
	{}



	bool SimpleDataBuilder::Flush()
	{
		m_Generator.Flush();

		return true;
	}




	bool SimpleDataBuilder::EmitBlank()
	{
		m_Generator.EmitBlank();

		return true;
	}


	bool SimpleDataBuilder::EmitComment(std::string comment, bool addSpacing)
	{
		m_Generator.EmitComment(comment, addSpacing);

		return true;
	}


	bool SimpleDataBuilder::EmitSeparatorComment()
	{
		return EmitComment(";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;", false);
	}


	bool SimpleDataBuilder::EmitLabel(std::string symbol)
	{
		m_Generator.EmitSymbol(symbol);

		return true;
	}


	bool SimpleDataBuilder::EmitValue(std::string symbol, property_type value, std::string comment)
	{
		switch (value.GetType())
		{
		case property_type::typeid_type::Byte:
			m_Generator.EmitByte(symbol, value.GetByteValue(), comment);
			break;

		case property_type::typeid_type::Word:
			m_Generator.EmitWord(symbol, value.GetWordValue(), comment);
			break;

		case property_type::typeid_type::Quad:
			m_Generator.EmitQuad(symbol, value.GetQuadValue(), comment);
			break;

		case property_type::typeid_type::String:
			m_Generator.EmitString(symbol, value.GetStringValue(), comment);
			break;
		}

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

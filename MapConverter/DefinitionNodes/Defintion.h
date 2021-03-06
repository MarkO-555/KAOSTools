//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "Builder/AsmFormatter.h"
#include <DefinitionNodes/Node.h>
#include <DefinitionNodes/SymbolicValue.h>
#include <vector>
#include <memory>


namespace DefinitionNodes
{

	class Defintion : public Node
	{
	public:

		using typeid_type = TypedNode::typeid_type;
		using symbolic_value_type = SymbolicValue::value_type;
		using formatter_type = Builder::AsmFormatter;


	public:

		explicit Defintion(name_type name);

		virtual bool AddVariable(name_type name, typeid_type type);
		virtual bool AddSymbolic(name_type name, symbolic_value_type value);

		virtual bool Generate(formatter_type& formatter) const;


	private:

		std::vector<std::unique_ptr<Node>>	m_Members;
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

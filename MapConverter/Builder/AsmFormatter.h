//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <Tiled/Tile.h>
#include <ostream>


namespace Builder
{

	class AsmFormatter
	{
	public:

		using value_type = int64_t;
		using size_type = size_t;

		explicit AsmFormatter(std::ostream& output);

		virtual ~AsmFormatter() = default;

		virtual void Comment(const std::string& comment);
		virtual void CommentEx(const std::string& comment);

		virtual void Symbolic(const std::string& name, value_type value, size_type width = 0);
		virtual void ReserveBytes(const std::string& name, size_t count);

		virtual void BeginStruct(const std::string& name);
		virtual void EndStruct(const std::string& name);

		virtual void EmitByte(const std::string& symbol, value_type value, const std::string& comment = std::string());
		virtual void EmitWord(const std::string& symbol, value_type value, const std::string& comment = std::string());
		virtual void EmitQuad(const std::string& symbol, value_type value, const std::string& comment = std::string());
		virtual void EmitString(const std::string& symbol, const std::string& value, const std::string& comment = std::string());


	protected:

		virtual void Instruction(const std::string& instruction);

		virtual void Instruction(
			const std::string& symbol,
			const std::string& instruction,
			const std::string& operand = std::string(),
			const std::string& comment = std::string());


	protected:

		static const size_t	m_LabelWidth = 24;
		static const size_t	m_InstrWidth = 12;
		static const size_t	m_OperandWidth = 16;

		std::ostream&		m_Output;
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

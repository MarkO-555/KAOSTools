//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Builder/AsmFormatter.h"
#include <KAOS/Common/Utilities.h>
#include <iomanip>
#include <sstream>


namespace Builder
{

	AsmFormatter::AsmFormatter(std::ostream& output)
		: m_Output(output)
	{}


	void AsmFormatter::Instruction(const std::string& instruction)
	{
		Instruction(std::string(), instruction);
	}

	void AsmFormatter::Instruction(
		const std::string& symbol,
		const std::string& instruction,
		const std::string& operand,
		const std::string& comment)
	{
		std::ostringstream buffer;

		buffer
			<< std::setw(m_LabelWidth - 1) << std::left << symbol << " "
			<< std::setw(m_InstrWidth - 1) << std::left << instruction << " "
			<< std::setw(m_OperandWidth - 1) << std::left << operand << " ";
		if (!comment.empty())
		{
			buffer << "*\t" << comment;
		}
		buffer << "\n";

		m_Output << buffer.str();
	}




	void AsmFormatter::Comment(const std::string& comment)
	{
		std::ostringstream buffer;

		if (!comment.empty())
		{
			buffer << "*\t" << comment;
		}
		buffer << "\n";

		m_Output << buffer.str();
	}


	void AsmFormatter::CommentEx(const std::string& comment)
	{
		m_Output << "*" << comment << "\n";
	}




	void AsmFormatter::Symbolic(const std::string& name, value_type value, size_type width)
	{
		std::ostringstream buffer;

		if (!width)
		{
			if (value < 256)
			{
				width = 2;
			}
			else if (value < 0x4000)
			{
				width = 4;
			}
			else
			{
				width = 8;
			}
		}


		buffer << '$' << std::hex << std::setw(width) << std::setfill('0') << value;
		Instruction(name, "equ", buffer.str());
	}


	void AsmFormatter::ReserveBytes(const std::string& name, size_t count)
	{
		Instruction(name, "rmb", std::to_string(count));
	}




	void AsmFormatter::BeginStruct(const std::string& name)
	{
		static const std::string commentBorder(70, '*');

		m_Output << "\n";
		CommentEx(commentBorder);
		Comment("Data structure for `" + name + "`");
		CommentEx(commentBorder);
		Instruction(name, "struct");
	}


	void AsmFormatter::EndStruct(const std::string& /*name*/)
	{
		Instruction("endstruct");
	}




	void AsmFormatter::EmitByte(const std::string& symbol, value_type value, const std::string& comment)
	{
		Instruction(symbol, "FCB", "$" + KAOS::Common::to_hex_string(value, 2), comment);
	}


	void AsmFormatter::EmitWord(const std::string& symbol, value_type value, const std::string& comment)
	{
		Instruction(symbol, "FDB", "$" + KAOS::Common::to_hex_string(value, 4), comment);
	}


	void AsmFormatter::EmitQuad(const std::string& symbol, value_type value, const std::string& comment)
	{
		Instruction(symbol, "FQB", "$" + KAOS::Common::to_hex_string(value, 8), comment);
	}


	void AsmFormatter::EmitString(const std::string& symbol, const std::string& value, const std::string& comment)
	{
		if (value.size() > 255)
		{
			throw std::invalid_argument("String value is too long and cannot be emitted as code.");
		}

		Instruction(symbol, "FCB", "$" + KAOS::Common::to_hex_string(value.size(), 2));
		Instruction(std::string(), "FCC", "/" + value + "/", comment);
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

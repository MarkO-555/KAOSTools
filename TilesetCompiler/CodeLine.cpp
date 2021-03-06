//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "CodeSegment.h"
#include <sstream>
#include <iomanip>


CodeLine::CodeLine(
	std::string instruction,
	int cycleCount)
	:
	m_Instruction(move(instruction)),
	m_CycleCount(cycleCount)
{}


CodeLine::CodeLine(
	std::string instruction,
	std::string operand,
	int cycleCount)
	:
	m_Instruction(move(instruction)),
	m_Operand(move(operand)),
	m_CycleCount(cycleCount)
{}


CodeLine::CodeLine(
	std::string instruction,
	std::string operand,
	std::string comment,
	int cycleCount)
	:
	m_Instruction(move(instruction)),
	m_Operand(move(operand)),
	m_Comment(move(comment)),
	m_CycleCount(cycleCount)
{}




std::string CodeLine::GetCode() const
{
	std::ostringstream output;

	output << 
		"\t"
		<< std::left << std::setw(8) << m_Instruction
		<< std::left << std::setw(14) << m_Operand
		<< std::left << std::setw(12) << ("*\t(" + std::to_string(m_CycleCount) + ")")
		<< m_Comment;

	return output.str();
}


unsigned int CodeLine::GetCycleCount() const
{
	return m_CycleCount;
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

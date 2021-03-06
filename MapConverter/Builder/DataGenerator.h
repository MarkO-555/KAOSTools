//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <string>
#include <vector>


namespace Builder
{

	class DataGenerator
	{
	public:

		using string_type = std::string;
		using int_type = int64_t;


		DataGenerator(
			std::ostream& output,
			size_t maxSymbolWidth = 16,
			size_t maxOpcodeWidth = 12,
			size_t maxOperandWidth = 64);

		void Reset();

		std::string GenerateLabel();

		void EmitInstanceHeader(const string_type& symbol, const string_type& comment = string_type());
		void EmitInstanceFooter(const string_type& symbol, const string_type& comment = string_type());

		void EmitBlank();
		void EmitComment(const string_type& comment, bool addSpacing = true);
		void EmitSymbol(const string_type& symbol, const string_type& comment = string_type());
		void EmitInstruction(const string_type& symbol, const string_type& opcode, const string_type& operand, const string_type& comment = string_type());

		void EmitByte(const string_type& symbol, int_type value, const string_type& comment = string_type());
		void EmitWord(const string_type& symbol, int_type value, const string_type& comment = string_type());
		void EmitQuad(const string_type& symbol, int_type value, const string_type& comment = string_type());
		void EmitString(const string_type& symbol, const string_type& txt, const string_type& comment = string_type());


		void Flush();


	private:

		enum class BlockType
		{
			Empty,
			Byte,
			Word,
			Quad
		};

		void DumpBuffer(
			const std::vector<int_type>& buffer,
			BlockType valueType,
			const string_type& symbol,
			const string_type& comment);
		void EmitValue(const string_type& symbol, BlockType blockType, int_type value, const string_type& comment);
		void SelectNextType(BlockType blockType);


	private:

		std::ostream&			m_Output;
		const size_t			m_MaxSymbolWidth;
		const size_t			m_MaxOpcodeWidth;
		const size_t			m_MaxOperandWidth;
		BlockType				m_CurrentType;
		std::vector<int_type>	m_CurrentValues;
		string_type				m_Symbol;
		string_type				m_Comment;
		unsigned int			m_LastLabelId;
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

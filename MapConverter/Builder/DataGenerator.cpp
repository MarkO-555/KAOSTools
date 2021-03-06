#include "Builder/DataGenerator.h"
#include <iomanip>
#include <sstream>
#include <iostream>


namespace
{
	template<class ValueType_>
	std::string to_hex_string(const ValueType_& value, unsigned int width)
	{
		std::ostringstream output;
		const uint64_t mask = ~0ULL >> ((sizeof(mask) - (width / 2)) * 8);

		output << std::setw(width) << std::hex << std::setfill('0') << (uint64_t(value) & mask);

		return output.str();
	}
}


namespace Builder
{


	DataGenerator::DataGenerator(
		std::ostream& output,
		size_t maxSymbolWidth,
		size_t maxOpcodeWidth,
		size_t maxOperandWidth)
		:
		m_Output(output),
		m_MaxSymbolWidth(maxSymbolWidth),
		m_MaxOpcodeWidth(maxOpcodeWidth),
		m_MaxOperandWidth(maxOperandWidth)

	{}




	void DataGenerator::Reset()
	{
		m_CurrentType = BlockType::Empty;
		m_CurrentValues.clear();
		m_Symbol.clear();
		m_Comment.clear();
		m_LastLabelId = 0;
	}


	std::string DataGenerator::GenerateLabel()
	{
		std::ostringstream output;

		output << 'L' << std::setw(5) << std::setfill('0') << ++m_LastLabelId;

		return output.str();
	}




	void DataGenerator::EmitInstanceHeader(const string_type& symbol, const string_type& comment)
	{
		Flush();

		if (!symbol.empty())
		{
			EmitSymbol(symbol, comment);
		}
		else if (!comment.empty())
		{
			EmitComment(comment);
		}

		Flush();
	}


	void DataGenerator::EmitInstanceFooter(const string_type& symbol, const string_type& comment)
	{
		Flush();

		if (!symbol.empty())
		{
			EmitSymbol(symbol, comment);
		}
		else if (!comment.empty())
		{
			EmitComment(comment);
		}

		Flush();
	}



	void DataGenerator::EmitBlank()
	{
		Flush();
		m_Output << "\n";
	}


	void DataGenerator::EmitComment(const string_type& comment, bool addSpacing)
	{
		Flush();
		m_Output << ";" << (addSpacing ? "  " : "") << comment << "\n";
	}




	void DataGenerator::EmitSymbol(const string_type& symbol, const string_type& comment)
	{
		Flush();
		EmitInstruction(symbol, "EQU", "*", comment);
	}


	void DataGenerator::EmitInstruction(const string_type& symbol, const string_type& opcode, const string_type& operand, const string_type& comment)
	{
		m_Output
			<< std::setw(m_MaxSymbolWidth - 1) << std::left << symbol
			<< " " << std::setw(m_MaxOpcodeWidth - 1) << std::left << opcode
			<< " " << std::setw(m_MaxOperandWidth - 1) << std::left << operand
			;

		if (!comment.empty())
		{
			m_Output << ";  " << comment;
		}

		m_Output << std::endl;
	}


	void DataGenerator::EmitByte(const string_type& symbol, int_type value, const string_type& comment)
	{
		EmitValue(symbol, BlockType::Byte, value, comment);
	}


	void DataGenerator::EmitWord(const string_type& symbol, int_type value, const string_type& comment)
	{
		EmitValue(symbol, BlockType::Word, value, comment);
	}


	void DataGenerator::EmitQuad(const string_type& symbol, int_type value, const string_type& comment)
	{
		EmitValue(symbol, BlockType::Quad, value, comment);
	}


	void DataGenerator::EmitString(const string_type& symbol, const string_type& value, const string_type& comment)
	{
		Flush();
		EmitWord(symbol, value.size(), comment);
		Flush();
		EmitInstruction("", "FCC", "/" + value + "/");
		Flush();
	}




	void DataGenerator::Flush()
	{
		DumpBuffer(m_CurrentValues, m_CurrentType, m_Symbol, m_Comment);

		m_CurrentType = BlockType::Empty;
		m_CurrentValues.clear();
		m_Symbol.clear();
		m_Comment.clear();
	}








	void DataGenerator::DumpBuffer(
		const std::vector<int_type>& buffer,
		BlockType valueType,
		const string_type& symbol,
		const string_type& comment)
	{
		if (!buffer.empty())
		{
			unsigned int width(0);
			string_type opcode;

			switch (valueType)
			{
			case BlockType::Byte:
				opcode = "FCB";
				width = 2;
				break;

			case BlockType::Word:
				opcode = "FDB";
				width = 4;
				break;

			case BlockType::Quad:
				opcode = "FQB";
				width = 8;
				break;

			default:
				throw std::runtime_error("Unsupported value type on flush");
			}

			string_type outputString;
			string_type activeSymbol(symbol);
			string_type activeComment(comment);
			for (const auto& value : buffer)
			{
				if (!outputString.empty())
				{
					outputString += ",";
				}

				outputString += "$" + to_hex_string(value, width);

				if (outputString.size() > m_MaxOperandWidth - 2)
				{
					EmitInstruction(activeSymbol, opcode, outputString, activeComment);
					outputString.clear();
					activeSymbol.clear();
					activeComment.clear();
				}
			}

			//	Emit final instruction if cache is not empty
			if (!outputString.empty())
			{
				EmitInstruction(activeSymbol, opcode, outputString, activeComment);
			}
		}
	}

	void DataGenerator::EmitValue(const string_type& symbol, BlockType blockType, int_type value, const string_type& comment)
	{
		if (!symbol.empty() || !comment.empty())
		{
			Flush();
		}

		SelectNextType(blockType);

		if (!symbol.empty())
		{
			m_Symbol = symbol;
		}

		if (!comment.empty())
		{
			m_Comment = comment;
		}

		m_CurrentValues.emplace_back(value);
	}


	void DataGenerator::SelectNextType(BlockType blockType)
	{
		if (m_CurrentType != blockType)
		{
			if (!m_CurrentValues.empty())
			{
				Flush();
			}

			m_CurrentType = blockType;
		}
	}

}

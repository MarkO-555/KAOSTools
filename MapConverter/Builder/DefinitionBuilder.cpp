//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DefinitionBuilder.h"


namespace Builder
{

	DefinitionBuilder::DefinitionBuilder(KAOS::Common::EventConsole& eventConsole, unsigned int shift)
		:
		m_EventConsole(eventConsole),
		m_Shift(shift)
	{}




	DefinitionBuilder::shiftvalue_type DefinitionBuilder::SetValueShift(shiftvalue_type shift)
	{
		std::swap(shift, m_Shift);

		return shift;
	}


	DefinitionBuilder::shiftvalue_type DefinitionBuilder::GetValueShift() const
	{
		return m_Shift;
	}




	bool DefinitionBuilder::Begin(name_type name)
	{
		if (m_CurrentDefinition)
		{
			m_EventConsole.Error("Definition already in progress.");
			return false;
		}

		m_Definitions.emplace_back(std::make_unique<definition_type>(move(name)));
		m_CurrentDefinition = m_Definitions.back().get();

		return true;
	}


	bool DefinitionBuilder::End()
	{
		if (!m_CurrentDefinition)
		{
			m_EventConsole.Error("Definition not in progress. Cannot terminate.");
			return false;
		}

		m_CurrentDefinition = nullptr;

		return true;
	}


	bool DefinitionBuilder::AddVariable(name_type name, typeid_type type)
	{
		if (!m_CurrentDefinition)
		{
			m_EventConsole.Error("Definition not in progress. Cannot add variable.");
			return false;
		}

		return m_CurrentDefinition->AddVariable(move(name), type);
	}


	bool DefinitionBuilder::AddSymbolic(name_type name, long value)
	{
		if (!m_CurrentDefinition)
		{
			m_EventConsole.Error("Definition not in progress. Cannot add symbolic.");
			return false;
		}

		return m_CurrentDefinition->AddSymbolic(move(name), value << m_Shift);
	}




	bool DefinitionBuilder::Generate(formatter_type& formatter) const
	{
		if (m_CurrentDefinition)
		{
			m_EventConsole.Error("Definition already in progress. Cannot generate definition code.");
			return false;
		}

		for (const auto& definition : m_Definitions)
		{
			definition->Generate(formatter);
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

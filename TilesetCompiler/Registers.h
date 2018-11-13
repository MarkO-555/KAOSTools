//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <string>
#include <optional>


class WordAccRegister
{
public:

	struct traits
	{
		using value_type = uint16_t;
		using subvalue_type = uint8_t;

		static const unsigned int	FillWidth = 4;
		static const unsigned int	HiShift = 8;
		static const subvalue_type	SubValueMask = 0xff;

	};

	using traits_type = traits;
	using value_type = traits_type::value_type;
	using subvalue_type = traits_type::subvalue_type;


public:

	WordAccRegister& operator=(const WordAccRegister& other);

	bool operator!=(const WordAccRegister& other) const;

	//bool operator==(const WordAccRegister& other) const;

	bool HasValue() const;
	value_type GetValue() const;

	std::string GenerateLoad(value_type newWord);


protected:

	WordAccRegister(
		std::string	fullRegName,
		std::string	hiRegName,
		std::string	loRegName);

	WordAccRegister(
		value_type value,
		std::string	fullRegName,
		std::string	hiRegName,
		std::string	loRegName);


private:

	const std::string			m_FullRegName;
	const std::string			m_HiRegName;
	const std::string			m_LoRegName;
	std::optional<value_type>	m_Value;
	subvalue_type				m_LoHalf = 0;
	subvalue_type				m_HiHalf = 0;
};




class WRegister : public WordAccRegister
{
public:

	WRegister();
	explicit WRegister(value_type value);

	using WordAccRegister::operator=;
};


class DRegister : public WordAccRegister
{
public:

	DRegister();
	explicit DRegister(value_type value);

	using WordAccRegister::operator=;
};




class QRegister
{
public:

	using value_type = uint32_t;

	std::string GenerateLoad(uint32_t newQuad);


private:

	DRegister	m_Accd;
	WRegister	m_Accw;
	std::optional<value_type>	m_Value;
};




inline bool operator!=(const WordAccRegister::value_type& value, const WordAccRegister& other)
{
	return !other.HasValue() || value != other.GetValue();
}


//bool operator==(const WordAccRegister::value_type& value, const WordAccRegister& other);




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
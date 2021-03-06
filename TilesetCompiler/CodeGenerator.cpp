//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "CodeGenerator.h"
#include "Registers.h"
#include <KAOS/Common/Utilities.h>
#include <KAOS/Imaging/ImageUtils.h>
#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <type_traits>
#include <sstream>


void CodeGenerator::GeneratePaletteCode(
	std::ostream& output,
	const KAOS::Imaging::Palette& palette) const
{
	output << "TilesetPalette\tFCB\t";

	for (const auto& color : palette)
	{
		output
			<< (&color != &palette.front() ? "," : "")
			<< static_cast<int>(KAOS::Imaging::ConvertColorToRGBRGB(color));
	}
}



void CodeGenerator::GenerateTileCode(
	std::ostream& output,
	IntermediateImage image,
	unsigned int id) const
{
	output << "*\n";
	for (const auto& row : image)
	{
		output << "*\t\t" << KAOS::Common::to_hex_string(row.GetPixelsAsQuad(0), 8) << "\n";
	}
	output << "*\n";

	//	Consolidate
	for (auto i(0ull); i < image.size(); ++i)
	{
		auto& info(image[i]);
		for (auto x(i + 1), count(image.size()); x < count; )
		{
			auto& other(image[x]);
			if (info.ComparePixels(other))
			{
				info.AddOffsets(other.GetOffsets());
				image.erase_at(x);
				--count;
			}
			else
			{
				++x;
			}
		}
	}

	const auto labelBase("Tile" + std::to_string(id));
	auto codeSegment(Generate4ByteRowCode(image));

	codeSegment.Append("rts", 4);

	output
		<< "***************************************************************\n"
		<< "* Tile #" << id << " (" << codeSegment.GetCycleCount() << " cycles)\n"
		<< "***************************************************************\n"
		<< labelBase << "\n"
		<< "\tFDB\t" << std::to_string(id) << "\n"
		<< "\tFDB\t" << labelBase << "DataEnd-" << labelBase << "DataBegin\n"
		<< labelBase << "DataBegin\n"
		<< codeSegment.GetCode()
		<< labelBase << "DataEnd\n"
		<< "*\n";
}




void CodeGenerator::GenerateTileAliasCode(
	std::ostream& output,
	unsigned int id,
	unsigned int aliasId) const
{
	const auto labelBase("Tile" + std::to_string(id));

	output
		<< "***************************************************************\n"
		<< "* Tile #" << id << "\n"
		<< "***************************************************************\n"
		<< labelBase << "\n"
		<< "\tFDB\t$" << KAOS::Common::to_hex_string(id, 4) << "\n"
		<< "\tFDB\t$" << KAOS::Common::to_hex_string(0x8000 | aliasId, 4) << "\n"
		<< "*\n";
}




CodeSegment CodeGenerator::Generate4ByteRowCode(const IntermediateImage& image) const
{
	QRegister regState;
	CodeSegment codeSegment;

	for(const auto& row : image)
	{
		if (row.GetOffsetCount() == 0)
		{
			continue;
		}

		codeSegment += regState.GenerateLoad(row.GetPixelsAsQuad(0));
		for (const auto& offset : row.GetOffsets())
		{
			auto cycleCount = 8;
			//	TODO: Move to helper function
			if (offset == 0)
			{
				cycleCount += 0;
			}
			else if (offset >= -128 && offset <= 127)
			{
				cycleCount += 1;
			}
			else
			{
				cycleCount += 3;
			}

			codeSegment.Append("STQ", "$" + KAOS::Common::to_hex_string(offset, 4) + ",y", cycleCount);
		}
	}

	return codeSegment;
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

//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2006-2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "MapLayer.h"
#include "Map.h"
#include "Utils.h"
#include "pugixml.hpp"
#include <iostream>
#include <deque>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <optional>


bool LoadObjectList(const std::string& filename, std::map<std::string, unsigned int>& objectList)
{
	pugi::xml_document doc;

	auto result(doc.load_file(filename.c_str()));
	if (!result)
	{
		std::cerr << "Unable to open `" << filename << "`\n";
		return false;
	}

	auto objectdefs(doc.children("objectdefs"));
	if(std::distance(objectdefs.begin(), objectdefs.end()) > 1)
	{
		std::cerr << "Multiple maps not supported\n";
		return false;
	}

	auto objects(objectdefs.begin()->children("object"));
	for (const auto& object : objects)
	{
		const auto idAttribute(object.attribute("id"));
		const auto id(idAttribute.as_int());
		const auto objectName(object.attribute("name"));
		objectList[objectName.as_string()] = id;
	}

	return true;
}




bool GenerateCode(std::ostream& output, const Map& map, const std::map<std::string, unsigned int>& objectList)
{
	bool hasError(false);

	const auto nameSize(map.GetName().size());
	output << "\tFCC\t`MAPX`\n";
	output << "\tFCC\t`" << map.GetName() << "`\n";
	output << "\tZMB\t" << 32 - nameSize << "\n";
	output << "\tFDB\t"
		<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << map.GetWidth()
		<< ','
		<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << map.GetHeight()
		<< '\n';

	for (const auto& layer : map.GetLayers())
	{
		unsigned int column = 0;
		for (const auto& id : *layer)
		{
			if (column == 0)
			{
				output << "\tFCB\t";
			}
			else
			{
				output << ',';
			}

			output << '$' << std::setfill('0') << std::hex << std::fixed << std::setw(2) << id;

			++column;
			if (column == 32 || column == layer->GetWidth())
			{
				column = 0;
				output << '\n';
			}
		}
	}

	for (const auto& objectGroup : map.GetObjectGroups())
	{
		output
			<< "*\n"
			<< "* Object Group: " << objectGroup->GetName() << "\n"
			<< "*\n";

		output << "\tFDB\t"
			<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << objectGroup->size() << "\n";

		for (const auto& object : *objectGroup)
		{
			const auto typeIdPtr(objectList.find(object->GetType()));
			const auto typeId(typeIdPtr == objectList.end() ? -1 : typeIdPtr->second);
			if (typeIdPtr == objectList.end())
			{
				std::cerr << "Object type `" << object->GetType() << "` not found.\n";
				hasError = true;
			}

			output << "\tFDB\t"
				<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << typeId
				<< ','
				<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << object->GetXPos()
				<< ','
				<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << object->GetYPos()
				<< "\t\t*\t" << object->GetName()
				<< '\n';
		}
	}

	return !hasError;
}



int main(int argc, const char **argv)
{
	std::deque<std::string> args(argv + 1, argv + argc);
	std::string outputDirectory;
	std::optional<unsigned int> emptyId;
	std::optional<std::string> filename;
	std::optional<std::string> objectListFilename;
	bool hasError(false);


	while (!args.empty())
	{
		const auto originalArg(args.front());
		args.pop_front();
		if (originalArg.empty())
		{
			continue;
		}

		if (originalArg[0] == '-')
		{
			auto arg(originalArg);

			arg.erase(arg.begin());
			if (arg.empty() || arg[0] != '-')
			{
				std::cerr << "Unknown option `" << arg << "`\n";
				return EXIT_FAILURE;
			}
			arg.erase(arg.begin());


			const auto argOffset(arg.find('='));
			std::string value;

			if (argOffset != arg.npos)
			{
				value = arg.substr(argOffset + 1);
				arg.resize(argOffset);
			}


			if (arg == "output")
			{
				if (!outputDirectory.empty())
				{
					std::cerr << "Warning: Output directory file already set to `" << outputDirectory << "`\n";
				}
				else
				{
					outputDirectory = value;
				}
			}
			else if (arg == "objectlist")
			{
				if (objectListFilename.has_value())
				{
					std::cerr << "Warning: Object list file already set to `" << *objectListFilename << "`\n";
				}
				else
				{
					objectListFilename = value;
				}
			}
			else if (arg == "emptyid")
			{
				if (emptyId.has_value())
				{
					std::cerr << "Warning: Empty tile ID already set to `" << emptyId.value() << "`\n";
				}
				else
				{
					emptyId = std::stoul(value);
				}
			}
			else
			{
				std::cerr << "Unknown argument `" << originalArg << "`\n";
				return EXIT_FAILURE;
			}
		}

		else
		{
			if (filename.has_value())
			{
				std::cerr << "Multiple files not supported. File `" << originalArg << " ignored.`\n";
				hasError = true;
			}

			filename = originalArg;
		}
	}


	//	Errors
	if (!filename.has_value())
	{
		std::cerr << "Input filename not set\n";
		hasError = true;
	}

	if (outputDirectory.empty())
	{
		std::cerr << "WARNING: Output directory is not set. Using current working directory.\n";
		outputDirectory = ".";
	}

	if (hasError)
	{
		return EXIT_FAILURE;
	}


	//	Warnings
	if (!emptyId.has_value())
	{
		std::cerr << "Empty tile ID not set. Defaulting to 0\n";
		emptyId = 0;
	}


	std::map<std::string, unsigned int> objectList;
	if (objectListFilename.has_value())
	{
		LoadObjectList(*objectListFilename, objectList);
	}

	Map map;
	if(!map.Load(*filename, emptyId.value()))
	{
		return EXIT_FAILURE;
	}

	std::ostringstream outputString;
	if (!GenerateCode(outputString, map, objectList))
	{
		return EXIT_FAILURE;
	}


	auto outputFilename(MakePath(outputDirectory, GetFilenameFromPath(map.GetFilename()) + ".asm"));
	std::ofstream output(outputFilename);
	if (!output.is_open())
	{
		std::cerr << "Unable to create `" << outputFilename << "`\n";
	}

	output << outputString.str();
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

//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <Tiled/Map.h>
#include <Tiled/Tileset.h>
#include <KAOS/Common/Utilities.h>
#include <pugixml/pugixml.hpp>
#include <iostream>
#include <deque>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <optional>


bool ParseObjectList(const std::string& filename, std::map<std::string, unsigned int>& objectList)
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




bool GenerateCode(
	std::ostream& output,
	const KAOS::Tiled::Map& map,
	const std::map<std::string, unsigned int>& objectList,
	const unsigned int emptyCellId)
{
	bool hasError(false);

	const auto mapDimensions(map.GetDimensions());
	const auto mapName(map.GetName());

	output << "\tFCC\t`MAPX`\n";
	output << "\tFCC\t`" << mapName << "`\n";
	output << "\tZMB\t" << 32 - mapName.size() << "\n";
	output << "\tFDB\t"
		<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << mapDimensions.GetWidth()
		<< ','
		<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << mapDimensions.GetHeight()
		<< '\n';

	const auto allLayers(map.GetLayers());
	std::vector<std::shared_ptr<const KAOS::Tiled::TilesetLayer>> tilesetLayers;
	std::vector<std::shared_ptr<const KAOS::Tiled::ObjectGroupLayer>> objectGroupLayers;


	for (const auto& layer : allLayers)
	{
		auto tilesetLayer(std::dynamic_pointer_cast<const KAOS::Tiled::TilesetLayer>(layer));
		if (tilesetLayer)
		{
			tilesetLayers.emplace_back(tilesetLayer);
			continue;
		}

		auto objectGroupLayer(std::dynamic_pointer_cast<const KAOS::Tiled::ObjectGroupLayer>(layer));
		if (objectGroupLayer)
		{
			objectGroupLayers.emplace_back(objectGroupLayer);
			continue;
		}
	}

	if (tilesetLayers.size() > 1)
	{
		std::cerr << "Multiple layers not supported\n";
		return false;
	}



	for (const auto& layer : tilesetLayers)
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

			const auto convertedId(id == 0 ? emptyCellId : id - 1);

			output << '$' << std::setfill('0') << std::hex << std::fixed << std::setw(2) << convertedId;

			++column;
			if (column == 32 || column == layer->GetDimensions().GetWidth())
			{
				column = 0;
				output << '\n';
			}
		}
	}

	for (const auto& objectGroup : objectGroupLayers)
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

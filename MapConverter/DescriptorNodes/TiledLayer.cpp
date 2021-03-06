//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/TiledLayer.h"
#include "Builder/MapDataSource.h"
#include <KAOS/Common/Logging.h>
#include <KAOS/Common/xml.h>
#include <iostream>
#include  <iomanip>


namespace DescriptorNodes
{

	bool TiledLayer::Parse(const pugi::xml_node& node)
	{
		const auto& indexAttr(node.attribute("index"));
		const auto& srcAttr(node.attribute("src"));

		if (!indexAttr.empty() && !srcAttr.empty())
		{
			KAOS::Logging::Error("TiledLayer cannot have both an `index` and `src` attributes");
			return false;
		}

		if (indexAttr.empty() && srcAttr.empty())
		{
			KAOS::Logging::Error("TiledLayer must have either an `index` or `src` attribute");
			return false;
		}

		std::optional<uint64_t> layerIndex;
		if (!indexAttr.empty())
		{
			layerIndex = indexAttr.as_ullong();
		}

		std::optional<std::string> layerName;
		if (!srcAttr.empty())
		{
			layerName = srcAttr.as_string();
		}

		decltype(m_Signature) signature;
		KAOS::Common::XML::LoadAttribute(node, "signature", signature);
		

		if (!DescriptorNode::Parse(node))
		{
			return false;
		}


		m_LayerIndex = move(layerIndex);
		m_LayerName = move(layerName);
		m_Signature = move(signature);


		return true;
	}



	bool TiledLayer::CompileDefinition(definitionbuilder_type& /*builder*/) const
	{
		return true;
	}

	bool TiledLayer::CompileInstance(databuilder_type& builder, const datasource_type& dataSource) const
	{
		auto& mapDataSource(dynamic_cast<const Builder::MapDataSource&>(dataSource));

		std::shared_ptr<Builder::MapDataSource::tilesetlayer_type> layer;
		if (m_LayerName.has_value())
		{
			layer = mapDataSource.QueryTilesetLayer(*m_LayerName);
			if (!layer)
			{
				KAOS::Logging::Error("Unable to find tiled layer named `" + *m_LayerName + "`");
				return false;
			}
		}
		if (m_LayerIndex.has_value())
		{
			layer = mapDataSource.QueryTilesetLayer(*m_LayerIndex);
			if (!layer)
			{
				KAOS::Logging::Error("Unable to find tiled layer at index `" + std::to_string(*m_LayerIndex) + "`");
				return false;
			}
		}


		builder.EmitBlank();
		builder.EmitSeparatorComment();
		builder.EmitComment("Map layer `" + layer->GetName() + "'");
		builder.EmitSeparatorComment();
		if (!GetSymbol().empty())
		{
			builder.EmitLabel(GetSymbol());
		}

		const signature_type signature(m_Signature.has_value() ? m_Signature.value() : Signature);

		builder.EmitValue(std::string(), signature, "Block Signature");
		builder.EmitValue(std::string(), static_cast<Builder::DataBuilder::property_type::quad_type>(layer->size()), "Number of cells");
		builder.EmitComment("");

		const auto emptyCellId(0U);	//	FIXME: This needs to come from somewhere (datasource?)
		for (const auto& id : *layer)
		{
			const uint32_t convertedId(id == 0 ? emptyCellId : id - 1);

			//	FIXME: Check for bounds error
			builder.EmitValue(std::string(), databuilder_type::property_type::byte_type(convertedId));
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

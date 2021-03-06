//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "DescriptorNodes/OptionTable.h"
#include "DescriptorNode.h"
#include <vector>
#include <map>
#include <functional>
#include <optional>
#include <algorithm>


namespace DescriptorNodes
{

	class CompositeNode : public DescriptorNode
	{
	public:

		using factoryfunction_type = std::unique_ptr<DescriptorNode>();
		using factory_type = std::map<std::string, std::function<factoryfunction_type>>;
		using collection_type = std::vector<std::shared_ptr<DescriptorNode>>;

		bool Parse(const pugi::xml_node& node) override;

		bool CompileDefinition(definitionbuilder_type& builder) const override;
		bool CompileInstance(databuilder_type& builder, const datasource_type& dataSource) const override;

		template<class Type_, class PredicateType_>
		std::shared_ptr<Type_> QueryMemberIf(const PredicateType_& predicate) const;

		template<class UnaryFunctionType_>
		void ForEachMember(UnaryFunctionType_& unaryFunction) const;


	protected:

		virtual bool CompileMemberDefinitions(definitionbuilder_type& builder) const;
		virtual bool CompileInstanceMembers(databuilder_type& builder, const datasource_type& dataSource) const;

		virtual std::optional<collection_type> ParseMembers(const pugi::xml_node& node, const factory_type& exemplars) const;
		virtual const factory_type& GetExemplars() const;
		virtual std::unique_ptr<OptionTable> CreateOptionTable(const collection_type& members) const;

	
	protected:

		std::unique_ptr<OptionTable>	m_OptionTable;
	private:
		collection_type					m_Members;
	};


	template<class Type_, class PredicateType_>
	inline std::shared_ptr<Type_> CompositeNode::QueryMemberIf(const PredicateType_& predicate) const
	{
		auto member(std::find_if(m_Members.begin(), m_Members.end(), predicate));
		
		return member != m_Members.end() ? std::dynamic_pointer_cast<Type_>(*member) : nullptr;
	}

	template<class UnaryFunctionType_>
	inline void CompositeNode::ForEachMember(UnaryFunctionType_& unaryFunction) const
	{
		std::for_each(m_Members.begin(), m_Members.end(), unaryFunction);
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

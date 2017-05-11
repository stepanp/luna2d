//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2017 Stepan Prokofjev
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "lunapurchases.h"
#include "lunaconfig.h"
#include "lunalog.h"
#include "lunalua.h"

using namespace luna2d;

const std::string CONFIG_PRODUCTS_NAME = "inAppPurchasesProducts";

static LuaTable GetPurchasesTable()
{
	return LUNAEngine::SharedLua()->GetGlobalTable().GetTable("luna").GetTable("purchases");
}


// Read list of aliases for product ids from config
void LUNAPurchases::ReadProductsFromConfig()
{
	auto jsonAliasesList = LUNAEngine::Shared()->GetConfig()->GetCustomValues()[CONFIG_PRODUCTS_NAME];

	// Aliases list has aliases and product ids
	if(jsonAliasesList.is_object())
	{
		for(const auto& jsonProduct : jsonAliasesList.object_items())
		{
			if(!jsonProduct.second.is_string())
			{
				LUNA_LOGE("Product id for in-app purchases must be string");
				continue;
			}
			productAliases[jsonProduct.first] = jsonProduct.second.string_value();
		}
	}

	// Aliases list has only product ids
	else if(jsonAliasesList.is_array())
	{
		for(const auto& jsonProductId : jsonAliasesList.array_items())
		{
			if(!jsonProductId.is_string())
			{
				LUNA_LOGE("Product id for in-app purchases must be string");
				continue;
			}
			productAliases[jsonProductId.string_value()] = jsonProductId.string_value();
		}
	}

	else if(!jsonAliasesList.is_null())
	{
		LUNA_LOGE("\"%s\" should be array or object", CONFIG_PRODUCTS_NAME.c_str());
	}
}

// Get product id by alias
std::string LUNAPurchases::GetProductIdForAlias(const std::string& alias)
{
	auto it = productAliases.find(alias);
	if(it == productAliases.end()) return "";
	return it->second;
}

// Get all aliases for given product id
std::vector<std::string> LUNAPurchases::GetAliasesForProductId(const std::string& productId)
{
	std::vector<std::string> ret;

	for(const auto& entry : productAliases)
	{
		if(entry.second == productId) ret.push_back(entry.first);
	}

	return ret;
}

// Calls when products info successfully fetched
void LUNAPurchases::OnProductsFetched(const std::vector<std::string>& availableProducts)
{
	auto tblAds = GetPurchasesTable();
	auto fnProductsFetched = tblAds.GetFunction("onProductsFetched");

	if(!fnProductsFetched) return;

	std::vector<std::string> availableProductsAliases;

	for(const auto& productId : availableProducts)
	{
		auto aliases = GetAliasesForProductId(productId);
		availableProductsAliases.insert(availableProductsAliases.end(), aliases.begin(), aliases.end());
	}

	fnProductsFetched.CallVoid(availableProductsAliases);
}

// Calls when product with given product id has been purchased
void LUNAPurchases::OnProductPurchased(const std::string& productId)
{
	auto tblAds = GetPurchasesTable();
	auto fnProductPurchased = tblAds.GetFunction("onProductPurchased");

	if(!fnProductPurchased) return;

	for(const auto& alias : GetAliasesForProductId(productId))
	{
		fnProductPurchased.CallVoid(alias);
	}
}

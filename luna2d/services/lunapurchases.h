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

#include "lunaengine.h"

namespace luna2d{

class LUNAPurchases
{
public:
	virtual ~LUNAPurchases() {}

protected:
	std::unordered_map<std::string, std::string> productAliases;

protected:
	// Read list of aliases for product ids from config
	void ReadProductsFromConfig();

	// Get product id by alias
	std::string GetProductIdForAlias(const std::string& alias);

	// Get all aliases for given product id
	std::vector<std::string> GetAliasesForProductId(const std::string& productId);

public:
	// Fetch products info from server
	virtual void FetchProducts() = 0;

	// Purchase product with given id
	virtual void PurchaseProduct(const std::string& productId) = 0;

	// Restore purchased products
	virtual void RestoreProducts() = 0;

	// Calls when products info successfully fetched
	void OnProductsFetched(const std::vector<std::string>& availableProducts);

	// Calls when product with given product id has been purchased
	void OnProductPurchased(const std::string& productId);
};

}

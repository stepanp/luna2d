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

#include "lunaiospurchases.h"
#include "lunaconfig.h"
#include "lunansstring.h"
#include "lunalog.h"
#include "lunaiosservices.h"
#include "lunamacro.h"
#import <UIKit/UIKit.h>

using namespace luna2d;

@interface ProductRequestDelegate : NSObject<SKProductsRequestDelegate>

@end

@implementation ProductRequestDelegate

- (void) productsRequest:(SKProductsRequest*) request didReceiveResponse:(SKProductsResponse*) response
{
	auto purchases = std::static_pointer_cast<LUNAIosPurchases>(LUNAEngine::SharedServices()->GetPurchases());
	purchases->OnFetchProducts(response);
}

-(void) request:(SKRequest*) request didFailWithError:(NSError*) error
{
	NSLog(@"Failed to retrieve in-app purchases product list: %@", error.localizedDescription);
}

@end


@interface PurchasesDelegate : NSObject <SKPaymentTransactionObserver>


@end

@implementation PurchasesDelegate

-(void) paymentQueue:(SKPaymentQueue*) queue updatedTransactions:(NSArray<SKPaymentTransaction*>*) transactions
{
	for(SKPaymentTransaction *transaction in transactions)
	{
		switch(transaction.transactionState)
		{
			case SKPaymentTransactionStatePurchased:
			case SKPaymentTransactionStateRestored:
			{
				auto purchases = std::static_pointer_cast<LUNAIosPurchases>(LUNAEngine::SharedServices()->GetPurchases());
				purchases->OnProductPurchased(FromNsString(transaction.payment.productIdentifier), true);
				
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
				break;
			}
			case SKPaymentTransactionStateFailed:
			{
				NSString* error = [transaction.error localizedDescription];
				if(error) NSLog(@"In-app purchase failed: %@", error);
				
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
				break;
			}
			case SKPaymentTransactionStateDeferred:
			case SKPaymentTransactionStatePurchasing:
			default:
				break;
		};
	}
}

-(void) request:(SKRequest*)request didFailWithError:(NSError*) error
{
	NSLog(@"Failed to perform in-app purchase %s", error.localizedDescription);
	
	auto purchases = std::static_pointer_cast<LUNAIosPurchases>(LUNAEngine::SharedServices()->GetPurchases());
	purchases->OnProductPurchased("", false);
}

@end


LUNAIosPurchases::LUNAIosPurchases()
{
	ReadProductsFromConfig();
	
	if(productAliases.empty()) return;
	
	purchasesDelegate = [[PurchasesDelegate alloc] init];
	[[SKPaymentQueue defaultQueue] addTransactionObserver:purchasesDelegate];
}

SKProduct* LUNAIosPurchases::GetProduct(const std::string& productId)
{
	return [products objectForKey:ToNsString(GetProductIdForAlias(productId))];
}

// Fetch products info from server
void LUNAIosPurchases::FetchProducts()
{
	if(productAliases.empty()) LUNA_RETURN_ERR("NO ALIASES");
	if([products count] > 0) return;
	
	NSMutableSet* productSet = [NSMutableSet set];
	for(const auto& entry : productAliases)
	{
		auto productId = entry.second;
		[productSet addObject:ToNsString(productId)];
	}
	
	SKProductsRequest* request = [[SKProductsRequest alloc] initWithProductIdentifiers:productSet];
	request.delegate = [[ProductRequestDelegate alloc] init];
	[request start];
}

// Purchase product with given id
void LUNAIosPurchases::PurchaseProduct(const std::string& productId)
{
	SKProduct* product = GetProduct(productId);
	
	if(!product) LUNA_RETURN_ERR("Product \"%s\" not found", productId.c_str());
	
	SKPayment* payment = [SKPayment paymentWithProduct:product];
	[[SKPaymentQueue defaultQueue] addPayment:payment];
}

// Restore purchased products
void LUNAIosPurchases::RestoreProducts()
{
	[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

void LUNAIosPurchases::OnFetchProducts(SKProductsResponse* response)
{
	for(NSString* invalidIdentifier in response.invalidProductIdentifiers)
	{
		LUNA_LOGE("Invalid iTunes Store product id \"%s\"", [invalidIdentifier UTF8String]);
	}
	
	[products removeAllObjects];
	std::vector<std::string> availableProducts;
	
	for(SKProduct* product in response.products)
	{
		NSString* productId = product.productIdentifier;
		
		[products setObject:product forKey:productId];
		availableProducts.push_back(FromNsString(productId));
	}
	
	LUNAPurchases::OnProductsFetched(availableProducts);
}

void LUNAIosPurchases::OnProductPurchased(const std::string& productId, bool isSuccess)
{
	LUNA_LOG("OnProductPurchased %s %s", productId.c_str(), isSuccess ? "true" : "false");
	if(isSuccess) LUNAPurchases::OnProductPurchased(productId);
}


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

#include "lunaandroidpurchases.h"
#include "lunafiles.h"
#include "lunalog.h"
#include "lunamacro.h"

using namespace luna2d;

LUNAAndroidPurchases::LUNAAndroidPurchases()
{
	jni::Env env;

	// Get ref to java wrapper class
	jclass localRef = env->FindClass("com/stepanp/luna2d/services/LunaPurchases");
	javaPurchases = reinterpret_cast<jclass>(env->NewGlobalRef(localRef));
	env->DeleteLocalRef(localRef);

	// Get java wrapper method ids
	javaFetchProducts = env->GetStaticMethodID(javaPurchases, "fetchProducts", "([Ljava/lang/String;)V");
	javaPurchaseProduct = env->GetStaticMethodID(javaPurchases, "purchaseProduct", "(Ljava/lang/String;)V");
	javaRestoreProducts = env->GetStaticMethodID(javaPurchases, "restoreProducts", "()V");

	ReadProductsFromConfig();
}

// Fetch products info from server
void LUNAAndroidPurchases::FetchProducts()
{
	if(productAliases.empty()) LUNA_RETURN_ERR("Cannot fetch products."
		"Any product id not found in \"inAppPurchasesProducts\" value in config");

	jobjectArray javaProductIds = jni::Env()->NewObjectArray(productAliases.size(), 
		jni::Env()->FindClass("java/lang/String"), jni::Env()->NewStringUTF(""));

	int i = 0;
	for(const auto& entry : productAliases)
	{
		jni::Env()->SetObjectArrayElement(javaProductIds, i, jni::Env()->NewStringUTF(entry.second.c_str())); 

		i++;
	}

	jni::Env()->CallStaticVoidMethod(javaPurchases, javaFetchProducts, javaProductIds);
}

// Purchase product with given id
void LUNAAndroidPurchases::PurchaseProduct(const std::string& productId)
{
	jni::Env()->CallStaticVoidMethod(javaPurchases, javaPurchaseProduct,
		jni::ToJString(GetProductIdForAlias(productId)).j_str());
}

// Restore purchased products
void LUNAAndroidPurchases::RestoreProducts()
{
	jni::Env()->CallStaticVoidMethod(javaPurchases, javaRestoreProducts);
}

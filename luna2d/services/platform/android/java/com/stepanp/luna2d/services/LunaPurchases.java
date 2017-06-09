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

package com.stepanp.luna2d.services;

import java.util.Arrays;
import java.util.List;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import com.android.billing.util.IabHelper;
import com.android.billing.util.IabResult;
import com.android.billing.util.Inventory;
import com.android.billing.util.Purchase;
import com.stepanp.luna2d.services.api.LunaActivityListener;
import com.stepanp.luna2d.services.api.LunaServicesApi;

public class LunaPurchases
{
    private static final int RC_REQUEST = 10001;

    private static IabHelper helper;
    private static Inventory inventory;

	public static void init()
	{
		if(!LunaServicesApi.hasConfigValue("googlePlayPublicKey"))
		{
			Log.e(LunaServicesApi.getLogTag(), "Application's public key \"googlePlayPublicKey\" should be set in config");
			return;
		}

		Activity activity = LunaServicesApi.getSharedActivity();
		String rsaKey = LunaServicesApi.getConfigString("googlePlayPublicKey");

		helper = new IabHelper(activity, rsaKey);
		helper.enableDebugLogging(true);

		LunaServicesApi.addActivityListener(activityListener);
	}

	private static void queryInventory(final String[] productIds)
	{
		try
		{
			helper.queryInventoryAsync(true, Arrays.asList(productIds), null, queryInventoryListener);
		}
		catch(IabHelper.IabAsyncInProgressException e)
		{
			e.printStackTrace();
		}
	}
    
    // Fetch products info from server
    public static void fetchProducts(final String[] productIds)
    {
        if(helper == null || inventory != null) return;

		if(!helper.isSetupDone())
		{
			helper.startSetup(new IabHelper.OnIabSetupFinishedListener()
			{
				@Override
				public void onIabSetupFinished(IabResult result)
				{
					if (result.isFailure())
					{
						Log.e(LunaServicesApi.getLogTag(), "Problem setting up in-app billing: " + result);
						return;
					}

					queryInventory(productIds);
				}
			});

			return;
		}

		LunaServicesApi.runInUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				queryInventory(productIds);
			}
		});
    }

    // Purchase product with given id
    public static void purchaseProduct(String productId)
    {
        if(inventory == null)
        {
            Log.e(LunaServicesApi.getLogTag(), "Products info not fetched from server");
            return;
        }

        if(!inventory.hasDetails(productId))
        {
            Log.e(LunaServicesApi.getLogTag(), "Product \"" + productId + "\" not found");
            return;
        }

        try
        {
            Activity activity = LunaServicesApi.getSharedActivity();
            helper.launchPurchaseFlow(activity, productId, RC_REQUEST, purchaseListener);
        }
        catch(IabHelper.IabAsyncInProgressException e)
        {
            e.printStackTrace();
        }
    }

    // Restore purchased products
    public static void restoreProducts()
    {
        if(inventory == null)
        {
            Log.e(LunaServicesApi.getLogTag(), "Products info not fetched from server");
            return;
        }

        for(String productId : inventory.getAllSkus())
        {
            if(inventory.hasPurchase(productId)) onProductPurchased(productId);
        }
    }

    private static native void onFetchProducts(String[] productIds);

    private static native void onProductPurchased(String productId);

    private static IabHelper.QueryInventoryFinishedListener queryInventoryListener = new IabHelper.QueryInventoryFinishedListener()
    {
        @Override
        public void onQueryInventoryFinished(IabResult result, final Inventory inventory)
        {
            if(helper == null) return;

            if(result.isFailure())
            {
                Log.e(LunaServicesApi.getLogTag(), "Failed to query inventory: " + result);
                return;
            }

            LunaPurchases.inventory = inventory;

			LunaServicesApi.runInRenderThread(new Runnable()
			{
				@Override
				public void run()
				{
					List<String> productIds = inventory.getAllSkus();
					onFetchProducts(productIds.toArray(new String[productIds.size()]));
				}
			});
        }
    };

    private static IabHelper.OnIabPurchaseFinishedListener purchaseListener = new IabHelper.OnIabPurchaseFinishedListener()
    {
        @Override
        public void onIabPurchaseFinished(IabResult result, final Purchase info)
        {
            if(helper == null) return;

            if(result.isFailure())
            {
                Log.e(LunaServicesApi.getLogTag(), "Error purchasing: " + result);
                return;
            }

			LunaServicesApi.runInRenderThread(new Runnable()
			{
				@Override
				public void run()
				{
					onProductPurchased(info.getSku());
				}
			});
        }
    };

    private static LunaActivityListener activityListener = new LunaActivityListener()
    {
        @Override
        public void onStart(Activity activity) {}

        @Override
        public void onResume(Activity activity) {}

        @Override
        public void onPause(Activity activity) {}

        @Override
        public void onStop(Activity activity) {}

        @Override
        public void onDestroy(Activity activity)
        {
            if(helper == null) return;

            try
            {
                helper.dispose();
            }
            catch(IabHelper.IabAsyncInProgressException e)
            {
                e.printStackTrace();
            }

            helper = null;
            inventory = null;
        }

        @Override
        public boolean onBackPressed(Activity acitivity) { return false; }

        @Override
        public void onNetworkStateChanged(Activity acitivity, boolean connected) {}

        @Override
        public boolean onActivityResult(int requestCode, int resultCode, Intent intent)
        {
            return helper.handleActivityResult(requestCode, resultCode, intent);
        }
    };
}

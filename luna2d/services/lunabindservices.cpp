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

#include "lunabindservices.h"
#include "lunaservices.h"
#include "lunaads.h"
#include "lunapurchases.h"
#include "lunasharing.h"
#include "lunastore.h"
#include "lunaleaderboards.h"
#include "lunalua.h"

using namespace luna2d;

// Bind "luna.ads" module
static void BindAds(const std::shared_ptr<LUNAAds>& ads, LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblAds(lua);
	tblLuna.SetField("ads", tblAds);
	
	tblAds.SetField("getPhysicalBannerHeight", LuaFunction(lua, ads.get(), &LUNAAds::GetPhysicalBannerHeight));
	tblAds.SetField("getBannerHeight", LuaFunction(lua, ads.get(), &LUNAAds::GetBannerHeight));
	tblAds.SetField("isBannerReady", LuaFunction(lua, ads.get(), &LUNAAds::IsBannerReady));
	tblAds.SetField("isInterstitialReady", LuaFunction(lua, ads.get(), &LUNAAds::IsInterstitalReady));
	tblAds.SetField("isRewardedVideoReady", LuaFunction(lua, ads.get(), &LUNAAds::IsRewardedVideoReady));
	tblAds.SetField("showBanner", LuaFunction(lua, ads.get(), &LUNAAds::ShowBanner));
	tblAds.SetField("showInterstitial", LuaFunction(lua, ads.get(), &LUNAAds::ShowInterstital));
	tblAds.SetField("showRewardedVideo", LuaFunction(lua, ads.get(), &LUNAAds::ShowRewardedVideo));
}

// Bind "luna.purchases" module
static void BindPurchases(const std::shared_ptr<LUNAPurchases>& purchases, LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblPurchases(lua);
	tblLuna.SetField("purchases", tblPurchases);

	tblPurchases.SetField("fetchProducts", LuaFunction(lua, purchases.get(), &LUNAPurchases::FetchProducts));
	tblPurchases.SetField("purchaseProduct", LuaFunction(lua, purchases.get(), &LUNAPurchases::PurchaseProduct));
	tblPurchases.SetField("restoreProducts", LuaFunction(lua, purchases.get(), &LUNAPurchases::RestoreProducts));
}

// Bind "luna.sharing" module
static void BindSharing(const std::shared_ptr<LUNASharing>& sharing, LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblShare(lua);
	tblLuna.SetField("share", tblShare);

	tblShare.SetField("text", LuaFunction(lua, sharing.get(), &LUNASharing::Text));
	tblShare.SetField("image", LuaFunction(lua, sharing.get(), &LUNASharing::Image));
}

// Bind "luna.store" module
static void BindStore(const std::shared_ptr<LUNAStore>& store, LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblStore(lua);
	tblLuna.SetField("store", tblStore);

	tblStore.SetField("getUrl", LuaFunction(lua, store.get(), &LUNAStore::GetUrl));
	tblStore.SetField("openPage", LuaFunction(lua, store.get(), &LUNAStore::OpenPage));
}

// Bind "luna.leaderboards" module
static void BindLeaderboards(const std::shared_ptr<LUNALeaderboards>& leaderboards, LuaScript* lua, LuaTable& tblLuna)
{
	LuaTable tblLeaderboards(lua);
	tblLuna.SetField("leaderboards", tblLeaderboards);

	tblLeaderboards.SetField("submitScore", LuaFunction(lua, leaderboards.get(), &LUNALeaderboards::SubmitScore));
	tblLeaderboards.SetField("open", LuaFunction(lua, leaderboards.get(), &LUNALeaderboards::Open));
}

void luna2d::BindServices()
{
	auto lua = LUNAEngine::SharedLua();
	auto tblLuna = lua->GetGlobalTable().GetTable("luna");
	auto services = LUNAEngine::SharedServices();

	BindAds(services->GetAds(), lua, tblLuna);
	BindPurchases(services->GetPurchases(), lua, tblLuna);
	BindSharing(services->GetSharing(), lua, tblLuna);
	BindStore(services->GetStore(), lua, tblLuna);
	BindLeaderboards(services->GetLeaderboards(), lua, tblLuna);
}

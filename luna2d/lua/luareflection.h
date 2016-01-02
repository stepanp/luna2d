//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2016 Stepan Prokofjev
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

#pragma once

#include "luatable.h"

// VS2013 not supports constexpr 
#if LUNA_PLATFORM == LUNA_PLATFORM_WP
	#define constexpr	
#endif

//-------------------------------------------------------------------------
// Macro for simple lua reflection and safe-cast
// Any classes binding to lua via LuaClass<T>
// must have LUNA_USERDATA macro in begin of class declaration
// (or LUNA_USERDATA_DERIVED macro for derived from already binded classes)
//-------------------------------------------------------------------------

#define LUNA_USERDATA(cls) \
	static_assert(std::is_class<cls>::value, "Type \"" #cls "\" is must be a class"); \
	protected: \
		/* Ref to shared_ptr passed to lua */ \
		std::shared_ptr<LuaWeakRef> _ref = std::make_shared<LuaWeakRef>(nil); \
	public: \
		void _SetLuaRef(const std::shared_ptr<LuaWeakRef>& ref) { _ref = ref; } \
		std::shared_ptr<LuaWeakRef> _GetLuaRef() { return _ref; } \
		\
		/* Forced release shared_ptr passed to Lua */ \
		/* i.e. userdata object in lua will still exists, but references nothing */ \
		/* and it will can be safety deleted by GC */ \
		void _KillLuaRef() \
		{ \
			if(!_ref || *_ref == nil) return; \
			\
			lua_State* luaVm = _ref->GetLuaVm(); \
			LuaStack<LuaWeakRef*>::Push(luaVm, _ref.get()); \
			std::shared_ptr<cls>* ptr = *static_cast<std::shared_ptr<cls>**>(lua_touserdata(luaVm, -1)); \
			lua_pop(luaVm, 1); \
			\
			ptr->reset(); \
			_ref->Release(); \
		} \
		\
		/* Unique id of type */ \
		/* Using for safe-cast between lua userdata and C++ object */ \
		/* NOTE: Specific value of id may me different on different source versions */ \
		/* SEE: "luaptr.h" */ \
		static int _GetTypeId() { return __COUNTER__; } \
		\
		/* Get name of userdata class\baseclass */ \
		/* Using for getting metatable for userdata in runtime */ \
		/* SEE: "luaclass.h", "luaptr.h" */ \
		static const char* _GetTypeName() { return #cls; } \
		static const char* _GetBaseTypeName() { return nullptr; } \
		\
		constexpr static bool _AssertIsBaseOf() { return true; } \
	private: \

#define LUNA_USERDATA_DERIVED(basecls, cls) \
	static_assert(std::is_class<basecls>::value && std::is_class<cls>::value, \
		"Types \"" #basecls "\" and \"" #cls "\" are must be a classes"); \
	public:  \
		static int _GetTypeId() { return __COUNTER__; } \
		\
		static const char* _GetTypeName() { return #cls; } \
		static const char* _GetBaseTypeName() { return basecls::_GetTypeName(); } \
		\
		constexpr static bool _AssertIsBaseOf() \
		{ \
			static_assert(std::is_base_of<basecls,cls>::value, "\"" #basecls "\" is not base class of \"" #cls "\""); \
			return true; \
		} \
	private:

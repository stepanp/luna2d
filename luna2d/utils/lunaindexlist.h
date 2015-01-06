//------------------------------------------------------------
// luna2d engine
// lunaindexlist.h
// Implementation of "indices trick"
// Using to get index of unpacked element of variadic template
// Copyright 2014 Stepan Prokofjev
//------------------------------------------------------------

#pragma once

namespace luna2d{

template<size_t ... Index>
struct LUNAIndexList {};

template<size_t N, size_t ... Index>
struct _MakeIndexList : _MakeIndexList<N - 1, N - 1, Index ...> {};

template<size_t ... Index>
struct _MakeIndexList<0, Index ...> : LUNAIndexList<Index ...> {};

template<typename ... T>
using LUNAMakeIndexList = _MakeIndexList<sizeof...(T)>;

}

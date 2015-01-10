//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2014 Stepan Prokofjev
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

//---------------------------------
// Recursively log all table fields
//---------------------------------
#define LUNA_LUA_LOG_TABLE \
"luna.log.table = function(table, prefix) \
	prefix = prefix or '' \
\
	for k,v in pairs(table) do \
		if type(v) == 'table' and v ~= table then  \
			luna.log(prefix .. '[' .. tostring(k) .. '] = table') \
			luna.log.table(v, prefix .. '  ') \
		else \
			luna.log(prefix .. '[' .. tostring(k) .. '] = ' .. tostring(v)) \
		end \
	end \
\
	local meta = getmetatable(table) \
	if meta and meta.__index and meta.__index ~= table and type(meta.__index) == 'table' then \
		luna.log.table(meta.__index) \
	end \
end"

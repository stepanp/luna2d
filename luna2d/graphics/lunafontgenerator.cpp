//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2015 Stepan Prokofjev
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

#include "lunafontgenerator.h"

using namespace luna2d;

LUNAFontGenerator::~LUNAFontGenerator()
{
	if(face) FT_Done_Face(face);
	if(library) FT_Done_FreeType(library);
}

bool LUNAFontGenerator::Load(const std::string& filename, LUNAFileLocation location)
{
	FT_Error error = FT_Init_FreeType(&library);
	if(error) return false;

	auto buffer = LUNAEngine::SharedFiles()->ReadFile(filename, location);
	error = FT_New_Memory_Face(library, &buffer[0], buffer.size(), 0, &face);
	if(error) return false;





	return true;
}

std::shared_ptr<LUNAFont> LUNAFontGenerator::CreateFont(int size)
{
	return nullptr;
}


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

#include "lunatexture.h"
#include "lunaglm.h"

// Default attribute names
#define LUNA_ATTRIBUTE_POSITION "a_position"
#define LUNA_ATTRIBUTE_COLOR "a_color"
#define LUNA_ATTRIBUTE_TEX_COORDS "a_texCoords"

namespace luna2d{

class LUNAShader
{
public:
	LUNAShader(const char *vertexSource, const char *fragmentSource);
	~LUNAShader();

private:
	GLuint program;
	GLint a_position, a_color, a_texCoords, u_transformMatrix, u_texture; // Default attributes

private:
	GLuint LoadShader(GLenum shaderType, const char *source); // Load and compile shader
	GLuint CreateProgram(const char *vertexSource, const char *fragmentSource); // Link vertex and fragment shaders to shader program
	void FetchDefaultAttributes();

public:
	void SetPositionAttribute(float *vertexArray);
	void SetColorAttribute(float *vertexArray);
	void SetTexCoordsAttribute(float *vertexArray);
	void SetTransformMatrix(const glm::mat4& matrix);
	void SetTextureUniform(LUNATexture *texture);

	void Bind();
	void Unbind();

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
public:
	inline void Reload(const char *vertexSource, const char *fragmentSource)
	{
		program = CreateProgram(vertexSource, fragmentSource);
		FetchDefaultAttributes();
	}
#endif
};

}

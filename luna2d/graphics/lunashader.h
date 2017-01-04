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

#pragma once

#include "lunatexture.h"
#include "lunaglm.h"

namespace luna2d{

class LUNAShader : public LUNAAsset
{
	LUNA_USERDATA_DERIVED(LUNAAsset, LUNAShader)

public:
	LUNAShader(const std::string& vertexSource, const std::string& fragmentSource);
	virtual ~LUNAShader();

private:
	GLuint program = 0;
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;

	// Default attributes
	GLint a_position = -1;
	GLint a_color = -1;
	GLint a_texCoords = -1;
	GLint u_transformMatrix = -1;
	GLint u_texture = -1;

private:
	// Load and compile shader
	GLuint LoadShader(GLenum shaderType, const std::string& source);

	// Link vertex and fragment shaders into shader program
	void CreateGlProgram(const std::string& vertexSource, const std::string& fragmentSource);

	// Fetch default attributes and uniforms
	void FetchDefaultAttributes();

	// Add default preprocessor directives to vertex shader source
	std::string PreprocessVertex(const std::string& source);

	// Add default preprocessor directives to fragment shader source
	std::string PreprocessFragment(const std::string& source);

public:
	bool IsValid();
	bool HasColorAttribute();
	bool HasTexture();
	void SetPositionAttribute(const std::vector<float>& vertexArray);
	void SetColorAttribute(const std::vector<float>& vertexArray);
	void SetTexCoordsAttribute(const std::vector<float>& vertexArray);
	void SetTransformMatrix(const glm::mat4& matrix);
	void SetTextureUniform(const LUNATexture& texture);

	void Bind();
	void Unbind();

#if LUNA_PLATFORM == LUNA_PLATFORM_ANDROID
private:
	std::string reloadPath;

public:
	inline void SetReloadPath(const std::string& path)
	{
		reloadPath = path;
		LUNAEngine::SharedAssets()->SetAssetReloadable(this, true);
	}

	inline virtual void Reload()
	{
		if(reloadPath.empty()) return;

		auto files = LUNAEngine::SharedFiles();
		std::string vertexSource = files->ReadFileToString(reloadPath);
		std::string fragmentSource = files->ReadFileToString(files->ReplaceExtension(reloadPath, "frag"));

		if(!vertexSource.empty() && !fragmentSource.empty())
		{
			CreateGlProgram(vertexSource, fragmentSource);
			FetchDefaultAttributes();

			if(IsValid()) return;
		}

		LUNA_LOGE("Cannot reload shader from path \"%s\"", reloadPath.c_str());
	}

	inline void Reload(const std::string& vertexSource, const std::string& fragmentSource)
	{
		CreateGlProgram(vertexSource, fragmentSource);
		FetchDefaultAttributes();
	}

#endif
};

}

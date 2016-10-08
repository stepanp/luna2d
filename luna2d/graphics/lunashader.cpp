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

#include "lunashader.h"
#include "lunaplatform.h"
#include "lunarenderer.h"

using namespace luna2d;

LUNAShader::LUNAShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	CreateProgram(vertexSource, fragmentSource);
	FetchDefaultAttributes();
}

LUNAShader::~LUNAShader()
{
	if(!program) return;

	Unbind();
	glDetachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);
}

// Load and compile shader
GLuint LUNAShader::LoadShader(GLenum shaderType, const std::string& source)
{
	GLuint shader = glCreateShader(shaderType);

	if(shader)
	{
		const char* cSource = source.c_str();
		glShaderSource(shader, 1, &cSource, nullptr);
		glCompileShader(shader);

		GLint compileStatus = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

		// Print error log
		if(!compileStatus)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if(infoLen)
			{
				std::string info;
				info.reserve(infoLen);
				glGetShaderInfoLog(shader, infoLen, nullptr, &info[0]);
				LUNA_LOGE("Could not compile shader %s:\n%s",
					shaderType == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : "GL_FRAGMENT_SHADER", info.c_str());
			}

			glDeleteShader(shader);
			shader = 0;
		}
	}

	return shader;
}

// Link vertex and fragment shaders into shader program
void LUNAShader::CreateProgram(const std::string& vertexSource, const std::string& fragmentSource)
{
	vertexShader = LoadShader(GL_VERTEX_SHADER, vertexSource);
	if(!vertexShader) return;

	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentSource);
	if(!fragmentShader) return;

	program = glCreateProgram();

	if(program)
	{
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

		// Print error log
		if(!linkStatus)
		{
			GLint infoLen = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
			if(infoLen)
			{
				std::string info;
				info.reserve(infoLen);
				glGetProgramInfoLog(program, infoLen, nullptr, &info[0]);
				LUNA_LOGE("Could not link program:\n%s", info.c_str());
			}

			glDeleteProgram(program);
			vertexShader = 0;
			fragmentShader = 0;
			program = 0;
		}
	}
}

// Fetch default attributes and uniforms
void LUNAShader::FetchDefaultAttributes()
{
	a_position = glGetAttribLocation(program, "a_position");
	a_color = glGetAttribLocation(program, "a_color");
	a_texCoords = glGetAttribLocation(program, "a_texCoords");
	u_transformMatrix = glGetUniformLocation(program, "u_transformMatrix");
	u_texture = glGetUniformLocation(program, "u_texture");
}

bool LUNAShader::IsValid()
{
	return glIsProgram(program);
}

bool LUNAShader::HasColorAttribute()
{
	return a_color != -1;
}

bool LUNAShader::HasTexture()
{
	return u_texture != -1 && a_texCoords != -1;
}

void LUNAShader::Bind()
{
	glUseProgram(program);
}

void LUNAShader::Unbind()
{
	glUseProgram(0);
}

void LUNAShader::SetPositionAttribute(const std::vector<float>& vertexArray)
{
	glEnableVertexAttribArray(a_position);
	glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, RENDER_ELEMENT_PER_VERTEX * sizeof(float), &vertexArray[0]);
}

void LUNAShader::SetColorAttribute(const std::vector<float>& vertexArray)
{
	if(!HasColorAttribute()) return;

	glEnableVertexAttribArray(a_color);
	glVertexAttribPointer(a_color, 4, GL_FLOAT, GL_FALSE, RENDER_ELEMENT_PER_VERTEX * sizeof(float), &vertexArray[0] + 2);
}

void LUNAShader::SetTexCoordsAttribute(const std::vector<float>& vertexArray)
{
	if(!HasTexture()) return;

	glEnableVertexAttribArray(a_texCoords);
	glVertexAttribPointer(a_texCoords, 2, GL_FLOAT, GL_FALSE, RENDER_ELEMENT_PER_VERTEX * sizeof(float), &vertexArray[0] + 6);
}

void LUNAShader::SetTransformMatrix(const glm::mat4& matrix)
{
	glUniformMatrix4fv(u_transformMatrix, 1, GL_FALSE, &matrix[0][0]);
}

void LUNAShader::SetTextureUniform(const LUNATexture& texture)
{
	if(!HasTexture()) return;

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();
	glUniform1i(u_texture, 0);
}

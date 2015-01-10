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

#include "lunashader.h"
#include "lunaplatform.h"
#include "lunarenderer.h"

using namespace luna2d;

LUNAShader::LUNAShader(const char *vertexSource, const char *fragmentSource)
{
	program = CreateProgram(vertexSource, fragmentSource);
	FetchDefaultAttributes();
}

LUNAShader::~LUNAShader()
{
	//glDeleteShader(program);
}

// Load and compile shader
GLuint LUNAShader::LoadShader(GLenum shaderType, const char *source)
{
	GLuint shader = glCreateShader(shaderType);

	if(shader)
	{
		glShaderSource(shader, 1, &source, 0);
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

// Link vertex and fragment shaders to shader program
GLuint LUNAShader::CreateProgram(const char *vertexSource, const char *fragmentSource)
{
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertexSource);
	if(!vertexShader) return 0;

	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentSource);
	if(!fragmentShader) return 0;

	GLuint program = glCreateProgram();
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
			program = 0;
		}
	}

	return program;
}

void LUNAShader::FetchDefaultAttributes()
{
	a_position = glGetAttribLocation(program, "a_position");
	a_color = glGetAttribLocation(program, "a_color");
	a_texCoords = glGetAttribLocation(program, "a_texCoords");
	u_transformMatrix = glGetUniformLocation(program, "u_transformMatrix");
	u_texture = glGetUniformLocation(program, "u_texture");
}

void LUNAShader::Bind()
{
	glUseProgram(program);
}

void LUNAShader::Unbind()
{
	glUseProgram(0);
}

void LUNAShader::SetPositionAttribute(float *vertexArray)
{
	glEnableVertexAttribArray(a_position);
	glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, LUNA_ELEMENT_PER_VERTEX * sizeof(float), vertexArray);
}

void LUNAShader::SetColorAttribute(float *vertexArray)
{
	if(a_color == -1) return;

	glEnableVertexAttribArray(a_color);
	glVertexAttribPointer(a_color, 4, GL_FLOAT, GL_FALSE, LUNA_ELEMENT_PER_VERTEX * sizeof(float), vertexArray + 2);
}

void LUNAShader::SetTexCoordsAttribute(float *vertexArray)
{
	if(a_texCoords == -1) return;

	glEnableVertexAttribArray(a_texCoords);
	glVertexAttribPointer(a_texCoords, 2, GL_FLOAT, GL_FALSE, LUNA_ELEMENT_PER_VERTEX * sizeof(float), vertexArray + 6);
}

void LUNAShader::SetTransformMatrix(const glm::mat4& matrix)
{
	glUniformMatrix4fv(u_transformMatrix, 1, GL_FALSE, &matrix[0][0]);
}

void LUNAShader::SetTextureUniform(LUNATexture *texture)
{
	if(u_texture == -1) return;

	glActiveTexture(GL_TEXTURE0);
	texture->Bind();
	glUniform1i(u_texture, 0);
}

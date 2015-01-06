//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014 Stepan Prokofjev
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

//-------------------
// Qt OpenGL includes
//-------------------
#include <QtOpenGL>
#include <QOpenGLFunctions>

namespace luna2d{ namespace LUNAQtGl{

extern QOpenGLFunctions* functions;

void InitFunctions();

}}

#define glBindTexture luna2d::LUNAQtGl::functions->glBindTexture
#define glBlendFunc luna2d::LUNAQtGl::functions->glBlendFunc
#define glClear luna2d::LUNAQtGl::functions->glClear
#define glClearColor luna2d::LUNAQtGl::functions->glClearColor
#define glClearStencil luna2d::LUNAQtGl::functions->glClearStencil
#define glColorMask luna2d::LUNAQtGl::functions->glColorMask
#define glCopyTexImage2D luna2d::LUNAQtGl::functions->glCopyTexImage2D
#define glCopyTexSubImage2D luna2d::LUNAQtGl::functions->glCopyTexSubImage2D
#define glCullFace luna2d::LUNAQtGl::functions->glCullFace
#define glDeleteTextures luna2d::LUNAQtGl::functions->glDeleteTextures
#define glDepthFunc luna2d::LUNAQtGl::functions->glDepthFunc
#define glDepthMask luna2d::LUNAQtGl::functions->glDepthMask
#define glDisable luna2d::LUNAQtGl::functions->glDisable
#define glDrawArrays luna2d::LUNAQtGl::functions->glDrawArrays
#define glDrawElements luna2d::LUNAQtGl::functions->glDrawElements
#define glEnable luna2d::LUNAQtGl::functions->glEnable
#define glFinish luna2d::LUNAQtGl::functions->glFinish
#define glFlush luna2d::LUNAQtGl::functions->glFlush
#define glFrontFace luna2d::LUNAQtGl::functions->glFrontFace
#define glGenTextures luna2d::LUNAQtGl::functions->glGenTextures
#define glGetBooleanv luna2d::LUNAQtGl::functions->glGetBooleanv
#define glGetError luna2d::LUNAQtGl::functions->glGetError
#define glGetFloatv luna2d::LUNAQtGl::functions->glGetFloatv
#define glGetIntegerv luna2d::LUNAQtGl::functions->glGetIntegerv
#define glGetString luna2d::LUNAQtGl::functions->glGetString
#define glGetTexParameterfv luna2d::LUNAQtGl::functions->glGetTexParameterfv
#define glGetTexParameteriv luna2d::LUNAQtGl::functions->glGetTexParameteriv
#define glHint luna2d::LUNAQtGl::functions->glHint
#define glIsEnabled luna2d::LUNAQtGl::functions->glIsEnabled
#define glIsTexture luna2d::LUNAQtGl::functions->glIsTexture
#define glLineWidth luna2d::LUNAQtGl::functions->glLineWidth
#define glPixelStorei luna2d::LUNAQtGl::functions->glPixelStorei
#define glPolygonOffset luna2d::LUNAQtGl::functions->glPolygonOffset
#define glReadPixels luna2d::LUNAQtGl::functions->glReadPixels
#define glScissor luna2d::LUNAQtGl::functions->glScissor
#define glStencilFunc luna2d::LUNAQtGl::functions->glStencilFunc
#define glStencilMask luna2d::LUNAQtGl::functions->glStencilMask
#define glStencilOp luna2d::LUNAQtGl::functions->glStencilOp
#define glTexImage2D luna2d::LUNAQtGl::functions->glTexImage2D
#define glTexParameterf luna2d::LUNAQtGl::functions->glTexParameterf
#define glTexParameterfv luna2d::LUNAQtGl::functions->glTexParameterfv
#define glTexParameteri luna2d::LUNAQtGl::functions->glTexParameteri
#define glTexParameteriv luna2d::LUNAQtGl::functions->glTexParameteriv
#define glTexSubImage2D luna2d::LUNAQtGl::functions->glTexSubImage2D
#define glViewport luna2d::LUNAQtGl::functions->glViewport
#define glActiveTexture luna2d::LUNAQtGl::functions->glActiveTexture
#define glAttachShader luna2d::LUNAQtGl::functions->glAttachShader
#define glBindAttribLocation luna2d::LUNAQtGl::functions->glBindAttribLocation
#define glBindBuffer luna2d::LUNAQtGl::functions->glBindBuffer
#define glBindFramebuffer luna2d::LUNAQtGl::functions->glBindFramebuffer
#define glBindRenderbuffer luna2d::LUNAQtGl::functions->glBindRenderbuffer
#define glBlendColor luna2d::LUNAQtGl::functions->glBlendColor
#define glBlendEquation luna2d::LUNAQtGl::functions->glBlendEquation
#define glBlendEquationSeparate luna2d::LUNAQtGl::functions->glBlendEquationSeparate
#define glBlendFuncSeparate luna2d::LUNAQtGl::functions->glBlendFuncSeparate
#define glBufferData luna2d::LUNAQtGl::functions->glBufferData
#define glBufferSubData luna2d::LUNAQtGl::functions->glBufferSubData
#define glCheckFramebufferStatus luna2d::LUNAQtGl::functions->glCheckFramebufferStatus
#define glClearDepthf luna2d::LUNAQtGl::functions->glClearDepthf
#define glCompileShader luna2d::LUNAQtGl::functions->glCompileShader
#define glCompressedTexImage2D luna2d::LUNAQtGl::functions->glCompressedTexImage2D
#define glCompressedTexSubImage2D luna2d::LUNAQtGl::functions->glCompressedTexSubImage2D
#define glCreateProgram luna2d::LUNAQtGl::functions->glCreateProgram
#define glCreateShader luna2d::LUNAQtGl::functions->glCreateShader
#define glDeleteBuffers luna2d::LUNAQtGl::functions->glDeleteBuffers
#define glDeleteFramebuffers luna2d::LUNAQtGl::functions->glDeleteFramebuffers
#define glDeleteProgram luna2d::LUNAQtGl::functions->glDeleteProgram
#define glDeleteRenderbuffers luna2d::LUNAQtGl::functions->glDeleteRenderbuffers
#define glDeleteShader luna2d::LUNAQtGl::functions->glDeleteShader
#define glDepthRangef luna2d::LUNAQtGl::functions->glDepthRangef
#define glDetachShader luna2d::LUNAQtGl::functions->glDetachShader
#define glDisableVertexAttribArray luna2d::LUNAQtGl::functions->glDisableVertexAttribArray
#define glEnableVertexAttribArray luna2d::LUNAQtGl::functions->glEnableVertexAttribArray
#define glFramebufferRenderbuffer luna2d::LUNAQtGl::functions->glFramebufferRenderbuffer
#define glFramebufferTexture2D luna2d::LUNAQtGl::functions->glFramebufferTexture2D
#define glGenBuffers luna2d::LUNAQtGl::functions->glGenBuffers
#define glGenerateMipmap luna2d::LUNAQtGl::functions->glGenerateMipmap
#define glGenFramebuffers luna2d::LUNAQtGl::functions->glGenFramebuffers
#define glGenRenderbuffers luna2d::LUNAQtGl::functions->glGenRenderbuffers
#define glGetActiveAttrib luna2d::LUNAQtGl::functions->glGetActiveAttrib
#define glGetActiveUniform luna2d::LUNAQtGl::functions->glGetActiveUniform
#define glGetAttachedShaders luna2d::LUNAQtGl::functions->glGetAttachedShaders
#define glGetAttribLocation luna2d::LUNAQtGl::functions->glGetAttribLocation
#define glGetBufferParameteriv luna2d::LUNAQtGl::functions->glGetBufferParameteriv
#define glGetFramebufferAttachmentParameteriv luna2d::LUNAQtGl::functions->glGetFramebufferAttachmentParameteriv
#define glGetProgramiv luna2d::LUNAQtGl::functions->glGetProgramiv
#define glGetProgramInfoLog luna2d::LUNAQtGl::functions->glGetProgramInfoLog
#define glGetRenderbufferParameteriv luna2d::LUNAQtGl::functions->glGetRenderbufferParameteriv
#define glGetShaderiv luna2d::LUNAQtGl::functions->glGetShaderiv
#define glGetShaderInfoLog luna2d::LUNAQtGl::functions->glGetShaderInfoLog
#define glGetShaderPrecisionFormat luna2d::LUNAQtGl::functions->glGetShaderPrecisionFormat
#define glGetShaderSource luna2d::LUNAQtGl::functions->glGetShaderSource
#define glGetUniformfv luna2d::LUNAQtGl::functions->glGetUniformfv
#define glGetUniformiv luna2d::LUNAQtGl::functions->glGetUniformiv
#define glGetUniformLocation luna2d::LUNAQtGl::functions->glGetUniformLocation
#define glGetVertexAttribfv luna2d::LUNAQtGl::functions->glGetVertexAttribfv
#define glGetVertexAttribiv luna2d::LUNAQtGl::functions->glGetVertexAttribiv
#define glGetVertexAttribPointerv luna2d::LUNAQtGl::functions->glGetVertexAttribPointerv
#define glIsBuffer luna2d::LUNAQtGl::functions->glIsBuffer
#define glIsFramebuffer luna2d::LUNAQtGl::functions->glIsFramebuffer
#define glIsProgram luna2d::LUNAQtGl::functions->glIsProgram
#define glIsRenderbuffer luna2d::LUNAQtGl::functions->glIsRenderbuffer
#define glIsShader luna2d::LUNAQtGl::functions->glIsShader
#define glLinkProgram luna2d::LUNAQtGl::functions->glLinkProgram
#define glReleaseShaderCompiler luna2d::LUNAQtGl::functions->glReleaseShaderCompiler
#define glRenderbufferStorage luna2d::LUNAQtGl::functions->glRenderbufferStorage
#define glSampleCoverage luna2d::LUNAQtGl::functions->glSampleCoverage
#define glShaderBinary luna2d::LUNAQtGl::functions->glShaderBinary
#define glShaderSource luna2d::LUNAQtGl::functions->glShaderSource
#define glStencilFuncSeparate luna2d::LUNAQtGl::functions->glStencilFuncSeparate
#define glStencilMaskSeparate luna2d::LUNAQtGl::functions->glStencilMaskSeparate
#define glStencilOpSeparate luna2d::LUNAQtGl::functions->glStencilOpSeparate
#define glUniform1f luna2d::LUNAQtGl::functions->glUniform1f
#define glUniform1fv luna2d::LUNAQtGl::functions->glUniform1fv
#define glUniform1i luna2d::LUNAQtGl::functions->glUniform1i
#define glUniform1iv luna2d::LUNAQtGl::functions->glUniform1iv
#define glUniform2f luna2d::LUNAQtGl::functions->glUniform2f
#define glUniform2fv luna2d::LUNAQtGl::functions->glUniform2fv
#define glUniform2i luna2d::LUNAQtGl::functions->glUniform2i
#define glUniform2iv luna2d::LUNAQtGl::functions->glUniform2iv
#define glUniform3f luna2d::LUNAQtGl::functions->glUniform3f
#define glUniform3fv luna2d::LUNAQtGl::functions->glUniform3fv
#define glUniform3i luna2d::LUNAQtGl::functions->glUniform3i
#define glUniform3iv luna2d::LUNAQtGl::functions->glUniform3iv
#define glUniform4f luna2d::LUNAQtGl::functions->glUniform4f
#define glUniform4fv luna2d::LUNAQtGl::functions->glUniform4fv
#define glUniform4i luna2d::LUNAQtGl::functions->glUniform4i
#define glUniform4iv luna2d::LUNAQtGl::functions->glUniform4iv
#define glUniformMatrix2fv luna2d::LUNAQtGl::functions->glUniformMatrix2fv
#define glUniformMatrix3fv luna2d::LUNAQtGl::functions->glUniformMatrix3fv
#define glUniformMatrix4fv luna2d::LUNAQtGl::functions->glUniformMatrix4fv
#define glUseProgram luna2d::LUNAQtGl::functions->glUseProgram
#define glValidateProgram luna2d::LUNAQtGl::functions->glValidateProgram
#define glVertexAttrib1f luna2d::LUNAQtGl::functions->glVertexAttrib1f
#define glVertexAttrib1fv luna2d::LUNAQtGl::functions->glVertexAttrib1fv
#define glVertexAttrib2f luna2d::LUNAQtGl::functions->glVertexAttrib2f
#define glVertexAttrib2fv luna2d::LUNAQtGl::functions->glVertexAttrib2fv
#define glVertexAttrib3f luna2d::LUNAQtGl::functions->glVertexAttrib3f
#define glVertexAttrib3fv luna2d::LUNAQtGl::functions->glVertexAttrib3fv
#define glVertexAttrib4f luna2d::LUNAQtGl::functions->glVertexAttrib4f
#define glVertexAttrib4fv luna2d::LUNAQtGl::functions->glVertexAttrib4fv
#define glVertexAttribPointer luna2d::LUNAQtGl::functions->glVertexAttribPointer
#define glTexLevelParameteriv luna2d::LUNAQtGl::functions->glTexLevelParameteriv

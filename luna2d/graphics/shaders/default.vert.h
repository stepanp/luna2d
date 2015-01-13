//---------------------------------------
// luna2d engine
// default.vert.h - Default vertex shader
// Copyright 2014-2015 Stepan Prokofjev
//---------------------------------------

#pragma once

#define LUNA_DEFAULT_VERT_SHADER \
"uniform mat4 u_transformMatrix; \n\
\
attribute vec4 a_position; \n\
attribute vec4 a_color; \n\
attribute vec2 a_texCoords; \n\
\
varying vec4 v_color; \n\
varying vec2 v_texCoords; \n\
\
void main() \n\
{ \n\
	v_color = a_color; \n\
	v_texCoords = a_texCoords; \n\
	gl_Position = u_transformMatrix * a_position; \n\
}"

//-----------------------------------------
// luna2d engine
// default.frag.h - Default fragment shader
// Copyright 2014 Stepan Prokofjev
//-----------------------------------------

#pragma once

#define LUNA_DEFAULT_FRAG_SHADER \
"#ifdef GL_ES \n\
precision mediump float; \n\
#define LOWP lowp \n\
#else \n\
#define LOWP \n\
#endif \n\
\
uniform sampler2D u_texture; \n\
\
varying LOWP vec4 v_color; \n\
varying vec2 v_texCoords; \n\
\
void main() \n\
{ \n\
	gl_FragColor = v_color * texture2D(u_texture, v_texCoords); \n\
}"


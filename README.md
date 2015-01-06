luna2d
======

Cross-platform game engine for mobile platforms. Engine is aimed to write all game logic in Lua. But general features written in native code. Uses OpenGL ES 2.0 \ OpenGL 2.1 on mobile\desktop platofrms.

## Supported platforms
* Android 2.3+
* iOS 6.0+
* Desktop Qt platforms (But tested only on Windows)

## Build requirements
* Windows: Qt 5.4+, compiler with support C++11 (e.g. MinGW 4.8)
* Android: NDK r9+
* iOS: XCode, iOS SDK 8.1+

## Tools
All tools written in C++\Qt and supports same desktop platforms as engine.

#### Emulator
Emulator for useful testing games on desktop. Can run games in different screen resolutions. 

#### Pipeline
Tool for automatic preparing graphical assets. Includes image resizer and texture packer. 
Supports PSD using **[libqpsd](https://github.com/Code-ReaQtor/libqpsd)**. libqpsd binaries for Windows can be found in **[thirdparty](luna2d/thirdparty/libqpsd)** folder.
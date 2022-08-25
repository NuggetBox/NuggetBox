// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef NUGGETBOX_PCH
#define NUGGETBOX_PCH

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// add headers that you want to pre-compile here
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <fstream>
#include <WinUser.h>
#include <array>
#include <cassert>
#include <functional>
#include <memory>
#include <unordered_map>
#include <wrl.h>
#include <memory>
#include <filesystem>
#include <iostream>
#include <string>

#endif //NUGGETBOX_PCH
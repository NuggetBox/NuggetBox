workspace "NuggetBox"
	startproject "ModelViewer"
	architecture "x64"

	configurations {
		"Debug",
		"Release"
	}

include "Source/ModelViewer"
include "Source/GraphicsEngine"
include "Source/Utility"
include "External"
workspace "NuggetBox"
	startproject "ModelViewer"
	architecture "x64"

	configurations {
		"Debug",
		"Release"
	}

include "ModelViewer"
include "GraphicsEngine"
include "Utility"
include "External"
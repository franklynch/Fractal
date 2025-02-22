-- premake5.lua
workspace "Fractal"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Fractal"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "Fractal"
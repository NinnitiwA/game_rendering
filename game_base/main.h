#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <assert.h>



#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>
#include <DXGI.h>

#pragma warning(pop)



#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "dxgi.lib")


//#define SCREEN_WIDTH	(1280)			// ウインドウの幅
//#define SCREEN_HEIGHT	(720)			// ウインドウの高さ
#define SCREEN_WIDTH	(1366)			// ウインドウの幅
#define SCREEN_HEIGHT	(768)			// ウインドウの高さ


HWND GetWindow();


//[ImGui]
#include "imgui\\imgui.h"
#include "imgui\\imgui_impl_win32.h"
#include "imgui\\imgui_impl_dx11.h"
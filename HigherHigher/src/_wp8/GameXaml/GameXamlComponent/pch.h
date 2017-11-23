#pragma once
#pragma comment( lib, "xaudio2.lib")
#pragma comment( lib, "mfplat.lib")
#include <wrl\wrappers\corewrappers.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include "../SourceCPP/Utils.h"
#if _DEBUG
#define PDEBUG Utils::Loga
#else
#define PDEBUG(...)
#endif
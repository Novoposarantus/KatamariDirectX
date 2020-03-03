#pragma once


#ifdef _DEBUG
#ifdef _WIN64
	#define ShaderFolder() std::wstring shaderFolder = L"";  if(IsDebuggerPresent() == TRUE) { shaderFolder = L"..\\x64\\Debug\\"; };
#else
	#define ShaderFolder() std::wstring shaderFolder = L""; if(IsDebuggerPresent() == TRUE) { shaderFolder = L"..\\Debug\\"; };
#endif
#else
#ifdef _WIN64
	#define ShaderFolder() std::wstring shaderFolder = L""; if(IsDebuggerPresent() == TRUE) { shaderFolder = L"..\\x64\\Release\\"; };
#else
	#define ShaderFolder() std::wstring shaderFolder = L""; if(IsDebuggerPresent() == TRUE) { shaderFolder = L"..\\Release\\"; };
#endif
#endif
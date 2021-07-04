#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <algorithm>

#define WINWIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#undef min
#undef max

namespace fs = std::filesystem;

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	if (!lpCmdLine) return -1;

	PathUnquoteSpacesW(lpCmdLine);

	if (CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE) != S_OK) {
		MessageBoxA(NULL, "CoInitializeEx has failed", "", MB_ICONERROR);
		return -1;
	}

	int result = 0;

	try {
		fs::path exe_path(lpCmdLine);

		if (!fs::exists(exe_path)) return -1;

		auto rule_name = "_Allow - " + exe_path.string();

		ShellExecuteA(NULL, "runas", "cmd.exe",
			("/c netsh advfirewall firewall add rule "
				"name=\"" + rule_name + "\" dir=out program=\"" + exe_path.string() + "\" profile=any action=allow").c_str(),
			NULL, SW_SHOWNORMAL);

		MessageBoxA(NULL, "successfully added the rule", "", MB_ICONINFORMATION);
	} catch (std::exception& ex) {
		MessageBoxA(NULL, ex.what(), "", MB_ICONERROR);
		result = -1;
	}

	CoUninitialize();

	return result;
}
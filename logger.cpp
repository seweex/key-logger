#include "logger.h"

logger* logger::_ActiveLoggerPtr = nullptr;

logger::logger() :
	_Hook				{ 0 },
	_KeyboardLayout		{ 0 },
	_KeyCallback		{ 0 } {}
logger::~logger()
{
	disuse();
}

LRESULT logger::hookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	logger* curr = logger::_ActiveLoggerPtr;
	if (curr)
	{
		if (nCode >= 0 and wParam == WM_KEYDOWN and curr->_KeyCallback) 
		{
			HWND frgWindowHandle = GetForegroundWindow();
			DWORD frgWindowPID = GetWindowThreadProcessId(frgWindowHandle, 0);

			KBDLLHOOKSTRUCT kbStruct = *(KBDLLHOOKSTRUCT*)lParam;
			HKL kbl = GetKeyboardLayout(frgWindowPID);
			WCHAR winTitle[128];
			GetWindowTextW(frgWindowHandle, winTitle, 128);

			curr->_KeyCallback(kbStruct, kbl, winTitle);
		}

		return CallNextHookEx(curr->_Hook, nCode, wParam, lParam);
	}

	return 0;
}

void logger::bindKeyCallback(KCBCK _Fn) noexcept
{
	_KeyCallback = _Fn;
}

int logger::use()
{
	logger::_ActiveLoggerPtr = this;
	return !(_Hook = SetWindowsHookExW(WH_KEYBOARD_LL, this->hookCallback, 0, 0));
}

void logger::disuse()
{
	logger::_ActiveLoggerPtr = nullptr;
	UnhookWindowsHookEx(_Hook);
}
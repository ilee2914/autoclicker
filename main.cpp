#include <windows.h>
#include <winuser.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

void displayHelp() {
	cout << "------------------Help-----------------\n"
		 << "Press CTRL and LEFT CLICK to double click"
		 << "\n------------------Help-----------------" << endl;
}

void record(fstream &file) {
	file.close();
	file.open("myclicks.txt", fstream::in | fstream::out | fstream::trunc);

	cout << "\n\nPress SHIFT and LEFT CLICK to record left click" << endl;
	cout << "Press CTRL and Q to quit recording" << endl;
	cout << "Press CTRL and H to display help" << endl;
	POINT loc;
	while (true) {
		GetCursorPos(&loc);
		if (GetAsyncKeyState(VK_LSHIFT) && GetAsyncKeyState(VK_LBUTTON)) {
			cout << loc.x << " " << loc.y << " " << "l" << endl;
			file << loc.x << " " << loc.y << " " << "l" << endl;
			Sleep(250);
		} 
		else if (GetAsyncKeyState(VK_LCONTROL)) {
			if (GetKeyState('Q') < 0) {
				return;
			}
			else if (GetKeyState('H') < 0) {
				displayHelp();
			} else if (GetAsyncKeyState(VK_LBUTTON)) {
				cout << loc.x << " " << loc.y << " " << "d" << endl;
				file << loc.x << " " << loc.y << " " << "d" << endl; //WM_LBUTTONDBLCLK
			}
			Sleep(250);
		}

	}
}

void createSettings(fstream &settings) {
	settings.close();
	settings.open("settings.ini", fstream::in | fstream::out | fstream::trunc);

	cout << "\n\nCreating settings" << endl;
	int times = 0;
	int delay = 0;
	cout << "Times to loop? ";
	cin >> times;
	cout << "Delay? (in ms) ";
	cin >> delay;
	settings << times << " " << delay;
}

void click(fstream &input, fstream &settings) {
	cout << "\n\nPress CTRL and D to start program" << endl;
	cout << "Press CTRL and Q to quit run" << endl;
	while (true) {
		if (!(GetAsyncKeyState(VK_LCONTROL) && GetKeyState('D') < 0)) {
			continue;
		} 
		int times = 0;
		int delay = 0;
		int dTime = GetDoubleClickTime();
		settings >> times >> delay;
		HWND hnd = FindWindow(NULL, TEXT("MapleStory"));
		SetForegroundWindow(hnd);
		SetActiveWindow(hnd);
		for (int i = 0; i < times; i++) {
			int x, y;
			char type;
			while (!input.eof()) {
				if ((GetAsyncKeyState(VK_LCONTROL) && GetKeyState('Q') < 0)) {
					return;
				}
				input >> x >> y >> type;
				SetActiveWindow(hnd);
				cout << x << " " << y << " " << type << endl;
				
				//SetCursorPos(x, y);
				//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				
				INPUT Inputs[3] = { 0 };

				int cx = x * 65535 / 1920;
				int cy = y * 65535 / 1080;
				if (x < 0 || x > 1920 || y < 0 || y > 1080) continue;

				Inputs[0].type = INPUT_MOUSE;
				Inputs[0].mi.dx = cx; // desired X coordinate
				Inputs[0].mi.dy = cy; // desired Y coordinate
				Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

				Inputs[1].type = INPUT_MOUSE;
				Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

				Inputs[2].type = INPUT_MOUSE;
				Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
				if (type == 'd') {
					SendInput(3, Inputs, sizeof(INPUT));
					Sleep(dTime / 4);
				}

				SendInput(3, Inputs, sizeof(INPUT));

				Sleep(delay);
			}
			input.clear();
			input.seekg(0, ios::beg);
		}
		break;
	}

	settings.clear();
	settings.seekg(0, ios::beg);
}

int displayMenu() {
	int value;
	cout << GetDoubleClickTime();
	cout << "What would you like to do?" << endl
		<< "1. Record new script" << endl
		<< "2. Play a script" << endl
		<< "3. Create new settings" << endl
		<< "4. Quit program" << endl;
	cin >> value;
	return value;
}
int main() {
	fstream input("myclicks.txt", fstream::in | fstream::out);
	fstream settings("settings.ini", fstream::in | fstream::out);

	if (!input || !settings) {
		cout << "File could not be opened" << endl;
		system("pause");
		exit(0);
	}
	while (true) {
		int val = displayMenu();
		if (val == 1) {
			record(input);
			cout << "\n" << endl;
		}
		else if (val == 2) {
			click(input, settings);
			cout << "\n" << endl;
		}
		else if (val == 3) {
			createSettings(settings);
			cout << "\n" << endl;
		} 
		else {
			break;
		}

	}
	cout << "Done";
	input.close();
	settings.close();
	system("PAUSE");
}
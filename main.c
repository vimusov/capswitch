/*
    capswitch - A tiny utility that switches keyboard layout on CapsLock.

    Copyright (C) 2017,2018 Vadim Kuznetsov <vimusov@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <windows.h>

HHOOK hook = NULL;

LRESULT CALLBACK on_keyboard_event(int nCode, WPARAM wParam, LPARAM lParam)
{
    do {
        if (nCode != HC_ACTION)
            break;

        if (((KBDLLHOOKSTRUCT*) lParam)->vkCode != VK_CAPITAL)
            break;

        if (wParam != WM_KEYDOWN)
            return 1;

        HWND hWnd = GetForegroundWindow();

        if (hWnd)
            PostMessage(hWnd, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM) HKL_NEXT);

        return 1;

    } while (0);

    return CallNextHookEx(hook, nCode, wParam, lParam);
}

void start()
{
    MSG msg;

    hook = SetWindowsHookEx(WH_KEYBOARD_LL, on_keyboard_event, GetModuleHandle(0), 0);

    if (!hook)
        ExitProcess(1);

    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hook);

    ExitProcess(0);
}

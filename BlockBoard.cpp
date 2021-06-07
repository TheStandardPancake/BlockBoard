/**
*Copyright 2021 Boyd Kirkman
*
*This file is part of BlockBoard.
*
*    BlockBoard is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    BlockBoard is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with BlockBoard.  If not, see <https://www.gnu.org/licenses/>.
*/

//This is going to be the client interactive application for making a post and transfering funds.


/*
TODO:
-P2P connection network
-Block creating --> binding both posts and transactions
-Time stamp server
-Code formating features for posts
-mining system --> creating a tool for mining
-creating a tool for viewing posts
*/

#include <windows.h>

#define HOME 1
#define SEND_PP 2
#define RECIEVE_PP 3
#define VIEW_POSTS 4
#define CREATE_POSTS 5
#define VIEW_BAL_TRANS 6
#define SETTINGS 7
#define SUBMIT_KEY 8

int windowState = 0;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

//Handling the menu
void menu(HWND);
HMENU hMenu;

//Handle logining in and key collection
void login(HWND);
HWND hKey;
wchar_t key[100];

//dealing with other pages
//void homePage(HWND);



//window creation stuff
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)SETTINGS;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"WindowClass";
    wc.lpfnWndProc = WindowProcedure;


    if(!RegisterClassW(&wc))
    {
        return -1;
    }

    CreateWindowW(L"WindowClass",L"Block Board", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 900, 650, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    while(GetMessage(&msg,NULL,NULL,NULL)) //This is the windows process messaging loop
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    {
    case WM_COMMAND: //this will handle when a menu item is pressed
        switch(wp)
        {
        case HOME:
            windowState = HOME;
            break;
        case SEND_PP:
            windowState = SEND_PP;
            break;
        case RECIEVE_PP:
            windowState = RECIEVE_PP;
            break;
        case VIEW_POSTS:
            windowState = VIEW_POSTS;
            break;
        case CREATE_POSTS:
            windowState = CREATE_POSTS;
            break;
        case VIEW_BAL_TRANS:
            windowState = VIEW_BAL_TRANS;
            break;
        case SETTINGS:
            windowState = SETTINGS;
            break;
        case SUBMIT_KEY:
            GetWindowTextW(hKey, key, 100);
            windowState = HOME;
            menu(hWnd); // setup the menu
            break;
        }
        break;
    case WM_CREATE: //this is where the gui elements are built on the window
        switch(windowState)
        {
        case HOME:
            menu(hWnd); // setup the menu
            //homePage(hWnd);
            break;
        case SEND_PP:
            menu(hWnd); // setup the menu
            break;
        case RECIEVE_PP:
            menu(hWnd); // setup the menu
            break;
        case VIEW_POSTS:
            menu(hWnd); // setup the menu
            break;
        case CREATE_POSTS:
            menu(hWnd); // setup the menu
            break;
        case VIEW_BAL_TRANS:
            menu(hWnd); // setup the menu
            break;
        case SETTINGS:
            menu(hWnd); // setup the menu
            break;
        default:
            login(hWnd);
        }

        break;
    case WM_DESTROY: //This handles when the window is closed
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp); //handles any messages that are not utalised with the switch
    }
}

void menu(HWND hWnd) //setting up of the menu
{
    hMenu = CreateMenu();
    AppendMenu(hMenu, MF_STRING, HOME, "Home");
    AppendMenu(hMenu, MF_STRING, SEND_PP, "Send PP");
    AppendMenu(hMenu, MF_STRING, RECIEVE_PP, "Recieve PP");
    AppendMenu(hMenu, MF_STRING, VIEW_POSTS, "View Posts");
    AppendMenu(hMenu, MF_STRING, CREATE_POSTS, "Create Posts");
    AppendMenu(hMenu, MF_STRING, VIEW_BAL_TRANS, "View Balance and Transactions");
    AppendMenu(hMenu, MF_STRING, SETTINGS, "Settings");

    SetMenu(hWnd, hMenu);
}

void login(HWND hWnd)
{
    CreateWindowW(L"Static", L"Enter Your Key:", WS_VISIBLE | WS_CHILD | SS_CENTER, 400, 200, 100, 50, hWnd, NULL, NULL, NULL);
    hKey = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_PASSWORD, 325, 250, 250, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Button", L"Submit", WS_VISIBLE | WS_CHILD | SS_CENTER, 413, 275, 75, 25, hWnd, (HMENU)SUBMIT_KEY, NULL, NULL);
}

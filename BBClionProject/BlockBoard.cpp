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
-create an asymetric encyption scheme with the entered key
-link the tcp functionalities
-create a function to browse the post data inside of the block chain
-figure out what to put in settings
*/

#include <windows.h>
#include "picosha2.h"
//#include "p2p.h"  // Temporarily not included as there are some bugs to fix so as to allow compile !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <string>

// The following are all translating integer assignments into meaningful and readable commands for use in checking what the current state of the window is and what buttons/actions have taken place
#define HOME 1
#define SEND_PP 2
#define RECIEVE_PP 3
#define VIEW_POSTS 4
#define CREATE_POSTS 5
#define VIEW_BAL_TRANS 6
#define SETTINGS 7
#define SUBMIT_KEY 8
#define START_P2P 9

int windowState = 0;
int WIDTH = 900;
int HEIGHT = 650;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

//Handling the menu
void menu(HWND);
HMENU hMenu;

//Handle the different pages (These are the prototype functions
void login(HWND);
void homePage(HWND);
void send(HWND);
void recieve(HWND);
void view(HWND);
void create(HWND);
void viewBal(HWND);
void settings(HWND);

//handle key collection
HWND hKey;
wchar_t lKey[100];
std::string sKey;
std::string key;

//other misc. prototype functions
BOOL CALLBACK DestoryChildCallback(HWND, LPARAM);
std::string wchar2string(wchar_t*);
const wchar_t* string2wchar(std::string);



//window creation stuff
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)3;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"WindowClass";
    wc.lpfnWndProc = WindowProcedure;


    if(!RegisterClassW(&wc))
    {
        return -1;
    }

    CreateWindowW(L"WindowClass",L"Block Board", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, WIDTH, HEIGHT, NULL, NULL, NULL, NULL);

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
    case WM_COMMAND: //this will handle when a menu item is pressed ----------------------------<<
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
            GetWindowTextW(hKey, lKey, 100); // collect the raw key submitted and save in the variable named "lKey"
            std::string sKey = wchar2string(lKey);
            key = picosha2::hash256_hex_string(sKey); // convert the key to a SHA256 hash
            windowState = HOME;
            menu(hWnd); // setup the menu
            UpdateWindow(hWnd);
            break;
        }
        break;
    case WM_CREATE: //this is when the window is first created --------------------------------------<<
        login(hWnd);
        UpdateWindow(hWnd);
        break;
    case WM_PAINT: // this controls what is painted to the window -----------------------------------<<
        switch(windowState)
        {
        case HOME:
            EnumChildWindows(hWnd, DestoryChildCallback, NULL); //clear all child windows
            homePage(hWnd);
            windowState = 0;
            break;
        case SEND_PP:
            EnumChildWindows(hWnd, DestoryChildCallback, NULL); //clear all child windows
            send(hWnd);
            windowState = 0;
            break;
        case RECIEVE_PP:
            EnumChildWindows(hWnd, DestoryChildCallback, NULL); //clear all child windows
            recieve(hWnd);
            windowState = 0;
            break;
        case VIEW_POSTS:
            EnumChildWindows(hWnd, DestoryChildCallback, NULL); //clear all child windows
            view(hWnd);
            windowState = 0;
            break;
        case CREATE_POSTS:
            EnumChildWindows(hWnd, DestoryChildCallback, NULL); //clear all child windows
            create(hWnd);
            windowState = 0;
            break;
        case VIEW_BAL_TRANS:
            EnumChildWindows(hWnd, DestoryChildCallback, NULL); //clear all child windows
            viewBal(hWnd);
            windowState = 0;
            break;
        case SETTINGS:
            EnumChildWindows(hWnd, DestoryChildCallback, NULL); //clear all child windows
            settings(hWnd);
            windowState = 0;
            break;
        }
        UpdateWindow(hWnd);
        break;
    case WM_DESTROY: //This handles when the window is closed ---------------------------------<
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp); //handles any messages that are not utalised with the switch
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Functions that handle painting the different windows etc. ---------------------------------------<

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

void login(HWND hWnd) //login page
{
    CreateWindowW(L"Static", L"Enter Your Key:", WS_VISIBLE | WS_CHILD | SS_CENTER, (WIDTH/2)-50, 50, 100, 35, hWnd, NULL, NULL, NULL);
    hKey = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_PASSWORD, (WIDTH/2)-125, 250, 250, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Button", L"Submit", WS_VISIBLE | WS_CHILD | SS_CENTER, (WIDTH/2)-38, 275, 76, 25, hWnd, (HMENU)SUBMIT_KEY, NULL, NULL);
}

void homePage(HWND hWnd) //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Home page>>>
{
    CreateWindowW(L"Static", L"Welcome to the home page!", WS_VISIBLE | WS_CHILD | SS_CENTER, (WIDTH/2)-150, 25, 300, 19, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Button", L"Start P2P", WS_VISIBLE | WS_CHILD | SS_CENTER, (WIDTH/2)-38, 275, 76, 25, hWnd, (HMENU)START_P2P, NULL, NULL);
}

void send(HWND hWnd) //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Make transaction page>>>
{
    CreateWindowW(L"Static", L"Send Post Points!", WS_VISIBLE | WS_CHILD | SS_CENTER, (WIDTH/2)-100, 25, 200, 19, hWnd, NULL, NULL, NULL);
}

void recieve(HWND hWnd) //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Address information page>>>
{
    CreateWindowW(L"Static", L"Recieve Post Points at the Address Below!", WS_VISIBLE | WS_CHILD | SS_CENTER, (WIDTH/2)-250, 25, 500, 19, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Edit", string2wchar("PPADD256"+key), WS_VISIBLE | WS_CHILD | SS_CENTER, (WIDTH/2)-300, 200, 600, 19, hWnd, NULL, NULL, NULL);
}

void view(HWND hWnd) //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Post viewing page>>>
{
    CreateWindowW(L"Static", L"View Posts Here!", WS_VISIBLE | WS_CHILD | SS_CENTER, (WIDTH/2)-100, 25, 200, 19, hWnd, NULL, NULL, NULL);
}

void create(HWND hWnd) //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Post creation page>>>
{
    CreateWindowW(L"Static", L"Create Posts Here!", WS_VISIBLE | WS_CHILD | SS_CENTER, (WIDTH/2)-100, 25, 200, 19, hWnd, NULL, NULL, NULL);
}

void viewBal(HWND hWnd) //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~View balance and transactions page>>>
{
    CreateWindowW(L"Static", L"Your Current Balance and Past transactions", WS_VISIBLE | WS_CHILD | SS_CENTER, 250, 25, 400, 19, hWnd, NULL, NULL, NULL);
}

void settings(HWND hWnd) //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Settings page>>>
{
    CreateWindowW(L"Static", L"Settings", WS_VISIBLE | WS_CHILD | SS_CENTER, 400, 25, 100, 19, hWnd, NULL, NULL, NULL);
}

//------------------Useful function for clearing windows etc------------------<<

BOOL CALLBACK DestoryChildCallback(HWND hwnd, LPARAM lParam)
{
    if (hwnd != NULL)
    {
        DestroyWindow(hwnd);
    }

    return TRUE;
}

//----------------------------------------------Cryptographic hashing / string manipualtion functions---------------------<<

std::string wchar2string(wchar_t* str) // converting wchar_t to a string
{
    std::string mystring;
    while(*str)
    {
        mystring += (char)*str++;
    }
    return  mystring;
}

std::wstring string2wstring(std::string str)
{
    std::wstring wstr;
    for(int i = 0; i < str.length(); ++i)
        wstr += wchar_t( str[i] );
    return wstr;
}

const wchar_t* string2wchar(std::string str) // converting a string to a wchar_t
{
    std::wstring wstr = string2wstring(str);
    const wchar_t* wchar = wstr.c_str();
    return wchar;
}


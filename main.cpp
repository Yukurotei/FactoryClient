#include <winsock2.h>
#include <iostream>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#include "main.h"

#include "java/objects/launchWrapper.h"
#include "java/classes/minecraft.h"

#include "modules/module.cpp"
#include "modules/module.h"
#include "settings/settings.cpp"
#include "settings/settings.h"
#include "event/events.h"

#include "util/socket/socketServer.cpp"

#pragma comment(lib, "ws2_32")

static WNDPROC window_proc;
const CHAR * WindowName = "Minecraft* 1.21";
HWND window = NULL;

bool running = true;

LRESULT CALLBACK h_WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_KEYDOWN)
    {
        std::cout << "Key press ran " << (char)wParam << std::endl;
        FireGlobalKeyPress((char)wParam);
    }
    return CallWindowProc(window_proc, hWnd, uMsg, wParam, lParam);
}

void InitWndProc()
{
    window = FindWindowA(NULL, WindowName);
    if (!window)
    {
        printf("Window not found, failed to setup wndproc(Inject in title screen)\n");
        return;
    }
    printf("Window found\n");
    window_proc = (WNDPROC)SetWindowLongPtrA(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(h_WndProc));
}

void FactoryClient_Main(HMODULE hModule)
{
    //Mandatory init
    InitializeWinsock();
    InitWndProc();
    jobject loader = GetFabricLoader(env);
    jobject mcInstance = GetMinecraftInstance(env);
    //Module
    Rocket *rocketModule = new Rocket("Rocket", 'F');
    //rocketModule->enabled = false; //Make sure
    modules.push_back(rocketModule);
    FastPlace *fastPlace = new FastPlace("FastPlace", 'R');
    modules.push_back(fastPlace);
    TestModule *testModule = new TestModule("TestModule", 'O'); //The keybind actually don't do anything
    modules.push_back(testModule);
    //Socket
    SOCKET listenSocket = CreateListenSocket();
    SOCKET clientSocket = Accept(listenSocket);

    auto f = HandleClientConnectionAsyncly(clientSocket);

    printf("Executing main loop\n");
    //Main loop
    while (running)
    {
        FireClientTickEvent();


        //TODO: replace w module
        if (GetAsyncKeyState(VK_END))
        {
            break;
        }
        Sleep(10); //100 tick per sec
    }

    printf("Detaching Factory Client...\n");
    if (window != NULL)
    {
        SetWindowLongPtrA(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(window_proc));
    }
    SendDataToClient("fServer2Client-ShutdownGUI", listenSocket, clientSocket);
    while (stopFromDetaching)
    {
        if (!stopFromDetaching)
        {
            break;
        }
    }
    CloseSocket(listenSocket);
    CleanupWinsock();
    
    printf("Detached Factory Client\n");
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return;
}

void SetRunning(bool value)
{
    running = value;
}
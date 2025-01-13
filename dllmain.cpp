#include <winsock2.h>
#include "Windows.h"
#include <iostream>

#include "main.h"
#include "jni/jni.h"

FILE* pfile = nullptr;

JavaVM* vm;
JNIEnv* env = NULL;  

void init(HMODULE hModule){
    AllocConsole();
    freopen_s(&pfile, "CONOUT$", "w", stdout);

    printf("FactoryClient Attatched, injecting...\n");

    jsize vmCount;
    if (JNI_GetCreatedJavaVMs(&vm, 1, &vmCount) != JNI_OK || vmCount == 0)
    {
        printf("JVM not found\n");
        return;
    }
    jint res = vm->GetEnv((void**)&env, JNI_VERSION_21);
    if (res == JNI_EDETACHED)
    {
        res = vm->AttachCurrentThread((void**)&env, nullptr);
    }

    if (res != JNI_OK)
    {
        printf("Failed to setup JNI\n");
        return;
    }
    if (env != nullptr)
    {
        printf("JNI initialized\n");
        printf("FactoryClient Injected\n");
        FactoryClient_Main(hModule);
    }
}



BOOL APIENTRY DllMain(HMODULE module, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch(ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, module, 0, 0);
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
#include <winsock2.h>
#include <Windows.h>
#include <iostream>

#include "jni/jni.h"

//#define long long long

extern JavaVM* vm;
extern JNIEnv* env;

extern void FactoryClient_Main(HMODULE hModule);
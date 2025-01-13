#pragma once
#include "../../jni/jni.h"
#include "../classes/minecraft.h"

jobject GetPlayer()
{
    jfieldID playerField = env->GetFieldID(GetMinecraftClass(), "field_1724", "Lnet/minecraft/class_746;");
    if (playerField == NULL)
    {
        printf("Error(GetPlayer): Getting player failed(Player field ID doesn't exist)\n");
        return NULL;
    }
    jobject player = env->GetObjectField(GetMinecraftInstance(env), playerField);
    if (player == NULL)
    {
        printf("ERROR(GetPlayer): No player found, is the fields correct?\n");
        env->DeleteLocalRef(player);
        return NULL;
    }
    return player;
}

void player_jump()
{
    jfieldID playerField = env->GetFieldID(GetMinecraftClass(), "field_1724", "Lnet/minecraft/class_746;");
    if (playerField == NULL)
    {
        printf("Error(player_jump): Getting player failed(Player field ID doesn't exist)\n");
        return;
    }
    jobject player = env->GetObjectField(GetMinecraftInstance(env), playerField);
    if (player == NULL)
    {
        printf("ERROR(player_jump): No player found, is the fields correct?\n");
        env->DeleteLocalRef(player);
        return;
    }
    jclass playerClass = env->GetObjectClass(player);

    //Call jump
    jmethodID jumpMethod = env->GetMethodID(playerClass, "method_6043", "()V");
    env->CallVoidMethod(player, jumpMethod);

    //Free mem
    env->DeleteLocalRef(player);
    env->DeleteLocalRef(playerClass);
}

double GetPlayerLastX()
{
    jobject player = GetPlayer();
    jclass playerClass = env->GetObjectClass(player);

    //Get lastX field
    jfieldID lastXField = env->GetFieldID(playerClass, "field_3926", "D");
    if (lastXField == NULL)
    {
        printf("Error(GetPlayerLastX): lastXField is NULL\n");
        return 0;
    }
    double returnValue = env->GetDoubleField(player, lastXField);
    env->DeleteLocalRef(player);
    env->DeleteLocalRef(playerClass);
    return returnValue;
}

double GetPlayerLastY()
{
    jobject player = GetPlayer();
    jclass playerClass = env->GetObjectClass(player);

    //Get lastY field
    jfieldID lastYField = env->GetFieldID(playerClass, "field_3940", "D");
    if (lastYField == NULL)
    {
        printf("Error(GetPlayerLastY): lastYField is NULL\n");
        return 0;
    }
    double returnValue = env->GetDoubleField(player, lastYField);
    env->DeleteLocalRef(player);
    env->DeleteLocalRef(playerClass);
    return returnValue;
}

double GetPlayerLastZ()
{
    jobject player = GetPlayer();
    jclass playerClass = env->GetObjectClass(player);

    //Get lastZ field
    jfieldID lastZField = env->GetFieldID(playerClass, "field_3924", "D");
    if (lastZField == NULL)
    {
        printf("Error(GetPlayerLastZ): lastZField is NULL\n");
        return 0;
    }
    double returnValue = env->GetDoubleField(player, lastZField);
    env->DeleteLocalRef(player);
    env->DeleteLocalRef(playerClass);
    return returnValue;
}
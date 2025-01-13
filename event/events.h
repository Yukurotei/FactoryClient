#pragma once

#include "../modules/module.h"

void FireClientTickEvent()
{
    FireGlobalClientTick();
}

void FireKeyPressEvent(int keyCode)
{
    FireGlobalKeyPress(keyCode);
}
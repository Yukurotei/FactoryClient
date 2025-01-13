#include "../settings/settings.h"
#include "../event/events.h"
#include "module.h"
#include "../main.h"
#include <vector>

#include "../java/functions/playerFunctions.h"

std::vector<Module*> modules;

//Test module

void registerSetting(Setting* setting, std::string moduleName){
    std::string mapName = moduleName + "-=-" + setting->name;
    std::map<std::string, Setting*> sMap = getSettingsMap();
    settingsMap[mapName] = setting;
}

void Rocket::init()
{
    enabled = false;
}

void Rocket::onEnable()
{

}

void Rocket::onDisable()
{

}

void Rocket::toggle()
{
    enabled = !enabled;
    if (enabled)
    {
        onEnable();
    } else {
        onDisable();
    }
}

void Rocket::onClientTick()
{
    if (enabled)
    {
        player_jump();
    }
}

void Rocket::onKeyPress(int pressedKeyCode)
{
    if ((char)pressedKeyCode == keyCode)
    {
        printf("Rocket toggled\n");
        toggle();
    }
}

//Rclicker

void FastPlace::init()
{
    enabled = false;
    ConvertableValue cVal = ConvertableValue();
    cVal.insertIntValue(0, false);
    IntSetting* rClickDelay = new IntSetting("delay", "delay of fastplace", cVal);
    registerSetting(rClickDelay, this->name);
}

void FastPlace::onEnable()
{

}

void FastPlace::onDisable()
{

}

void FastPlace::toggle()
{
    enabled = !enabled;
    if (enabled)
    {
        onEnable();
    } else {
        onDisable();
    }
}

void FastPlace::onClientTick()
{
    int delay = ((IntSetting*)GetSettingInModule(this, "delay"))->getCurrentValue();
    if (enabled)
    {
        std::cout << delay << std::endl;
        SetRightClickDelay(delay);
    }
}

void FastPlace::onKeyPress(int pressedKeyCode)
{
    if ((char)pressedKeyCode == keyCode)
    {
        printf("FastPlace toggled\n");
        toggle();
    }
}

//Test module

void TestModule::init()
{
    enabled = false;
}

void TestModule::toggle()
{
    enabled = !enabled;
    if (enabled)
    {
        onEnable();
    } else {
        onDisable();
    }
}

void TestModule::onEnable()
{

}

void TestModule::onDisable()
{

}

void TestModule::onClientTick()
{
    
}

void TestModule::onKeyPress(int pressedKeyCode)
{
    if ((char)pressedKeyCode == 'O')
    {
        printf("Running get world...\n");
        jobject world = GetWorld();
        printf("Ran get world\n");
    }
    if ((char)pressedKeyCode == 'P')
    {
        printf("Running get entity block pos...\n");
        jobject blockPos = GetEntityLastBlockPos(GetPlayer());
    }
    if ((char)pressedKeyCode == 'K')
    {
        printf("Running get entity pos...\n");
        double entityX = GetPlayerLastX();
        double entityY = GetPlayerLastY();
        double entityZ = GetPlayerLastZ();
        std::cout << "Player X: " << entityX << std::endl;
        std::cout << "Player Y: " << entityY << std::endl;
        std::cout << "Player Z: " << entityX << std::endl;
    }
}
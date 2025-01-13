#pragma once

#include "../event/events.h"
#include "../main.h"
#include <vector>
#include "../util/stringUtil.cpp"
#include "../settings/settings.h"

class Module
{
    public:
        std::string name;
        int keyCode;
        bool enabled;
        virtual void init(){}
        virtual void toggle(){}
        virtual void onEnable(){}
        virtual void onDisable(){}
        virtual void onKeyPress(int pressedKeyCode){}
        virtual void onClientTick(){}
        Module(std::string name, int key)
        {
            this->init();
            //this->settings = initSettingVector(this->settings);
            this->name = name;
            keyCode = key;
        }
};

extern std::vector<Module*> modules;

Module* GetModule(std::string name)
{
    for (int i = 0; i < modules.size(); i++)
    {
        if (toLower(modules[i]->name) == toLower(name))
        {
            return modules[i];
        }
    }
    return nullptr;
}

Setting* GetSettingInModule(Module* module, std::string name)
{
    std::map<std::string, Setting*> sMap = getSettingsMap();
    std::string settingName = module->name + "-=-" + name;
    for (auto const& [key, val] : sMap)
    {
        if (toLower(key) == toLower(settingName))
        {
            return val;
        }
    }
    return nullptr;
}

void FireGlobalKeyPress(int keyCode)
{
    for (int i = 0; i < modules.size(); i++)
    {
        modules[i]->onKeyPress(keyCode);
    }
}

void FireGlobalClientTick()
{
    for (int i = 0; i < modules.size(); i++)
    {
        modules[i]->onClientTick();
    }
}

class Rocket : public Module
{
    public:
        void init();
        void toggle();
        void onEnable();
        void onDisable();
        void onKeyPress(int pressedKeyCode);
        void onClientTick();
        Rocket(std::string name, int key) : Module(name, key)
        {
            this->init();
            //this->settings = initSettingVector(this->settings);
            this->name = name;
            keyCode = key;
        }
};

class FastPlace : public Module
{
    public:
        void init();
        void toggle();
        void onEnable();
        void onDisable();
        void onKeyPress(int pressedKeyCode);
        void onClientTick();
        FastPlace(std::string name, int key) : Module(name, key)
        {
            this->init();
            //this->settings = initSettingVector(this->settings);
            this->name = name;
            keyCode = key;
        }
};

class TestModule : public Module
{
    public:
        void init();
        void toggle();
        void onEnable();
        void onDisable();
        void onKeyPress(int pressedKeyCode);
        void onClientTick();
        TestModule(std::string name, int key) : Module(name, key)
        {
            this->init();
            //this->settings = initSettingVector(this->settings);
            this->name = name;
            keyCode = key;
        }
};
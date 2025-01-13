#pragma once

#include "../main.h"
#include "../util/stringUtil.cpp"

#include <iostream>
#include <string>
#include <vector>
#include <map>

class ConvertableValue
{
    public:
        std::string stringValue;
        int intValue;
        float floatValue;
        bool booleanValue;
        std::string originalValue;

        std::string getStringValue()
        {
            if (stringValue != "")
            {
                return stringValue;
            }
            return "";
        }

        void insertStringValue(std::string stringVal, boolean forceTryConvert)
        {
            stringValue = stringVal;
            if (isNumber(stringVal) && !isFloat(stringVal))
            {
                intValue = std::stoi(stringVal);
                floatValue = intValue;
            } else if (!isNumber(stringVal) && isFloat(stringVal)) {
                floatValue = std::stof(stringVal);
                intValue = std::abs(floatValue);
            } else if (isNumber(stringVal) && isFloat(stringVal)) {
                intValue = std::stoi(stringVal);
                floatValue = std::stof(stringVal);
            } else {
                intValue = 0;
                floatValue = 0;
            }

            if (forceTryConvert)
            {
                if (stringVal == "0")
                {
                    booleanValue = false;
                } else if (stringVal == "1")
                {
                    booleanValue = true;
                } else {
                    booleanValue = false;
                }
            } else {
                if (toLower(stringVal) != "true" && toLower(stringVal) != "false")
                {
                    booleanValue = false;
                    originalValue = "str";
                    return;
                }
            }
            if (toLower(stringVal) == "true")
            {
                booleanValue = true;
            } else if (toLower(stringVal) == "false")
            {
                booleanValue = false;
            }
            originalValue = "str";
        }

        int getIntValue()
        {
            return intValue;
        }

        void insertIntValue(int intVal, bool tryRelateBool)
        {
            intValue = intVal;
            stringValue = std::to_string(intVal);
            floatValue = intVal;
            if (tryRelateBool)
            {
                if (intValue == 0)
                {
                    booleanValue = false;
                } else if (intValue == 1)
                {
                    booleanValue = true;
                } else {
                    booleanValue = false;
                }
            } else {
                booleanValue = false;
            }
            originalValue = "int";
        }

        float getFloatValue()
        {
            return floatValue;
        }

        void insertFloatValue(float floatVal, bool tryRelateBool)
        {
            floatValue = floatVal;
            intValue = std::abs(floatVal);
            stringValue = std::to_string(floatVal);
            //Simple so copied from setInt
            if (tryRelateBool)
            {
                if (intValue == 0)
                {
                    booleanValue = false;
                } else if (intValue == 1)
                {
                    booleanValue = true;
                } else {
                    booleanValue = false;
                }
            } else {
                booleanValue = false;
            }
            originalValue = "flo";
        }

        boolean getBooleanValue()
        {
            return booleanValue;
        }

        void insertBooleanValue(bool booleanVal)
        {
            booleanValue = booleanVal;
            double result;
            if (booleanVal == true)
            {
                result = 1;
            } else {
                result = 0;
            }
            intValue = result;
            floatValue = result;
            stringValue = std::to_string(result);
            originalValue = "boo";
        }
};

class Setting
{
    public:
        std::string name;
        std::string description;
        ConvertableValue value;
        auto getCurrentValue();
        void setCurrentConvertable(ConvertableValue value){}
        Setting(std::string name, std::string description, ConvertableValue value)
        {
            this->name = name;
            this->description = description;
            this->value = value;
        }
};

std::map<std::string, Setting*> settingsMap;

std::map<std::string, Setting*> getSettingsMap()
{
    return settingsMap;
}

class IntSetting : public Setting
{
    public:
        int getCurrentValue();
        void setCurrentConvertable(ConvertableValue value);
        void setCurrentInt(int value);
        IntSetting(std::string name, std::string description, ConvertableValue value) : Setting(name, description, value)
        {
            this->name = name;
            this->description = description;
            this->value = value;
        }
};

class FloatSetting : public Setting
{
    public:
        float getCurrentValue();
        void setCurrentConvertable(ConvertableValue value);
        void setCurrentFloat(float value);
        FloatSetting(std::string name, std::string description, ConvertableValue value) : Setting(name, description, value)
        {
            this->name = name;
            this->description = description;
            this->value = value;
        }
};

class ModeSetting : public Setting
{
    public:
        std::vector<std::string> modes;
        std::string defaultMode;
        std::string getCurrentMode();
        void setCurrentConvertable(ConvertableValue value);
        void setCurrentMode(std::string value);
        void cycle();
        ModeSetting(std::string name, std::string description, ConvertableValue defaultMode, std::vector<std::string> allModes) : Setting(name, description, value)
        {
            this->name = name;
            this->description = description;
            this->value = defaultMode;
            this->defaultMode = defaultMode.getStringValue();
            this->modes = allModes;
        }
};

class BoolSetting : public Setting
{
    public:
        bool getCurrentState();
        void setCurrentConvertable(ConvertableValue value);
        void setCurrentState(bool value);
        BoolSetting(std::string name, std::string description, ConvertableValue value) : Setting(name, description, value)
        {
            this->name = name;
            this->description = description;
            this->value = value;
        }
};
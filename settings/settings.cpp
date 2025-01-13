#include "settings.h"
#include "../main.h"
#include "../util/vectorUtil.cpp"

int IntSetting::getCurrentValue()
{
    return this->value.getIntValue();
}

void IntSetting::setCurrentConvertable(ConvertableValue value)
{
    this->value = value;
}

void IntSetting::setCurrentInt(int value)
{
    ConvertableValue cVal = ConvertableValue();
    cVal.insertIntValue(value, false);
    this->setCurrentConvertable(cVal);
}

float FloatSetting::getCurrentValue()
{
    return this->value.getFloatValue();
}

void FloatSetting::setCurrentConvertable(ConvertableValue value)
{
    this->value = value;
}

void FloatSetting::setCurrentFloat(float value)
{
    ConvertableValue cVal = ConvertableValue();
    cVal.insertFloatValue(value, false);
    this->setCurrentConvertable(cVal);
}

std::string ModeSetting::getCurrentMode()
{
    return this->value.getStringValue();
}

void ModeSetting::setCurrentConvertable(ConvertableValue value)
{
    this->value = value;
}

void ModeSetting::setCurrentMode(std::string stringVal)
{
    ConvertableValue cVal = ConvertableValue();
    cVal.insertStringValue(stringVal, false);
    this->setCurrentConvertable(cVal);
}

void ModeSetting::cycle()
{
    std::string nextMode = getNextElementOfStringVector(this->modes, this->value.getStringValue());
    this->setCurrentMode(nextMode);
}

void BoolSetting::setCurrentConvertable(ConvertableValue value)
{
    this->value = value;
}

void BoolSetting::setCurrentState(bool value)
{
    ConvertableValue cVal = ConvertableValue();
    cVal.insertBooleanValue(value);
    this->setCurrentConvertable(cVal);
}
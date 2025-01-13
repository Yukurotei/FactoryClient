#pragma once
#include "../main.h"

#include <string>
#include <sstream>

auto toLower(std::string text)
{
    std::string txt = text;
    for (auto& letter : txt)
    {
        letter = tolower(letter);
    }

    return txt;
}

bool isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool isFloat(std::string myString) {
    std::istringstream iss(myString);
    float f;
    iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}
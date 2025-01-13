#pragma once

#include <vector>
#include <string>

int getCurrentIndexOfStringVector(std::vector<std::string> vector, std::string element)
{
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i] == element) {
            return i;
        }
    }
    
    return -1;
}

std::string getNextElementOfStringVector(std::vector<std::string> vector, std::string currentElement) {
    int currentElementPosition = getCurrentIndexOfStringVector(vector, currentElement);
    std::string nextElement;
    if (currentElementPosition == vector.size())
    {
        nextElement = vector[0];
    } else {
        nextElement = vector[currentElementPosition + 1];
    }
    return nextElement;
}
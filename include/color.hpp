#pragma once
#include <iostream>

std::string changeColor(std::string& str) {
    return "\x1B[93m" + str + "\033[0m";  // yellow by default
}
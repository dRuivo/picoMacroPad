#pragma once
#include "macro_handling.hpp"

void loadDefaultMacros();
bool loadConfigFromFlash();
bool saveConfigToFlash();

// Extern declaration - actual definition is in macro_memory.cpp
extern MacroKey current_macros[16];
#pragma once

#include "InputEventsType.h"
#include <array>

using namespace std;

using ButtonStates = array<bool, static_cast<size_t>(InputEventsType::COUNT)>;

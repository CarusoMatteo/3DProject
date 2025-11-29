#pragma once

#include <array>
#include "InputEventsType.h"

using namespace std;

using ButtonStates = array<bool, static_cast<size_t>(InputEventsType::count)>;
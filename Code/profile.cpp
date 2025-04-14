#include "profile.h"
#include <iostream>

Profile::Profile(double basalRate, int carbohydrateRatio, int correctionFactor, int targetGlucoseLevel, string profileName)
{ // im sorry eli, but this is easir to read (for me at least)
    _basalRate = basalRate;
    _carbohydrateRatio = carbohydrateRatio;
    _correctionFactor = correctionFactor;
    _targetGlucoseLevel = targetGlucoseLevel;
    _profileName = profileName;
}

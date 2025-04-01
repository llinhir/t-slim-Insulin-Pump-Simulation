#include "profile.h"
#include <iostream>

Profile::Profile(int basalRate, int carbohydrateRatio, int correctionFactor, int targetGlucoseLevel, string profileName, string password) :
    _basalRate(basalRate),
    _carbohydrateRatio(carbohydrateRatio), 
    _correctionFactor(correctionFactor), 
    _targetGlucoseLevel(targetGlucoseLevel), 
    _profileName(profileName),
    _password(password)
{

}

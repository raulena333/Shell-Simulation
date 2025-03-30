#include "AirResistance.h"

const float AirResistance::R = 287.05;  // J/(kg·K)

float AirResistance::calculateSoundVelocity(float height)
{
    float adiabaticIndex = 1.4f; 
    float T0 = 15; // ºC at sea level
    float L = 0.0065f; // standard lapse rate
    float Tc;

    if (height < 11000.0f)
        Tc = (T0 - L * height) + 275.15f;
    else
        Tc = (-56.5f) + 273.15f; 

    return sqrt(adiabaticIndex * R * Tc);
        
}

float AirResistance::calculateMachNumber(glm::vec3 velocity, float height)
{
    return velocity.length() / calculateSoundVelocity(height);
}


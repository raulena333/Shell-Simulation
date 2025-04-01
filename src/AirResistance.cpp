#include "AirResistance.h"

const float AirResistance::R = 287.05f;   // J/(kg·K)
const float AirResistance::g0 = 9.80665f; // m/s²
const float AirResistance::T0 = 288.15f;  // Kelvin at sea level

// Computes sound velocity dynamically based on altitude
float AirResistance::calculateSoundVelocity(float height)
{
    float adiabaticIndex = 1.4f;
    float Tc = calculateTemperature(height); // Temperature at height

    return std::sqrt(adiabaticIndex * R * Tc);
}

float AirResistance::calculateTemperature(float height)
{
    if (height < 11000.0f)
        return T0 - 0.0065f * height; // Troposphere
    else if (height < 20000.0f && height >= 11000.0f)
        return 216.65f; // Tropopause (constant)
    else if (height < 32000.0f && height >= 20000.0f)
        return 216.65f + 0.001f * (height - 20000.0f); // Stratosphere
    else if (height < 47000.0f && height >= 32000.0f)
        return 228.65f + 0.0028f * (height - 32000.0f); // Upper Stratosphere
    else if (height < 51000.0f && height >= 47000.0f)
        return 270.65f; // Mesosphere (constant)
    else if (height < 71000.0f && height >= 51000.0f)
        return 270.65f - 0.0028f * (height - 51000.0f); // Upper Mesosphere
    else
        return 200.0f + 0.002f * (height - 71000.0f); // Thermosphere (approx)
}

// Computes Mach number dynamically
float AirResistance::calculateMachNumber(glm::vec3 velocity, float height)
{
    return static_cast<float>(glm::length(velocity))/ calculateSoundVelocity(height);
}

float AirResistance::calculateLiftCoefficient(float angle, glm::vec3 velocity, float height)
{
    float CL0 = 0.f; // Base lift coefficient
    float CLalpha = 2 * static_cast<float>(M_PI); // Lift curve slope for thin airfoils
    float alpha = glm::radians(angle); // Convert angle to radians

    float machNumber = calculateMachNumber(velocity, height); // Mach number at height

    float CL = CL0 + CLalpha * alpha; // Basic lift coefficient calculation without stall

    // Stall condition check
    float alpha_stall = glm::radians(15.0f);  // Stall angle (15 degrees)
    float C_L_max = 1.8f;  // Max lift coefficient (airfoil-dependent)
    if (angle > 15.0f)  // Stall correction
    {
        float k = 0.1f;
        CL = C_L_max - k * std::pow(angle - 15.0f, 2);
    }

    // Compressibility correction for Mach number
    if (machNumber > 0.7f && machNumber < 1.2f) // Transonic region
    {
        CL /= std::sqrt(1 - machNumber * machNumber);
    }
    else if (machNumber >= 1.2f) // Supersonic
    {
        CL = (4 * alpha) / std::sqrt(machNumber * machNumber - 1);
    }

    // Reynolds number correction
    CL *= (1.0f - std::exp(-calculateReynoldsNumber(velocity, height) / 1e6f));

    return glm::clamp(CL, -C_L_max, C_L_max); // Prevent unrealistic values
}

// Computes drag coefficient dynamically based on Mach number
float AirResistance::calculateDynamicDragCoefficient(float machNumber)
{
    float C_d0 = 0.295f;  // Base drag coefficient
    float k = (machNumber > 1.2f) ? 2.0f : 1.5f;

    if (machNumber < 0.8f)
        return C_d0;
    else if (machNumber < 1.2f)
        return C_d0 + k * (machNumber - 0.8f);
    else
        return C_d0 + 0.2f * k * std::log(machNumber - 1.2f + 1.0f);
}

// 🔹 Getter that updates drag coefficient dynamically
float AirResistance::getDragCoefficient(float machNumber)
{
    dragCoefficient = calculateDynamicDragCoefficient(machNumber);
    return dragCoefficient;
}

// Computes air density dynamically based on altitude
float AirResistance::calculateAirDensity(float height)
{
    float rho0 = 1.225f;  // kg/m³ at sea level
    float L = 0.0065f;    // K/m standard lapse rate
    float M = 0.0289644f; // kg/mol, molar mass of air

    float exponent = (g0 * M) / (R * L) - 1;
    return rho0 * std::pow((1 - (L * height) / T0), exponent);
}

// 🔹 Getter that updates cross-sectional area dynamically
float AirResistance::getCrossSectionalArea(float angle) const
{
    return crossSectionalArea * std::cos(angle);
}

// Computes drag force dynamically using updated values
glm::vec3 AirResistance::calculateDragForce(glm::vec3 velocity, float height, float angle)
{
    // Prevent division by zero by checking if velocity is close to zero
    if (glm::length(velocity) < 1e-3f) { // Threshold to avoid numerical errors
        return glm::vec3(0.0f);  // No drag force when stationary
    }

    float machNumber = calculateMachNumber(velocity, height);
    float dynamicDragCoefficient = getDragCoefficient(machNumber); // Updates coefficient
    float airDensity = calculateAirDensity(height);
    float effectiveCrossSectionalArea = getCrossSectionalArea(angle); // Updates area

    glm::vec3 dragDirection = -glm::normalize(velocity);  // Safe now since velocity isn't zero
    float dragMagnitude = 0.5f * dynamicDragCoefficient * airDensity * effectiveCrossSectionalArea * static_cast<float>(std::pow(glm::length(velocity), 2));

    return dragDirection * dragMagnitude;
}

// Computes terminal velocity dynamically
float AirResistance::calculateTerminalVelocity(float massProjectile, float height)
{
    float airDensityAtHeight = calculateAirDensity(height);
    
    if (airDensityAtHeight < 0.0001f)
        return std::numeric_limits<float>::infinity();

    return std::sqrt((2 * massProjectile * g0) / (getDragCoefficient(calculateMachNumber(glm::vec3(0.0f), height)) * airDensityAtHeight * crossSectionalArea));
}

float AirResistance::calculateAirViscosity(float temperature)
{
    float mu0 = 1.7894e-5f; // Pa s (dynamic viscosity at reference temperature T0)
    float C = 110.4f;       // (K) Temperature coefficient

    return mu0 * std::pow(temperature / T0, 1.5f) * (T0 + C) / (temperature + C);
}

float AirResistance::calculateReynoldsNumber(glm::vec3 velocity, float height)
{
    float airDensity = calculateAirDensity(height);
    float temperature = calculateTemperature(height);
    float airViscosity = calculateAirViscosity(temperature);
    float characteristicLength = std::sqrt(crossSectionalArea);
    float velocityMagnitude = static_cast<float>(glm::length(velocity));

    return (airDensity * velocityMagnitude * characteristicLength) / airViscosity;
}

glm::vec3 AirResistance::calculateStokesDrag(glm::vec3 velocity, float temperature)
{
    // Prevent division by zero by checking if velocity is close to zero
    if (glm::length(velocity) < 1e-3f) { // Threshold to avoid numerical errors
        return glm::vec3(0.0f);  // No stokes drag when stationary
    }

    float airViscosity = calculateAirViscosity(temperature);
    float radius = std::sqrt(crossSectionalArea / static_cast<float>(M_PI));

    float dragMagnitude = 6 * static_cast<float>(M_PI) * airViscosity * radius * static_cast<float>(glm::length(velocity));
    return -glm::normalize(velocity) * dragMagnitude;
}

glm::vec3 AirResistance::computeTotalDrag(glm::vec3 velocity, float height, float angle, float temperature)
{
    float reynoldsNumber = calculateReynoldsNumber(velocity, height);

    if (reynoldsNumber < 1000.0f)
        return calculateStokesDrag(velocity, temperature);
    else
        return calculateDragForce(velocity, height, angle);
} 

glm::vec3 AirResistance::computeWindDrag(glm::vec3 velocity, float height, float angle, glm::vec3 windVelocity)
{
    glm::vec3 relativeVelocity = velocity - windVelocity;
    float temperature = calculateTemperature(height);
    return computeTotalDrag(relativeVelocity, height, angle, temperature);
}

glm::vec3 AirResistance::calculateCoriolisForce(glm::vec3 velocity, float massProjectile)
{
    float omega = 7.2921159e-5f; // Earth's angular velocity in rad/s
    glm::vec3 coriolisForce = -2.0f * massProjectile * glm::cross(glm::vec3(0.0f, 0.0f, omega), velocity);
    return coriolisForce;
}

float AirResistance::aerodynamicHeating(glm::vec3 velocity, float height, float angle)
{
    float airDensity = calculateAirDensity(height);
    float machNumber = calculateMachNumber(velocity, height);
    float dynamicDragCoefficient = getDragCoefficient(machNumber);
    float effectiveCrossSectionalArea = getCrossSectionalArea(angle);

    return 0.5f * airDensity * static_cast<float>(std::pow(glm::length(velocity), 3)) * dynamicDragCoefficient * effectiveCrossSectionalArea;
}

glm::vec3 AirResistance::calculateBouyancyForce(float height, float volume)
{
    float airDensity = calculateAirDensity(height);
    return -airDensity * g0 * volume * glm::vec3(0.0f, 0.0f, 1.0f); // Buoyant force acts upwards
}

glm::vec3 AirResistance::calculateLiftForce(glm::vec3 velocity, float height, float angle, float massProjectile)
{
    float liftCoefficient = calculateLiftCoefficient(angle, velocity, height);
    float airDensity = calculateAirDensity(height);
    float effectiveCrossSectionalArea = getCrossSectionalArea(angle);

    glm::vec3 liftDirection = glm::cross(glm::normalize(velocity), glm::vec3(0.0f, 0.0f, 1.0f)); // Lift acts perpendicular to velocity
    float liftMagnitude = 0.5f * liftCoefficient * airDensity * effectiveCrossSectionalArea * static_cast<float>(std::pow(glm::length(velocity), 2));

    return liftDirection * liftMagnitude;
}

glm::vec3 AirResistance::gravitationalForce(float height)
{
    float gravitationalConstant = 6.67430e-11f; // m³/(kg·s²)
    float massEarth = 5.972e24f; // kg
    float radiusEarth = 6371000.0f; // m
    
    float force = (gravitationalConstant * massEarth) / ((radiusEarth + height) * (radiusEarth + height));
    return glm::vec3(0.0f, 0.0f, -force); // Gravitational force acts downwards
}

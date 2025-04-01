#ifndef AIRRESISTANCE_H
#define AIRRESISTANCE_H

#include <glm/glm.hpp>
#include <cmath>

// Define M_PI if it's not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class AirResistance
{
    private:
        float dragCoefficient;   // Updated dynamically
        float crossSectionalArea;

    public:
        // Constructor initializes only static characteristics
        AirResistance(float dragCoefficient, float crossSectionalArea)
        : dragCoefficient(dragCoefficient), crossSectionalArea(crossSectionalArea) {}

        static const float R;  // Universal gas constant (J/(kg·K))
        static const float g0; // Gravity on Earth (m/s²)
        static const float T0; // Standard temperature at sea level (K)

        // Setters
        void setDragCoefficient(float coeff) { dragCoefficient = coeff; }
        void setCrossSectionalArea(float area) { crossSectionalArea = area; }

        // Getters (Now always return dynamically updated values)
        float getDragCoefficient(float machNumber);
        float getCrossSectionalArea(float angle) const;

        // Main functions
        glm::vec3 calculateDragForce(glm::vec3 velocity, float height, float angle);
        float calculateDynamicDragCoefficient(float machNumber);
        float calculateMachNumber(glm::vec3 velocity, float height);
        float calculateSoundVelocity(float height);
        float calculateAirDensity(float height);
        float calculateTemperature(float height);

        // Additional functions
        float calculateTerminalVelocity(float massProjectile, float height);
        float calculateAirViscosity(float temperature);
        float calculateReynoldsNumber(glm::vec3 velocity, float height);
        glm::vec3 calculateStokesDrag(glm::vec3 velocity, float temperature);
        glm::vec3 computeTotalDrag(glm::vec3 velocity, float height, float angle, float temperature);
        glm::vec3 computeWindDrag(glm::vec3 velocity, float height, float angle, glm::vec3 windVelocity);
        glm::vec3 calculateCoriolisForce(glm::vec3 velocity, float massProjectile);
        float aerodynamicHeating(glm::vec3 velocity, float height, float angle);
        glm::vec3 calculateBouyancyForce(float height, float volume);
        float calculateLiftCoefficient(float angle, glm::vec3 velocity, float height);
        glm::vec3 calculateLiftForce(glm::vec3 velocity, float height, float angle, float massProjectile);
        glm::vec3 gravitationalForce(float height);

};  
#endif
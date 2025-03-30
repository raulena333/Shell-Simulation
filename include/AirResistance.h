#ifndef AIRRESISTANCE_H
#define AIRRESISTANCE_H

#include <Projectile.h>
#include <glm/glm.hpp>

class AirResistance
{
    private:
        // Characteristics of the air
        float machNumber;
        float dragCoeffcient;
        float airDensity;
        float crossSectionalArea;
        float airTempertature;

    public:
        // Constructor to initialize the air resistance parameters
        AirResistance(float dragCoefficient, float airDensity, float crossSectionalArea, float airTemperature)
        : dragCoeffcient(dragCoefficient), airDensity(airDensity), airTempertature(airTempertature), 
        crossSectionalArea(crossSectionalArea) {}

        static const float R;  // Universal gas constant (J/(kg·K))

        // Setters
        void setDragCoefficient(float coeff) {dragCoeffcient = coeff; }
        void setAirDensity(float density) {airDensity = density; }
        void setCrossSectionalArea(float area) {crossSectionalArea = area; }
        void setMatchNumber(float number) {machNumber = number; }
        void setAirTemperature(float temp) {airTempertature = temp; }

        // Getters
        float getDragCoefficient() const {return dragCoeffcient; }
        float getAirDensity() const {return airDensity; }
        float getCrossSectionalArea() const {return crossSectionalArea; }
        float getAirTemperature() const {return airTempertature; }
        float getMachNumber() const {return machNumber; }

        // Main functions
        float calculateDragForce(glm::vec3 velocity);
        float calculateMachNumber(glm::vec3 velocity, float height);
        float calculateSoundVelocity(float height);
};

#endif 
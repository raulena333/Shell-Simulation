#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <glm/glm.hpp>

class Projectile
{
    private:
        double mass;
        glm::vec3 velocity;
        glm::vec3 position;
        double diameter;
    
    public:
        double CalculateKineticEnergy();
        void UpdatePositionOverTime();

};

#endif
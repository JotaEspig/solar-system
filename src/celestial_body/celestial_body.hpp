#pragma once

#include <cstdint>
#include <vector>

class CelestialBody
{
    std::string name;
    double radius;
    double semi_major_axis;
    double mass;
    bool emit_light;
    std::vector<*CelestialBody> satellites;

    public:
    CelestialBody(std::string _name, uint16_t _radius, bool _emit_light);
    ~CelestialBody();
};
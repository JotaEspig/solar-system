#pragma once

#include <string>
#include <cstdint>
#include <vector>

class CelestialBody
{
    std::string _name;
    double _radius;
    double _semi_major_axis;
    double _mass;
    uint32_t _color;
    bool _emit_light;
    std::vector<CelestialBody*> _satellites;

    public:
    CelestialBody(std::string name, double radius, double semi_major_axis,
                  double mass, uint32_t color, bool emitlight);
    ~CelestialBody();
    std::string name();
    double radius();
    double semi_major_axis();
    double mass();
    uint32_t color();
    bool emit_light();
    std::vector<CelestialBody*> satellites();
};

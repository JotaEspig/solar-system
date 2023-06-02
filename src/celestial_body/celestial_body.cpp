#include "celestial_body.hpp"

#include <string>
#include <cstdint>

CelestialBody::CelestialBody(std::string name, double radius,
                             double semi_major_axis, double mass,
                             uint32_t color, bool emit_light)
{
    _name = name;
    _radius = radius;
    _semi_major_axis = semi_major_axis;
    _mass = mass;
    _color = color;
    _emit_light = emit_light;
}

CelestialBody::~CelestialBody()
{
}

std::string CelestialBody::name()
{
    return _name;
}

double CelestialBody::radius()
{
    return _radius;
}

double CelestialBody::semi_major_axis()
{
    return _semi_major_axis;
}

double CelestialBody::mass()
{
    return _mass;
}

uint32_t CelestialBody::color()
{
    return _color;
}

bool CelestialBody::emit_light()
{
    return _emit_light;
}

std::vector<CelestialBody*> CelestialBody::satellites()
{
    return _satellites;
}

#include "celestial_body.hpp"

#include <cstdint>

CelestialBody::CelestialBody(std::string _name, uint16_t _radius, bool _emit_light)
{
    name = _name;
    radius = _radius;
    emit_light = _emit_light;
}

CelestialBody::~CelestialBody()
{
}
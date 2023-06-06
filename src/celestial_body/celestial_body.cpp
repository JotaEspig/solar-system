#include "celestial_body.hpp"

#include <string>
#include <cstdint>
#include <cmath>

static const double GRAVITY = 0.000000000066742;

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

double CelestialBody::period()
{
    // Kepler's third law
    // TODO checks is giving the right result
    return sqrt((4 * pow(M_PI, 2) * pow(_semi_major_axis, 3))
                / (GRAVITY * _central_body->_mass));
}

// Setters
void CelestialBody::set_central_body(CelestialBody *central_body)
{
    _central_body = central_body;
}

void CelestialBody::set_satellites(std::vector<CelestialBody*> satellites)
{
    _satellites = satellites;
}

// Getters
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

CelestialBody *CelestialBody::central_body()
{
    return _central_body;
}

std::vector<CelestialBody*> CelestialBody::satellites()
{
    return _satellites;
}

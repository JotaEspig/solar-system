#pragma once

#include <string>
#include <chrono>

#include <SDL2/SDL.h>

#include "../celestial_body/celestial_body.hpp"

class Application
{
    std::string title;
    uint16_t width;
    uint16_t height;
    double distance_scale;
    double time_scale;
    double principal_body_scale;
    double satellites_scale;
    SDL_Window *window;
    SDL_Surface *screen_surface;
    SDL_Renderer *renderer;

    SDL_Point get_body_orbital_pos(CelestialBody *body, long double);
    void draw();
    void draw_body(CelestialBody *body);
    void draw_circle(uint16_t center_x, uint16_t center_y, uint16_t radius);

    public:
    Application(uint16_t _width = 800, uint16_t _height = 800, std::string _title = "Solar System");
    ~Application();
    void run();
};

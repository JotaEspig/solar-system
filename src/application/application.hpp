#pragma once

#include <string>
#include <ctime>

#include <SDL2/SDL.h>

#include "../celestial_body/celestial_body.hpp"

class Application
{
    std::string title;
    uint16_t width;
    uint16_t height;
    SDL_Window *window;
    SDL_Surface *screen_surface;
    SDL_Renderer *renderer;

    SDL_Point get_body_orbital_pos(CelestialBody *body, time_t time);
    void draw();
    void draw_body(CelestialBody *body);
    void draw_circle(uint16_t center_x, uint16_t center_y, uint16_t radius);

    public:
    Application(uint16_t _width = 800, uint16_t _height = 800, std::string _title = "Solar System");
    ~Application();
    void run();
};

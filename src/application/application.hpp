#pragma once

#include <string>

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

    void draw();
    void draw_body(CelestialBody *body);

    public:
    Application(uint16_t _width = 800, uint16_t _height = 800, std::string _title = "Solar System");
    ~Application();
    void run();
};

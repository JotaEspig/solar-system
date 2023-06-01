#pragma once

#include <string>

#include <SDL2/SDL.h>

class Application
{
    std::string title;
    uint16_t width;
    uint16_t height;
    SDL_Window *window;
    SDL_Surface *screen_surface;
    SDL_Renderer *renderer;

    void draw();
    void draw_body(uint16_t center_x, uint16_t center_y, uint16_t radius);

    public:
    Application(uint16_t _width = 800, uint16_t _height = 800, std::string _title = "Solar System");
    ~Application();
    void run();
};
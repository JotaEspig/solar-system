#include "application.hpp"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>

#include "../celestial_body/celestial_body.hpp"

/*

https://www.youtube.com/watch?v=83GMVNaaEDA
Ver esse video para aplicar luz do sol aos planetas

*/

Application::Application(uint16_t _width, uint16_t _height, std::string _title)
{
    width = _width;
    height = _height;
    title = _title;

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, height,
                              SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        std::cout << "Error when creating window. SDL_Error: " << SDL_GetError() << "\n";
        exit(-1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        std::cout << "Error when creating renderer. SDL_Error: " << SDL_GetError() << "\n";
        exit(-1);
    }

    screen_surface = SDL_GetWindowSurface(window);
    SDL_FillRect(screen_surface, NULL,
                 SDL_MapRGB(screen_surface->format, 0x00, 0x00, 0x00));
    SDL_UpdateWindowSurface(window);
}

Application::~Application()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Application::draw()
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    auto body = new CelestialBody("terra", 380, 0, 10, 0xFFF200, false);
    draw_body(body);
    delete body;

    SDL_RenderPresent(renderer);
}

void Application::draw_body(CelestialBody *body)
{
    int32_t r = (body->color() >> 16) & 0xFF;
    int32_t g = (body->color() >> 8) & 0xFF;
    int32_t b = body->color() & 0xFF;
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    if (body->semi_major_axis() == 0)
    {
        int center_x = width / 2;
        int center_y = height / 2;
        const int32_t diameter = (body->radius() * 2);

        int32_t x = (body->radius() - 1);
        int32_t y = 0;
        int32_t tx = 1;
        int32_t ty = 1;
        int32_t error = (tx - diameter);

        while (x >= y)
        {
            //  Each of the following renders an octant of the circle
            SDL_RenderDrawLine(renderer, center_x - x, center_y - y,
                               center_x + x, center_y - y);
            SDL_RenderDrawLine(renderer, center_x - x, center_y + y,
                               center_x + x, center_y + y);
            SDL_RenderDrawLine(renderer, center_x - y, center_y - x,
                               center_x + y, center_y - x);
            SDL_RenderDrawLine(renderer, center_x - y, center_y + x,
                               center_x + y, center_y + x);

            if (error <= 0)
            {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0)
            {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    }
}

void Application::run()
{
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            default:
                break;
            }
        }

        draw();
    }
}

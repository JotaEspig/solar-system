#include "application.hpp"

#include <iostream>
#include <string>
#include <chrono>
// TODO Try chrono
#include <cmath>

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
    distance_scale = 200;
    time_scale = 300;
    principal_body_scale = 0.5;
    satellites_scale = 2;

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

SDL_Point Application::get_body_orbital_pos(CelestialBody *body, 
                                            long double seconds)
{
    double period = body->period();
    double rotation = time_scale * 2 * M_PI * seconds / period;
    std::cout << period << "\n";
    SDL_Point point;
    point.x = distance_scale * body->semi_major_axis() * sin(rotation);
    point.y = distance_scale * body->semi_major_axis() * cos(rotation);
    return point;
}

void Application::draw()
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    auto body = new CelestialBody("sol", 109.17, 0, 333000, 0xFFF200, false);
    auto body2 = new CelestialBody("terra", 1, 1, 1, 0x00FFFF, false);
    auto body3 = new CelestialBody("Jupiter", 11, 3, 318, 0xFFA500, false);
    auto body4 = new CelestialBody("marte", 0.52, 1.5, 0.1, 0xFF0000, false);
    auto body5 = new CelestialBody("Venus", 0.94, 0.72, 0.815, 0xFFA500, false);
    body2->set_central_body(body);
    body3->set_central_body(body);
    body4->set_central_body(body);
    body5->set_central_body(body);
    draw_body(body);
    draw_body(body2);
    draw_body(body3);
    draw_body(body4);
    draw_body(body5);

    SDL_RenderPresent(renderer);
    delete body;
    delete body2;
    delete body3;
    delete body4;
    delete body5;
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
        draw_circle(center_x, center_y, principal_body_scale * body->radius());
    }
    else
    {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        long double millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        SDL_Point point = get_body_orbital_pos(body, millis / 1000); 
        std::cout << millis << "-> " << point.x << "," << point.y << "\n";
        draw_circle(width / 2 + point.x, height / 2 + point.y, satellites_scale * body->radius());
    }
}

void Application::draw_circle(uint16_t center_x, uint16_t center_y, uint16_t radius)
{
    const int32_t diameter = (radius * 2);
    int32_t x = (radius - 1);
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

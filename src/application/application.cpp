#include "application.hpp"

#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <vector>

#include <SDL2/SDL.h>
#include "../rapidjson/document.h"

#include "../celestial_body/celestial_body.hpp"

/*

https://www.youtube.com/watch?v=83GMVNaaEDA
Ver esse video para aplicar luz do sol aos planetas

*/

Application::Application(uint16_t _width, uint16_t _height, std::string _title)
{
    init(_width, _height, _title);
}

std::vector<CelestialBody*> read_satellites(const rapidjson::Value &central_body_json)
{
    const rapidjson::Value &bodies_json = central_body_json["satellites"];
    std::vector<CelestialBody*> vec;

    for (rapidjson::SizeType i = 0; i < bodies_json.Size(); i++)
    {
        const rapidjson::Value &body_json = bodies_json[i];
        auto body = new CelestialBody(body_json["name"].GetString(),
                                      body_json["radius"].GetDouble(),
                                      body_json["semi_major_axis"].GetDouble(),
                                      body_json["mass"].GetDouble(),
                                      body_json["color"].GetInt(),
                                      body_json["emit_light"].GetBool());
        vec.push_back(body);
    }

    return vec;
}

Application::Application(rapidjson::Document &doc, uint16_t _width,
                         uint16_t _height, std::string _title)
{
    init(_width, _height, _title);
    // TODO Fix, because it's not adding earth
    const rapidjson::Value &bodies_json = doc["bodies"];
    for (rapidjson::SizeType i = 0; i < bodies_json.Size(); i++)
    {
        const rapidjson::Value &body_json = bodies_json[i];
        auto body = new CelestialBody(body_json["name"].GetString(),
                                      body_json["radius"].GetDouble(),
                                      body_json["semi_major_axis"].GetDouble(),
                                      body_json["mass"].GetDouble(),
                                      body_json["color"].GetInt(),
                                      body_json["emit_light"].GetBool());

        body->set_satellites(read_satellites(body_json));
        for (auto &e : body->satellites())
            e->set_central_body(body);

        _bodies.push_back(body);
    }
}

Application::~Application()
{
    for (auto &e: _bodies)
        delete e;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Application::init(uint16_t _width, uint16_t _height, std::string _title)
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
    for (auto &body: _bodies)
        draw_body(body);

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

void Application::set_distance_scale(double value)
{
    distance_scale = value;
}

void Application::set_time_scale(double value)
{
    time_scale = value;
}

void Application::set_principal_body_scale(double value)
{
    principal_body_scale = value;
}

void Application::set_satellites_scale(double value)
{
    satellites_scale = value;
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

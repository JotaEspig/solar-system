#include "application.hpp"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>

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
    SDL_RenderSetDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // do    
    draw_body(width / 2, height / 2, 50);

    SDL_RenderPresent(renderer);
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
    }
}
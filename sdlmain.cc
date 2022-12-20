#define WIN
#include <iostream>
#include <map>
#include <functional>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h> 
#include <stdarg.h>
#include <math.h>
#include <tgmath.h>
#include <stdio.h>
#include <algorithm>
#include <map>
#include <list>
#include <string>
#include <sstream>
#include <fstream>
#ifdef WIN
#include <windows.h>
#include <thread>
#pragma comment(lib, "Winmm.lib")
#endif

#include "SDL/SDL.h"
#pragma comment(lib, "SDL/lib/x64/SDL2.lib")
#pragma comment(lib, "SDL/lib/x64/SDL2main.lib")
#pragma comment(lib, "SDL/lib/x64/SDL2test.lib")

using namespace std;

#define		IMAGESCALE		512				// 画布尺寸
#define		real			float			// 精度类型
#define		EPSINON			1e-3			// 精度
#define		MAXZ			100.0
#define		EXPORT_SCALE	1.0
#define		USIZE0			0.01f
#define		USIZE			(USIZE0 / EXPORT_SCALE)
#define		UNITSCALE		USIZE

#include "com.hpp"
#include "str.hpp"

// math
#include "functor.hpp"
#include "coord.hpp"
#include "phase.hpp"
#include "differ.hpp"

uint	 color = 0xFFFFFFFF;
real	gtime = 0;
int		state = 1;
bool    bplay = false;

SDL_Renderer* renderer = 0;

// ui
#include "SDL/SDL_test_font.h"
#include "UI/Window.hpp"

#define SET_COLOR(r,g,b,a) SDL_SetRenderDrawColor(renderer, r, g, b, a);
#define PTSET(x, y, r, cor)  Window::draw_circle(x, y, r, {255,255,255,255})
#define PSET(x, y, size, cor)  SDL_RenderDrawPoint(renderer, x, y)
#define PSET6(x, y, size, cor)  SDL_RenderDrawPoint(renderer, x*6, y*6)
#define LINE(x1, y1, x2, y2,cor)  Window::draw_line(x1, y1, x2, y2, {255,255,200,255})
#define TEXT(x, y, s)  SDLTest_DrawString(renderer, x, y, s)
extern void dophg(const char* phg);

#include "path.hpp"

// lua
namespace Global
{
    std::map<std::string, std::string> stringmap;
    std::map<std::string, bool> boolmap;
    std::map<std::string, int> intmap;
    std::map<std::string, float> floatmap;
    std::map<std::string, vec3> vec3map;
    std::map<std::string, coord3> coord3map;
}
extern "C" {
#include "lua/lua53.h"
}
#include "lua/lua_api.hpp"
#include "PHG/PHGAPP.hpp"

void dophg(const char* phg)
{
    PHGAPP::dofile(phg);
}
// ################################################################################################
// Contain
// ################################################################################################

// ------------------------------------------------------------------------------------------------
#include "draw.hpp"

// ------------------------------------------------------------------------------------------------
// init
// ------------------------------------------------------------------------------------------------
void init()
{
    dolua("script/main.lua");

    PHGAPP::setup();

    CALC([](PHGAPP::code& cd, char o, int args)->var {
        if (o == '+')
        {
            ASSERT_RET(ROOT);
            var a = GET_PARAM(1);
            var b = GET_PARAM(2);
            var c = a + b;

            POP_SPARAM;
            return c;;
        }
        return 0;
        });

   // getchar();
}
// ************************************************************************************************
// UI
// ************************************************************************************************
void onlbdown(int ix, int iy)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
   
    state = 1;
    Global::vec3map["mouse"] = vec3(ix, iy, 0) / 2;

    dolua("onlbtn()", false);
}
void onmotion(int ix, int iy)
{
    if (state)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        Global::vec3map["mouse"] = vec3(ix, iy, 0) / 2;

        dolua("onmouse()", false);
    }
}/*
void onplay()
{
    dolua("onplay()");
}*/
int main(int argc, char* argv[])
{
    //初始化SDL
    SDL_Init(SDL_INIT_VIDEO);
    //创建一个窗口和一个渲染器
    SDL_Window* window;
   // SDL_Renderer* renderer;
    // Create an application window with the following settings:
    /* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
    window = SDL_CreateWindow(
        "Constaint Physics",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        1024,                               // width, in pixels
        768,                               // height, in pixels
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderSetScale(renderer, 2, 2);

    SDL_RenderClear(renderer);

    init();

    int quit = 0;
    do {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            quit = 1;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                quit = 1;
                break;
            }
            else if (event.key.keysym.sym == SDLK_BACKSPACE) {

                dolua("script/main.lua");
                SDL_SetRenderDrawColor(renderer, 10, 100, 180, 255);
                SDL_RenderClear(renderer);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
                SDL_RenderClear(renderer);
                onlbdown(event.button.x, event.button.y);
            }
            break;
        case SDL_MOUSEMOTION:
            if(event.button.button == SDL_BUTTON_MIDDLE)
            {
                SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
                SDL_RenderClear(renderer);
                onmotion(event.button.x, event.button.y);
            }
            break;
        default:
            break;
        }
        //if(bplay)
         //   onplay();
        SDL_RenderPresent(renderer);
    } while (!quit);

    //destory renderer
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();

    return 0;
}
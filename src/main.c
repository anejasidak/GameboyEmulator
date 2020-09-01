#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "config.h"
#include "gameboy.h"
#include "test.h"

static SDL_Window *window;
static SDL_Renderer *renderer;

int initializeView()
{

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Unable to initialize SDL: %s", SDL_GetError());
        return 0;
    }

    /* Create a window for the emulator.
    *  Note: 
    *       The origin gameboy display is 160 by 144 pixels. 
    *       To have better viewing, the display has been scaled up.
    *       See config.h to change scale value.
    */
    window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * DISPLAY_SCALE,
        DISPLAY_HEIGHT * DISPLAY_SCALE,
        0);

    renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Please provide a file to load\n");
        return -1;
    }
    struct s_gameboy *gameboy = (struct s_gameboy *)malloc(sizeof(struct s_gameboy));
    intializeSystem(gameboy);

    if (/*!initializeView() || */ !readCartridge(argv[1]))
    {
        return -1;
    }

    loadGameIntoMemory();

    //test_allTests(gameboy);

    bool isActive = true;

    while (isActive)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                isActive = false;
            }
            break;
            }
        }

        if (!isActive)
        {
            break;
        }
        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        // SDL_RenderClear(renderer);
        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        executeNextInstruction();
    }

    // Close the window and quit SDL to  clean up all initialized subsystems.
    // SDL_DestroyWindow(window);
    // SDL_DestroyRenderer(renderer);
    // SDL_Quit();

    free(gameboy);
    return 0;
}
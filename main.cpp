#include <stdio.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Forward declarations
void handleMouseClick(int x, int y);
float getDistance(int x1, int y1, int x2, int y2);

// Global variables
SDL_Window *window = nullptr;
SDL_Surface *screen = nullptr;
bool quit = false;

// Track mouse state
int mouseX = 0;
int mouseY = 0;
bool mouseDown = false;

// Store click positions and their effect radii
struct ClickEffect {
    int x;
    int y;
    int radius;
    Uint8 r, g, b;
};
std::vector<ClickEffect> clickEffects;

// Get distance between two points
float getDistance(int x1, int y1, int x2, int y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx*dx + dy*dy);
}

void handleMouseClick(int x, int y) {
    // Create a new click effect with random color
    ClickEffect effect;
    effect.x = x;
    effect.y = y;
    effect.radius = 30;  // Initial radius of effect
    effect.r = std::rand() % 256;
    effect.g = std::rand() % 256;
    effect.b = std::rand() % 256;
    clickEffects.push_back(effect);
    
    printf("Click at (%d, %d)\n", x, y);
}

void mainloop() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseDown = true;
                handleMouseClick(event.button.x, event.button.y);
                break;
            case SDL_MOUSEBUTTONUP:
                mouseDown = false;
                break;
            case SDL_MOUSEMOTION:
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                if (mouseDown) {
                    handleMouseClick(mouseX, mouseY);
                }
                break;
        }
    }

    // Draw base gradient
    if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
    
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            Uint8 r = x;
            Uint8 g = y;
            Uint8 b = 255 - x;
            Uint8 a = 255;

            // Apply click effects
            for (const auto& effect : clickEffects) {
                float dist = getDistance(x, y, effect.x, effect.y);
                if (dist < effect.radius) {
                    float influence = 1.0f - (dist / effect.radius);
                    r = (Uint8)((1.0f - influence) * r + influence * effect.r);
                    g = (Uint8)((1.0f - influence) * g + influence * effect.g);
                    b = (Uint8)((1.0f - influence) * b + influence * effect.b);
                }
            }

            // Calculate the correct pixel offset
            int offset = y * screen->pitch / sizeof(Uint32) + x;
            *((Uint32*)screen->pixels + offset) = SDL_MapRGBA(screen->format, r, g, b, a);
        }
    }
    
    if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
    SDL_UpdateWindowSurface(window);
}

int main(int argc, char** argv) {
    printf("Click anywhere on the window to create color effects!\n");

    // Initialize random seed
    std::srand(std::time(nullptr));

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create an SDL window
    window = SDL_CreateWindow("Interactive SDL2 Example", 
                            SDL_WINDOWPOS_UNDEFINED, 
                            SDL_WINDOWPOS_UNDEFINED,
                            256, 256, 
                            SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Get the window surface
    screen = SDL_GetWindowSurface(window);
    if (!screen) {
        printf("SDL_GetWindowSurface Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, 1);
#else
    while (!quit) {
        mainloop();
        SDL_Delay(16);
    }
#endif

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
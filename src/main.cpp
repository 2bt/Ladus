#include "app.hpp"
#ifdef WASM

#define EXPORT __attribute__((visibility("default"))) extern "C"

EXPORT int width() { return app::WIDTH; }
EXPORT int height() { return app::HEIGHT; }
EXPORT void init() {
    app::init();
}
EXPORT uint32_t const* pixels() { return app::screen.pixels(); }
EXPORT void update(uint32_t input_bits) {
    app::update(input_bits);
    app::draw();
}

#else
#include <SDL.h>


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Ladus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          app::WIDTH * 3, app::HEIGHT * 3, SDL_WINDOW_RESIZABLE);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(renderer, app::WIDTH, app::HEIGHT);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ABGR8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             app::WIDTH, app::HEIGHT);
    app::init();

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            default: break;
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = false;
                break;
            }
        }
        const Uint8* ks = SDL_GetKeyboardState(nullptr);

        uint32_t bits = 0;
        bits |= !!ks[SDL_SCANCODE_LEFT ] << 0;
        bits |= !!ks[SDL_SCANCODE_RIGHT] << 1;
        bits |= !!ks[SDL_SCANCODE_UP   ] << 2;
        bits |= !!ks[SDL_SCANCODE_DOWN ] << 3;
        bits |= !!ks[SDL_SCANCODE_X    ] << 4;
        bits |= !!ks[SDL_SCANCODE_C    ] << 5;

        app::update(bits);
        app::draw();


        SDL_UpdateTexture(texture, nullptr, app::screen.pixels(), app::WIDTH * 4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_Quit();
}

#endif

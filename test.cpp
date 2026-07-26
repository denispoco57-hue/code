#include "lib.hpp"

int main(int argc,char* argv[]){
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "sdl init video failed = " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("testing", 800,600,SDL_WINDOW_BORDERLESS);

    if (!window) {
        std::cerr << "window couldnt be created = " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    bool run = true;
    SDL_Event e;
    while (run) {
        while (e.type = SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) 
                run = false;
        }
    }

    // cleanup

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}  
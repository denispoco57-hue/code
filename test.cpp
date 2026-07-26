#include "lib.hpp"


int main(int argc,char* argv[]){
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "sdl init video failed = " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("denis_sec", 800,600,SDL_WINDOW_BORDERLESS);
    if (!window) {
        std::cerr << "window couldnt be created = " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nil);
    if (!renderer) {
        std::cerr << "couldnt make a renderer = " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* image = IMG_LoadTexture(renderer,"D:/vs/code/test.jpg");
    if (!image) {
        std::cerr << "couldnt load image = " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }

    bool run = true;
    SDL_Event e;
    while (run) {
        while (e.type = SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) run = false; break;

            SDL_SetRenderDrawColor(renderer,30, 30, 40, 255);
            SDL_RenderClear(renderer);
            SDL_RenderTexture(renderer,image,nil,nil);
            SDL_RenderPresent(renderer);
        }
    }

    // cleanup

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(image);
    SDL_Quit();
    std::cout<<std::endl;
    return 0;
}  
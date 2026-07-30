#include "lib.hpp"

int math_random(int s, int e) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(s,e);
    return distrib(gen);
}
void task_wait(int time) {
    if (time >= 1 ) 
        std::this_thread::sleep_for(std::chrono::seconds(time));
    else if (time >= 0.001 and time < 1)
        std::this_thread::sleep_for(std::chrono::milliseconds(time*1000));
}

int main(int argc,char* argv[]){
    if (!SDL_INIT_VIDEO) {
        std::cerr << "couldnt initialize vid in SDL = " << SDL_GetError() << '\n';
        return 1;
    }
    if (!TTF_Init()) {
        std::cerr << "couldnt initialize ttf in SDL = " << SDL_GetError() << '\n';
        return 1;
    }
    SDL_Window* win = SDL_CreateWindow("...",675,888,SDL_WINDOW_FULLSCREEN);
    if (!win) {
        std::cerr << "couldnt create a window in SDL = " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* rend = SDL_CreateRenderer(win,nil);
    if (!rend) {
        std::cerr << "couldnt create a renderer in SDL = " << SDL_GetError() << '\n';
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    TTF_Font* font = TTF_OpenFont("D:/vs/code/fonts/RobotoCondensed-Regular.ttf",32.0f);
    if (!font) {
        std::cerr << "couldnt open font in SDL = " << SDL_GetError() << '\n';
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    std::string fullt1 = "_ L0ad1ing, please wa1t ^";
    int fullt1_l = fullt1.length();
    size_t start = 1;
    SDL_Color w255 = {255,255,255,255};
    SDL_Texture* ttext = nil;
    SDL_FRect dst = {0};


    bool check = false;
    SDL_Event e;
    bool run = true;
    while (run) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) run = false;
        }
        if (check == false) {
            check = true;
            task_wait(math_random(100,1000)/250);
        }
        while (start != fullt1_l) {
        if (ttext) SDL_DestroyTexture(ttext);
        std::string_view partial(fullt1.data(),start);
        SDL_Surface* tsurf = TTF_RenderText_Blended(font,std::string(partial).c_str(),0,w255);
        ttext = SDL_CreateTextureFromSurface(rend,tsurf);
        SDL_FRect dst = {1,1,(float)tsurf->w,(float)tsurf->h};
        SDL_SetRenderDrawColor(rend,0, 0, 0, 0);
        SDL_RenderClear(rend);
        SDL_RenderTexture(rend,ttext,nil,&dst);
        SDL_RenderPresent(rend);
        start++;
        task_wait(math_random(100,1000)/1000);
        }
        if (ttext) SDL_DestroyTexture(ttext);
        SDL_Surface* tsurf = TTF_RenderText_Blended(font,fullt1.data(),0,w255);
        ttext = SDL_CreateTextureFromSurface(rend,tsurf);
        SDL_FRect dst = {1,1,(float)tsurf->w,(float)tsurf->h};
        SDL_SetRenderDrawColor(rend,0, 0, 0, 0);
        SDL_RenderClear(rend);
        SDL_RenderTexture(rend,ttext,nil,&dst);
        SDL_RenderPresent(rend);
    }

    //cleanup
    SDL_DestroyTexture(ttext);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    std::cout<<std::endl;
    return 0;
}  
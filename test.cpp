#include "lib.hpp"

int math_random(int s, int e) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(s,e);
    return distrib(gen);
}
void task_wait(float time) {std::this_thread::sleep_for(std::chrono::milliseconds((int)(time*1000)));}


int main(int argc,char* argv[]){
    if (!SDL_Init(SDL_INIT_VIDEO)) {
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

    struct text {
        float x;
        float y;
        std::string tix;
    };
    
    struct ttext {
        SDL_Texture* tit;
        SDL_FRect dsts;
    };
    
    std::vector<ttext> ttexty;
    std::vector<text> texty;

    text t1 = {1,1, "_ L0ad1ing, please wa1t ^"};
    text t2 = {1,40, "[  6.7065234] sd 0:0:0:0:0dbdbu25 [New] Assuming drive cache: write through"};
    text t3 = {1,80, "Valid path for Init Volume"};
    text t4 = {1,160, "---------------------------------"};
    text t5 = {1,200, "kolko is gey?"};
    text t6 = {1,250, "noooo way"};
    text t7 = {1,290, "🤐🤐"};


     texty = {t1,t2,t3,t4,t5,t6,t7};

    std::string str = " ";
    size_t len = 0;
    size_t start = 1;
    SDL_Color w255 = {255,255,255,255};
    SDL_Texture* tetext = nil;
    SDL_FRect dst = {0};

    

    bool check = false;
    bool done = false;
    bool run = true;

    SDL_Event e;

    while (run) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) run = false;
        }
        if (check == false) {
            check = true;
            task_wait(math_random(100,1000)/100.0f);
        }
        if (!done) {
            for (size_t g = 0; g < texty.size();++g) {

            if (tetext != nil) SDL_DestroyTexture(tetext);
            tetext = nil;

            str = texty[g].tix;
            len = str.length();
            start = 1;

            while (start < len) {

                while (SDL_PollEvent(&e)) { // FIX: stay responsive to quit while typing, not just between lines
                        if (e.type == SDL_EVENT_QUIT) run = false;
                    }
                if (!run) break;

                std::string partial(str.data(),start);
                SDL_Surface* surf = TTF_RenderText_Blended(font,partial.c_str(),0,w255);

                if (tetext != nil) SDL_DestroyTexture(tetext);

                tetext = SDL_CreateTextureFromSurface(rend,surf);
                dst = {texty[g].x,texty[g].y,(float)surf->w,(float)surf->h};

                SDL_DestroySurface(surf);

                SDL_SetRenderDrawColor(rend,0,0,0,0);
                SDL_RenderClear(rend);

                if (!ttexty.empty()) {
                    for (size_t i = 0; i < ttexty.size(); ++i) { // FIX: .size(), not sizeof()
                        SDL_RenderTexture(rend,ttexty[i].tit,nil,&ttexty[i].dsts); // FIX: tit is a pointer now, no &
                    }
                }

                SDL_RenderTexture(rend,tetext,nil,&dst);
                SDL_RenderPresent(rend);

                task_wait(0.03f);
                ++start;
            }
            if (!run) break;

            if (tetext) SDL_DestroyTexture(tetext);
            SDL_Surface* surf = TTF_RenderText_Blended(font,str.c_str(),0,w255);
            tetext = SDL_CreateTextureFromSurface(rend,surf);
            dst = {texty[g].x,texty[g].y,(float)surf->w,(float)surf->h};
            ttexty.emplace_back(tetext,dst);
            tetext = nil;
            SDL_DestroySurface(surf);
         }
        }
        done = true;

    }

    //cleanup
    if (tetext) SDL_DestroyTexture(tetext);
    for (auto& t : ttexty ) SDL_DestroyTexture(t.tit);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    std::cout<<std::endl;
    return 0;
}  
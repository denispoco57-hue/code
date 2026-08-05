#include "lib.hpp"

std::string const vers = "0.13.1 mixer";

int math_random(int s, int e) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(s,e);
    return distrib(gen);
}
void task_wait(float time) {std::this_thread::sleep_for(std::chrono::milliseconds((int)(time*1000)));}


int main(int argc,char* argv[]){

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        std::cerr << "couldnt initialize vid or audio in SDL = " << SDL_GetError() << '\n';
        return 1;
    }
    if (!MIX_Init()) {
        std::cerr << "couldnt initialize mix = " << SDL_GetError() << '\n';
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

    SDL_SetRenderVSync(rend,SDL_RENDERER_VSYNC_ADAPTIVE);
    
    if (!rend) {
        std::cerr << "couldnt create a renderer in SDL = " << SDL_GetError() << '\n';
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    TTF_Font* font = TTF_OpenFont("D:/vs/code/fonts/GeistPixel-Regular-VariableFont_ELSH.ttf",32.0f);
    if (!font) {
        std::cerr << "couldnt open font in SDL D:/vs/code/fonts/GeistPixel-Regular-VariableFont_ELSH.ttf = " << SDL_GetError() << '\n';
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,nil);
    if (!mixer) {
        std::cerr << "couldnt create an mixer = " << SDL_GetError() << '\n';
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    MIX_Audio* audio = MIX_LoadAudio(mixer,"D:/vs/code/sfx/loading.mp3",false);
    if (!audio) {
        std::cerr << " couldnt load audio D:/vs/code/sfx/loading.mp3 = " << SDL_GetError() << '\n';
        SDL_DestroyRenderer(rend);
        MIX_DestroyMixer(mixer);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    MIX_Track* track = MIX_CreateTrack(mixer);
    if (!track) {
        std::cerr << "couldnt create a track = " << SDL_GetError() << '\n';
    }
    MIX_SetTrackAudio(track,audio);

    struct text {
        SDL_Color clr;
        float x;
        float y;
        std::string tix;
    };
    
    struct ttext {
        SDL_Texture* tit;
        SDL_FRect dsts;
    };

    SDL_Color w255 = {255,255,255,255};
    SDL_Color geen = {0, 255, 0, 255};
    SDL_Color red = {255,0,0,255};

    std::vector<ttext> ttexty;
    std::vector<text> texty;
    std::vector<std::string> sty;

    std::ifstream input("D:/vs/code/info.txt");

    if (!input.is_open()) {
        std::cerr << "couldnt open txt file D:/vs/code/info.txt" << '\n';
        return 1;
    }
    std::string bleh;

    while (std::getline(input,bleh)){
        sty.push_back(bleh);
    }

    texty.emplace_back(w255,1,1, sty[0]);
    texty.emplace_back(w255,1,40, sty[1]);
    texty.emplace_back(w255,1,80, sty[2]);
    texty.emplace_back(w255,1,140, sty[3]);
    texty.emplace_back(w255,1,200, sty[4] + std::to_string(math_random(103,15032)) + sty[5]);
    texty.emplace_back(w255,1,250, sty[6]);
    texty.emplace_back(w255,1,290, sty[7]);
    texty.emplace_back(w255,1,320,sty[3]);
    texty.emplace_back(w255,1,350,sty[8]);
    texty.emplace_back(w255,1000, 350 ,sty[9]);
    texty.emplace_back(geen,1100,350,sty[10]);
    texty.emplace_back(w255,1,390,sty[11]);
    texty.emplace_back(w255,1000, 390 ,sty[9]);
    texty.emplace_back(geen,1100,390,sty[10]);
    texty.emplace_back(w255,1,450,sty[12]);
    texty.emplace_back(red,1,500,sty[13]);
    texty.emplace_back(red,1,550, sty[14]);
    texty.emplace_back(geen,900,550, sty[15]);
    texty.emplace_back(geen,1,600, sty[16]);
    texty.emplace_back(w255,1,650,sty[17] + vers);
    texty.emplace_back(geen,1,700, sty[18]);

    sty.clear();
    input.clear();
    input.close();

    std::string str = " ";
    size_t len = 0;
    size_t start = 1;
    SDL_Texture* tetext = nil;
    SDL_FRect dst = {0};

    bool check = false;
    bool done = false;
    bool run = true;

    SDL_Event e;

    while (run) {
        task_wait(0.001);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) run = false;
        }
        if (check == false) {
            
            check = true;
            task_wait(math_random(100,1000)/100.0f);
            MIX_PlayTrack(track,0);

        }
        if (!done) {

            for (size_t g = 0; g < texty.size();++g) {

                if (tetext != nil) SDL_DestroyTexture(tetext);
                tetext = nil;

                str = texty[g].tix;
                len = str.length();
                start = 1;


                while (start < len) {

                    if (e.type == SDL_EVENT_QUIT) run = false;
                        
                    std::string partial(str.data(),start);
                    partial = partial + "⬛";
                    SDL_Surface* surf = TTF_RenderText_Blended(font,partial.c_str(),0,texty[g].clr);

                    if (tetext != nil) SDL_DestroyTexture(tetext);

                    tetext = SDL_CreateTextureFromSurface(rend,surf);
                    std::cerr << "making texture" << '\n';
                    dst = {texty[g].x,texty[g].y,(float)surf->w,(float)surf->h};

                    SDL_DestroySurface(surf);

                    SDL_SetRenderDrawColor(rend,0,0,0,0);
                    SDL_RenderClear(rend);

                    if (!ttexty.empty()) {
                        for (size_t i = 0; i < ttexty.size(); ++i) { 
                            SDL_RenderTexture(rend,ttexty[i].tit,nil,&ttexty[i].dsts); 
                        }
                    }

                    SDL_RenderTexture(rend,tetext,nil,&dst);
                    SDL_RenderPresent(rend);

                    task_wait(math_random(100,1000)/25000.0f);
                    ++start;
                 }
            

            if (!run) break;

            if (tetext) SDL_DestroyTexture(tetext);
            SDL_Surface* surf = TTF_RenderText_Blended(font,str.c_str(),0,texty[g].clr);
            tetext = SDL_CreateTextureFromSurface(rend,surf);
            dst = {texty[g].x,texty[g].y,(float)surf->w,(float)surf->h};
            ttexty.emplace_back(tetext,dst);
            tetext = nil;
            SDL_DestroySurface(surf);

            done = true;
            std::cerr << "done" << '\n';

         }
         std::cerr << "clearing in the end for testing" << '\n';
        SDL_SetRenderDrawColor(rend,0,0,0,0);
        SDL_RenderClear(rend);
        }
        
        
    }

    //cleanup

    if (tetext) SDL_DestroyTexture(tetext);
    for (auto& t : ttexty ) SDL_DestroyTexture(t.tit);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    MIX_Quit();
    std::cout<<std::endl;
    return 0;
}  
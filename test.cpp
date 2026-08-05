#include "lib.hpp"

std::string const vers = "0.13.1 mixer";

bool check;
bool run;
bool done;

std::vector<Uint8> render_draw_color;

SDL_Renderer* rend;
SDL_Window* win;


struct base_render {
    SDL_Color clr;
    float x;
    float y;
    std::string tix;
};
    
struct done_render {
    SDL_Texture* tit;
    SDL_FRect dsts;
};

std::vector<done_render> done_render_text;
std::vector<base_render> base_render_text;
std::vector<std::string> sty;

int math_random(int s, int e) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(s,e);
    return distrib(gen);
}
void task_wait(float time) {std::this_thread::sleep_for(std::chrono::milliseconds((int)(time*1000)));}
void type_writer(TTF_Font* font,const std::string& current,float x, float y, SDL_Color color){
     for (size_t g = 0; g < current.length();++g) {

        std::string partial(current.data(),g);
        partial += "🥱";


        SDL_Surface* c_surface = TTF_RenderText_Blended(font,partial.c_str(),0,color);
        SDL_Texture* c_texture = SDL_CreateTextureFromSurface(rend,c_surface);
        SDL_FRect dst = {x,y,(float)c_surface->w,(float)c_surface->h};

        SDL_SetRenderDrawColor(rend,render_draw_color[0],render_draw_color[1],render_draw_color[2],render_draw_color[3]);
        SDL_RenderClear(rend);

        SDL_RenderTexture(rend,c_texture,nil,&dst);

        SDL_DestroySurface(c_surface);

        if (!done_render_text.empty()) {
            for (size_t jerk = 0; jerk < done_render_text.size(); ++jerk) SDL_RenderTexture(rend,done_render_text[jerk].tit,nil,&done_render_text[jerk].dsts);
            
        }

        SDL_RenderPresent(rend);
        task_wait(math_random(100,1000)/25000.0f);
    }


    SDL_Surface* c_surface = TTF_RenderText_Blended(font,current.c_str(),0,color);
    SDL_Texture* c_texture = SDL_CreateTextureFromSurface(rend,c_surface);
    SDL_FRect dst = {x,y,(float)c_surface->w,(float)c_surface->h};
    done_render_text.emplace_back(c_texture,dst);
    SDL_DestroySurface(c_surface);
    
}

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
    win = SDL_CreateWindow("...",675,888,SDL_WINDOW_FULLSCREEN);
    if (!win) {
        std::cerr << "couldnt create a window in SDL = " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }
    rend = SDL_CreateRenderer(win,nil);

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

   

    SDL_Color w255 = {255,255,255,255};
    SDL_Color geen = {0, 255, 0, 255};
    SDL_Color red = {255,0,0,255};


    std::ifstream input("D:/vs/code/info.txt");

    if (!input.is_open()) {
        std::cerr << "couldnt open txt file D:/vs/code/info.txt" << '\n';
        return 1;
    }
    std::string bleh;

    while (std::getline(input,bleh)){
        sty.push_back(bleh);
    }

    base_render_text.emplace_back(w255,1,1, sty[0]);
    base_render_text.emplace_back(w255,1,40, sty[1]);
    base_render_text.emplace_back(w255,1,80, sty[2]);
    base_render_text.emplace_back(w255,1,140, sty[3]);
    base_render_text.emplace_back(w255,1,200, sty[4] + std::to_string(math_random(103,15032)) + sty[5]);
    base_render_text.emplace_back(w255,1,250, sty[6]);
    base_render_text.emplace_back(w255,1,290, sty[7]);
    base_render_text.emplace_back(w255,1,320,sty[3]);
    base_render_text.emplace_back(w255,1,350,sty[8]);
    base_render_text.emplace_back(w255,1000, 350 ,sty[9]);
    base_render_text.emplace_back(geen,1100,350,sty[10]);
    base_render_text.emplace_back(w255,1,390,sty[11]);
    base_render_text.emplace_back(w255,1000, 390 ,sty[9]);
    base_render_text.emplace_back(geen,1100,390,sty[10]);
    base_render_text.emplace_back(w255,1,450,sty[12]);
    base_render_text.emplace_back(red,1,500,sty[13]);
    base_render_text.emplace_back(red,1,550, sty[14]);
    base_render_text.emplace_back(geen,900,550, sty[15]);
    base_render_text.emplace_back(geen,1,600, sty[16]);
    base_render_text.emplace_back(w255,1,650,sty[17] + vers);
    base_render_text.emplace_back(geen,1,700, sty[18]);

    sty.clear();
    input.clear();
    input.close();

    std::string str = " ";
    size_t len = 0;
    size_t start = 1;

    check = false;
    done = false;
    run = true;

    SDL_Event e;

    while (run) {
        task_wait(0.001);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) run = false;
        }
        if (check == false) {
            
            check = true;
            task_wait(math_random(100,1000)/1000.0f);
            MIX_PlayTrack(track,0);

        }
        if (!done) {
            render_draw_color = {0,0,0,0};
            for (int lox = 0; lox < base_render_text.size();++lox) type_writer(font,base_render_text[lox].tix,base_render_text[lox].x,base_render_text[lox].y,base_render_text[lox].clr);
        }
        done = true;
    }

    //cleanup

    for (auto& t : done_render_text ) SDL_DestroyTexture(t.tit);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    MIX_Quit();
    std::cout<<std::endl;
    return 0;
}  
#include "lib.hpp"

std::string const vers = "0.13.2 mixerf";

bool check;
bool run;
bool done;

std::vector<Uint8> render_draw_color = {0,0,0,0};

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

std::vector<std::vector<done_render>> main_textures(2);
//so the first vector contains structs which are already done animating, and can be merged
//second contains structs which are still in animation
std::vector<base_render> base_render_text;
std::vector<std::string> sty;

/// @brief returns a  random number from s-e (ints)
/// @param s the start int
/// @param e the end int
/// @return a random number from s to e
int math_random(int s, int e) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(s,e);
    return distrib(gen);
}

/// @brief uses this_thread sleep for, genuinly similar to lua task_wait() - accepts floats
/// @param time time (float)
void task_wait(float time) {std::this_thread::sleep_for(std::chrono::milliseconds((int)(time*1000)));}
/// @brief renders textures in main_textures depending on the type
/// @param type 0 = render from the main_textures[0], 1 = corresponding to [1], 2 means render from both
void render_all_textures(int type){
    if (type == 0 or type == 2) for (size_t bob = 0; bob < main_textures[0].size();++bob) SDL_RenderTexture(rend,main_textures[0][bob].tit,0,&main_textures[0][bob].dsts);
    if (type == 1 or type == 2) for (size_t bob = 0; bob < main_textures[1].size();++bob) SDL_RenderTexture(rend,main_textures[1][bob].tit,0,&main_textures[1][bob].dsts);
}
/// @brief  type writer text test
/// @param font  TTF_Font*
/// @param current  std::string
/// @param x float
/// @param y float
/// @param color SDL_color
/// @param cor_X int starting point of the type curve etc, anyhting if type = -1
/// @param cor_Y int starting point of the type curve etc, anything if type = -1
/// @param type int -2; appear but with transparency tween; -1 - just appear; 0 - linear; 1 - bezier curve with 2 points ( will make later)
/// @param time float; time for the linear bezier transparency etc, anything if type = -1
void type_writer(TTF_Font* font, const std::string& current, float x, float y, SDL_Color color, int type, int cor_X, int cor_Y, int time) {
    std::vector<std::string> letters;
    for (char c : current) letters.emplace_back(1, c); 
    float cur_x = x;

    if (type == -1) {
        for (size_t i = 0; i < letters.size(); ++i) {
            SDL_Surface* c_surface = TTF_RenderText_Blended(font, letters[i].c_str(), 0, color);
            if (!c_surface) continue;

            SDL_Texture* c_texture = SDL_CreateTextureFromSurface(rend, c_surface);
            SDL_FRect dst { cur_x, y, (float)c_surface->w, (float)c_surface->h };
            cur_x += c_surface->w;

            SDL_SetRenderDrawColor(rend, render_draw_color[0], render_draw_color[1], render_draw_color[2], render_draw_color[3]);
            SDL_RenderClear(rend);
            main_textures[1].emplace_back(c_texture, dst);
            SDL_RenderTexture(rend, c_texture, nil, &dst);
            render_all_textures(2);
            SDL_DestroySurface(c_surface);

            SDL_RenderPresent(rend);
            task_wait(0.01f);
        }
    }
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
        task_wait(0.016f);
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
            for (int lox = 0; lox < base_render_text.size();++lox) type_writer(font,base_render_text[lox].tix,base_render_text[lox].x,base_render_text[lox].y,base_render_text[lox].clr,-1,math_random(-10,10),math_random(-10,10),1);
        }
        done = true;
    }

    //cleanup

    for (auto& t : main_textures[0] ) SDL_DestroyTexture(t.tit);
    for (auto& t : main_textures[1] ) SDL_DestroyTexture(t.tit);

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    MIX_Quit();
    std::cout<<std::endl;
    return 0;
}  
#include "lib.hpp"


int main(int argc,char* argv[]){

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "cant initialize video = " << SDL_GetError() << '\n';
        return 1;}
    
    SDL_Window* window = SDL_CreateWindow("new_sec",100,100,SDL_WINDOW_BORDERLESS);
    if (!window) {
        std::cerr << "window could not be created = " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;}

    SDL_Renderer* rend = SDL_CreateRenderer(window,nil);
    if (!rend) {
        std::cerr << "renderer could not be created = " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;}

    AVFormatContext* fc = nil;
    if (avformat_open_input(&fc,"D:/vs/code/lego.mp4",nil,nil) < 0) {
        std::cerr << "couldnt open the video file \n";
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(rend);
        SDL_Quit();
        return 1;
    }
    avformat_find_stream_info(fc,nil);

    const AVCodec* codec = nil;
    int vid_id = av_find_best_stream(fc,AVMEDIA_TYPE_VIDEO,-1,-1,&codec,0);
    if (vid_id  < 0) {
        std::cerr << "couldt find a stream \n";
        avformat_close_input(&fc);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(rend);
        SDL_Quit();
        return 1;
    }

    AVCodecContext* cc = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(cc,fc->streams[vid_id]->codecpar);
    if (avcodec_open2(cc,codec,nil)<0) {
        std::cerr << "codec wasnt able to open himself 😭\n";
        avformat_close_input(&fc);
        avcodec_free_context(&cc);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(rend);
        SDL_Quit(); 
        return 1;
    }

    AVPacket* pack = av_packet_alloc();
    AVFrame* frm = av_frame_alloc();
    
    SDL_Texture* img = SDL_CreateTexture(rend,SDL_PIXELFORMAT_IYUV,SDL_TEXTUREACCESS_STREAMING,cc->width,cc->height);
    bool run = true;
    SDL_Event e;

    while (run){
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                std::cout<<"exiting..."<<'\n';
                run = false;
            } 
        }
        if (av_read_frame(fc,pack) >= 0){
            if (pack->stream_index == vid_id) {
                if (avcodec_send_packet(cc,pack) == 0) {
                    while (avcodec_receive_frame(cc,frm) == 0){
                       SDL_UpdateYUVTexture(img, nil,
                            frm->data[0], frm->linesize[0],
                            frm->data[1], frm->linesize[1],
                            frm->data[2], frm->linesize[2]);

                        SDL_RenderClear(rend);
                        SDL_RenderTexture(rend,img,nil,nil);
                        SDL_RenderPresent(rend);

                        SDL_Delay(16);
                    }
                }
            }
            av_packet_unref(pack);
        } else {
            av_seek_frame(fc, vid_id, 0, AVSEEK_FLAG_BACKWARD);
            avcodec_flush_buffers(cc);
        }

    }
    // cleanup
    av_frame_free(&frm);
    av_packet_free(&pack);
    avcodec_free_context(&cc);
    avformat_close_input(&fc);

    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout<<std::endl;
    return 0;
}  
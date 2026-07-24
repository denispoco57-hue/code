#include <iostream>
#include <unordered_map>
#include <string>
#include <chrono>
#include <thread>
#include <string_view>
#include <conio.h>
#include <windows.h>
std::string g_ver = "0.01";

void print_denis(){
    std::cout << R"(                                                                                                
                                          :}<~.                                          .:+]^.                                          
                                          ]@%#}{{@[*:                              .-<{%{}}#@@=                                          
                                         .#@@%##{}[[[}@}+.                     :<%%}[[[{{#%%@@>                                          
                                          }@@@@@@%%##{}[[[{[-              .*{}[[}}{#%%%@@@@@@*                                          
                                               .-+(%@@%%#{}[[{]:         =#}[}}##%%@@{>=:.                                               
                                                      .>@@@%{[}@^       }#[}#@@@}=.                                                      
                                                          .>@@%-         >@@#~                                                           
                                                                                                                                         
                                                                                                                                         
                                                        =%@@@{~          .>%@@%>:                                                        
                                                       {@@< .(@[.       :@@%= -{@^.                                                      
                                                     :{@@@%<+<}@}       )@@@{*+(#@*                                                      
                                                     =@@@@@@@@@@%~     .#@@@@@@@@@#:                                                     
                                                     >@@@%%%%%%@@^     +@@@%%%%%@@@-                                                     
                                                     ^@#}[[[[[}#@*     +@{}[[[[[}%@-                                                     
                                                     -@#))))))<{{      .#}<))))<(%)                                                      
                                                      =@#>**+^}@=       ^@[^*+*<%{.                                                      
                                                       -{@@%%@{:         :[@@%@@<.                                                       
                                                                                                                                         
                                                                                                                                         
                                                                                                                                         
                                                                                                                                         
                                                                                                                                         
                                                                                                                                         
                                                                                                                                         
                                                                                      ~-.                                                
                                                                                    .-+}@@>.                                             
                                                                                   .*[@@@@@@<                                            
                                                                            .~<{@@@%]<<){@@=^@-                                          
                                                           .:-~~==+>)[%@@@@@#[)*+=~+^*>#@@[.  >(.                                        
                                                           .~(@%]()<^+=~-:.    :<~.*[@@@@%:                                              
                                                              -#@[:           .=<(>+^}[%@*                                               
                                                                :[@@*.         .-=.=+]@%~                                                
                                                                   -[@@%>~.      :>%@%+                                                  
                                                                      .:*[@@@@@@@{)-                    )" << std::endl;
}

void task_wait(int time, std::string type) {
    // micro - microseconds,mili - miliseconds, sec - seconds, min - minutes
    if (type == "micro") 
        std::this_thread::sleep_for(std::chrono::microseconds(time));
    else if(type == "mili") 
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    else if (type == "sec") 
        std::this_thread::sleep_for(std::chrono::seconds(time));
    else if (type == "min")
        std::this_thread::sleep_for(std::chrono::minutes(time));
    else 
    std::cout << "bleh" << '\n';
}

int main(){
    std::cout << "Enter a username:" << '\n';
    std::string user = "";
    std::cin >> user;
    if (user == "NeWt0nk") {
        std::cout << "Enter passwords for this user:" << '\n';
        std::string pass = "";
        std::cin >> pass;
        if (pass == "fahk1putinz0") {
            std::cout << "Login completed, starting the loading sequence of ver " << g_ver <<'\n';
            std::cout << "printing denis" << '\n';
            print_denis();
            task_wait(3,"sec");
            // loading function or whatever
        } else
        std::cout << "Wrong password" << '\n';
    } else 
    std::cout << "No such user found" << '\n';
    std::cout << std::endl;
    return 0;
}

#include <iostream>
#include <unordered_map>
#include <string>
#include <chrono>
#include <thread>
#include <string_view>
#include <conio.h>

void task_wait(const std::string& tr, int time) {
    if (tr == "m") 
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    else if (tr == "mm") 
        std::this_thread::sleep_for(std::chrono::microseconds(time));
    else 
        std::this_thread::sleep_for(std::chrono::seconds(time));
    
}

void faileh(int timers, int times) {
    std::cout << "ErR0r --- couldn't grasp the bullshit sack of yours asx#1zx4x@x546tx12x8c" << '\n';
    std::cout << "you will be ganked in " << times <<'\n';
    for (int i = 0; i<=times; ++i) {
        std::cout << i << '\n';
        task_wait("dih",timers);
    }
    std::cout << std::endl;
}

std::string invtext() {
    char ch;
    std::string inpt = "";

    while (true) {

        ch = _getch();
        if (ch == '\r') {
            std::cout<<std::endl;
            break;
        }else if (ch == '\b') {
            if (!inpt.empty()) {
                inpt.pop_back();
                std::cout << "\b \b";
            }
        }else {
            inpt += ch;
            std::cout << "*";
        }
    }

    return inpt;
}

void del_l() {
    std::cout << "\x1b[1A" << "\x1b[2K" << std::flush;
}

int main() {
        std::unordered_map<std::string,std::string> pass;
        pass["NeWt0n"] = "fuhk1all0ruzzianss";

        std::string user;
        std::string pasen;

        std::cout << "enter user" << '\n';
        std::getline(std::cin,user);
        del_l();
        if (pass.find(user) != pass.end()) {
            std::cout << "enter pass" << '\n';
            pasen = invtext();
            if (pass[user] == pasen){
                del_l();
                std::cout<<"Login completed, --> moving onto loading sequence.."<<'\n';
                task_wait("1",5);
            }else
                faileh(1,10);
        
        }
        else{
            faileh(1,5);
        }
        std::cout << std::endl;
    return 0;
}

#include <string>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>

#include <fmod.hpp>
#include "fmod_errors.h"

#include "Sound.h"
#include "IPC.h"

//standard unix headers, need this to get present working directory
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

int main( int argc, char *argv[]) {

    IPC ipc("/tmp/fifo");

    if(!ipc.isOnlyInstance()) {
        if(argv[1] == NULL) {
            ipc.SendMessage("kill");
            return 0;
        } else {
            ipc.SendMessage(argv[1]);
            return 0; 
        }
    } else if(ipc.isOnlyInstance()) {
        
        if(argv[1] == NULL) {
            
            perror("Error: track name invalid");
            return -2;
        }
        
        std::string music_dir;
        const char* home_dir;
        std::string track = argv[1];
        
        //Get home directory of user
        home_dir = getenv("HOME");
        if(home_dir == NULL) {
            //Get home directory if it is not defined in the environment variable
            home_dir = getpwuid(getuid())->pw_dir;
            if(home_dir == NULL) {
                perror("Error: could not get home directory");
                return -1;
            }
        }
        
        music_dir = home_dir;
        music_dir += "/Music/";
        music_dir += track;
        
        Sound song;
        std::cout << "Playing file: " << track << std::endl;
        song.createSound(music_dir.c_str());
        song.playSound(false);
        
        std::string msg;
        bool running = true;
        
        while(song.isPlaying() && running) {
            //GetMessage() is blocking which might need to be changed
            msg = ipc.GetMessage();
            
            //Events
            if(msg == "kill") {
                std::cout << "Killed!" << std::endl;
                running = false;
            }
        }
        
        std::cout << track << " stopped, closing." << std::endl;
        return 0;
    }
}

#ifndef PIPE_H
#define PIPE_H

#include <string>

#define MAX_BUF 1024

class Pipe{

  public:

    //constructor sets home directory for fifo
    Pipe(const char* fifo_dir);
    ~Pipe();

    //returns true if there is only one instance
    bool isOnlyInstance();

    //sends a message to the original process
    void SendMessage(const char* msg);

    //listens for an incomming message and returns it
    std::string GetMessage();

  private:
  
    int pid_fd;
    const char* fifo; //named pipe
    bool onlyInstance;
    char message[MAX_BUF]; //Used to receive messages
};

#endif
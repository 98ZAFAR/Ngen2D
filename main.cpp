#include "platform/SDLApp.h"

int main(){
    SDLApp app;

    if(!app.Init()){
        return -1;
    }

    app.Run();
    app.Shutdown();
    return 0;
}
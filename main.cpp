#include "platform/SDLApp.h"
#include "demo/Sandbox.h"


int main(int argc, char* argv[]){
    SDLApp app;
    Sandbox sandbox;
    SDL_Color boxColor = {255, 0, 0, 255};
    SDL_Color groundColor = {0, 255, 0, 255};

    if(!app.Init())
        return -1;

    while(app.IsRunning()){
        app.HandleEvents(sandbox.GetWorld());
        
        sandbox.Update();
        app.Clear();
        app.Paint(sandbox.GetWorld());
    }

    app.Shutdown();
    return 0;
}
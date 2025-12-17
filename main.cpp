#include "platform/SDLApp.h"
#include "demo/Sandbox.h"

int main(){
    SDLApp app;
    Sandbox sandbox;

    if(!app.Init())
        return -1;

    while(app.IsRunning()){
        app.HandleEvents();
        sandbox.Update();
        app.render();
    }

    app.Shutdown();
    return 0;
}
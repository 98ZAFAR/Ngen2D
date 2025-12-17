#include "platform/SDLApp.h"
#include "demo/Sandbox.h"

int main(int argc, char* argv[]){
    SDLApp app;
    Sandbox sandbox;

    if(!app.Init())
        return -1;

    while(app.IsRunning()){
        app.HandleEvents();
        sandbox.Update();

        app.Clear();
        RigidBody* box = sandbox.GetBox();
        app.DrawRect(box->position.x, box->position.y, 50, 50);
        app.Render();
    }

    app.Shutdown();
    return 0;
}
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
        app.HandleEvents();
        sandbox.Update();

        app.Clear();
        
        RigidBody* box = sandbox.GetBox();
        app.DrawRect(box->position.x, box->position.y, box->size.x, box->size.y, boxColor);
        
        RigidBody* ground = sandbox.GetGround();
        app.DrawRect(ground->position.x, ground->position.y, ground->size.x, ground->size.y, groundColor); // Draw ground
        app.Render();
    }

    app.Shutdown();
    return 0;
}
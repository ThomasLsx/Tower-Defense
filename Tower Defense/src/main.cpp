// main.cpp

/*
#include "Game.h"
int main()
{
    Game game;
    game.run();

    return 0;
}
*/

#include "app.h"

int main()
{

    App* application = new App();

    application->run();

    delete application;

    return 0;
}
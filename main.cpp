#include "game.h"

int main()
{
    Game game;
    while (game.isPlayed())
        game.Tick();
    return 0;
}

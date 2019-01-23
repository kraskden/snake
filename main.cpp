#include "game.h"

int main()
{
    Game game(30, 20);
    while (game.isPlayed())
        game.Tick();
    return 0;
}

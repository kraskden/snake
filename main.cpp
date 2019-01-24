#include "game.h"

int main()
{
    Game game(10, 10);
    while (game.isPlayed())
        game.Tick();
    return 0;
}

#include "game.h"

// TODO:
// 1) VECTOR to MONEY
// 2) ITERATOR TO RANDOM STUFF GENERATOR [MAX LIMIT]

int main()
{
    Game game(10, 10);
    while (game.isPlayed())
        game.Tick();
    return 0;
}

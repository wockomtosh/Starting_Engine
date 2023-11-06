#include <iostream>
#include "MonsterChase.h"

#define MONSTER_CHASE

int main()
{
#ifdef MONSTER_CHASE
    MonsterChase* game = new MonsterChase();
    game->run();
#endif

}
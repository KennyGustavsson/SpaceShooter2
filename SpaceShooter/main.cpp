#include "Game.h"

int main() 
{
    bool Restart = true;

    // Restart is abit thrown together it should not remake the window.
    while (Restart) 
    {
        Game GameInstance = Game();
        Restart = GameInstance.StartGame();
    }

	return 0;
}
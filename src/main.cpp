#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "Flock.hpp"

int main() {
    Flock Flock;

    while (Flock.running())
    {
        Flock.update();

        Flock.render();
    }

    return 0;
}
#include "wall.h"
#include <iostream>

int main() {
    Wall wall(3.5, 2.4);
    std::cout << wall.GetHeight() << " " << wall.GetWidth() << std::endl;
}
#include "wall.h"
#include "builder.h"
#include <iostream>

int main() {
    Builder tom;
    // Попросите Тома посчитать кирпичи для стены 3.5 х 2.45
    Wall wall(3.5, 2.5);
    std::cout << tom.CalcBricksNeeded(wall);
}
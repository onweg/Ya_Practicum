// Определите методы класса здесь
#include "carpenter.h"
#include "square_calculation.h"

int Carpenter::CalcShelves(const Wall& wall) {
    return CalcSquare(wall.GetHeight(), wall.GetWidth()) / 2;
}
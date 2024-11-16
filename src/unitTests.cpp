#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include <GraphTheory.h>

#include "cCellRotating.h"

bool test2()
{
    cGrid grid;
    grid.addRow({'-', 'L'});
    grid.addRow({'-', 'L'});
    grid.startFinish(
        cGrid::sRowColSide(0, 0, 'l'),
        cGrid::sRowColSide(1, 1, 'r'));
    grid.graphEdges();
    return true;
}

bool test1()
{
    auto Grid = gen1();
    Grid.graphEdges();
    return true;
}

bool unitTests()
{
    if (!test2())
        return false;
    if (!test1())
        return false;
    return true;
}

cGrid gen1()
{
    cGrid ret;
    ret.addRow({'L', '-', 'L'});
    ret.addRow({'L', '-', 'L'});
    ret.addRow({'-', '-', 'L'});
    ret.startFinish(
        cGrid::sRowColSide(0, 0, 'l'),
        cGrid::sRowColSide(2, 2, 'r'));
    return ret;
}
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
    auto grid = gen2();
    grid.graphEdges();

    // int ei = grid.findEdge("start", "0_0_2_l");
    // ei = grid.findEdge("0_0_2_l", "0_0_2_b");
    // ei = grid.findEdge("0_0_2_b", "0_0_1_b");
    // ei = grid.findEdge("0_0_1_b", "0_0_0_b");
    // ei = grid.findEdge("0_0_0_b", "1_0_0_t");
    // ei = grid.findEdge("1_0_0_t", "1_0_0_r");
    // ei = grid.findEdge("1_0_0_r", "1_1_0_l");
    // ei = grid.findEdge("1_1_0_l", "1_1_0_r");
    // ei = grid.findEdge("1_1_0_r", "finish");
    if ( ! grid.path().size())
        return false;
    return true;
}

bool test1()
{
    auto Grid = gen1();
    Grid.graphEdges();
    if ( ! Grid.path().size())
        return false;
    return true;
}

bool unitTests()
{
    bool ret = true;
    if (!test2())
        ret = false;
    if (!test1())
        ret = false;
    if (ret)
        std::cout << "Unit Tests passed\n";
    else
    {
        std::cout << "Unit Tests FAILED!!!\n";
        exit(1);
    }
    return ret;
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
cGrid gen2()
{
    cGrid ret;
    ret.addRow({'L', '-'});
    ret.addRow({'L', '-'});
    ret.startFinish(
        cGrid::sRowColSide(0, 0, 'l'),
        cGrid::sRowColSide(1, 1, 'r'));
    return ret;
}
cGrid gen3()
{
    cGrid ret;
    ret.addRow({'L', '-', 'L'});
    ret.addRow({'L', '-', 'L'});
    ret.addRow({'-', 'L', 'L'});
    ret.startFinish(
        cGrid::sRowColSide(0, 0, 'l'),
        cGrid::sRowColSide(2, 2, 'r'));
    return ret;
}
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

// Windex GUI framework  https://github.com/JamesBremner/windex
#include <wex.h>

// Pathfinder graph theory library https://github.com/JamesBremner/PathFinder
#include <GraphTheory.h>


#include "cCellRotating.h"
#include "cGUI.h"

void cGUI::draw(wex::shapes &S)
{
    const int colsize = 50;
    const int rowsize = 20;
    const int xoff = 20;
    int yoff = -myGrid.rowCount() * rowsize;

    // display grid
    // for (int layer = 3; layer >= 0; layer--)
    int layer = 0;
    {
        yoff += myGrid.rowCount() * rowsize;
        // S.text("Layer " + std::to_string(layer),
        //        {0, yoff});
        // yoff += 20;

        for (int row = 0; row < myGrid.rowCount(); row++)
            for (int col = 0; col < myGrid.colCount(); col++)
            {
                std::string st{myGrid.cell(row, col).myType};
                S.text(st,
                       {col * colsize + xoff, row * rowsize + yoff});
            }
    }

    // display path
    yoff += 80;
    auto path = myGrid.path();
    if (!path.size())
    {
        S.text("No path found",
               {xoff, yoff});
        return;
    }

    // S.text("Path through extended grid ( row, col )",
    //        {0, yoff});
    // yoff += 20;
    // for (auto &vertex : myGrid.path())
    // {
    //     S.text(vertex,
    //            {xoff, yoff});
    //     yoff += 20;
    // }
    S.text("Path through grid ( row_col_rot )",
           {0, yoff});
    yoff += 20;
    for (auto &vertex : myGrid.path2D())
    {
        S.text(vertex,
               {xoff, yoff});
        yoff += 20;
    }
}

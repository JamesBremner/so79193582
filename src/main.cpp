#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"

class cCell
{
public:
    char myType;
    cCell(char t)
        : myType(t)
    {
    }
    // Edge connection for specified layer
    // Implements https://github.com/user-attachments/assets/4f5318e5-f4ad-4769-82c3-97b2ed48c5c4
    std::string connects(int layer) const;
};
class cGrid
{
public:
    std::vector<std::vector<cCell>> myCells;
    void addRow(const std::vector<char> &types);
    int rowCount() const;
    int colCount() const;
    const cCell &cell(int row, int col) const;
};

cGrid gen1()
{
    cGrid ret;
    ret.addRow({'L', '-', 'L'});
    ret.addRow({'L', '-', 'L'});
    ret.addRow({'-', '-', 'L'});
    return ret;
}

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Rotating Cells",
              {50, 50, 1000, 500})
    {

        myGrid = gen1();

        fm.events().draw(
            [&](PAINTSTRUCT &ps)
            {
                wex::shapes S(ps);
                draw(S);
            });

        show();
        run();
    }

private:
    cGrid myGrid;

    // draw the edge connections for the layers
    void draw(wex::shapes &S);
};

std::string cCell::connects(int layer) const
{
    switch (myType)
    {
    case 'L':
        switch (layer)
        {
        case 0:
            return "tr";
        case 1:
            return "rb";
        case 2:
            return "bl";
        case 3:
            return "lt";
        default:
            return "";
        }

    case '-':
        switch (layer)
        {
        case 0:
            return "lr";
        case 1:
            return "tb";
        default:
            return "";
        }

    default:
        return "";
    }
}

void cGrid::addRow(const std::vector<char> &types)
{
    if (myCells.size())
    {
        if (types.size() != myCells[0].size())
            throw std::runtime_error(
                "Bad column count");
    }

    std::vector<cCell> row;
    for (char t : types)
    {
        row.emplace_back(t);
    }
    myCells.push_back(row);
}
int cGrid::rowCount() const
{
    return myCells.size();
}
int cGrid::colCount() const
{
    if (!myCells.size())
        return 0;
    return myCells[0].size();
}
const cCell &
cGrid::cell(int row, int col) const
{
    return myCells[row][col];
}

void cGUI::draw(wex::shapes &S)
{
    const int colsize = 50;
    const int rowsize = 20;
    const int xoff = 20;
    int yoff = - myGrid.rowCount() * rowsize;
    for (int layer = 3; layer >= 0; layer--)
    {
        yoff += myGrid.rowCount() * rowsize;
        S.text("Layer " + std::to_string(layer),
               {0, yoff});
        yoff += 20;

        for (int row = 0; row < myGrid.rowCount(); row++)
            for (int col = 0; col < myGrid.colCount(); col++)
            {
                S.text(myGrid.cell(row, col).connects(layer),
                       {col * colsize + xoff, row * rowsize + yoff});
            }
    }
}

main()
{
    cGUI theGUI;
    return 0;
}

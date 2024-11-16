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
    void graphEdges() const;
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
        myGrid.graphEdges();

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
            return "  ";
        }

    case '-':
        switch (layer)
        {
        case 0:
            return "lr";
        case 1:
            return "tb";
        default:
            return "  ";
        }

    default:
        return "  ";
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
    if (row >= 0 && row < rowCount() &&
        col >= 0 && col < colCount())
        return myCells[row][col];
    static cCell null(' ');
    return null;
}

void cGrid::graphEdges() const
{
    std::stringstream ss;

    // cell internal connections
    for (int layer = 3; layer >= 0; layer--)
        for (int row = 0; row < rowCount(); row++)
            for (int col = 0; col < colCount(); col++)
            {
                std::string sc = cell(row, col).connects(layer);
                if (sc == "  ")
                    continue;
                ss << std::to_string(row)
                   << "_" << std::to_string(col)
                   << "_" << std::to_string(layer)
                   << "_" << sc[0] << " "
                   << std::to_string(row)
                   << "_" << std::to_string(col)
                   << "_" << std::to_string(layer)
                   << "_" << sc[1] << " 0\n";
            }

    // layer internal connections
    for (int layer = 3; layer >= 0; layer--)
        for (int row = 0; row < rowCount(); row++)
            for (int col = 0; col < colCount(); col++)
            {
                std::string srcl = std::to_string(row) +
                                   "_" + std::to_string(col) +
                                   "_" + std::to_string(layer);

                int adj_row, adj_col;
                std::string sc, sc_adj;

                // check for connection with cell in same row, next column
                adj_row = row;
                adj_col = col + 1;
                sc = cell(row, col).connects(layer);
                sc_adj = cell(adj_row, adj_col).connects(layer);
                if (sc_adj != "  ")
                {
                    if (sc[0] == 'r' || sc[1] == 'r')
                    {
                        if (sc_adj[0] == 'l' || sc_adj[1] == 'l')
                            ss << srcl
                               << "_r "
                               << std::to_string(adj_row)
                               << "_" << std::to_string(adj_col)
                               << "_" << std::to_string(layer)
                               << "_l 1\n";
                    }
                }

                // check for connection with cell in next row, same column
                adj_row = row + 1;
                adj_col = col;
                sc = cell(row, col).connects(layer);
                sc_adj = cell(adj_row, adj_col).connects(layer);
                if (sc_adj != "  ")
                {
                    if (sc[0] == 'b' || sc[1] == 'b')
                    {
                        if (sc_adj[0] == 't' || sc_adj[1] == 't')
                            ss << srcl
                               << "_b "
                               << std::to_string(adj_row)
                               << "_" << std::to_string(adj_col)
                               << "_" << std::to_string(layer)
                               << "_t 1\n";
                    }
                }
            }

    // connections between layers
    for (int layer1 = 3; layer1 >= 1; layer1--)
    {
        int layer2 = layer1 - 1;
        for (int row = 0; row < rowCount(); row++)
            for (int col = 0; col < colCount(); col++) {
                std::string srcl1 = std::to_string(row) +
                                    "_" + std::to_string(col) +
                                    "_" + std::to_string(layer1);
                std::string srcl2 = std::to_string(row) +
                                    "_" + std::to_string(col) +
                                    "_" + std::to_string(layer2);
                ss << srcl1 << "_t " << srcl2 << "_t 0\n";
                ss << srcl1 << "_r " << srcl2 << "_r 0\n";
                ss << srcl1 << "_b " << srcl2 << "_b 0\n";
                ss << srcl1 << "_l " << srcl2 << "_l 0\n";
            }
    }

    std::cout << ss.str();
}

void cGUI::draw(wex::shapes &S)
{
    const int colsize = 50;
    const int rowsize = 20;
    const int xoff = 20;
    int yoff = -myGrid.rowCount() * rowsize;
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

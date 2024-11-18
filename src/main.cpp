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

#include "cGUI.h"

#include "cCellRotating.h"


cCell::cCell()
{}

cCell::cCell(char t)
    : myType(t)
{
    if (t != 'L' && t != '-') {
        std::string st { t };
        throw std::runtime_error(
            "Bad call type " + st);
    }
}

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
cGrid::cGrid()
{
    myGD.g.directed(false);
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
void cGrid::startFinish(
    const cCellSide &start,
    const cCellSide &finish)
{
    myStart = start;
    myFinish = finish;
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
double cGrid::pathCost() const
{
    return myPath.second;
}
std::vector<std::string>
cGrid::path() const
{
    std::vector<std::string> ret;
    for (int ei : myPath.first)
        ret.push_back(myGD.g.userName(ei));
    return ret;
}
std::vector<std::string>
cGrid::path2D() const
{
    return myPath2D;
}
const cCell &
cGrid::cell(int row, int col) const
{
    if (row >= 0 && row < rowCount() &&
        col >= 0 && col < colCount())
        return myCells[row][col];
    static cCell null;
    return null;
}

void cGrid::makeGraph()
{
    std::stringstream ss;

    myGD.edgeWeight.clear();

    // cell internal connections
    for (int layer = 3; layer >= 0; layer--)
        for (int row = 0; row < rowCount(); row++)
            for (int col = 0; col < colCount(); col++)
            {
                std::string sc = cell(row, col).connects(layer);
                if (sc == "  ")
                    continue;
                std::string sv1 = std::to_string(row) +
                                  "_" + std::to_string(col) +
                                  "_" + std::to_string(layer) +
                                  "_" + sc[0];
                std::string sv2 = std::to_string(row) +
                                  "_" + std::to_string(col) +
                                  "_" + std::to_string(layer) +
                                  "_" + sc[1];
                ss << sv1 << " " << sv2 << " 0\n";
                myGD.g.add(sv1, sv2);
                myGD.edgeWeight.push_back(1);
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
                sc_adj = cell(adj_row, adj_col).connects(layer);
                if (sc_adj != "  ")
                {
                    if (sc_adj[0] == 'l' || sc_adj[1] == 'l')
                    {
                        std::string sv1 = srcl + "_r";
                        std::string sv2 = cCellSide::label(adj_row, adj_col, layer, 'l');
                        // ss << sv1 << " " << sv2 << " 1\n";
                        myGD.g.add(sv1, sv2);
                        myGD.edgeWeight.push_back(1);
                    }
                }

                // check for connection with cell in next row, same column
                adj_row = row + 1;
                adj_col = col;
                sc_adj = cell(adj_row, adj_col).connects(layer);
                if (sc_adj != "  ")
                {
                    if (sc_adj[0] == 't' || sc_adj[1] == 't')
                    {
                        std::string sv1 = srcl + "_b";
                        std::string sv2 = cCellSide::label(adj_row, adj_col, layer, 't');
                        // ss << sv1 << " " << sv2 << " 1\n";
                        myGD.g.add(sv1, sv2);
                        myGD.edgeWeight.push_back(1);
                    }
                }
            }

    // connections between layers
    for (int layer1 = 3; layer1 >= 1; layer1--)
    {
        int layer2 = layer1 - 1;
        for (int row = 0; row < rowCount(); row++)
            for (int col = 0; col < colCount(); col++)
            {
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
                myGD.g.add(srcl1 + "_t", srcl2 + "_t");
                myGD.edgeWeight.push_back(1);
                myGD.g.add(srcl1 + "_t", srcl2 + "_t");
                myGD.edgeWeight.push_back(1);
                myGD.g.add(srcl1 + "_r", srcl2 + "_r");
                myGD.edgeWeight.push_back(1);
                myGD.g.add(srcl1 + "_b", srcl2 + "_b");
                myGD.edgeWeight.push_back(1);
                myGD.g.add(srcl1 + "_l", srcl2 + "_l");
                myGD.edgeWeight.push_back(1);
            }
    }

    // std::cout << ss.str();

    // connect start and finish
    for (int layer = 0; layer < 4; layer++)
    {
        std::string sc = cell(myStart.row, myStart.col).connects(layer);
        if (sc[0] == myStart.side || sc[1] == myStart.side)
        {
            myGD.g.add(
                "start",
                myStart.label(layer));
            myGD.edgeWeight.push_back(1);
        }
        sc = cell(myFinish.row, myFinish.col).connects(layer);
        if (sc[0] == myFinish.side || sc[1] == myFinish.side)
        {
            myGD.g.add(
                "finish",
                 myFinish.label(layer));
            myGD.edgeWeight.push_back(1);
        }
    }
    myGD.startName = "start";
    myGD.endName = "finish";
}

void cGrid::solve()
{
    makeGraph();

    // run Dijkstra algorithm from Pathfinder library
    myPath = raven::graph::path(myGD);

    collapsePath();
}

void cGrid::collapsePath()
{
    auto pathVertexLabels = path();
    std::string prevlabel, prevSide;
    for (auto &label : pathVertexLabels)
    {
        std::string side = label.substr(label.length() - 1);
        if (prevlabel.empty())
        {
            prevlabel = label;
            myPath2D.push_back(label);
            continue;
        }
        if (label == "finish")
        {
            myPath2D.push_back(label);
            return;
        }

        // check if this is an internal cell connection
        if (label.substr(0, label.length() - 2) ==
                prevlabel.substr(0, prevlabel.length() - 2) &&
            side != prevSide)
        {
            myPath2D.push_back(label.substr(0, label.length() - 2));
            std::cout << myPath2D.back() << "\n";
            int dbg = 0;
        }

        prevlabel = label;
        prevSide = side;
    }
}

int cGrid::findEdge(
    const std::string v1,
    const std::string v2) const
{
    return myGD.g.find(v1, v2);
}

main()
{
    unitTests();

    cGUI theGUI;
    return 0;
}

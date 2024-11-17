#pragma once

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
    cGrid();

    void addRow(const std::vector<char> &types);
    struct sRowColSide
    {
        int row;
        int col;
        char side;
        sRowColSide()
        {
        }
        sRowColSide(int r, int c, char s)
            : row(r),
              col(c),
              side(s)
        {
        }
    };
    void startFinish(
        const sRowColSide &start,
        const sRowColSide &finish);

    // construct graph in extended grid
    void graphEdges();
    void collapsePath();

//////////////////  Getters //////////////////

    int rowCount() const;
    int colCount() const;
    const cCell &cell(int row, int col) const;
    int findEdge(
        const std::string v1,
        const std::string v2) const;
    double pathCost() const;
    std::vector<std::string> path() const;
    std::vector<std::string> path2D() const;

private:
    std::vector<std::vector<cCell>> myCells;
    raven::graph::sGraphData myGD;
    sRowColSide myStart;
    sRowColSide myFinish;
    raven::graph::path_cost_t myPath;
    std::vector<std::string> myPath2D;
};

cGrid gen1();
cGrid gen2();

bool unitTests();

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
    std::vector<std::vector<cCell>> myCells;
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

    int rowCount() const;
    int colCount() const;
    const cCell &cell(int row, int col) const;
    void graphEdges();
    double pathCost() const;

private:
    raven::graph::sGraphData myGD;
    sRowColSide myStart;
    sRowColSide myFinish;
    raven::graph::path_cost_t myPath;
};

cGrid gen1();
cGrid gen2();

bool unitTests();

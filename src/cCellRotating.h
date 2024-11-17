#pragma once

/// A square cell that connects two of its sides
class cCell
{
public:
    char myType;

    /** CTOR
     * @param[in] t the type.  '-' connects opposite sides, 'L' connects adjacent sides
     * 
     * Exception thrown if t is not '-' or 'L'
     */
    cCell(char t);

    /** Edge connections for specified layer
     * @param[in] layer number of rotations
     * @return 2 char string with codes for connected sides, "  " if rotation number not valid
     * 
     * side codes are t, r, b, l ( top, left, bottom, right )
     * e.g. "lr" means the left and right sides are connected
     * 
     * Implements https://github.com/user-attachments/assets/4f5318e5-f4ad-4769-82c3-97b2ed48c5c4
     * 
     */
    std::string connects(int layer) const;
};

/// @brief  A 2D grid of rotating cells
class cGrid
{
public:


//////////////////  Setters //////////////////
    cGrid();

/** Add row to grid
 * @param[in] types vector of chars giving cell types in row
 * 
 * exception thrown if column count not equal to first row added
 */
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

    /** Specify starting and finishing cell sides
     * 
     * @param[in] start 
     * @param[in] finish 
     */
    void startFinish(
        const sRowColSide &start,
        const sRowColSide &finish);

//////////////////  Solver //////////////////

    // find path through grid of rotating cells
    void solve();


//////////////////  Getters //////////////////

    /// Number of rows
    int rowCount() const;

    /// Number of columns
    int colCount() const;

    /// @brief cell at location in zeroth layer
    /// @param row 
    /// @param col 
    /// @return const ref to cell

    const cCell &cell(int row, int col) const;

    /// @brief index of graph edge
    /// @param v1 vertex label
    /// @param v2 vertex label
    /// @return edge index

    int findEdge(
        const std::string v1,
        const std::string v2) const;

    double pathCost() const;

    /// @brief 3D path
    /// @return vector of vertex labels ( row_col_rot_side )

    std::vector<std::string> path() const;

    /// @brief 2D path
    /// @return vector of vertex labels ( row_col_rot )
    /// rot is the number of 90 dgree clockwise rotations required for path
    
    std::vector<std::string> path2D() const;

private:
    std::vector<std::vector<cCell>> myCells;    // input grid of cells extended to three dimensions
    raven::graph::sGraphData myGD;              // equivalent graph ( vertices and edges )
    sRowColSide myStart;                        // starting cell side
    sRowColSide myFinish;                       // finishing cell side
    raven::graph::path_cost_t myPath;           // 3d path
    std::vector<std::string> myPath2D;          // 2d path with rotations

    void collapsePath();
};

// generate example grids
cGrid gen1();
cGrid gen2();
cGrid gen3();

// run unit tests
bool unitTests();

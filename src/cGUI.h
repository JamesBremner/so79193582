#include "cCellRotating.h"

class cGUI
{
public:
    cGUI();

private:
    wex::gui &fm;
    wex::menu * myFileMenu;

    cGrid myGrid;

    void menus();

    // draw the edge connections for the layers
    void draw(wex::shapes &S);
};
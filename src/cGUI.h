#include "cCellRotating.h"

class cStarterGUI
{
public:
    /** CTOR
     * @param[in] title will appear in application window title
     * @param[in] vlocation set location and size of appplication window
     */
    cStarterGUI(
        const std::string &title,
        const std::vector<int> &vlocation)
        : fm(wex::maker::make())
    {
        fm.move(vlocation);
        fm.text(title);

    }
    void show()
    {
        fm.show();
    }
    void run()
    {
        fm.run();
    }

protected:
    wex::gui &fm;
};
class cGUI : public cStarterGUI
{
public:
    cGUI();

private:
    cGrid myGrid;
    wex::menu * myFileMenu;

    void menus();

    // draw the edge connections for the layers
    void draw(wex::shapes &S);
};
#ifndef MENU_H
#define MENU_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <list>

typedef unsigned int actionID_t;

class Tree {
  public:
    // Constructor
    Tree();

    // Specify which screen to draw on
    Adafruit_SSD1306 *screen;
    void addScreen(Adafruit_SSD1306 *display) { screen=display; };

    // ID, label and callback function
    actionID_t ID;
    char text[32];
    void (*callback) (actionID_t);

    // Building up the menu structure
    Tree* addItem(const actionID_t ID, const char *text, void (*callback) (actionID_t));

    // Subrows
    std::vector<Tree> subItems; 

    // Change the selection
    int16_t selectedItem;
    Tree* select(int16_t item) { selectedItem=item; return &subItems[item]; };
    Tree* selection()          { return &subItems[selectedItem]; };
    void next()                { selectedItem++; if(selectedItem>subItems.size()-1) selectedItem=0; };
    void previous()            { selectedItem--; if(selectedItem<0) selectedItem=subItems.size()-1; };
    void triggerCallback();

    // Width and location of the current item
    uint16_t width;
    uint16_t pos;

    // Scrolling of the subrow such that the selected item is visible
    int16_t offset;            // Determines how much the row must be moved left or right
    int16_t lowerLimit=  5;    // Determines how near the selected item can come to the left 
    int16_t upperLimit=125;    // Determines how near the selected item can come to the right
    void alignPositions();

   // Drawing the row on the screen
    void draw(int16_t y);

 };

#endif // MENU_H

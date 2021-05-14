#ifndef DISPLAY_H
#define DISPLAY_H

#include <U8g2lib.h>
#include <list>
#include "gui.h"

using namespace std;

enum tSequence {tsMainWindow, tsGotoTopRow, tsGotoSecondRow};

class tDisplay {

  public:
    // Main canvas
    tGroup  wnMain; 
    tLabel  lbTime; 
  
    // Top row with first row of buttons
    tGroup  rwMain;     
    tButton btBrightness;  
    tButton btColor;
    tButton btTimer;
    tButton btBack;
  
    // Second row on brightness
    tGroup rwBrightness;
    tButton btBrightness25;
    tButton btBrightness35;
    tButton btBrightness50;
    tButton btBrightness70;
    tButton btBrightness100;
    
    // Second row on color
    tGroup  rwColor;
    tButton btColorRed;
    tButton btColorOrange;
    tButton btColorYellow;
    tButton btColorWhite;
  
    // Sequencing the movements
    tSequence sequence;
    bool ready;
    int selectedMain, selectedBrightness, selectedColor, selectedTimer;
  
    tDisplay(U8G2 &u8g2);
    void step();
    void display(U8G2 &u8g2) { wnMain.draw(u8g2); };
    void mainWindow();
    void showTopRow();
    void showSecondRow();
    
};

#endif // DISPLAY_H

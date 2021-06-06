#ifndef DISPLAY_H
#define DISPLAY_H

#include "data.h"
#include "gui.h"
#include <U8g2lib.h>
#include <list>

using namespace std;

class tDisplay {

  public:
    // Main canvas
    tGroup  grpMain; 
    tLabel  lblTime; 
  
    // Top row with first row of buttons
    tGroup  rowMain;     
    tButton btnBrightness;  
    tButton btnColor;
    tButton btnTimer;
    tButton btnBack;
  
    // Second row on brightness
    tGroup  rowBrightness;
    tButton btnBrightness25;
    tButton btnBrightness35;
    tButton btnBrightness50;
    tButton btnBrightness70;
    tButton btnBrightness100;
    tButton btnBrightnessBack;
    
    // Second row on color
    tGroup  rowColor;
    tButton btnColorWhite;
    tButton btnColorYellow;
    tButton btnColorOrange;
    tButton btnColorRed;
    tButton btnColorBack;

    // Second row on timer  
    tGroup rowTimer;
    tButton btnTimer03;
    tButton btnTimer05;
    tButton btnTimer10;
    tButton btnTimer20;
    tButton btnTimerOff;
    tButton btnTimerBack;

    // Sequencing the movements
    bool ready() { return grpMain.ready(); };

    // Current selection in the menu
    int8_t selectedRow;
    tMenuItem mainMenu, brightnessMenu, colorMenu, timerMenu;

    // Input from buttons
    void nextButton();
    tMenuItem selectButton();
  
    tDisplay(U8G2 &u8g2);
    void step();
    void display(U8G2 &u8g2);
    void showMain();
    void showTopRow();
    void showSecondRow();

  protected:
    void addToRow(U8G2 &u8g2, tGroup* row, tButton* button, tButton* prevButton, string label, uint32_t ID);
    void selectNextItem(tGroup* row, tButton* btnCurrent, tButton* btnNew, tButton* btnRight);
}; // tDisplay

#endif // DISPLAY_H

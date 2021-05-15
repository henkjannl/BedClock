#ifndef DISPLAY_H
#define DISPLAY_H

#include "data.h"
#include "gui.h"
#include <U8g2lib.h>
#include <list>

using namespace std;

enum tMainMenu       { mmBrightness, mmColor, mmTimer, mmBack};
enum tBrightnessMenu { bm25, bm35, bm50, bm70, bm100, bmBack};
enum tColorMenu      { cmWhite, cmYellow, cmOrange, cmRed, cmBack};
enum tTimerMenu      { tm3, tm5, tm10, tm20, tmOff, tmBack};

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
    tMainMenu       mainMenu;
    tBrightnessMenu brightnessMenu;
    tColorMenu      colorMenu;
    tTimerMenu      timerMenu;

    // Currently selected row
    int selectedRow;

    // Input from buttons
    void nextButton();
    void selectButton(tData &data);
  
    tDisplay(U8G2 &u8g2);
    void step();
    void display(U8G2 &u8g2) { grpMain.draw(u8g2); };
    void showMain();
    void showTopRow();
    void showSecondRow();

  protected:
    void addToRow(U8G2 &u8g2, tGroup* row, tButton* button, tButton* prevButton, string label);
    void selectNextItem(tGroup* row, tButton* btnCurrent, tButton* btnNew, tButton* btnRight);
};

/* ===============================================================================//
// SINCE ARDUINO DOES NOT SEEM TO LIKE SEPARATE .H AND .CPP FILES, .CPP GOES HERE //
// ===============================================================================*/

tDisplay::tDisplay(U8G2 &u8g2) {
  // Clock
  lblTime.setFont(u8g2_font_crox5hb_tr);  
  lblTime.setText(u8g2, "12:34");
  grpMain.addChild(lblTime);

  // Main row
  rowMain.setY(32);
  grpMain.addChild(rowMain);

  addToRow(u8g2, &rowMain, &btnBrightness, NULL,           "BRIGHTNESS");
  addToRow(u8g2, &rowMain, &btnColor,      &btnBrightness, "COLOR"     );
  addToRow(u8g2, &rowMain, &btnTimer,      &btnColor,      "TIMER"     );
  addToRow(u8g2, &rowMain, &btnBack,       &btnTimer,      "BACK"      );

  // Row with brightness values  
  rowBrightness.setY(32);
  grpMain.addChild(rowBrightness);

  addToRow(u8g2, &rowBrightness, &btnBrightness25,   NULL,              "25%" );
  addToRow(u8g2, &rowBrightness, &btnBrightness35,   &btnBrightness25,  "35%" );
  addToRow(u8g2, &rowBrightness, &btnBrightness50,   &btnBrightness35,  "50%" );
  addToRow(u8g2, &rowBrightness, &btnBrightness70,   &btnBrightness50,  "70%" );
  addToRow(u8g2, &rowBrightness, &btnBrightness100,  &btnBrightness70,  "100%");
  addToRow(u8g2, &rowBrightness, &btnBrightnessBack, &btnBrightness100, "BACK");

  // Row with color values  
  rowColor.setY(32);
  grpMain.addChild(rowColor);

  addToRow(u8g2, &rowColor, &btnColorWhite,  NULL,            "WHITE" );
  addToRow(u8g2, &rowColor, &btnColorYellow, &btnColorWhite,  "YELLOW");
  addToRow(u8g2, &rowColor, &btnColorOrange, &btnColorYellow, "ORANGE");
  addToRow(u8g2, &rowColor, &btnColorRed,    &btnColorOrange, "RED"   );
  addToRow(u8g2, &rowColor, &btnColorBack,   &btnColorRed,    "BACK"  );

  // Row with timer values  
  rowTimer.setY(32);
  grpMain.addChild(rowTimer);

  addToRow(u8g2, &rowTimer, &btnTimer03,   NULL,         "3 min" );
  addToRow(u8g2, &rowTimer, &btnTimer05,   &btnTimer03,  "5 min" );
  addToRow(u8g2, &rowTimer, &btnTimer10,   &btnTimer05,  "10 min");
  addToRow(u8g2, &rowTimer, &btnTimer20,   &btnTimer10,  "20 min");
  addToRow(u8g2, &rowTimer, &btnTimerOff,  &btnTimer20,  "OFF"   );
  addToRow(u8g2, &rowTimer, &btnTimerBack, &btnTimerOff, "BACK"  );

  // Current selection in the menu
  mainMenu=mmBrightness;
  brightnessMenu=bm25;
  colorMenu=cmWhite;
  timerMenu=tm3;

  // Currently selected row
  selectedRow=0;
};

void tDisplay::step() {
  grpMain.step();
};

void tDisplay::showMain() {
  grpMain.moveY(0);
  rowMain.moveY(33);
  rowBrightness.moveY(33);
};

void tDisplay::showTopRow() {
  rowMain.moveY(    lblTime.getHeight()-2);
  grpMain.moveY(31-(lblTime.getHeight()-2+rowMain.getHeight()));

  int safeY=lblTime.getHeight()-2+2*rowMain.getHeight()+2;
  rowBrightness.moveY(safeY);
  rowColor     .moveY(safeY);
  rowTimer     .moveY(safeY);
};

void tDisplay::showSecondRow() {
  rowMain.moveY(lblTime.getHeight()-2);
  if     (mainMenu==mmBrightness) rowBrightness.moveY(lblTime.getHeight()-2+rowMain.getHeight()-1);
  else if(mainMenu==mmColor     ) rowColor     .moveY(lblTime.getHeight()-2+rowMain.getHeight()-1);
  else if(mainMenu==mmTimer     ) rowTimer     .moveY(lblTime.getHeight()-2+rowMain.getHeight()-1);
  grpMain.moveY(31-(lblTime.getHeight()-2+rowMain.getHeight()+rowBrightness.getHeight()));
};

void tDisplay::addToRow(U8G2 &u8g2, tGroup* row, tButton* button, tButton* prevButton, string label) {
  button->setText(u8g2, label); 
  if(prevButton==NULL)
    button->selected=true;
  else
    button->setX(prevButton->localRight()-1);
  row->addChild(*button);
};

void tDisplay::selectNextItem(tGroup* row, tButton* btnCurrent, tButton* btnNew, tButton* btnRight) {
  int dx;

  btnCurrent->selected=false;
  btnNew->selected=true;

  // Move the row so that the selected item and preferably the next one are visible
  dx=0;
  
  if( (btnRight!=NULL) && ( (btnRight->globalRight()+3) > 128) ) dx= 128-(btnRight->globalRight()+3);
  if(                       (btnNew->globalX()+dx       <   0) ) dx= -btnNew->globalX();

  row->moveX(row->globalX()+dx);
};

void tDisplay::nextButton() {
  
  if(selectedRow<0) {
    selectedRow=0;
    showTopRow();
  }
  else if(selectedRow==0) {
    if     (mainMenu==mmBrightness) { selectNextItem(&rowMain, &btnBrightness, &btnColor,      &btnTimer); mainMenu=mmColor;      }
    else if(mainMenu==mmColor     ) { selectNextItem(&rowMain, &btnColor,      &btnTimer,      &btnBack ); mainMenu=mmTimer;      }
    else if(mainMenu==mmTimer     ) { selectNextItem(&rowMain, &btnTimer,      &btnBack,       NULL     ); mainMenu=mmBack;       }
    else if(mainMenu==mmBack      ) { selectNextItem(&rowMain, &btnBack,       &btnBrightness, &btnColor); mainMenu=mmBrightness; };
    showTopRow();
  } // selectedRow==0
  else {
    switch(mainMenu) {
      case mmBrightness:
        if      (brightnessMenu==bm25  ) { selectNextItem(&rowBrightness, &btnBrightness25  , &btnBrightness35  , &btnBrightness50  ); brightnessMenu=bm35;   }
        else if (brightnessMenu==bm35  ) { selectNextItem(&rowBrightness, &btnBrightness35  , &btnBrightness50  , &btnBrightness70  ); brightnessMenu=bm50;   }
        else if (brightnessMenu==bm50  ) { selectNextItem(&rowBrightness, &btnBrightness50  , &btnBrightness70  , &btnBrightness100 ); brightnessMenu=bm70;   }
        else if (brightnessMenu==bm70  ) { selectNextItem(&rowBrightness, &btnBrightness70  , &btnBrightness100 , &btnBrightnessBack); brightnessMenu=bm100;  }
        else if (brightnessMenu==bm100 ) { selectNextItem(&rowBrightness, &btnBrightness100 , &btnBrightnessBack, NULL              ); brightnessMenu=bmBack; }
        else if (brightnessMenu==bmBack) { selectNextItem(&rowBrightness, &btnBrightnessBack, &btnBrightness25  , &btnBrightness35  ); brightnessMenu=bm25;   };
        break;

      case mmColor:
        if      (colorMenu==cmWhite ) { selectNextItem(&rowColor, &btnColorWhite , &btnColorYellow, &btnColorOrange); colorMenu=cmYellow; }
        else if (colorMenu==cmYellow) { selectNextItem(&rowColor, &btnColorYellow, &btnColorOrange, &btnColorRed   ); colorMenu=cmOrange; }
        else if (colorMenu==cmOrange) { selectNextItem(&rowColor, &btnColorOrange, &btnColorRed   , &btnColorBack  ); colorMenu=cmRed;    }
        else if (colorMenu==cmRed   ) { selectNextItem(&rowColor, &btnColorRed   , &btnColorBack  , NULL           ); colorMenu=cmBack;   }
        else if (colorMenu==cmBack  ) { selectNextItem(&rowColor, &btnColorBack  , &btnColorWhite , &btnColorYellow); colorMenu=cmWhite;  };
        break;

      case mmTimer:
        if      (timerMenu==tm3   ) { selectNextItem(&rowTimer, &btnTimer03  , &btnTimer05,   &btnTimer10);   timerMenu=tm5;    }
        else if (timerMenu==tm5   ) { selectNextItem(&rowTimer, &btnTimer05  , &btnTimer10,   &btnTimer20);   timerMenu=tm10;   }
        else if (timerMenu==tm10  ) { selectNextItem(&rowTimer, &btnTimer10  , &btnTimer20,   &btnTimerOff);  timerMenu=tm20;   }
        else if (timerMenu==tm20  ) { selectNextItem(&rowTimer, &btnTimer20  , &btnTimerOff,  &btnTimerBack); timerMenu=tmOff;  }
        else if (timerMenu==tmOff ) { selectNextItem(&rowTimer, &btnTimerOff , &btnTimerBack, NULL);          timerMenu=tmBack; }
        else if (timerMenu==tmBack) { selectNextItem(&rowTimer, &btnTimerBack, &btnTimer03,   &btnTimer03);   timerMenu=tm3;    };
        break;

    } // switch mainMenu
    showSecondRow();
    
  }; // selectedRow!=0
};

void tDisplay::selectButton(tData &data) {
  
  if(selectedRow==-1) {
    selectedRow=0;
    showTopRow();
  } // selectedRow==-1
  
  else if(selectedRow== 0) {
    if(mainMenu==mmBack) { 
      selectedRow=-1; 
      showMain();
    } // mainMenu==mmBack
    else {
      selectedRow=1; 
      showSecondRow();
    } // mainMenu!=mmBack
  } // selectedRow==0
  
  else { // selectedRow==1

    portENTER_CRITICAL(&dataAccessMux);

    switch(mainMenu) {
      
      case mmBrightness:

        if      (brightnessMenu==bm25  ) data.lightIntensity=li25;
        else if (brightnessMenu==bm35  ) data.lightIntensity=li35;
        else if (brightnessMenu==bm50  ) data.lightIntensity=li50;
        else if (brightnessMenu==bm70  ) data.lightIntensity=li70;
        else if (brightnessMenu==bm100 ) data.lightIntensity=li100;
   
        break;
        
      case mmColor: 

        if      (colorMenu==cmWhite  ) data.lightColor = lcWhite;
        else if (colorMenu==cmYellow ) data.lightColor = lcYellow;
        else if (colorMenu==cmOrange ) data.lightColor = lcOrange;
        else if (colorMenu==cmRed    ) data.lightColor = lcRed;
        
        break;
        
      case mmTimer: 

        if      (timerMenu==tm3  ) data.timerDuration = td03;
        else if (timerMenu==tm5  ) data.timerDuration = td05;
        else if (timerMenu==tm10 ) data.timerDuration = td10; 
        else if (timerMenu==tm20 ) data.timerDuration = td20; 
        else if (timerMenu==tmOff) data.timerDuration = tdOff;
                
        break;
        
      // mmBack is not a likely case. We could not have entered the second row
                      
    }; // switch mainMenu

    portEXIT_CRITICAL(&dataAccessMux);

    // Whatever we selected, we can go back to the top row now
    selectedRow=0; 
    showTopRow();
      
  }; // selectedRow>0

};

#endif // DISPLAY_H

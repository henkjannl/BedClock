#include "display.h"
#include <U8g2lib.h>
#include <list>

using namespace std;

tDisplay::tDisplay(U8G2 &u8g2) {

  // Set selected items
  selectedRow=-1;
  mainMenu=mainBrightness;
  brightnessMenu=brightness25;
  colorMenu=colorWhite;
  timerMenu=timer03;

  // Clock
  //lblTime.setFont(u8g2_font_crox5hb_tr);  
  lblTime.setFont(u8g2_font_fur17_tr);
  lblTime.setText(u8g2, "--:--");
  grpMain.addChild(lblTime);

  // Main row
  rowMain.setY(33);
  grpMain.addChild(rowMain);
                 
  addToRow(u8g2, &rowMain, &btnBrightness, NULL,           "BRIGHTNESS", mainBrightness);
  addToRow(u8g2, &rowMain, &btnColor,      &btnBrightness, "COLOR"     , mainColor     );
  addToRow(u8g2, &rowMain, &btnTimer,      &btnColor,      "TIMER"     , mainTimer     );
  addToRow(u8g2, &rowMain, &btnBack,       &btnTimer,      "BACK"      , mainBack      );

  // Row with brightness values  
  rowBrightness.setY(33);
  grpMain.addChild(rowBrightness);

  addToRow(u8g2, &rowBrightness, &btnBrightness25,   NULL,              "25%" , brightness25);
  addToRow(u8g2, &rowBrightness, &btnBrightness35,   &btnBrightness25,  "35%" , brightness35);
  addToRow(u8g2, &rowBrightness, &btnBrightness50,   &btnBrightness35,  "50%" , brightness50);
  addToRow(u8g2, &rowBrightness, &btnBrightness70,   &btnBrightness50,  "70%" , brightness70);
  addToRow(u8g2, &rowBrightness, &btnBrightness100,  &btnBrightness70,  "100%", brightness100);
  addToRow(u8g2, &rowBrightness, &btnBrightnessBack, &btnBrightness100, "BACK", brightnessBack);

  // Row with color values  
  rowColor.setY(33);
  grpMain.addChild(rowColor);

  addToRow(u8g2, &rowColor, &btnColorWhite,  NULL,            "WHITE" , colorWhite );
  addToRow(u8g2, &rowColor, &btnColorYellow, &btnColorWhite,  "YELLOW", colorYellow);
  addToRow(u8g2, &rowColor, &btnColorOrange, &btnColorYellow, "ORANGE", colorOrange);
  addToRow(u8g2, &rowColor, &btnColorRed,    &btnColorOrange, "RED"   , colorRed   );
  addToRow(u8g2, &rowColor, &btnColorBack,   &btnColorRed,    "BACK"  , colorBack  );

  // Row with timer values  
  rowTimer.setY(33);
  grpMain.addChild(rowTimer);

  addToRow(u8g2, &rowTimer, &btnTimer03,   NULL,         "3 min" , timer03  );
  addToRow(u8g2, &rowTimer, &btnTimer05,   &btnTimer03,  "5 min" , timer05  );
  addToRow(u8g2, &rowTimer, &btnTimer10,   &btnTimer05,  "10 min", timer10  );
  addToRow(u8g2, &rowTimer, &btnTimer20,   &btnTimer10,  "20 min", timer20  );
  addToRow(u8g2, &rowTimer, &btnTimerOff,  &btnTimer20,  "OFF"   , timerOff );
  addToRow(u8g2, &rowTimer, &btnTimerBack, &btnTimerOff, "BACK"  , timerBack);
}; // tDisplay::tDisplay

void tDisplay::step() {
  grpMain.step();
}; // tDisplay::step

void tDisplay::display(U8G2 &u8g2) { 
  struct tm timeinfo;
  if(getLocalTime(&timeinfo)){
    char currentTime[6];
    strftime(currentTime,sizeof(currentTime),"%H:%M",&(timeinfo)); // Hour 00-23 + Minute 00-59
    lblTime.setText(u8g2, currentTime);
  }
  
  grpMain.draw(u8g2); 
}; // tDisplay::display

void tDisplay::showMain() {
  selectedRow=-1;
  grpMain.moveY(0);
  rowMain.moveY(33);
  rowBrightness.moveY(33);
  rowColor     .moveY(33);
  rowTimer     .moveY(33);
}; // tDisplay::showMain

void tDisplay::showTopRow() {
  selectedRow=0;

  int16_t row0y = lblTime.getHeight()-2;
  int16_t row1y = lblTime.getHeight()-2+  rowMain.getHeight()-1;
  int16_t row2y = lblTime.getHeight()-2+2*rowMain.getHeight()-1;

  rowMain.moveY(row0y);
  grpMain.moveY(31-(row0y+rowMain.getHeight()));

  rowBrightness.moveY(row2y);
  rowColor     .moveY(row2y);
  rowTimer     .moveY(row2y);
}; // tDisplay::showTopRow

void tDisplay::showSecondRow() {
  selectedRow=1;
  
  int16_t row0y = lblTime.getHeight()-2;
  int16_t row1y = lblTime.getHeight()-2+  rowMain.getHeight()-1;
  int16_t row2y = lblTime.getHeight()-2+2*rowMain.getHeight()-1;

  rowMain.moveY(row0y);
  
  rowBrightness.moveY( (mainMenu==mainBrightness) ? row1y : row2y);
  rowColor     .moveY( (mainMenu==mainColor     ) ? row1y : row2y);
  rowTimer     .moveY( (mainMenu==mainTimer     ) ? row1y : row2y);
  
  grpMain.moveY(31-(row1y+rowBrightness.getHeight()));
}; // tDisplay::showSecondRow

void tDisplay::addToRow(U8G2 &u8g2, tGroup* row, tButton* button, tButton* prevButton, string label, uint32_t ID) {
  button->setText(u8g2, label); 
  button->ID=ID;
  
  if(prevButton==NULL)
    button->selected=true;
  else
    button->setX(prevButton->localRight()-1);
  row->addChild(*button);
}; // tDisplay::addToRow

void tDisplay::selectNextItem(tGroup* row, tButton* btnCurrent, tButton* btnNew, tButton* btnRight) {
  int dx;

  btnCurrent->selected=false;
  btnNew->selected=true;

  // Move the row so that the selected item and preferably the next one are visible
  dx=0;
  
  if( (btnRight!=NULL) && ( (btnRight->globalRight()+3) > 128) ) dx= 128-(btnRight->globalRight()+3);
  if(                       (btnNew->globalX()+dx       <   0) ) dx= -btnNew->globalX();

  row->moveX(row->globalX()+dx);
}; // tDisplay::selectNextItem

void tDisplay::nextButton() {
  
  if(selectedRow<0) {
    showTopRow();
  }
  else if(selectedRow==0) {
    if     (mainMenu==mainBrightness) { selectNextItem(&rowMain, &btnBrightness, &btnColor,      &btnTimer); mainMenu=mainColor;      }
    else if(mainMenu==mainColor     ) { selectNextItem(&rowMain, &btnColor,      &btnTimer,      &btnBack ); mainMenu=mainTimer;      }
    else if(mainMenu==mainTimer     ) { selectNextItem(&rowMain, &btnTimer,      &btnBack,       NULL     ); mainMenu=mainBack;       }
    else if(mainMenu==mainBack      ) { selectNextItem(&rowMain, &btnBack,       &btnBrightness, &btnColor); mainMenu=mainBrightness; };
    showTopRow();
  } // selectedRow==0
  else {
    switch(mainMenu) {
      case mainBrightness:
        if      (brightnessMenu==brightness25  ) { selectNextItem(&rowBrightness, &btnBrightness25  , &btnBrightness35  , &btnBrightness50  ); brightnessMenu=brightness35;   }
        else if (brightnessMenu==brightness35  ) { selectNextItem(&rowBrightness, &btnBrightness35  , &btnBrightness50  , &btnBrightness70  ); brightnessMenu=brightness50;   }
        else if (brightnessMenu==brightness50  ) { selectNextItem(&rowBrightness, &btnBrightness50  , &btnBrightness70  , &btnBrightness100 ); brightnessMenu=brightness70;   }
        else if (brightnessMenu==brightness70  ) { selectNextItem(&rowBrightness, &btnBrightness70  , &btnBrightness100 , &btnBrightnessBack); brightnessMenu=brightness100;  }
        else if (brightnessMenu==brightness100 ) { selectNextItem(&rowBrightness, &btnBrightness100 , &btnBrightnessBack, NULL              ); brightnessMenu=brightnessBack; }
        else if (brightnessMenu==brightnessBack) { selectNextItem(&rowBrightness, &btnBrightnessBack, &btnBrightness25  , &btnBrightness35  ); brightnessMenu=brightness25;   };
        break;

      case mainColor:
        if      (colorMenu==colorWhite ) { selectNextItem(&rowColor, &btnColorWhite , &btnColorYellow, &btnColorOrange); colorMenu=colorYellow; }
        else if (colorMenu==colorYellow) { selectNextItem(&rowColor, &btnColorYellow, &btnColorOrange, &btnColorRed   ); colorMenu=colorOrange; }
        else if (colorMenu==colorOrange) { selectNextItem(&rowColor, &btnColorOrange, &btnColorRed   , &btnColorBack  ); colorMenu=colorRed;    }
        else if (colorMenu==colorRed   ) { selectNextItem(&rowColor, &btnColorRed   , &btnColorBack  , NULL           ); colorMenu=colorBack;   }
        else if (colorMenu==colorBack  ) { selectNextItem(&rowColor, &btnColorBack  , &btnColorWhite , &btnColorYellow); colorMenu=colorWhite;  };
        break;

      case mainTimer:
        if      (timerMenu==timer03  ) { selectNextItem(&rowTimer, &btnTimer03  , &btnTimer05,   &btnTimer10);   timerMenu=timer05;   }
        else if (timerMenu==timer05  ) { selectNextItem(&rowTimer, &btnTimer05  , &btnTimer10,   &btnTimer20);   timerMenu=timer10;   }
        else if (timerMenu==timer10  ) { selectNextItem(&rowTimer, &btnTimer10  , &btnTimer20,   &btnTimerOff);  timerMenu=timer20;   }
        else if (timerMenu==timer20  ) { selectNextItem(&rowTimer, &btnTimer20  , &btnTimerOff,  &btnTimerBack); timerMenu=timerOff;  }
        else if (timerMenu==timerOff ) { selectNextItem(&rowTimer, &btnTimerOff , &btnTimerBack, NULL);          timerMenu=timerBack; }
        else if (timerMenu==timerBack) { selectNextItem(&rowTimer, &btnTimerBack, &btnTimer03,   &btnTimer03);   timerMenu=timer03;   };
        break;

    } // switch mainMenu
    showSecondRow();
    
  }; // selectedRow!=0
}; // tDisplay::nextButton

tMenuItem tDisplay::selectButton() {
  
  if(selectedRow==-1) { 
    // Clock was selected, just display first row but do not change selection
    showTopRow();
    return mainMenu;
  } // selectedRow==-1
  
  else if(selectedRow== 0) {
    if(mainMenu==mainBack) { 
      showMain();
      return mainClock;
    } // mainMenu==mainBack
    else {
      showSecondRow();
      return mainMenu;
    } // mainMenu!=mmBack
  } // selectedRow==0
  
  else { // selectedRow==1

    switch(mainMenu) {
      case mainBrightness:
        if(brightnessMenu==brightnessBack) {
          showTopRow();
          return mainMenu;
        }
        else {
          showTopRow();
          return brightnessMenu;
        }
        break;

      case mainColor:
        if(colorMenu==colorBack) {
          showTopRow();
          return mainMenu;
        }
        else {
          showTopRow();
          return colorMenu;
        }
        break;

      case mainTimer:
        if(timerMenu==timerBack) {
          showTopRow();
          return mainMenu;
        }
        else {
          showTopRow();
          return timerMenu;
        }
        break;
    }; // switch case  
  }; // selectedRow>0
}; // tDisplay::selectButton

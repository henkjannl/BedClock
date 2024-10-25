
#include "display.h"

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

  addToRow(u8g2, &rowBrightness, &btnBrightness25,   NULL,             "25%" );
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

  
  if(selectedRow<0) showMain();
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

void tDisplay::selectButton() {
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
    switch(mainMenu) {
      
      case mmBrightness:

        if      (brightnessMenu==bm25  ) Serial.println("Brightness 25%");  
        else if (brightnessMenu==bm35  ) Serial.println("Brightness 35%");  
        else if (brightnessMenu==bm50  ) Serial.println("Brightness 50%");  
        else if (brightnessMenu==bm70  ) Serial.println("Brightness 70%");  
        else if (brightnessMenu==bm100 ) Serial.println("Brightness 100%");
   
        break;
        
      case mmColor: 

        if      (colorMenu==cmWhite  ) Serial.println("White" );  
        else if (colorMenu==cmYellow ) Serial.println("Yellow");  
        else if (colorMenu==cmOrange ) Serial.println("Orange");  
        else if (colorMenu==cmRed    ) Serial.println("Red"   );
        
        break;
        
      case mmTimer: 

        if      (timerMenu==tm3  ) Serial.println("3 min");  
        else if (timerMenu==tm5  ) Serial.println("5 min");  
        else if (timerMenu==tm10 ) Serial.println("10 min");  
        else if (timerMenu==tm20 ) Serial.println("20 min");  
        else if (timerMenu==tmOff) Serial.println("Off");
                
        break;
        
      // mmBack is not a likely case. We could not have entered the second row
                      
    }; // switch mainMenu

    // Whatever we selected, we can go back to the top row now
    selectedRow=0; 
    showTopRow();
      
  }; // selectedRow>0

};

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

    // Main screen, split in three sections horizontally: full info, night mode, empty screen
    tGroup  grpMainScreen; 

    // First section of main screen, to be complemented with weather and quote
    tLabel  lblTime; 
    tLabel  lblTemperature; 

    // Second section of main screen
    tLabel  lblTimeThin; 
    tLabel  lblQuote1; 
    tLabel  lblQuote2; 

    // Third section of main screen is empty
    // ..

    // Top row with first row of buttons
    tGroup  rowMain;     
    tButton btnBrightness;  
    tButton btnColor;
    tButton btnTimer;
    tButton btnScreenContrast;
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

    // Second row on screen contrast
    tGroup  rowScreenContrast;
    tButton btnScreenContrastAuto;
    tButton btnScreenContrast10;
    tButton btnScreenContrast35;
    tButton btnScreenContrast100;
    tButton btnScreenContrastBack;

    // Sequencing the movements
    bool ready() { return grpMain.ready(); };

    // Current selection in the menu
    int8_t selectedRow;
    tMenuItem mainScreen;         // Which main screen is selected
    tMenuItem mainMenu;           // Which option in the first row is selected
    tMenuItem brightnessMenu;     // Which option in the brightness row is selected
    tMenuItem colorMenu;          // Which option in the color row is selected
    tMenuItem timerMenu;          // Which option in the timer row is selected
    tMenuItem screenContrastMenu; // Which option in the screen constrast row is selected

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


/* 
 * SEPARATING .CPP AND .H LEADS TO COMPILE ERROR IN ARDUINO SOMEHOW, SO ADDING .CPP HERE 
 */

tDisplay::tDisplay(U8G2 &u8g2) {

  // Set selected items
  selectedRow=-1;
  mainScreen=mainFull;
  mainMenu=mainBrightness;
  brightnessMenu=brightness25;
  colorMenu=colorWhite;
  timerMenu=timer03;
  screenContrastMenu=screenContrastAuto;

  // Main screen
  grpMain.addChild(grpMainScreen);

  grpMainScreen.addChild(lblTime);
  lblTime.setFont(u8g2_font_crox2c_tf);  
  lblTime.setText(u8g2, "--:--");

  grpMainScreen.addChild(lblTemperature);
  lblTemperature.setX(80);
  lblTemperature.setFont(u8g2_font_crox2c_tf);  
  lblTemperature.setText(u8g2, "");

  grpMainScreen.addChild(lblTimeThin);
  lblTimeThin.setFont(u8g2_font_crox2c_tf);
  lblTimeThin.setText(u8g2, "--:--");
  uint8_t  w = lblTimeThin.getWidth(u8g2, "--:--");
  lblTimeThin.setX(128+60-0.5*w);
  lblTimeThin.setY(0);

  grpMainScreen.addChild(lblQuote1);
  //lblQuote1.setFont(u8g2_font_t0_11_te);
  lblQuote1.setFont(u8g2_font_profont11_tf);
  lblQuote1.setText(u8g2, ".");
  w = lblQuote1.getWidth(u8g2, ".");
  lblQuote1.setX(128+60-0.5*w);
  lblQuote1.setY(12);

  grpMainScreen.addChild(lblQuote2);
  //lblQuote2.setFont(u8g2_font_t0_11_te);
  lblQuote2.setFont(u8g2_font_profont11_tf);
  lblQuote2.setText(u8g2, ".");
  w = lblQuote2.getWidth(u8g2, ".");
  lblQuote2.setX(128+60-0.5*w);
  lblQuote2.setY(22);

  // Main row
  rowMain.setY(33);
  grpMain.addChild(rowMain);
                 
  addToRow(u8g2, &rowMain, &btnBrightness,     NULL,               "BRIGHTNESS", mainBrightness    );
  addToRow(u8g2, &rowMain, &btnColor,          &btnBrightness,     "COLOR",      mainColor         );
  addToRow(u8g2, &rowMain, &btnTimer,          &btnColor,          "TIMER",      mainTimer         );
  addToRow(u8g2, &rowMain, &btnScreenContrast, &btnTimer,          "SCREEN",     mainScreenContrast);
  addToRow(u8g2, &rowMain, &btnBack,           &btnScreenContrast, "BACK",       mainBack          );

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

  // Row with screen contrast
  rowScreenContrast.setY(33);
  grpMain.addChild(rowScreenContrast);

  addToRow(u8g2, &rowScreenContrast, &btnScreenContrastAuto, NULL,                   "AUTO", screenContrastAuto);
  addToRow(u8g2, &rowScreenContrast, &btnScreenContrast10,   &btnScreenContrastAuto, "10%",  screenContrast10  );
  addToRow(u8g2, &rowScreenContrast, &btnScreenContrast35,   &btnScreenContrast10,   "35%",  screenContrast35  );
  addToRow(u8g2, &rowScreenContrast, &btnScreenContrast100,  &btnScreenContrast35,   "100%", screenContrast100 );
  addToRow(u8g2, &rowScreenContrast, &btnScreenContrastBack, &btnScreenContrast100,  "BACK", screenContrastBack);
  
}; // tDisplay::tDisplay

void tDisplay::step() {
  grpMain.step();
}; // tDisplay::step

void tDisplay::display(U8G2 &u8g2) { 
  uint16_t  w;
  static char outsideTemp[16];
  
  struct tm timeinfo;
  if(getLocalTime(&timeinfo)){
    char currentTime[6];
    strftime(currentTime,sizeof(currentTime),"%H:%M",&(timeinfo)); // Hour 00-23 + Minute 00-59
    lblTime    .setText(u8g2, currentTime);
    lblTimeThin.setText(u8g2, currentTime);
    w=lblTimeThin.getWidth(u8g2, currentTime);
    lblTimeThin.moveX(128+60-0.5*w);
  }

  if(data.quoteAvailable==dqRefreshed) {
    w=lblQuote1.getWidth(u8g2, data.quote);
    
    if(w<128) {
      // Just a single quote
      lblQuote1.setText(u8g2, data.quote);
      lblQuote1.moveX(128+60-0.5*w);
      lblQuote1.moveY(17);
      lblQuote2.setText(u8g2, ""); // hide the second line 
      lblQuote2.moveY(17);
    }
    else {
      // Split in two quotes
      string quote1=data.quote;
      string quote2="";
      uint16_t lengthDiff=quote1.length();

      for(int i=0; i<data.quote.length(); i++) {

        if(isspace(data.quote[i])) {
          if(i>(data.quote.length()-i)) {
            if(i-(data.quote.length()-i)<lengthDiff) {
              quote1=data.quote.substr(0, i);
              quote2=data.quote.substr(i+1);
              lengthDiff=abs(quote1.length()-quote2.length());
              Serial.printf("%d | %d diff %d\n", quote1.length(), quote2.length(), lengthDiff);
            } // i-quote.length()<lengthDiff
          } // i>quote.length()-i
        else { // i<quote.length()-i            
            if(data.quote.length()-2*i<lengthDiff) {
              quote1=data.quote.substr(0, i);
              quote2=data.quote.substr(i+1);
              lengthDiff=abs(quote1.length()-quote2.length());
              Serial.printf("%d | %d diff %d\n", quote1.length(), quote2.length(), lengthDiff);
            } // data.quote.length()-2*i<lengthDiff
          } // i<data.quote.length()-i
        } // whitespace

      } // for i

      // If one of the lines is still too long, request a new quote
      uint16_t w1=lblQuote1.getWidth(u8g2, quote1);
      if(w1>128) data.requestQuote=true;
  
      uint16_t w2=lblQuote2.getWidth(u8g2, quote2);
      if(w2>128) data.requestQuote=true;

      Serial.printf("Quote 1: %s string length %d bitmap length %d\n", quote1.c_str(), quote1.length(), w1);
      Serial.printf("Quote 2: %s string length %d bitmap length %d\n", quote2.c_str(), quote2.length(), w2);
      Serial.println( data.requestQuote ? "Still too long" : "Length OK");
  
      if(!data.requestQuote) { // No new quote requested, so lines fit the screen
        lblQuote1.setText(u8g2, quote1);
        lblQuote1.moveX(128+60-0.5*w1);
        lblQuote1.moveY(12);
        lblQuote2.setText(u8g2, quote2);
        lblQuote2.moveX(128+60-0.5*w2);
        lblQuote2.moveY(22);
      } // !requestQuote

    } // else clause of w<128

    data.quoteAvailable=dqDisplayed;
  } // data.quoteAvailable==dqRefreshed

  if(data.weatherAvailable==dqRefreshed) {
    snprintf (outsideTemp, sizeof(outsideTemp), "%0.1f??C", data.outsideTemp);
    Serial.printf("Outside temperature %.1f is displayed as %s\n", data.outsideTemp, outsideTemp);
    lblTemperature.setText(u8g2, outsideTemp);
    w=lblTemperature.getWidth(u8g2, outsideTemp);
    lblTemperature.setX(124-w);
    data.weatherAvailable=dqDisplayed; 
  }

  grpMain.draw(u8g2); 

  // Display bar graph with precipitation
  int16_t x=0;
  int16_t y=grpMain.globalY();

  for (tPrecipitation & p : data.precipitation) {
    // int h=18.0*log(p.prec+1); // Curve through (0,0) (0.25,3) (3,25)
    // int h=36.0*log(p.prec+1); // Curve through (0,0) (0.25,8) (3,50)
    int h=(int) 12.0*log(p.prec+1); // Curve through (0,0) (0.25,2.6) (3,20)
    if(h>20) h=20;
    u8g2.drawVLine(x++, y+31-h, h);
    u8g2.drawVLine(x++, y+31-h, h);
  }
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

  int16_t row0y = 32;
  int16_t row1y = 32+  rowMain.getHeight()-1;
  int16_t row2y = 32+2*rowMain.getHeight()-1;

  rowMain.moveY(row0y);
  grpMain.moveY(-rowMain.getHeight()-1);

  rowBrightness    .moveY(row2y);
  rowColor         .moveY(row2y);
  rowTimer         .moveY(row2y);
  rowScreenContrast.moveY(row2y);
}; // tDisplay::showTopRow

void tDisplay::showSecondRow() {
  selectedRow=1;
  
  int16_t row0y = 32;
  int16_t row1y = 32+  rowMain.getHeight()-1;
  int16_t row2y = 32+2*rowMain.getHeight()-1;

  rowMain.moveY(row0y);
  
  rowBrightness    .moveY( (mainMenu==mainBrightness    ) ? row1y : row2y);
  rowColor         .moveY( (mainMenu==mainColor         ) ? row1y : row2y);
  rowTimer         .moveY( (mainMenu==mainTimer         ) ? row1y : row2y);
  rowScreenContrast.moveY( (mainMenu==mainScreenContrast) ? row1y : row2y);
  
  grpMain.moveY(-2*rowMain.getHeight());
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
    // No rows are displayed, just the main screen
    switch(mainScreen) {
      case mainFull:
        mainScreen=mainNight;
        grpMainScreen.moveX(-128);
      break;

      case mainNight:
        mainScreen=mainEmpty;
        grpMainScreen.moveX(-256);
      break;

      case mainEmpty:
        mainScreen=mainFull;
        grpMainScreen.setX(128);
        grpMainScreen.moveX(0);
      break;
    }
  }
  else if(selectedRow==0) {
    // A single rows is displayed
    //                                                     which     button to be        button to           button that must     selected item
    //                                                     row       deselected          be selected         be fully displayed   in the top row
    if     (mainMenu==mainBrightness    ) { selectNextItem(&rowMain, &btnBrightness,     &btnColor,          &btnTimer         ); mainMenu=mainColor;          }
    else if(mainMenu==mainColor         ) { selectNextItem(&rowMain, &btnColor,          &btnTimer,          &btnScreenContrast); mainMenu=mainTimer;          }
    else if(mainMenu==mainTimer         ) { selectNextItem(&rowMain, &btnTimer,          &btnScreenContrast, &btnBack          ); mainMenu=mainScreenContrast; }
    else if(mainMenu==mainScreenContrast) { selectNextItem(&rowMain, &btnScreenContrast, &btnBack,           NULL              ); mainMenu=mainBack;           }
    else if(mainMenu==mainBack          ) { selectNextItem(&rowMain, &btnBack,           &btnBrightness,     &btnColor         ); mainMenu=mainBrightness;     };
    showTopRow();
  } // selectedRow==0
  else { // selectedRow>0
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

      case mainScreenContrast:
        if      (screenContrastMenu==screenContrastAuto) { selectNextItem(&rowScreenContrast, &btnScreenContrastAuto, &btnScreenContrast10,   &btnScreenContrast35  ); screenContrastMenu=screenContrast10;   }
        else if (screenContrastMenu==screenContrast10  ) { selectNextItem(&rowScreenContrast, &btnScreenContrast10,   &btnScreenContrast35,   &btnScreenContrast100 ); screenContrastMenu=screenContrast35;   }
        else if (screenContrastMenu==screenContrast35  ) { selectNextItem(&rowScreenContrast, &btnScreenContrast35,   &btnScreenContrast100,  &btnScreenContrastBack); screenContrastMenu=screenContrast100;  }
        else if (screenContrastMenu==screenContrast100 ) { selectNextItem(&rowScreenContrast, &btnScreenContrast100,  &btnScreenContrastBack, NULL                  ); screenContrastMenu=screenContrastBack; }
        else if (screenContrastMenu==screenContrastBack) { selectNextItem(&rowScreenContrast, &btnScreenContrastBack, &btnScreenContrastAuto, &btnScreenContrastAuto); screenContrastMenu=screenContrastAuto; }
        break;

    } // switch mainMenu
    showSecondRow();
    
  }; // selectedRow>0
}; // tDisplay::nextButton

tMenuItem tDisplay::selectButton() {
  
  if(selectedRow==-1) { 
    // Main screen was selected, just display first row but do not change selection
    showTopRow();
    return mainMenu;
  } // selectedRow==-1
  
  else if(selectedRow== 0) {
    if(mainMenu==mainBack) { 
      showMain();
      return mainScreen;
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

      case mainScreenContrast:
        if(screenContrastMenu==screenContrastBack) {
          showTopRow();
          return mainMenu;
        }
        else {
          showTopRow();
          return screenContrastMenu;
        }
        break;
        
      }; // switch case  
  }; // selectedRow>0
}; // tDisplay::selectButton

 
#endif // DISPLAY_H

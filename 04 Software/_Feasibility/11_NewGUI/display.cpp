
#include "display.h"

tDisplay::tDisplay(U8G2 &u8g2) {
  // Clock
  lbTime.setFont(u8g2_font_crox5hb_tr);  
  lbTime.setText(u8g2, "12:34");
  wnMain.addChild(lbTime);

  // Main row
  rwMain.setY(32);
  wnMain.addChild(rwMain);

  btBrightness.setText(u8g2, "BRIGHTNESS"); 
  btBrightness.selected=true;
  rwMain.addChild(btBrightness);
  
  btColor.setText(u8g2, "COLOR");           
  btColor.setX(btBrightness.localRight()-1);
  rwMain.addChild(btColor);
  
  btTimer.setText(u8g2, "TIMER");           
  btTimer.setX(btColor.localRight()-1);
  rwMain.addChild(btTimer);

  btBack.setText(u8g2, "BACK");             
  btBack.setX(btTimer.localRight()-1);
  rwMain.addChild(btBack);

  // Row with brightness values  
  rwBrightness.setY(32);
  wnMain.addChild(rwBrightness);
  
  btBrightness25.setText(u8g2, "25%");
  btBrightness25.selected=true;
  rwBrightness.addChild(btBrightness25);

  btBrightness35.setText(u8g2, "35%");
  btBrightness35.setX(btBrightness25.localRight()-1);
  rwBrightness.addChild(btBrightness35);

  btBrightness50.setText(u8g2, "50%");
  btBrightness50.setX(btBrightness35.localRight()-1);
  rwBrightness.addChild(btBrightness50);

  btBrightness70.setText(u8g2, "70%");
  btBrightness70.setX(btBrightness50.localRight()-1);
  rwBrightness.addChild(btBrightness70);

  btBrightness100.setText(u8g2, "100%");
  btBrightness100.setX(btBrightness70.localRight()-1);
  rwBrightness.addChild(btBrightness100);

  // Initialize main state
  sequence=tsMainWindow;
};

void tDisplay::step() {
  wnMain.step();
};

void tDisplay::mainWindow() {
  wnMain.moveY(0);
  rwMain.moveY(33);
  rwBrightness.moveY(33);
};

void tDisplay::showTopRow() {
  rwMain.moveY(      lbTime.getHeight()-2);
  rwBrightness.moveY(lbTime.getHeight()-2+rwMain.getHeight()+1);
  wnMain.moveY(32-  (lbTime.getHeight()-2+rwMain.getHeight()));
};

void tDisplay::showSecondRow() {
  rwMain.moveY(      lbTime.getHeight()-2);
  rwBrightness.moveY(lbTime.getHeight()-2+rwMain.getHeight()-1);
  wnMain.moveY(32-  (lbTime.getHeight()-2+rwMain.getHeight()+rwBrightness.getHeight()));
};

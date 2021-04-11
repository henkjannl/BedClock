#include "menu.h"
#include <Arduino.h>

Tree::Tree() {
  offset=0;
  screen=nullptr;
}

Tree* Tree::addItem(const actionID_t ID, const char *text, void (*callback) (actionID_t)) {

  int16_t x, y;
  uint16_t w, h;

  Tree child;

  child.ID=ID;
  sprintf(child.text, "%s", text);
  child.callback=callback;
  child.screen=screen;

  screen->getTextBounds(text,0,0, &x, &y, &w, &h); // Use the graphics library to determine the width of the text

  child.width=w;
  subItems.push_back(child);

  // Select child. last added item, so the user can add labels to this row
  selectedItem=subItems.size()-1;
  
  alignPositions();

  return &subItems[selectedItem];
}

void Tree::alignPositions() {
  int16_t x=0;
  uint16_t lbound, rbound;
  
  for (auto &item : subItems) {
      item.pos=x;
      x+=item.width+5;
  }  

  // child.t the row to left or right if the selected label is not visible
  lbound=subItems[selectedItem].pos;
  rbound=lbound+subItems[selectedItem].width;
  if(lbound+offset<lowerLimit) offset+=(lowerLimit-(lbound+offset));
  if(rbound+offset>upperLimit) offset-=((rbound+offset)-upperLimit);
}

void Tree::draw(int16_t y) {
  uint8_t index=0;

  alignPositions();

  screen->setTextSize(1);
  screen->setTextWrap(false);

  for (auto &item : subItems) {
    screen->setCursor(item.pos+offset,y+2);        
    if(index==selectedItem) {
      screen->fillRect(item.pos+offset-2, y, item.width+4, 12,1);
      screen->setTextColor(0); 
    }
    else {
      screen->setTextColor(1); 
    }
    screen->print(item.text);
    index++;
  }

}

void Tree::triggerCallback() {
  callback(ID);
}


#include <U8g2lib.h>
#include <list>
#include <string>

#include "gui.h"

using namespace std;

const float INCREMENT[] = {0.45, 0.25, 0.15, 0.10, 0.05}; // Increments a value in 5 steps, sum of values is 1

// Class tVariable
tVariable::tVariable() { 
  _value=0;
  _amplitude=0;
  _index=5;
  _ready=true;
};    

void tVariable::step() { 
  if(!_ready) { 
    _value+=_amplitude*INCREMENT[_index]; 
    _index++; 
    if(_index>4) _ready=true;
  }; // !_ready
};

void tVariable::setTarget(int target) {
  _amplitude=(float) target-_value;
  _index=0;
  _ready=false;
};

void tVariable::setValue(int value) { 
  _value=(float) value; 
  _ready=true;
};


// abstract class tAbstractScreenElement 
tAbstractScreenElement::tAbstractScreenElement() {
  _parent=NULL;
};

tAbstractScreenElement::tAbstractScreenElement(int x, int y) {
  _x.setValue(x);
  _y.setValue(y);
  _parent=NULL;
};

int tAbstractScreenElement::getWidth() {
  int result = _width;

  for(auto& child : _children) 
    if (result < child->getWidth()) result = child->getWidth();

  return result;
};

int tAbstractScreenElement::getHeight()  { 
  int result = _height;
  
  for(auto& child : _children) 
    if (result < child->getHeight()) result = child->getHeight();

  return result;
};

void tAbstractScreenElement::step() {
  _x.step();
  _y.step();
  for(auto& child : _children) child->step();
}

bool tAbstractScreenElement::ready() {
  if (!_x.ready()) return false;
  if (!_y.ready()) return false;
  for(auto& child : _children) if (!child->ready()) return false; 
  return true;
}

void tAbstractScreenElement::addChild(tAbstractScreenElement& child) {
  child._parent=this;
  _children.push_back(&child);
}

void tAbstractScreenElement::draw(U8G2 &u8g2) { 
  for(auto& child : _children) child->draw(u8g2);
  internalDraw(u8g2);
}

// Class tLabel
tLabel::tLabel() {
  _font=u8g2_font_calibration_gothic_nbp_t_all;  
}

void tLabel::setText(U8G2 &u8g2, string text) {
  u8g2.setFont(_font);
  _textWidth =u8g2.getStrWidth(text.c_str());
  _textHeight=u8g2.getMaxCharHeight()-2;
  _width=_textWidth;
  _height=_textHeight;
  _text=text;
};


void tLabel::internalDraw(U8G2 &u8g2) { 
  u8g2.setFont(_font);
  u8g2.setFontPosTop();
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1);
  u8g2.drawStr(globalX(), globalY(), _text.c_str());
};


// Class tButton 
tButton::tButton() {
  selected=false;
  //_font=u8g2_font_calibration_gothic_nbp_tr;  
  //_font=u8g2_font_t0_13_mf;   // 9 pixels high
  _font=u8g2_font_t0_11_mf;   // 8 pixels high
};

void tButton::setText(U8G2 &u8g2, string text) {
  u8g2.setFont(_font);
  _textWidth =u8g2.getStrWidth(text.c_str());
  _textHeight=u8g2.getMaxCharHeight()-1;
  _width =_textWidth+5;
  _height=_textHeight+5;
  _text=text;
};

void tButton::internalDraw(U8G2 &u8g2) { 
  u8g2.setFont(_font);
  u8g2.setFontPosTop();
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1);
  
  if(selected) {
    u8g2.drawBox(globalX(), globalY(), _width, _height);
    u8g2.setDrawColor(0);
  }
  else u8g2.drawFrame(globalX(), globalY(), _width, _height);

  u8g2.drawStr(globalX()+3, globalY()+3, _text.c_str());
};

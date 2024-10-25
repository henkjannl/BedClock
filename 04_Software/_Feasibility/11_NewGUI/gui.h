#ifndef GUI_H
#define GUI_H

#include <U8g2lib.h>
#include <list>

using namespace std;

// Variable that moves in 5 steps to a target value
// internal format is float but it exports to int 
class tVariable {

  public:
    tVariable();
    int  getValue() { return (int) _value+0.5; };
    void setValue(int value);    // Directly go to target without motion
    void setTarget(int target);  // Go to target with motion steps
    void step();
    bool ready() { return _ready; };

  private:
    float _value;
    float _amplitude;
    byte _index;
    bool _ready; 
};

class tAbstractScreenElement {
  
  public:
    tAbstractScreenElement();
    tAbstractScreenElement(int x, int y);

    int localX()  { return _x.getValue(); };
    int localY()  { return _y.getValue(); };
    int globalX() { return _x.getValue() + (_parent ? _parent->globalX() : 0); };
    int globalY() { return _y.getValue() + (_parent ? _parent->globalY() : 0); };

    void setX(int x) { _x.setValue(x); };
    void setY(int y) { _y.setValue(y); };
    void moveX(int x) { _x.setTarget(x); };
    void moveY(int y) { _y.setTarget(y); };

    int getHeight();
    int getWidth();
    int localRight()   { return localX() +getWidth();  };
    int localBottom()  { return localY() +getHeight(); };
    int globalRight()  { return globalX()+getWidth();  };
    int globalBottom() { return globalY()+getHeight(); };

    void step();
    bool ready(); // Checks if all motion incl. children is ready

    void addChild(tAbstractScreenElement& child);
    void draw(U8G2 &u8g2);

  protected:
    tVariable _x, _y;
    int _width, _height;
    tAbstractScreenElement* _parent;          // If an element has a parent, it adds (x,y) position of the parent to the position
    list<tAbstractScreenElement*> _children;  // If an element has children, it redraws all children
    virtual void internalDraw(U8G2 &u8g2)=0;  // This virtual function only redraws the current object
};

// A group is an empty element which superposes it's location to the children
class tGroup: public tAbstractScreenElement {
  protected:
    void internalDraw(U8G2 &u8g2) {}; 
};

// A label draws an unboxed text
class tLabel: public tAbstractScreenElement {
  public:

    tLabel();
    void setFont(const uint8_t* font) { _font=font; };
    void setText(U8G2 &u8g2, string text);
    
  protected:
    void internalDraw(U8G2 &u8g2);

    const uint8_t* _font;
    uint8_t _textWidth, _textHeight;
    string _text;
};

// A button draws a boxed text that can be selected
class tButton: public tLabel {
  public:

    tButton();
    void setText(U8G2 &u8g2, string text);

    bool selected;
    
  protected:
    void internalDraw(U8G2 &u8g2);
};


#endif // GUI_H

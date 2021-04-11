#include "key.h"
#include <Arduino.h>

Key::Key(const int pin, const int threshold) {
  _pin=pin;
  _isHigh=false;
  _threshold=threshold;

  // Feed the median filter with some initial samples to saturate the buffer
  for(int i=0; i<7; i++) {
    _medFilter.addSample(touchRead(_pin));  
  }
}

void Key::scan() {

  _medFilter.addSample(touchRead(_pin));

  bool touched=(_medFilter.getMedian()<_threshold);

  _pressed=false;

  if(_isHigh) {
    if(!touched) _isHigh=false;
  }
  else {
    if(touched) {
      _isHigh=true;
      _pressed=true;
    }
  }
}



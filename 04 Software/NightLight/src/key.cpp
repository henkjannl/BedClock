#include "key.h"
#include <Arduino.h>
#include <string>

using namespace std;

Key::Key(const int pin, const int threshold) {
  _pin=pin;
  _isHigh=false;
  _threshold=threshold;

  // Feed the median filter with some initial samples to saturate the buffer
  for(int i=0; i<KEY_MEDIAN_FILTER_WIDTH; i++) {
    _medFilter.addSample(touchRead(_pin));  
  }
}

void Key::scan() {

  int reading = touchRead(_pin);
  _medFilter.addSample(reading);
  _clicks.push_back(reading);

  // Ensure there are not too many values in the buffer
  while(_clicks.size()>100) _clicks.pop_front();

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


string Key::debug() {
  string result="Keys:\n"; 

  for(int click : _clicks) {
    result += click;
    result += "\n";
  }

  _clicks.clear();
  return result;
}


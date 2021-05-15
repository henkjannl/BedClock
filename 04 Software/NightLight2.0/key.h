#ifndef KEY_H
#define KEY_H

#include "MedianFilter.h"
#include <list>
#include <string> 

using namespace std;

const int KEY_MEDIAN_FILTER_WIDTH = 5;

class Key {
    public:
        Key(const int pin, const int threshold);  
        void scan();
        bool pressed() { return _pressed; }
        int  value() { return _medFilter.getMedian(); }
        string debug();
    private:
        list<int> _clicks;
        int _pin;
        int _threshold;
        int _pressed;
        int _isHigh;
        MedianFilter<int, KEY_MEDIAN_FILTER_WIDTH> _medFilter; // Implement a median filter with kernel size 5 to reject spikes
};

/* ===============================================================================//
// SINCE ARDUINO DOES NOT SEEM TO LIKE SEPARATE .H AND .CPP FILES, .CPP GOES HERE //
// ===============================================================================*/

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

#endif // KEY_H

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
        MedianFilter<int, KEY_MEDIAN_FILTER_WIDTH> _medFilter; // Implement a median filter with kernel size 3 to reject spikes
};

#endif // KEY_H
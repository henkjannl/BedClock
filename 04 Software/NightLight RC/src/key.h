#ifndef KEY_H
#define KEY_H

#include "MedianFilter.h"

class Key {
    public:
        Key(const int pin, const int threshold);  
        void scan();
        bool pressed() { return _pressed; }
        int  value() { return _medFilter.getMedian(); }
    private:
        int _pin;
        int _threshold;
        int _pressed;
        int _isHigh;
        MedianFilter<int, 3> _medFilter; // Implement a median filter with kernel size 3 to reject spikes
};

#endif // KEY_H
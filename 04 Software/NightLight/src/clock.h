#ifndef CLOCK_H
#define CLOCK_H


class Clock {
    public:
        Clock();  
        void scan();
        bool reliable();
        void requestTimeZone();
        void syncTime();
        void refreshTime();
        void currentTime(char *timeStr);

    private:
        bool timeZoneKnown;
        bool clockSynched;
        int timeDSToffset;
        int timeZoneOffset;
};

#endif // CLOCK_H
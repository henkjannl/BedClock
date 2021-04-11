#ifndef WEATHER_H
#define WEATHER_H

#include <list>

struct tPrecipitation {
  long t;
  float prec;
};

class Weather {
    public:
        Weather();  
        void getWeather();
        void scan();
        bool reliable();
        float getTemperature() {return temperature; }

    private:
        bool weatherKnown;
        float temperature;
        char weatherIcon[12];
        std::list<tPrecipitation> precipitation;
};

#endif // WEATHER_H
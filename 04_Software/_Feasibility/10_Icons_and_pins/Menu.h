#ifndef MENU_H
#define MENU_H

const byte mainIcons[]      = { 0, 1, 2, 3};    const int mainCount      =4;
const byte timerIcons[]     = { 4, 5, 6, 7};    const int timerCount     =4;
const byte intensityIcons[] = { 8, 9,10,11,12}; const int intensityCount =5;
const byte colorIcons[]     = {13,14,15,16};    const int colorCount     =4;
const byte exitIcons[]      = {17,18};          const int exitCount      =2;
const byte selectOffset=17;

struct tMenu {
  int mainSelection;
  int intensitySelection;
  int timerSelection;
  int colorSelection;
  int exitSelection;

  std::list<byte> main;
  std::list<byte> sub;

  int mainOffset=0;
  int subOffset=0;

  tMenu() {
    mainSelection=0;
    
    intensitySelection=0;
    timerSelection=0;
    colorSelection=0;
    exitSelection=0;

    tIcons();
  }
 
  void tIcons() {
    
    main.clear();
    for(int i=0; i<4; i++) {
      byte b=mainIcons[(mainSelection+i-1+mainCount) % mainCount]; 
      main.push_back(b);
    }

    // Add icon numbers to the submenu
    sub.clear();
    switch(mainSelection) {
      case 0: // Timer is selected
        for(int i=0; i<4; i++) {
          byte b=timerIcons[(timerSelection+i-1+timerCount) % timerCount]; 
          sub.push_back(b);
        }
        break;

      case 1: // Intensity is selected
        for(int i=0; i<4; i++) {
          byte b=intensityIcons[(intensitySelection+i-1+intensityCount) % intensityCount]; 
          sub.push_back(b);
        }
        break;
        
      case 2: // Color is selected
        for(int i=0; i<4; i++) {
          byte b=colorIcons[(colorSelection+i-1+colorCount) % colorCount];
          sub.push_back(b);
        }
        break;

      case 3: // Exit is selected
        for(int i=0; i<3; i++) {
          byte b=exitIcons[(exitSelection+i-1+exitCount) % exitCount];
          sub.push_back(b);
        }
        break;
    }
  }
  
  void IncMain() {
    mainSelection=(mainSelection+1) % mainCount;
    
    tIcons();
    mainOffset=icon_pitch;
  }
  
  void IncSub() {
    subOffset=icon_pitch;
    
    switch(mainSelection) {
      case 0: // Timer is selected
        timerSelection=(timerSelection+1)%timerCount;
        break;

      case 1: // Intensity is selected
        intensitySelection=(intensitySelection+1) % intensityCount;
        break;
        
      case 2: // Color is selected
        colorSelection=(colorSelection+1)%colorCount;
        break;

      case 3: // Exit is selected
        exitSelection=(exitSelection+1)%exitCount;
        break;
    }
    tIcons();
  }
};

#endif // MENU_H

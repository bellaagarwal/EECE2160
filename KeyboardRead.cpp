#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include "parse.h"

#define VAL_PRESS 1
#define VAL_RELEASE 0
#define VAL_HOLD 2

using namespace std;

class Keyboard {
  int fd;
public:
  Keyboard(char* dev) {
    //exit the application if the Keyboard object cannot be created
      fd = open(dev, O_RDONLY);
      if (fd == -1) {
            cout << "ERROR: could not open " << endl;
            exit(1);
            }
  }

  ~Keyboard() {
      close(fd);
  }

  /**
   * A static method (called via Keyboard::printEvent(e))
   * that is useful for debugging. This should print out
   * a useful message including the type, code, and value of the event
   * @param e an input_event object to be displayed on std::cout
  */
  static void printEvent(input_event e) {
    // keynames is defined in parse.h
      if(e.type == EV_KEY) {
                  cout << keynames[e.code].name<< endl;
      }
}

  /**
   * Returns the next event received from this keyboard device.
   * @return the next input_event whose type is EV_KEY
  */
  input_event getNextEvent() {
    input_event result;
      while(true) {
            int ret_val = read(fd, &result, sizeof(input_event));
   if (ret_val==-1)
     cout << "could not read from usb\n";
   else if (ret_val == 0)
     cout << "nothin gto read anymore\n";
            if(result.type == EV_KEY) {
                  return result;
            }
      }
 }

  /**
   * @param keycode - the code of the next event to return
   * @return an input event with type EV_KEY and code keycode
   */
  input_event getNextEvent(int keycode) {
      while(true) {
            input_event res = getNextEvent();
            if(res.code == keycode) {
                  return res;
            }
      }
  }


  /**
   * @return the code of the key that was next pressed down
   */
  int getKeyDown() {
    int result = getNextEvent().code;
    //TODO fill in this code
    return result;
  }


};

/**
 * This program should be run with a input device path
 * as the first argument in the shell
*/
int main(int argc, char**argv) {
  if (argc < 2) {
    cerr << "Usage: main <path to usb keyboard input device>" << endl;
    exit(1);
  }
  Keyboard kbd = Keyboard(argv[1]); 
  
  bool run = true;
  while(run) {
    input_event x = kbd.getNextEvent();
    int val = keynames[x.code].value;
    if(val == getKeyDown()) {
        kbd.printEvent(x);
    }
    cout << keynames[x.code].value << endl;
   if(val == 28) {
     run = false;
      break;
    }
  }

  return 0;
}

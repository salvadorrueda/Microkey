/* MicroKey. Physical keyboard shortcuts based on Arduino Micro and a keypad.
 *  
 *  
 *  Special thanks to Alexander Brevig for the Keypad library.
 */
#include <Keypad.h>
#include "Keyboard.h"

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// | | | |  | | | |    
// | | | |  | | | |    
//14 3 2 1 16 15 9 10

byte rowPins[ROWS] = { 14, 3, 2, 1}; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 16, 15, 9, 10}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char key = keypad.getKey();
  int i;
  
  if (key){
    Serial.println(key);
    if (key=='A') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('Z');
      Keyboard.releaseAll();
      Keyboard.write(KEY_RETURN);
    }
    if (key=='B') {
      Keyboard.println("fg");
      Keyboard.write(KEY_RETURN);
    }

    if (key=='C') {
      for (i=0; i<30; i++){
      Keyboard.println("fg");
      Keyboard.write(KEY_RETURN);
      delay(60000); // 1m before pause it
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('Z');
      Keyboard.releaseAll();
      Keyboard.write(KEY_RETURN);
      delay(120000); // 3m before resume it
      }      
    }

    if (key=='*') {
      Keyboard.println("*");
      Keyboard.write(KEY_RETURN);      
    }
    

  }
}

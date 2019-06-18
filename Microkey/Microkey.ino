/* MicroKey. Physical keyboard shortcuts based on Arduino Micro, a keypad and an oled display.  
 *  
 *     [Microkey     ]
 *     [             ]
 *     [             ]
 *     [             ]
 *     
 *  [ 1 ][ 2 ][ 3 ][ A ] 
 *  [ 4 ][ 5 ][ 6 ][ B ]
 *  [ 7 ][ 8 ][ 8 ][ C ]
 *  [ * ][ 0 ][ # ][ D ]
 *  
 *  
 *  Special thanks to Mark Stanley and Alexander Brevig for the Keypad library and Adafruit for SSD1306 driver.
 */

/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

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
// |  |  |  |  |  |  |  |    
// |  |  |  |  |  |  |  |    
//16 15 14  1  6 11 10  9

byte rowPins[ROWS] = { 16, 15, 14, 1}; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 6, 11, 10, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup(){
  //Serial.begin(9600);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);
  
}

bool locked = true;   // locked by default. Do nothing until unlock.
                         // set to 'false' to avoid lock MicroKey.
#define maxcmdl 4      // maximun cmd length      
char cod[maxcmdl+1] = "1234"; // to save the code to unlock. 4 nums and the \n character.
                          
char key = ' ';        // to save the value of the key pressed.
char cmd = ' ';        // to save the code of the command to execute.
int icmd;  

void loop(){
  
  /* 
   * The first time MicroKey is connected is locked.
   * You've to enter a code to unlock it. After that,
   * if you enter a code you will see the
   * text to send in the display and it will be sended
   * when you press 'A'
   * 
   */

  while(locked){ // locked by default. Do nothing until unlock. 
    welcome(); // Display initial message.
    locked = check_lock();
  }


  // unlocked
  welcome_unlocked();       
  
  key = keypad.getKey(); 
    
  if (key){
    // Activate. Execute the command "cmd".
    if (key=='A') {
      execute(cmd);
    }else{
      // Prepare the command to execute
      cmd=key; 
    }
  }
}

void welcome(){
  int i;
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Microkey cmd locked");
  display.println("Enter code to unlock");
  display.display();
}

void welcome_unlocked(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Microkey cmd ");
  display.println("'A' to confirm");
  display.println(cmd);
  //display.println(dcmd);
  print_cmd(cmd);
  display.display();
}

void execute(char cmd){
          //Keyboard.println(scmd);
          //Keyboard.write(KEY_RETURN);
 switch(cmd){
  case '1':
       Keyboard.println("Password 1");       
       break;
  case '2':
       Keyboard.println("Password 2");       
       break;
  case '3':
       Keyboard.println("Salvador");                          
       break;
  case '4':
       Keyboard.println("Salvador.rueda@gmail.com");                          
       break;
  }  
}

void print_cmd(char cmd){
 switch(cmd){
  case '1':
       display.println("Password 1");       
       break;
  case '2':
       display.println("Password 2");       
       break;
  case '3':
       display.println("Salvador");                          
       break;
  case '4':
       display.println("Salvador.rueda@gmail.com");                          
       break;
  } 
}

bool check_lock(){ // Wait for a 4 key code to mach with variable "cod". 
//#define maxcmdl 4      // maximun cmd length      
//char cod[maxcmdl+1] = "1234"; // to save the code to unlock. 4 nums and the \n character.

  bool locked = true;

  int icmd = 0;
  bool fail = false;  // true when a single character of the password fails.
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Microkey cmd locked");
  display.println("Enter code to unlock");
  
  while (icmd<maxcmdl){
    key = keypad.getKey();

    if(key){
      display.print(key);
      display.display();
      
      if(key == cod[icmd] && !fail) locked = false;
      else{
        fail = true;
        locked = true;
      }
      icmd++;   
    }
  }
  return locked;
}

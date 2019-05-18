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
  
  welcome(); // Display initial message. 

}

  bool locked = true;   // locked by default. Do nothing until unlock. 
  char key = ' ';        // to save the value of the key pressed.
  #define maxcmdl 4      // maximun cmd length      
  char cmd[maxcmdl+1] = "    "; // to save the code or command. 4 nums and the \n character.
  char cod[maxcmdl+1] = "1234"; // to save the code to unlock. 4 nums and the \n character.
  unsigned int icmd = 0;// index of cmd array. Possible values [0-4].
  unsigned int uicmd; // integer value of cmd char array.
  char scmd[50];        // string cmd to be send when 'A' is pressed.
  char dcmd[22];        // string to display related to the cmd.
  
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
    welcome();
    key = keypad.getKey();
    if (key && icmd<maxcmdl){
      cmd[icmd++] = key; // add 'key' to cmd
    }
    
    // Check for the code to unlock MicroKey.
    if (!strcmp(cmd,cod)){
      locked = false;
      strcpy(cmd,"    "); // Reset the code, ready for cmd.
      icmd = 0;           // Reset the index of cmd array.
      welcome_unlocked();     
    }else{
      if(icmd == maxcmdl){
        strcpy(cmd,"    "); // Reset the code
        icmd = 0;           // Reset the index of code array.
      }
    }
  }
  
  key = keypad.getKey();
    
  if (key){
    // Activate. Execute the command (scmd).
    if (key=='A') {
      Keyboard.println(scmd);
      Keyboard.write(KEY_RETURN);
      welcome_unlocked();       
    }else{
      // Add key to the code
      if(icmd == maxcmdl){
        strcpy(cmd,"    "); // Reset the code
        icmd = 0;           // Reset the index of code array.
      }   
      
      //cmd[icmd++]=key;
      // <!> One digit code
      cmd[0]=key; 
      
      // Update scmd
      update_scmd();
      
      // Update display  
      welcome_unlocked();    
    }

    // Press 'C' to clear current code. 
    if (key=='C') {
      strcpy(cmd,"    "); // Reset the code
      icmd = 0;           // Reset the index of code array.
      strcpy(scmd,"");    // clean display scmd. Text to write.
      strcpy(dcmd,"");    // clean display dcmd. Text to display.
                
      welcome_unlocked(); 
    }

    if (key=='D') {
      //for (i=0; i<1; j++){
        display.setCursor(0,0);
        display.println();
        cmd[icmd]=key;
        icmd++;
        display.println(cmd);
        display.println("Not D. Zoombie");
        //strcpy(scmd,"");
        display.display(); 
        
      for (;;){
      Keyboard.println("fg");
      Keyboard.write(KEY_RETURN);
      delay(60000); // 1m before pause it
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('Z');
      Keyboard.releaseAll();
      Keyboard.write(KEY_RETURN);
      delay(120000); // 2m before resume it
      } 
    }     
  }
}

void welcome(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Microkey cmd locked");
  display.println("Enter code to unlock");
  display.println(cmd);
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
  display.println(dcmd);
  display.display();
}

void update_scmd(){
 
 switch(cmd[0]){
  case '1':
       strcpy(scmd,"123456");
       strcpy(dcmd,"Password 1");       
       break;
  case '2':
       strcpy(scmd,"1234");       
       strcpy(dcmd,"Password 2");       
       break;
  case '3':
       strcpy(scmd,"Salvador");
       strcpy(dcmd,"Salvador");                          
       break;
  case '4':
       strcpy(scmd,"salvador.rueda@gmail.com");
       strcpy(dcmd,"Salvador.rueda@gmail.com");                          
       break;
  } 
}


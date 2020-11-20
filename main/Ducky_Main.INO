#include <Keyboard.h>
#include <SPI.h>
#include <SD.h>

/*CS   -> 8
 *CLK  -> 15
 *MOSI -> 16
 *MISO -> 14
 */

 /*DIP SWITCH
  * (MSB) 3,4,5,6
  *
  */

String script = "";

//Don't judge I like it this way.
char which_f(String strs){
        if(strs.indexOf("ESCAPE") != -1) return 177;
        if(strs.indexOf("ENTER") != -1) return 176;
        if(strs.indexOf("ALT") != -1) return 130;
        if(strs.indexOf("CONTROL") != -1) return 128;
        if(strs.indexOf("TAB") != -1) return 179;
        if(strs.indexOf("END") != -1) return 213;
        if(strs.indexOf("INSERT") != -1) return 209;
        if(strs.indexOf("DELETE") != -1) return 212;
        if(strs.indexOf("BACKSPACE") != -1) return 178;
        if(strs.indexOf("PAGEUP") != -1) return 211;
        if(strs.indexOf("PAGEDOWN") != -1) return 214;
        if(strs.indexOf("HOME") != -1) return 210;
        if(strs.indexOf("DOWN") != -1) return 217;
        if(strs.indexOf("UP") != -1) return 218;
        if(strs.indexOf("RIGHT") != -1) return 215;
        if(strs.indexOf("LEFT") != -1) return 216;
        if(strs.indexOf("SHIFT") != -1) return 129;
        if(strs.indexOf("F12") != -1) return 205;
        if(strs.indexOf("F11") != -1) return 204;
        if(strs.indexOf("F10") != -1) return 203;
        if(strs.indexOf("F9") != -1) return 202;
        if(strs.indexOf("F8") != -1) return 201;
        if(strs.indexOf("F7") != -1) return 200;
        if(strs.indexOf("F6") != -1) return 199;
        if(strs.indexOf("F5") != -1) return 198;
        if(strs.indexOf("F4") != -1) return 197;
        if(strs.indexOf("F3") != -1) return 196;
        if(strs.indexOf("F2") != -1) return 195;
        if(strs.indexOf("F1") != -1) return 194;
        else return 0x00;
}

void Send_GUI(char key){
  Keyboard.press(KEY_LEFT_GUI);
  if(key != 0x00) Keyboard.press(key);
  delay(100);
  Keyboard.releaseAll();
}

void Send_ALT(char key){
  Keyboard.press(KEY_LEFT_ALT);
  if(key != 0x00) Keyboard.press(key);
  delay(100);
  Keyboard.releaseAll();

}

void Send_CTRL(char key){
  Keyboard.press(KEY_LEFT_CTRL);
  if(key != 0x00) Keyboard.press(key);
  delay(100);
  Keyboard.releaseAll();
}

void parseScript(){
   //Aligning the commands and grabbing the first line of each instruction.
   String currentCommand = script.substring(0, script.indexOf("\n"));
   script.remove(0, script.indexOf("\n")+1);


   //What to do when the end of the script is reached? Do the harlem shake.
   if(currentCommand.indexOf("ND_SCRIPT") != -1){
    while(1);
   }

//*********************ENTER*******************************   
   if(currentCommand.indexOf("ENTER") != -1){
        Keyboard.press(KEY_RETURN);
        delay(100);
        Keyboard.releaseAll();
    }


//******************GUI************************************
    else if(currentCommand.indexOf("GUI") != -1){
       if(currentCommand.indexOf(" ") != -1){
          currentCommand.remove(0, currentCommand.indexOf(" ")+1);
          if(currentCommand.length() > 2){
            Send_GUI(which_f(currentCommand));
            
          }
          else{
            Send_GUI(currentCommand.c_str()[0]);
          }
        }
       else{
          Send_GUI(0x00);
        }
    }

//***************DELAY***********************************
    else if(currentCommand.indexOf("DELAY") != -1){
       currentCommand.remove(0, currentCommand.indexOf(" ")+1);
       delay(currentCommand.toInt());
    }



//*************STRING*************************************
    else if(currentCommand.indexOf("STRING") != -1){
       currentCommand.remove(0, currentCommand.indexOf(" ")+1);
       Keyboard.print(currentCommand);
    }


//*************ALT***************************************
    else if(currentCommand.indexOf("ALT") != -1){
       if(currentCommand.indexOf(" ") != -1){
          currentCommand.remove(0, currentCommand.indexOf(" ")+1);
          if(currentCommand.length() > 2){
            Send_ALT(which_f(currentCommand));
            
          }
          else{
            Send_ALT(currentCommand.c_str()[0]);
          }
        }
       else{
          Send_ALT(0x00);
        }
    }


//************CONTROL*************************************
    else if(currentCommand.indexOf("CONTROL") != -1){
       if(currentCommand.indexOf(" ") != -1){
          currentCommand.remove(0, currentCommand.indexOf(" ")+1);
          if(currentCommand.length() > 2){
            Send_CTRL(which_f(currentCommand));
          }
          else{
            Send_CTRL(currentCommand.c_str()[0]);
          }
        }
       else{
          Send_CTRL(0x00);
        }
    }

//***********ANY other unlisted key****************************************
    else{
        if(which_f(currentCommand) != 0x00){
          Keyboard.press(which_f(currentCommand));
          delay(100);
          Keyboard.releaseAll();
        }
    }
}

void setup() {
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  Keyboard.begin();
  delay(5000);

  if (!SD.begin(8)) while (1); //init error
  
  String fileToOpen = "";
  for(int i=3; i<7; i++){
    fileToOpen += String(digitalRead(i));
  }
  
  if(fileToOpen == "0000"){
    while(true); //position 0000 means the device is off. (NO SCRIPT EXECUTED).
  }
  
  File TextFile = SD.open(fileToOpen+".txt");
  if(TextFile){
    while(TextFile.available()){ 
      script += (char)TextFile.read();
    }
    TextFile.close(); //be polite and close everything that you open.
  }
}

void loop() {
    parseScript();
}

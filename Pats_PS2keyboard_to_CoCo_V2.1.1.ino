//  keyboard controler for CoCo 1,2,3
#include <PS2KeyAdvanced.h>
  #include <PS2KeyMap.h>
  #define DATAPIN 3  // PS/2 data I/O
  #define IRQPIN 2   // PS/2 clock I/O
  #define low LOW
  #define YES 1;
  #define NO 0;
  #define no 0;
  #define yes 1;
  PS2KeyAdvanced keyboard;
  PS2KeyMap keymap;
  uint16_t Wait_Time = 40;  // for debug use only Set to ? xxms for released program
  uint16_t code;
  int KEY_STROBE    =  9;
  int KEY_DATA      =  8;
  int KEY_RESET = 10;
  int CPU_RESET = 13;
  int Shift = 0;
  int KEY = 0;
  int SHIFT = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("PS2KeyMap plus PS2KeyAdvanced Libraries");
  keyboard.begin(DATAPIN, IRQPIN);
  // Disable Break codes (key release) from PS2KeyAdvanced
  keyboard.setNoBreak(1);
  // and set no repeat on CTRL, ALT, SHIFT, GUI while outputting
  keyboard.setNoRepeat(1);
  pinMode (KEY_DATA,OUTPUT);    //DATA        pin
  pinMode (KEY_STROBE,OUTPUT);  //STROBE      pin
  pinMode (KEY_RESET,OUTPUT);   //RESET       pin
  pinMode (CPU_RESET,OUTPUT);   //!CPU RESET  pin
  DDRC = B11111111; // set PORTC (digital 7~0) to output mt8808ae address slection
  digitalWrite(CPU_RESET,HIGH);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  code = keyboard.available();
  if (code > 0) 
  {
    code = keyboard.read();
    Serial.print("Value ");
        //un mapped codes processed here
        switch (code) 
     {      //       SHIFTED ARROW KEYS
                          // COCO SPECIAL KEYS
          //case 0x0040:
          case 0x2030: // <CNTL+0> = CoCo shifted "0" for upper and lower case 
          case 0x3030: // <SHIFT LOCK ON <CNTL+0>
          KEY = 0x20;  //   COCO 0 KEY
          SHIFT = 1;   //   COCO SHIFT KEY Activated
          break;

          case 0x4117: //up arrow shifted
          case 0x5117:
          KEY = 0x1B;
          SHIFT = 1;
          break;

          case 0x4115: //up arrow shifted
          case 0x5115:
          KEY = 0x1D;
          SHIFT = 1;
          break;

          case 0x190A:   //  CoCo <ALT> key   //special FUNCTION KEYS BELOW //
            KEY = 0x33;   // CoCo <ALT> key   //NOTE STILL NOT SETUP FULLY
            SHIFT = 0;                        //EITHER NEED KEY TABLE OR BIT TOGGLE + MATH
            break;                            // FOR COCO3 CNTL AND ALT KEYS

            case 0x90A:   //
            KEY = 0x33;   // CoCo <ALT> key
            SHIFT = 0;
            break;

            case 0x3108:   //
            KEY = 0x34;   // CoCo <CNTL> key
            SHIFT = 0;
            break;

            case 0x2108:   //
            KEY = 0x34;   // CoCo <CNTL> key
            SHIFT = 0;
            break;

          case 0x1161:   //
          KEY = 0x35;   // CoCo <F1> key
          SHIFT = 0;
          break;
          case 0x1162:   //
          KEY = 0x36;   // CoCo <F2> key
          SHIFT = 0;
          break;
          case 0x161:   //
          KEY = 0x35;   // CoCo <F1> key
          SHIFT = 0;
          break;
          case 0x162:   //
          KEY = 0x36;   // CoCo <F2> key
          SHIFT = 0;
          break;
          case 0x1117:    //ARROW KEY UP
          KEY = 0x1b;
          SHIFT=0;
          break;
          case 0x1118:    //ARROW KEY DOWN
          KEY = 0x1c;
          SHIFT=0;
          break;
          case 0x0008:
          case 0X011C:
          case 0X1008:
          case 0x1115:    //ARROW KEY LEFT
          KEY = 0x1d;
          SHIFT=0;
          break;
          case 0x1116:    //ARROW KEY RIGHT
          KEY = 0x1e;
          SHIFT=0;
          break;
          //     UNSHIFTED ARROW KEYS
          case 0x117:    //ARROW KEY UP
          KEY = 0x1b;
          SHIFT=0;
          break;
          case 0x118:    //ARROW KEY DOWN
          KEY = 0x1c;
          SHIFT=0;
          break;
          case 0x115:    //ARROW KEY LEFT
          KEY = 0x1d;
          SHIFT=0;
          break;
          case 0x116:    //ARROW KEY RIGHT
          KEY = 0x1e;
          SHIFT=0;
          break;
          case 0x0250:
          case 0x00FA:
          KEY = 0x0000;
          SHIFT=0;
          break;
          KEY=0;        //needed to flush key buffer
            SHIFT=0;
              code=0; 
 
     }
    Serial.print(code, HEX);
    code = keymap.remapKey(code);
    if (code > 0) 
    {
      if ((code & 0xFF))
      //if( ( code > 0x7F ) )
      {
        Serial.print(" mapped ");
        Serial.print(code, HEX);
        Serial.print(" - Status Bits ");
        Serial.print(code >> 8, HEX);
        Serial.print("  Code ");
        Serial.print(code & 0xFF, HEX);
        Serial.print("  ( ");
        Serial.write(code & 0xFF);
        Serial.print(" )\n");
      }
      // process keys to control CoCo PIA
      //found = 0;
      switch (code) {
        

          case 0x0008:
          case 0x1008:
          case 0x4008:
          KEY = 0x1D;  // Select key '<left arrow>' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;

        
        //case 0x0040:
        case 0x1040:
        case '@':
          KEY = 0x40;  // Select key '@' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'A':
          //remap key here
          // then send to coco keyboard encoder
          //MT8808AE data format
          KEY = 0x01;  // Select key 'A' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'a':
          /* ROW    DATA IS (0,0,AY2,AY1,AY0,AX2,AX1,AX0)
                  COLUMN DATA IS (RESET COMPUTER,,,,,DATA"ON/OFF",STROBE,RESET clears keyboard memory arry switchs)
              */
          KEY = 0x01;  // Select key 'a' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'B':
          KEY = 0x02;  // Select key 'B' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'b':
          KEY = 0x02;  // Select key 'b' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'C':
          KEY = 0x03;  // Select key 'C' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'c':
          KEY = 0x03;  // Select key 'c' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'D':
          KEY = 0x04;  // Select key 'D' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'd':
          KEY = 0x04;  // Select key 'd' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'E':
          KEY = 0x05;  // Select key 'E' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'e':
          KEY = 0x05;  // Select key 'e' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'F':
          KEY = 0x06;  // Select key 'F' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'f':
          KEY = 0x06;  // Select key 'f' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'G':
          KEY = 0x07;  // Select key 'G' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'g':
          KEY = 0x07;  // Select key 'g' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'H':
          KEY = 0x08;  // Select key 'H' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'h':
          KEY = 0x08;  // Select key 'h' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'I':
          KEY = 0x09;  // Select key 'I' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'i':
          KEY = 0x09;  // Select key 'i' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'J':
          KEY = 0x0A;  // Select key 'J' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'j':
          KEY = 0x0A;  // Select key 'j' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'K':
          KEY = 0x0B;  // Select key 'K' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'k':
          KEY = 0x0B;  // Select key 'k' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'L':
          KEY = 0x0C;  // Select key 'L' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'l':
          KEY = 0x0C;  // Select key 'l' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'M':
          KEY = 0x0D;  // Select key 'M' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'm':
          KEY = 0x0D;  // Select key 'm' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'N':
          KEY = 0x0E;  // Select key 'N' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'n':
          KEY = 0x0E;  // Select key 'n' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'O':
          KEY = 0x0F;  // Select key 'O' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'o':
          KEY = 0x0F;  // Select key 'o' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'P':
          KEY = 0x10;  // Select key 'P' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'p':
          KEY = 0x10;  // Select key 'p' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'Q':
          KEY = 0x11;  // Select key 'Q' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'q':
          KEY = 0x11;  // Select key 'q' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'R':
          KEY = 0x12;  // Select key 'R' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'r':
          KEY = 0x12;  // Select key 'r' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'S':
          KEY = 0x13;  // Select key 'S' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 's':
          KEY = 0x13;  // Select key 's' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'T':
          KEY = 0x14;  // Select key 'T' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 't':
          KEY = 0x14;  // Select key 't' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'U':
          KEY = 0x15;  // Select key 'U' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'u':
          KEY = 0x15;  // Select key 'u' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'V':
          KEY = 0x16;  // Select key 'V' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'v':
          KEY = 0x16;  // Select key 'v' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'W':
          KEY = 0x17;  // Select key 'W' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'w':
          KEY = 0x17;  // Select key 'w' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'X':
          KEY = 0x18;  // Select key 'X' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'x':
          KEY = 0x18;  // Select key 'x' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'Y':
          KEY = 0x19;  // Select key 'Y' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'y':
          KEY = 0x19;  // Select key 'y' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 'Z':
          KEY = 0x1A;  // Select key 'Z' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 1;
          break;
        case 'z':
          KEY = 0x1A;  // Select key 'z' convert key code so decoder can slect correct matrix key(s)
          SHIFT = 0;
          break;
        case 0x1020:
        case ' ':     //spacebar
          KEY = 0x1F;  // Select key 'spacebar' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
                      //NUMBERS START HERE
        case '0':
        case 0x1030:
          KEY = 0x20;  // Select key '0' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x1031:
        case '1':
          KEY = 0x21;  // Select key '1' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x1032:
        case '2':
          KEY = 0x22;  // Select key '2' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x1033:
        case '3':
          KEY = 0x23;  // Select key '3' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x1034:
        case '4':
          KEY = 0x24;  // Select key '4' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x1035:
        case '5':
          KEY = 0x25;  // Select key '5' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x1036:
        case '6':
          KEY = 0x26;  // Select key '6' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x1037:
        case '7':
          KEY = 0x27;  // Select key '7' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x1038:
        case '8':
          KEY = 0x28;  // Select key '8' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x1039:
        case '9':
          KEY = 0x29;  // Select key '9' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
                      // NOW SYMBOLS
        case 0x103A:
        case ':':
          KEY = 0x2A;  
          SHIFT = 0;  // Select key ':' convert key code so decoder can slect correct matrix key(s)
          break;
        case '[':
        case 0x105B:
          KEY = 0x1C;  // Select key '[' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        case 0x105D:
        case ']':
          KEY = 0x1E;  // Select key ']' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        case 0x103B:
        case ';':
          KEY = 0x2B;  // Select key ';' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x102C:
        case ',':
          KEY = 0x2C;  // Select key ',' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x102D:
        case '-':
          KEY = 0x2D;  // Select key '-' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x102E:
        case '.':
          KEY = 0x2E;  // Select key '.' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x102F:
        case '/':
          KEY = 0x2F;  // Select key '/' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 0;
          break;
        case 0x1021:
        case '!':
          KEY = 0x21;  // Select key '!' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        case 0x1022:
        case '"':
          KEY = 0x22;  // Select key '"' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        case 0x1023:
        case '#':
          KEY = 0x23;  // Select key '#' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        case 0x1024:
        case '$':
          KEY = 0x24;  // Select key '$' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        //case 0x1022:
         case 0X1025:
       case '%':
          KEY = 0x25;  // Select key '%' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        case 0x1026:
        case '&':
          KEY = 0x26;  // Select key '&' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        case 0x1027:
        case 0x0027:
          KEY = 0x27;  // Select key ''' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        case 0x1028:
        case '(':
          KEY = 0x28;  // Select key '(' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        case 0x1029:
        case ')':
          KEY = 0x29;  // Select key ')' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        case 0x102A:
        case '*':
          KEY = 0x2A;  // Select key '*' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        case 0x102B:
        case '+':
          KEY = 0x2B;  // Select key '+' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        //case 0x102C:
        case 0X103C:
        case '<':
          KEY = 0x2C;  // Select key '<' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        //case 0x102D:
        case 0x103D:
        case '=':
          KEY = 0x2D;  // Select key '=' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
        
        case 0X103E:
        case '>':
          KEY = 0x2E;  // Select key '>' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;        
        case '?':
          KEY = 0x2F;  // Select key '?' convert key code so decoder can slect correct matrix key(s)  
          SHIFT = 1;
          break;
           // CONTROL KEYS ,ENTER> ... ect              
          
          case 0x000D :      // <ENTER KEY> 
          case 0X100D:      // <ENTER WITH SHIFT LOCK ON>
          KEY = 0x30;  
          SHIFT = 0;
          break;
          case 0x301B:   //ESCAPE KEY + CNTL = <BREAK> key
          case 0x201B:   //ESCAPE KEY + CNTL
          KEY = 0x32;   // CoCo <Break> key
          SHIFT = 0;
          break;
          case 0x401B:   //ESCAPE KEY shifted
          case 0x501B:   //ESCAPE KEY shifted and shift lock on
          KEY = 0x34;   // CoCo <Break> key  
          SHIFT = 0;
          break;
          case 0x101B:   //ESCAPE KEY shift lock on
          case 0x001B:   //ESCAPE KEY
          KEY = 0x31;   // CoCo <CLEAR> key
          SHIFT = 0;
          break;
         
          
        
      case 0x387F:  // shift lock on  left _  CNTL+ALT+DEL
        case 0x287F:  // shift lock off left _  CNTL+ALT+DEL
        case 0x247F:  // shift lock off right _ CNTL+ALT+DEL
        case 0x347F:  // shift lock on  right _ CNTL+ALT+DEL
        case 0x281B:  // shift lock on  right _ CNTL+ALT+ESC
        case 0x381B:  // shift lock on  right _ CNTL+ALT+ESC

          digitalWrite(CPU_RESET, LOW); // RESET CPU
           delay(750);  // hold low for 1/2 second
          digitalWrite(CPU_RESET, HIGH); //RELEASE CPU RESET
          Serial.println("\n");
          Serial.println("CPU_RESET\n");
          break;
          case 0x1103:
          Shift=1;
          break;
                   
        }
            
        delay( 1 );
       
    }
      else
      Serial.println( " Keyboard protocol or function" );
      Serial.println(code);
     
        chrout(KEY,SHIFT);
            KEY    = 0;
            SHIFT  = 0;
            code   = 0; 
 
        //delay( 1 );
  }
        KEY=0;
        SHIFT=0;
        code=0; 
 
}
  //chrout:
void chrout(int KEY,int SHIFT)
{
  if (KEY != 0)
  {
         /*
              //>RESET  __________________________________________________/"""""""""""\_____________
              //>STROBE ________/""""""""\________________________/"""""""""""""""""""""""""""""""""
              //>ADDRESS(S)___<=============><======================================================
              //>DATA BIT==============<===========>================================================
              // SWITCH SLECTED =================<=========>========================================
              //DATA   DATA IS (0,0,AY2,AY1,AY0,AX2,AX1,AX0)
              //CONTROL DATA IS (RESET COMPUTER,,,,,DATA"ON/OFF",STROBE,RESET clears keyboard memory arry switchs)
              */
   if (SHIFT != 0) 
    {
      Serial.println("shifted char");
      // this just sets/pushes the shift KEY on/down
      PORTC=0x37; // shift key value
      digitalWrite(KEY_STROBE, HIGH);
      digitalWrite(KEY_DATA, HIGH);
      delay(10);
      digitalWrite(KEY_STROBE, LOW);
      
      //wait 1ms
      SHIFT = 0;
      //end of shifted function Shift key is still pressed/on
    }

      //slect the key that is pressed
      if (KEY == 0x40) { KEY  =0;}  // work around for @ symbol key code is 0
      Serial.println("unshifted char");
      PORTC=KEY;
      digitalWrite(KEY_STROBE, HIGH);
      digitalWrite(KEY_DATA, HIGH); //KEYPRESS DOWN
      delay(Wait_Time);
      digitalWrite(KEY_STROBE, LOW);
      PORTC=KEY;
      digitalWrite(KEY_STROBE, HIGH);
      digitalWrite(KEY_DATA, LOW); //KEYPRESS UP
      digitalWrite(KEY_STROBE, LOW);
      PORTC = 0;
      digitalWrite(KEY_RESET, HIGH);      // clears keyboard interface Matrix Switches
      digitalWrite(KEY_RESET,LOW);
      KEY=0;
      code=0; 
      SHIFT=0;
  
  }
}

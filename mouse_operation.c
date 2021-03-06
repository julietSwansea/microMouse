///
/// @file       mouse_operation.c
/// @author     Kyeong Soo (Joseph) Kim <k.s.kim@swansea.ac.uk>
/// @date       2012-02-21
///
/// @brief      Implements functions for mouse operation modes.
///
/// @remarks    This program is based on Dr Tim Davies' assembly version of
///             micro mouse program and the C port of it by Mr Gareth Evans.
///
/// @copyright  Copyright (C) 2012 Swansea University. All rights reserved.
///
/// @copyright  This software is written and distributed under the GNU General
///             Public License Version 2 (http://www.gnu.org/licenses/gpl-2.0.html).
///             You must not remove this notice, or any other, from this software.
///


#include "mouse.h"	// for the declaration of types, constants, variables and functions


void AvoidObstacle()                                   
{                                                                                                                                                           
    mouseMode = MOUSE_MODE_OBSTACLE_AVOIDING;

    for (;;) {
        // first move forward
        ControlMouse(MOUSE_ACTION_FORWARD);

        // first, check the status of touch bars
        if (!touchBarFrontLeft && !touchBarFrontRight && !touchBarRearLeft && !touchBarRearRight) {
        
            ControlMouse(MOUSE_ACTION_FORWARD);
            // neither is touched (i.e., both the values are zero)
            
            // then check the status of IF sensors
            if (!infraredFrontLeft && !infraredFrontRight) {
                // neither is touched (i.e., both the values are zero)
                // then, back to the loop
                ControlMouse(MOUSE_ACTION_FORWARD);
            }
            else if (infraredFrontLeft && !infraredFrontRight) {
                // left sensor detects; avoid left obstacle
                ControlMouse(MOUSE_ACTION_STOP);
                Delay(300);
                ControlMouse(MOUSE_ACTION_TURNRIGHT);
                Delay(300);
            }
            else if (!infraredFrontLeft && infraredFrontRight) {
                // right sensor detects; avoid right obstacle
                ControlMouse(MOUSE_ACTION_STOP);
                Delay(300);
                ControlMouse(MOUSE_ACTION_TURNLEFT);
                Delay(300);
            }
            else {
                // both sensors detect; avoid front obstacle
                ControlMouse(MOUSE_ACTION_STOP);
                 Delay(500);
                ControlMouse(MOUSE_ACTION_REVERSE);
                 Delay(500);
                ControlMouse(MOUSE_ACTION_TURNAROUND);	// 180 dgree turn
            } 
        }     
        
      else if (!touchBarFrontLeft && touchBarFrontRight) {
            // left bar is touched; avoid left obstacle
           
            ControlMouse(MOUSE_ACTION_STOP);
            Delay(500);
            ControlMouse(MOUSE_ACTION_REVERSE);
            Delay(500);
            ControlMouse(MOUSE_ACTION_TURNRIGHT);
            Delay(500);
        }        
        else if (touchBarFrontLeft && !touchBarFrontRight) {
            // right bar is touched; avoid right obstacle
            
            ControlMouse(MOUSE_ACTION_STOP);
            Delay(500);
            ControlMouse(MOUSE_ACTION_REVERSE);
            Delay(500);
            ControlMouse(MOUSE_ACTION_TURNLEFT);
            Delay(500);
        }  
           
         else if (!touchBarRearLeft && touchBarRearRight) { //test for left
            // left bar is touched; avoid left obstacle
             
            ControlMouse(MOUSE_ACTION_STOP);
            Delay(500);
            ControlMouse(MOUSE_ACTION_REVERSE);
            Delay(500);
            ControlMouse(MOUSE_ACTION_TURNRIGHT);
            Delay(500);
        }
        else if (touchBarRearLeft && !touchBarRearRight) { //test for right
            // right bar is touched; avoid right obstacle 
            
            ControlMouse(MOUSE_ACTION_STOP);
            Delay(500);
            ControlMouse(MOUSE_ACTION_REVERSE);
            Delay(500);
            ControlMouse(MOUSE_ACTION_TURNLEFT);
            Delay(500);
        } 
       else {
            // both bars are touched; avoid front obstacle 
            
            ControlMouse(MOUSE_ACTION_STOP); 
            Delay(500);
            ControlMouse(MOUSE_ACTION_REVERSE);
            Delay(500);
            ControlMouse(MOUSE_ACTION_TURNAROUND);	// 180 dgree turn
        }  
    } // end of for() loop
}


void LineFollowing ()
{    
     //#define black 1
     //#define white 0   
    byte fl,fr,rl,rr;
    byte fl_max,fl_min,fr_max,fr_min,rl_max,rl_min,rr_max,rr_min;
    byte flTH=0,frTH,rlTH,rrTH; //ldr thresholds
    byte tmp=0; // ldr adc variables 
    byte black=0;
    byte white=1;
     
    
    // speed control
    pwLeft = (word)(0.6*defaultSpeed);
    pwRight = (word)(0.6*defaultSpeed);
    
  while(tmp==0){
    
   // threshold    
  if (touchBarFrontLeft) {
    // on the white surface
    fl_max = ADCRead(0x00);
    fr_max = ADCRead(0x01);
    rl_max = ADCRead(0x02);
    rr_max = ADCRead(0x03);
    
    while (touchBarFrontLeft) {
    }
    
    // on the black surface
    fl_min = ADCRead(0x00);
    fr_min = ADCRead(0x01);
    rl_min = ADCRead(0x02);
    rr_min = ADCRead(0x03);
    tmp=1;    
    }
  }
  
  
  flTH = (byte)((fl_max + fl_min)/2.0);
  frTH = (byte)((fr_max + fr_min)/2.0);
  rlTH = (byte)((rl_max + rl_min)/2.0); 
  rrTH = (byte)((rl_max + rr_min)/2.0);
//}
    
    mouseMode = MOUSE_MODE_LINE_FOLLOWING;
  
    for (;;) {
       
        
       // ADC
       tmp = ADCRead(0x00);

       fl = tmp < flTH ? 0 : 1; // 1 -> white; 0 -> black
       
       tmp = ADCRead(0x01);
      
       fr = tmp < frTH ? 0 : 1;
       
       tmp = ADCRead(0x02);
 
       rl = tmp < rlTH ? 0 : 1;
       
       tmp = ADCRead(0x03);
       
       rr = tmp < rrTH ? 0 : 1;
       
     
     
       
      if (fl == 1 && fr == 1 && rl == 0 && rr == 0 ) {
            ControlMouse(MOUSE_ACTION_FORWARD);
      }
        
        
         //start here
        
       else if (fl == 0 && fr == 1){
            ControlMouse(MOUSE_ACTION_BRAKE); 
            Delay(1);
            ControlMouse(MOUSE_ACTION_TURNRIGHT);
            //Delay(5);                 
        } else if (fl == 1 && fr == 0) {     
            ControlMouse(MOUSE_ACTION_BRAKE);
            Delay(1);
            ControlMouse(MOUSE_ACTION_TURNLEFT);
            //Delay(5);
        } else if (rl == 1 && rr == 0){
          ControlMouse(MOUSE_ACTION_BRAKE);
           Delay(1);
          ControlMouse(MOUSE_ACTION_TURNRIGHT); 
          //Delay(5);
        } else if (rl == 0 && rr == 1){
          ControlMouse(MOUSE_ACTION_BRAKE); 
           Delay(1);
          ControlMouse(MOUSE_ACTION_TURNLEFT);
          //Delay(5);
        } else if (fr == 0 && rl == 1){
          ControlMouse(MOUSE_ACTION_BRAKE);
          Delay(1);
          ControlMouse(MOUSE_ACTION_TURNRIGHT);
          //Delay(5);
        } else if (fl == 0 && fr  == 1){
          ControlMouse(MOUSE_ACTION_BRAKE);
          Delay(1);
          ControlMouse(MOUSE_ACTION_TURNLEFT);
          //Delay(5);
        } 
       else if (fl == 0 && fr  == 0){
          ControlMouse(MOUSE_ACTION_BRAKE);
          Delay(50);
          ControlMouse(MOUSE_ACTION_TURNRIGHT);
          Delay(50);
        } 
        else if (fl == 0 && fr == 0 && rl == 1 && rr == 1){
          ControlMouse(MOUSE_ACTION_BRAKE);
          Delay(100);
          ControlMouse(MOUSE_ACTION_TURNRIGHT);
          Delay(50);
        } else if (fl == 0 && fr == 0 && rl == 1 && rr == 0){
          ControlMouse(MOUSE_ACTION_BRAKE);
          Delay(1);
          ControlMouse(MOUSE_ACTION_TURNLEFT);
          Delay(5);
        } else if (fl == 0 && fr == 0 && rl == 0 && rr == 1){
          ControlMouse(MOUSE_ACTION_BRAKE);
          Delay(50);
          ControlMouse(MOUSE_ACTION_TURNRIGHT);
          Delay(5);
        } else if (fl == 1 && fr == 0 && rl == 1 && rr == 0){
          ControlMouse(MOUSE_ACTION_BRAKE);
          Delay(1);
          ControlMouse(MOUSE_ACTION_TURNLEFT);
          Delay(5);
        } else if (fl == 0 && fr == 1 && rl == 1 && rr == 0){
          ControlMouse(MOTOR_ACTION_BRAKE);
          Delay(1);
          ControlMouse(MOUSE_ACTION_TURNRIGHT);
          Delay(5);
        } else if (fl == 0 && fr == 1 && rl == 1 && rr == 1){
          ControlMouse(MOTOR_ACTION_BRAKE);
          Delay(1);
          ControlMouse(MOUSE_ACTION_TURNRIGHT);
          Delay(5);
        } else if (fl == 0 && fr == 1 && rl == 0 && rr == 1){
          ControlMouse(MOUSE_ACTION_BRAKE);
          Delay(1);
          ControlMouse(MOUSE_ACTION_TURNRIGHT);
          Delay(5);
        } else if (fl == 1 && fr == 1 && rl == 0 && fr == 1){
          ControlMouse(MOUSE_ACTION_BRAKE);
          Delay(4);
          ControlMouse(MOUSE_ACTION_TURNLEFT);
          Delay(5);
        } 
        
        else                
              {
              
              ControlMouse(MOUSE_ACTION_FORWARD) ;
              
              
              }
      
           //end here
      
       
    
                 
      
    }// end of for() loop;
}


void Combat()
{
    // fill your code here for the combat mode 
    byte fl,fr,rl,rr;
    byte fl_max,fl_min,fr_max,fr_min,rl_max,rl_min,rr_max,rr_min;
    byte flTH=0,frTH,rlTH,rrTH; //ldr thresholds
    byte tbfl, tbfr, tbrl, tbrr;
    byte tmp=0; // ldr adc variables 
    byte black=0;
    byte white=1;
     
    
    // speed control
    pwLeft = 75;//(word)(0.8*defaultSpeed);
    pwRight =75;//(word)(0.8*defaultSpeed);
    
    while(tmp==0){
    
        // threshold    
        if (touchBarFrontLeft) {
            // on the white surface
            fl_max = ADCRead(0x00);
            fr_max = ADCRead(0x01);
            rl_max = ADCRead(0x02);
            rr_max = ADCRead(0x03);
    
            while (touchBarFrontLeft) {
            }
    
            // on the black surface
            fl_min = ADCRead(0x00);
            fr_min = ADCRead(0x01);
            rl_min = ADCRead(0x02);
            rr_min = ADCRead(0x03);
            tmp=1;    
        }
    }
  
    flTH = (byte)((fl_max + fl_min)/2.0);
    frTH = (byte)((fr_max + fr_min)/2.0);
    rlTH = (byte)((rl_max + rl_min)/2.0); 
    rrTH = (byte)((rl_max + rr_min)/2.0);
//}        
 
    mouseMode = MOUSE_MODE_COMBAT;
  
    for (;;) {   
        
        // ADC
        tmp = ADCRead(0x00);
        fl = tmp < flTH ? 0 : 1; // 1 -> white; 0 -> black
       
        tmp = ADCRead(0x01);
        fr = tmp < frTH ? 0 : 1;
       
        tmp = ADCRead(0x02); 
        rl = tmp < rlTH ? 0 : 1;
       
        tmp = ADCRead(0x03);
        rr = tmp < rrTH ? 0 : 1;
       
        tbfl = touchBarFrontLeft;
        tbfr = touchBarFrontRight;
        tbrr = touchBarRearRight;
        tbrl = touchBarRearLeft;      

        if (!infraredFrontLeft && !infraredFrontRight && !infraredRearLeft && !infraredRearRight) {       
            if (fl == 1 && fr == 1 && rl == 1 && rr == 1 ) {
                // default
                while ((tbfl == 1) || (tbfr == 1)) {
                    // back off
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(200);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);    
                    tbfl = touchBarFrontLeft;
                    tbfr = touchBarFrontRight;
                }
          
                if ((tbrl == 1) || (tbrr == 1)) {
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(50);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(25);       
                } else {
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);
                }
            } else if (fl == 0 || fr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_REVERSE);
                Delay(50);
                ControlMouse(MOUSE_ACTION_TURNAROUND);
                Delay(100);
            } else if (rl == 0 || rr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_FORWARD);
                Delay(25);
            }
            /*else {
                // both bars are touched; avoid front obstacle 
                ControlMouse(MOUSE_ACTION_STOP); 
                Delay(500);
                ControlMouse(MOUSE_ACTION_REVERSE);
                Delay(500);
                ControlMouse(MOUSE_ACTION_TURNAROUND);	// 180 dgree turn
            }*/
            ControlMouse(MOUSE_ACTION_FORWARD);
        } else if (!infraredFrontLeft && infraredFrontRight) {
            if (fl == 1 && fr == 1 && rl == 1 && rr == 1 ) {
                // default
                while ((tbfl == 1) || (tbfr == 1)) {
                    // back off
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(200);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);    
                    tbfl = touchBarFrontLeft;
                    tbfr = touchBarFrontRight;
                }
          
                if ((tbrl == 1) || (tbrr == 1)) {
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(50);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(25);       
                } else {
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);
                }
            } else if (fl == 0 || fr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_REVERSE);
                Delay(50);
                ControlMouse(MOUSE_ACTION_TURNAROUND);
                Delay(100);
            } else if (rl == 0 || rr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_FORWARD);
                Delay(25);
            }      
        
            ControlMouse(MOUSE_ACTION_TURNRIGHT);
            Delay(200);                        
         /*else {
            // just for TEST!
            ControlMouse(MOUSE_ACTION_STOP);
            Delay(500);
        }  */
        
      } else if (infraredFrontLeft && !infraredFrontRight) {
            if (fl == 1 && fr == 1 && rl == 1 && rr == 1 ) {
                // default
                while ((tbfl == 1) || (tbfr == 1)) {
                    // back off
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(200);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);    
                    tbfl = touchBarFrontLeft;
                    tbfr = touchBarFrontRight;
                }
          
                if ((tbrl == 1) || (tbrr == 1)) {
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(50);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(25);       
                } else {
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);
                }
            } else if (fl == 0 || fr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_REVERSE);
                Delay(50);
                ControlMouse(MOUSE_ACTION_TURNAROUND);
                Delay(100);
            } else if (rl == 0 || rr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_FORWARD);
                Delay(25);
            }      
            //ControlMouse(MOUSE_ACTION_REVERSE);
            //Delay(200);
            ControlMouse(MOUSE_ACTION_TURNLEFT);
            Delay(200);                
      } else if (infraredFrontLeft && infraredFrontRight) {
            if (fl == 1 && fr == 1 && rl == 1 && rr == 1 ) {
                // default
                while ((tbfl == 1) || (tbfr == 1)) {
                    // back off
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(200);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);    
                    tbfl = touchBarFrontLeft;
                    tbfr = touchBarFrontRight;
                }
          
                if ((tbrl == 1) || (tbrr == 1)) {
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(50);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(25);       
                } else {
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);
                }
            } else if (fl == 0 || fr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_REVERSE);
                Delay(50);
                ControlMouse(MOUSE_ACTION_TURNAROUND);
                Delay(100);
            } else if (rl == 0 || rr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_FORWARD);
                Delay(25);
            }      
            //ControlMouse(MOUSE_ACTION_REVERSE);
            //Delay(200);
            ControlMouse(MOUSE_ACTION_FORWARD);
            Delay(200);                  
      } else if (infraredRearLeft && infraredRearRight) {
            if (fl == 1 && fr == 1 && rl == 1 && rr == 1 ) {
                // default
                while ((tbfl == 1) || (tbfr == 1)) {
                    // back off
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(200);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);    
                    tbfl = touchBarFrontLeft;
                    tbfr = touchBarFrontRight;
                }
          
                if ((tbrl == 1) || (tbrr == 1)) {
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(50);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(25);       
                } else {
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);
                }
            } else if (fl == 0 || fr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_REVERSE);
                Delay(50);
                ControlMouse(MOUSE_ACTION_TURNAROUND);
                Delay(100);
            } else if (rl == 0 || rr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_FORWARD);
                Delay(25);
            }
              ControlMouse(MOUSE_ACTION_TURNAROUND);
              Delay(500);      
           // ControlMouse(MOUSE_ACTION_REVERSE);
            //Delay(200);
           // ControlMouse(MOUSE_ACTION_FORWARD);
            //Delay(200);                  
      } else if (infraredRearLeft && !infraredRearRight) {
            if (fl == 1 && fr == 1 && rl == 1 && rr == 1 ) {
                // default
                while ((tbfl == 1) || (tbfr == 1)) {
                    // back off
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(200);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);    
                    tbfl = touchBarFrontLeft;
                    tbfr = touchBarFrontRight;
                }
          
                if ((tbrl == 1) || (tbrr == 1)) {
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(50);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(25);       
                } else {
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);
                }
            } else if (fl == 0 || fr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_REVERSE);
                Delay(50);
                ControlMouse(MOUSE_ACTION_TURNAROUND);
                Delay(100);
            } else if (rl == 0 || rr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_FORWARD);
                Delay(25);
            }
            ControlMouse(MOUSE_ACTION_TURNAROUND);
            Delay(500);      
            //ControlMouse(MOUSE_ACTION_REVERSE);
            //Delay(200);
            //ControlMouse(MOUSE_ACTION_TURNLEFT);
            //Delay(200); 
      } else if (!infraredRearLeft && infraredRearRight) {
            if (fl == 1 && fr == 1 && rl == 1 && rr == 1 ) {
                // default
                while ((tbfl == 1) || (tbfr == 1)) {
                    // back off
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(200);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);    
                    tbfl = touchBarFrontLeft;
                    tbfr = touchBarFrontRight;
                }
          
                if ((tbrl == 1) || (tbrr == 1)) {
                    ControlMouse(MOUSE_ACTION_BRAKE);
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(50);
                    ControlMouse(MOUSE_ACTION_REVERSE);
                    Delay(25);       
                } else {
                    ControlMouse(MOUSE_ACTION_FORWARD);
                    Delay(25);
                }
            } else if (fl == 0 || fr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_REVERSE);
                Delay(50);
                ControlMouse(MOUSE_ACTION_TURNAROUND);
                Delay(100);
            } else if (rl == 0 || rr == 0){
                ControlMouse(MOUSE_ACTION_BRAKE);
                ControlMouse(MOUSE_ACTION_FORWARD);
                Delay(25);
            }
            ControlMouse(MOUSE_ACTION_TURNAROUND);
            Delay(500);      
            //ControlMouse(MOUSE_ACTION_REVERSE);
            //Delay(200);
            //ControlMouse(MOUSE_ACTION_TURNRIGHT);
            //Delay(200); 
      }
       
     
      
        else {
            // both bars are touched; avoid front obstacle 
            
            ControlMouse(MOUSE_ACTION_STOP); 
            Delay(500);
            ControlMouse(MOUSE_ACTION_REVERSE);
            Delay(500);
            ControlMouse(MOUSE_ACTION_TURNAROUND);	// 180 dgree turn
        }
        
      
    } // end of for()
}


// debug mode with simple command-line interface
void Debug()
{
    byte command;

    // display a welcome message with a list of commands
    SCISendNewLine();
    SCISendStr("Welcome to the debug mode of EG-252 sample micromouse programme!\r\n");
    SCISendNewLine();
    SCISendNewLine();
    SCISendStr("List of available commands:\r\n");
    SCISendStr("F\tForward\r\n");
    SCISendStr("R\tReverse\r\n");
    SCISendStr("S\tStop\r\n");
    SCISendStr("A\troate Anticlockwise\r\n");
    SCISendStr("C\trotate Clockwise\r\n");
    SCISendStr("V\tVeer left Clockwise\r\n");
    SCISendStr("B\tVeer right Clockwise\r\n");
    SCISendStr("+\tIncrement speed by 256 units\r\n");
    SCISendStr("-\tDecrement speed by 256 units\r\n");
    SCISendStr("D\tDisplay ADC value 7 through 0\r\n");
    SCISendStr("P\tDisplay PTA as binary number\r\n");

   while (1) {
        // display prompt and wait for a user input
        SCIDisplayPrompt();
        command = SCIGetChar();
        
        // TEST
        SCIDisplayBitString(command);
        SCIDisplayBitString((char)'F');
        SCIDisplayBitString((byte)'R');
        SCIDisplayBitString('S');
        if ((int)command == 70)
        {
            SCISendStr("Forward ... \r\n");
        }
        if ((command & 0b01111111) == 0x52)
        {
            SCISendStr("Reverse ... \r\n");
        }
        if ((int)command > 256) 
        {
            SCISendStr("Greater than 256\r\n");
        }
        else if ((int)command > 128) 
        {
            SCISendStr("Greater than 128\r\n");
        }
        else if ((int)command > 64)
        {
            SCISendStr("Greater than 64\r\n");
        }
        else
        {
            SCISendStr("Less than or eqal to 64\r\n");
        }
        // TEST

        switch(command) {
            case 70:  // ascii code for 'F'
                SCISendStr("Forward ... \r\n");
                break;
            case 82:  // ascii code for 'R'
                SCISendStr("Reverse ... \r\n");
                break;
            case 83:  // ascii code for 'S'
                SCISendStr("Stop ... \r\n");
                break;
            case 'A':
                break;
            case 'C':
                break;        
            case '+':
                break;
            case '-':
                break;
            case 'D':
                break;
            case 'P':
                break;
            case 'V':
                break;
            case 'B':
                break;
            case '?':
                break;
            default:
                break;
        }
    }   // end of while ()
}


// simple test mode for testing functions
void Test()
{
  int opDelay = 10000;
  
  while (1) {
    ControlMouse(MOUSE_ACTION_FORWARD);
    Delay(opDelay);
  /*
    ControlMouse(MOUSE_ACTION_FORWARD);
    Delay(opDelay);
    ControlMouse(MOUSE_ACTION_REVERSE);
    Delay(opDelay);
    ControlMouse(MOUSE_ACTION_TURNLEFT);
    Delay(opDelay);
    ControlMouse(MOUSE_ACTION_TURNRIGHT);
    Delay(opDelay);
    ControlMouse(MOUSE_ACTION_TURNAROUND);
    Delay(opDelay);
    ControlMouse(MOUSE_ACTION_STOP);
    Delay(opDelay);
    */
  }
}


// ADC test mode for line following sensors
/*void ADCTest()
{
    while (1)
    {
        ADCRead();
    }
}  */
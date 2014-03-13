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
                ControlMouse(MOTOR_ACTION_STOP);
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
    
  while(tmp==0){
    
   // threshold    
  if (touchBarFrontLeft) {
    // on the black surface
    fl_max = ADCRead(0x00);
    fr_max = ADCRead(0x01);
    rl_max = ADCRead(0x02);
    rr_max = ADCRead(0x03);
    
    while (touchBarFrontLeft) {
    }
    
    // on the white surface
    fl_min = ADCRead(0x00);
    fr_min = ADCRead(0x01);
    rl_min = ADCRead(0x02);
    rr_min = ADCRead(0x03);
    tmp=1;    
    }
  }
  
  
  flTH = (fl_max + fl_min)/2;
  frTH = (fr_max + fr_min)/2;
  rlTH = (rl_max + rl_min)/2; 
  rrTH = (rl_max + rr_min)/2;
//}
    
    mouseMode = MOUSE_MODE_LINE_FOLLOWING;
  
    for (;;) {
       
       // first move forward
        ControlMouse(MOUSE_ACTION_FORWARD);
        
       // ADC
       tmp = ADCRead(0x00);
       /*
       if (tmp < flTH) {
        fl = 1; // white
       else
        fl = 0; // black
       }
       */
       fl = tmp < flTH ? 0 : 1; // 0 -> black; 1 -> white
       
       tmp = ADCRead(0x01);
      
       fr = tmp < frTH ? 0 : 1;
       
       tmp = ADCRead(0x02);
 
       rl = tmp < rlTH ? 0 : 1;
       
       tmp = ADCRead(0x03);
       
       rr = tmp < rrTH ? 0 : 1;
       
       
  
   //}
      
        // first, check the status of touch bars
        if (fl == 0 && fr == 0 && rl == 0 && rr == 0 ) {
             ControlMouse(MOTOR_ACTION_FORWARD);
            // all is touched (i.e., both the values are one)
        }
            // then check the status of IF sensors
            else if (fl == 1 && fr == 0) {
                // front left whiteline detects 
                ControlMouse(MOUSE_ACTION_TURNLEFT); 
                Delay(30);                
            }
            else if (fl == 0 && fr == 1) {
                // front right whiteline detects
                ControlMouse(MOUSE_ACTION_TURNRIGHT);
                Delay(30);
            }
            else if (rl == 1 && rr == 0) {
                // rear left detects; avoid right obstacle
                ControlMouse(MOUSE_ACTION_TURNLEFT);
                Delay(30);
            } 
            else if (rl == 0 && rr == 1){
            // rear right white line detects
            ControlMouse(MOUSE_ACTION_TURNRIGHT);
            Delay(30);
            }
            else if (fr ==
              
           
           
      
    }// end of for() loop
}


void Combat()
{
    // fill your code here for the combat mode    
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
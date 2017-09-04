#include <Arduboy2.h>
#include "bitmaps.h"

Arduboy2 arduboy;
Sprites sprites;

// Things that make the game work the way it does
#define FRAMES_PER_SECOND 30   // The update and refresh speed

//VERMIN
#define UPTIME 10
#define MTIME 30
#define HUMP_LINE 45
#define HUMP_1_X 21
#define HUMP_2_X 38
#define HUMP_3_X 48 
#define HUMP_4_X 80
#define HUMP_5_X 95
#define MAN_1_X 22
#define MAN_2_X 22
#define MAN_3_X 22
// Storage Vars
unsigned char ncount;
unsigned char gamelevel;
unsigned char humplevel;
unsigned char subcount;
unsigned char gametime;
unsigned char ltimer;
unsigned char utimer;
unsigned char mtimer;
unsigned int score;
unsigned char mole;
unsigned char manpos;
unsigned char humpstat[6];  //humpstat[1] - humpstat[5]
unsigned char molemiss;
unsigned char hump;
unsigned char nexthump;
unsigned int tmr3ov;      //Timer 3 overflow counter
unsigned char tick;
unsigned int beep1;       //mole beep
unsigned int beep2;       //hit beep
unsigned int beep3;       //miss beep
unsigned int AUTOcount;
unsigned int leftBuffer;
unsigned int rightBuffer;
unsigned int MOLE_1_X = HUMP_1_X + 1;
unsigned int MOLE_2_X = HUMP_2_X + 2;
unsigned int MOLE_3_X = HUMP_3_X + 10;
unsigned int MOLE_4_X = HUMP_4_X - 6;
unsigned int MOLE_5_X = HUMP_5_X - 3;
unsigned int MOLE_LINE = HUMP_LINE - 8;
unsigned int MAN_Y = HUMP_LINE - 33;
unsigned int MAN_POS = 0;
unsigned int MAN_STATE = 0;
//  unsigned char mask;
//  char string[4];

//pseudo random numbers
unsigned char prand;

///MAN 84,20
//MOLE 12, 8
//LIFE 11, 6
//MOLE_CLEAR 1
unsigned int humpFrame;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(FRAMES_PER_SECOND);
  arduboy.clear();
  arduboy.display();
  delay(500);
  String title = "VERMUINO";
  arduboy.print(title);
  //arduboy.drawBitmap(MAN_1_X, MAN_Y, man_1u, 84, 20);
  arduboy.drawBitmap(MAN_2_X, MAN_Y, man_2u, 84, 20);
  //arduboy.drawBitmap(MAN_3_X, MAN_Y, man_3u, 84, 20); 
  drawHump(HUMP_1_X, HUMP_LINE, hump_1, 8, 3);
  drawHump(HUMP_2_X, HUMP_LINE, hump_2, 11, 3);
  drawHump(HUMP_3_X, HUMP_LINE, hump_3, 16, 3);
  drawHump(HUMP_4_X, HUMP_LINE, hump_4, 11, 3);
  drawHump(HUMP_5_X, HUMP_LINE, hump_5, 8, 3);
  arduboy.drawBitmap(MOLE_1_X, MOLE_LINE, mole_1, 12, 8);
  arduboy.drawBitmap(MOLE_2_X, MOLE_LINE, mole_1, 12, 8);
  arduboy.drawBitmap(MOLE_3_X, MOLE_LINE, mole_2, 12, 8);
  arduboy.drawBitmap(MOLE_4_X, MOLE_LINE, mole_3, 12, 8);
  arduboy.drawBitmap(MOLE_5_X, MOLE_LINE, mole_3, 12, 8);
  arduboy.drawBitmap(MOLE_1_X, MOLE_LINE, mole_1_hit, 12, 8);
  arduboy.drawBitmap(MOLE_2_X, MOLE_LINE, mole_1_hit, 12, 8);
  arduboy.drawBitmap(MOLE_3_X, MOLE_LINE, mole_2_hit, 12, 8);
  arduboy.drawBitmap(MOLE_4_X, MOLE_LINE, mole_3_hit, 12, 8);
  arduboy.drawBitmap(MOLE_5_X, MOLE_LINE, mole_3_hit, 12, 8);
  
  drawGround();
  arduboy.display();
  humpFrame = 2;
  
  gamelevel = 2;
  humplevel = 4;
  nexthump = 0;
  subcount = 0;
  ltimer = 0;
  utimer = 0;
  tmr3ov = 0;
  tick = 1;
  score = 0;
  manpos = 1;
  mole = 0;
  molemiss = 0;
  beep1 = 0;
  beep2 = 0;
  beep3 = 0;
  
  humpstat[0] = 0xff;
  humpstat[1] = 0;  //0 = off
  humpstat[2] = 0;  //0 = off
  humpstat[3] = 0;  //0 = off
  humpstat[4] = 0;  //0 = off
  humpstat[5] = 0;  //0 = off
  arduboy.setCursor(58, 48);
  while (!arduboy.buttonsState()) { }; // Wait for any key press  

}

void loop() {

  if (!(arduboy.nextFrame())) {  return; }
 
  arduboy.clear();
  arduboy.pollButtons();
  drawGround();
  drawMan(manpos, MAN_STATE);
  if ((utimer == 0) && (gamelevel > 0)) {
    if (leftPressed())
    {
      if(manpos > 0)
      {
        manpos--;
      } 
    } 
    if (rightPressed())
    {
      if(manpos < 2)
      {
        manpos++;
      } 
    }
  }

    if(tick == 1) {
      //tick = 0;
      AUTOcount++;
      if (AUTOcount > 3000) {
        AUTOcount = 0;
      }

      /*
      if (gamelevel == 0)
      {
        if (SWST_MODE)
        {
          gamelevel = 1;    //Game A
          gametime = 35;
          subcount = 10;
        } 
        if (SWST_PRO)
        {
          gamelevel = 2;    //Game B
          gametime = 25;
          subcount = 6;
        }
       
      }
      else
      {
        if (SWST_MODE)
        {
          if (sound) sound = 0;
          else sound = 0xff;
        } 
      */
    ltimer++;
    if (ltimer >= gametime){
      ltimer = 0;
    if (!mtimer)  //don't auto inc if mole miss sequence in progress
     {
        //speed 1 1 sec
        //1   on 4  10
        //speed 2 0.8 sec
        //11  on 3 8  7,8,9
        //19  on 2 6  4,5,6
        //25  on 4 4  3,4,5
        //speed 3 0.7
        //29  on 3 7
        //36  on 2 5
        //41  on 4 5
        //speed 4 0.6
        //46  on 3 8
        //54  on 2 5
        //59  on 4 3
        //speed 5 0.5
        //62  on 3 9
        //71  on 2 4
        //75  on 4 4
        //79  on 3 9
        //88  on 2 6
        //94  on 4 3
        //97  on 3 8
        //105 on 2 5
        //110 on 4 3
        //113 on 3 
        //
        
        if ((humpstat[1] > 0) && (humpstat[1] < 6)) humpstat[1]++; beep1 = 40;
        if ((humpstat[2] > 0) && (humpstat[2] < 6)) humpstat[2]++; beep1 = 40;
        if ((humpstat[3] > 0) && (humpstat[3] < 6)) humpstat[3]++; beep1 = 40;
        if ((humpstat[4] > 0) && (humpstat[4] < 6)) humpstat[4]++; beep1 = 40;
        if ((humpstat[5] > 0) && (humpstat[5] < 6)) humpstat[5]++; beep1 = 40;
        
        ncount=0;
        if (gamelevel == 1)
        {
          if (score < 10) gametime = 35;    //speed 1 1 sec
          else if (score < 25) gametime = 30; //speed 2 0.8 sec
          else if (score < 45) gametime = 25; //speed 3 0.7
          else if (score < 60) gametime = 20; //speed 4 0.6
          else gametime = 15; //speed 5 0.5
          
          if (subcount == 0)
          {
            if (humplevel == 4)
            {
              humplevel = 3;
              subcount = 4 + random3(); //4,5,6
            }
            else if(humplevel == 3)
            {
              humplevel = 2;
              subcount = 3 + random3(); //3,4,5
            } 
            else // humplevel == 2
            {
              humplevel = 4;
              subcount = 7 + random3(); //7,8,9
            } 
          }
          
          while((humpstat[nexthump]) && (ncount++<128))
          {
            nexthump = random1();
          } 
        }
        else if (gamelevel == 2)
        {
          if (score < 25) gametime = 25;  //speed 2 0.7 sec
          else if (score < 45) gametime = 20; //speed 3 0.6
          else if (score < 60) gametime = 15; //speed 4 0.5
          else gametime = 10; //speed 5 0.4
        
          if (subcount == 0)
          {
            if (humplevel == 4)
            {
              humplevel = 3;
              subcount = 4 + random3(); //4,5,6
            }
            else if(humplevel == 3)
            {
              humplevel = 2;
              subcount = 3 + random3(); //3,4,5
            } 
            else // humplevel == 2
            {
              humplevel = 4;
              subcount = 5 + random3(); //5,6,7
            } 
          }
        
          while((humpstat[nexthump]) && (ncount++<128))
          {
            nexthump = random2();
          } 
        }
        
        if (ncount<128) //successful random hump selection
        {
          if ((hump == 0) || ((hump == 1)&&(humpstat[1] == humplevel)) || ((hump == 2)&&(humpstat[2] == humplevel)) || ((hump == 3)&&(humpstat[3] == humplevel)) || ((hump == 4)&&(humpstat[4] == humplevel)) || ((hump == 5)&&(humpstat[5] == humplevel)))
          {
            hump = nexthump;
            if (hump == 1)
              humpstat[1] = 1;
            else if (hump == 2)
              humpstat[2] = 1;
            else if (hump == 3)
              humpstat[3] = 1;
            else if (hump == 4)
              humpstat[4] = 1;
            else if (hump == 5)
              humpstat[5] = 1;
            subcount--;
          }
        } 
      } 
    } 
    
    if (utimer) utimer--;
    if (mtimer) mtimer--;
    
    switch (humpstat[1])
    {
    case 0: drawHump(HUMP_1_X, HUMP_LINE, hump_1, 8, 0); break;
    case 1: drawHump(HUMP_1_X, HUMP_LINE, hump_1, 8, 1); break;
    case 2: drawHump(HUMP_1_X, HUMP_LINE, hump_1, 8, 2); break;
    case 3: drawHump(HUMP_1_X, HUMP_LINE, hump_1, 8, 3); break;
    case 4: arduboy.drawBitmap(MOLE_1_X, MOLE_LINE, mole_1, 12, 8); humpstat[1] = 5; break;
    case 5: if ((!molemiss)&&(manpos == 0)){arduboy.drawBitmap(MOLE_1_X, MOLE_LINE, mole_1_hit, 12, 8); MAN_STATE = 1; score++; utimer = UPTIME; humpstat[1] = 7;} break; //hit
    case 6: beep2 = 4000; arduboy.drawBitmap(0, 25, mole_clear, 1, 3); mtimer = MTIME; molemiss = 1; humpstat[1] = 8; break; //miss
    case 7: if (!utimer) {beep2 = 40; MAN_STATE = 0; arduboy.drawBitmap(HUMP_1_X, MOLE_LINE, mole_clear, 1, 8); drawHump(HUMP_1_X, HUMP_LINE, hump_1, 8, 0); humpstat[1] = 0;} break;
    case 8: drawHump(HUMP_1_X, HUMP_LINE, hump_1, 8, 2);  if (!mtimer) {beep1 = 40; mtimer = MTIME; humpstat[1] = 9;} break;
    case 9: drawHump(HUMP_1_X, HUMP_LINE, hump_1, 8, 1); if (!mtimer) {beep1 = 40; mtimer = MTIME; humpstat[1] = 10;} break;
    case 10: drawHump(HUMP_1_X, HUMP_LINE, hump_1, 8, 0); if (!mtimer) {beep1 = 40;  mole++; mtimer = MTIME; humpstat[1] = 11;} break;
    case 11: if (!mtimer) humpstat[1] = 0; if(humplevel==2) hump = 0; break;
    } 
    switch (humpstat[2])
    {
    case 0: drawHump(HUMP_2_X, HUMP_LINE, hump_2, 11, 0); break;
    case 1: drawHump(HUMP_2_X, HUMP_LINE, hump_2, 11, 1); break;
    case 2: drawHump(HUMP_2_X, HUMP_LINE, hump_2, 11, 2); break;
    case 3: drawHump(HUMP_2_X, HUMP_LINE, hump_2, 11, 3); break;
    case 4: arduboy.drawBitmap(MOLE_2_X, MOLE_LINE, mole_1, 12, 8); humpstat[2] = 5; break;
    case 5: if ((!molemiss)&&(manpos == 1)){arduboy.drawBitmap(MOLE_2_X, MOLE_LINE, mole_1_hit, 12, 8); MAN_STATE = 1; score++; utimer = UPTIME; humpstat[2] = 7;} break;  //hit
    case 6: beep2 = 4000; arduboy.drawBitmap(MOLE_2_X, MOLE_LINE, mole_clear, 1, 6); mtimer = MTIME; molemiss = 1; if (!mtimer) { humpstat[2] = 8;} break; //miss
    case 7: if (!utimer) {beep2 = 40; MAN_STATE = 0; arduboy.drawBitmap(MOLE_2_X, MOLE_LINE, mole_clear, 1, 4); drawHump(HUMP_2_X, HUMP_LINE, hump_2, 11, 0); humpstat[2] = 0;} break;
    case 8: drawHump(HUMP_2_X, HUMP_LINE, hump_2, 11, 2); if (!mtimer) {beep1 = 40; mtimer = MTIME; humpstat[2] = 9;} break;
    case 9: drawHump(HUMP_2_X, HUMP_LINE, hump_2, 11, 1); if (!mtimer) {beep1 = 40; mtimer = MTIME; humpstat[2] = 10;} break;
    case 10: drawHump(HUMP_2_X, HUMP_LINE, hump_2, 11, 0); if (!mtimer) {beep1 = 40;mole++; mtimer = MTIME; humpstat[2] = 11;} break;
    case 11: if (!mtimer) humpstat[2] = 0; if(humplevel==2) hump = 0; break;
    } 
    switch (humpstat[3])
    {
    case 0: drawHump(HUMP_3_X, HUMP_LINE, hump_3, 16, 0); break;
    case 1: drawHump(HUMP_3_X, HUMP_LINE, hump_3, 16, 1); break;
    case 2: drawHump(HUMP_3_X, HUMP_LINE, hump_3, 16, 2); break;
    case 3: drawHump(HUMP_3_X, HUMP_LINE, hump_3, 16, 3); break;
    case 4: arduboy.drawBitmap(MOLE_3_X, MOLE_LINE, mole_2, 12, 8); humpstat[3] = 5; break;
    case 5: if ((!molemiss) && (manpos == 0)){arduboy.drawBitmap(MOLE_3_X, MOLE_LINE, mole_2_hit, 12, 8); MAN_STATE = 2; score++; utimer = UPTIME; humpstat[3] = 7;} //hit
      else if ((!mtimer) && (manpos == 2)){arduboy.drawBitmap(MOLE_3_X, MOLE_LINE, mole_2_hit, 12, 8); MAN_STATE = 1; score++; utimer = UPTIME; humpstat[3] = 7;} break;
    case 6: beep2 = 4000; arduboy.drawBitmap(MOLE_3_X, MOLE_LINE, mole_clear, 1, 3); mtimer = MTIME; molemiss = 1; humpstat[3] = 8; break;
    case 7: if (!utimer) {beep2 = 40; MAN_STATE = 0; arduboy.drawBitmap(MOLE_3_X, MOLE_LINE, mole_clear, 1, 3); drawHump(HUMP_3_X, HUMP_LINE, hump_3, 16, 0); humpstat[3] = 0;} break;
    case 8: drawHump(HUMP_3_X, HUMP_LINE, hump_3, 16, 2); if (!mtimer) {beep1 = 40; mtimer = MTIME; humpstat[3] = 9;} break;
    case 9: drawHump(HUMP_3_X, HUMP_LINE, hump_3, 16, 1); if (!mtimer) {beep1 = 40; mtimer = MTIME; humpstat[3] = 10;} break;
    case 10: drawHump(HUMP_3_X, HUMP_LINE, hump_3, 16, 0); if (!mtimer) {beep1 = 40;  mole++; mtimer = MTIME; humpstat[3] = 11;} break;
    case 11: if (!mtimer) humpstat[3] = 0; if(humplevel==2) hump = 0; break;
    } 
    switch (humpstat[4])
    {
    case 0: drawHump(HUMP_4_X, HUMP_LINE, hump_4, 11, 0); break;
    case 1: drawHump(HUMP_4_X, HUMP_LINE, hump_4, 11, 1); break;
    case 2: drawHump(HUMP_4_X, HUMP_LINE, hump_4, 11, 2); break;
    case 3: drawHump(HUMP_4_X, HUMP_LINE, hump_4, 11, 3); break;
    case 4: arduboy.drawBitmap(MOLE_4_X, MOLE_LINE, mole_3, 12, 8); humpstat[4] = 5; break;
    case 5: if ((!molemiss)&&(manpos == 1)){arduboy.drawBitmap(MOLE_4_X, MOLE_LINE, mole_3_hit, 12, 8); MAN_STATE = 2; score++; utimer = UPTIME; humpstat[4] = 7;} break;  //hit
    case 6: beep2 = 4000; arduboy.drawBitmap(MOLE_4_X, MOLE_LINE, mole_clear, 52, 3); mtimer = MTIME; molemiss = 1; humpstat[4] = 8; break;
    case 7: if (!utimer) {beep2 = 40; MAN_STATE = 0; arduboy.drawBitmap(MOLE_4_X, MOLE_LINE, mole_clear, 1, 3); drawHump(HUMP_4_X, HUMP_LINE, hump_4, 11, 0); humpstat[4] = 0;} break;
    case 8: drawHump(HUMP_4_X, HUMP_LINE, hump_4, 11, 2); if (!mtimer) {beep1 = 40; mtimer = MTIME; humpstat[4] = 9;} break;
    case 9: drawHump(HUMP_4_X, HUMP_LINE, hump_4, 11, 1); if (!mtimer) {beep1 = 40; mtimer = MTIME; humpstat[4] = 10;} break;
    case 10: drawHump(HUMP_4_X, HUMP_LINE, hump_4, 11, 0); if (!mtimer) {beep1 = 40; mole++; mtimer = MTIME; humpstat[4] = 11;} break;
    case 11: if (!mtimer) humpstat[4] = 0; if(humplevel==2) hump = 0; break;
    } 
    switch (humpstat[5])
    {
    case 0: drawHump(HUMP_5_X, HUMP_LINE, hump_5, 8, 0); break;
    case 1: drawHump(HUMP_5_X, HUMP_LINE, hump_5, 8, 1); break;
    case 2: drawHump(HUMP_5_X, HUMP_LINE, hump_5, 8, 2); break;
    case 3: drawHump(HUMP_5_X, HUMP_LINE, hump_5, 8, 3); break;
    case 4: arduboy.drawBitmap(MOLE_5_X, MOLE_LINE, mole_3, 12, 8); humpstat[5] = 5; break;
    case 5: if ((!molemiss)&&(manpos == 2)){arduboy.drawBitmap(MOLE_5_X, MOLE_LINE, mole_3_hit, 12, 8); MAN_STATE = 2; score++; utimer = UPTIME; humpstat[5] = 7;} break;  //hit
    case 6: beep2 = 4000; arduboy.drawBitmap(MOLE_5_X, MOLE_LINE, mole_clear, 69, 3); mtimer = MTIME; molemiss = 1; humpstat[5] = 8; break;
    case 7: if (!utimer) {beep2 = 40; MAN_STATE = 0; arduboy.drawBitmap(MOLE_5_X, MOLE_LINE, mole_clear, 1, 3); arduboy.drawBitmap(HUMP_5_X, HUMP_LINE, hump_5, 8, 0); humpstat[5] = 0;} break;
    case 8: drawHump(HUMP_5_X, HUMP_LINE, hump_5, 8, 2); if (!mtimer) {beep1 = 40; mtimer = MTIME; humpstat[5] = 9;} break;
    case 9: drawHump(HUMP_5_X, HUMP_LINE, hump_5, 8, 1); if (!mtimer) {beep1 = 40; mtimer = MTIME; humpstat[5] = 10;} break;
    case 10: drawHump(HUMP_5_X, HUMP_LINE, hump_5, 8, 0); if (!mtimer) {beep1 = 40; mole++; mtimer = MTIME; humpstat[5] = 11;} break;
    case 11: if (!mtimer) humpstat[5] = 0; if(humplevel==2) hump = 0; break;
    }
    
    if (!mtimer) molemiss = 0;
    /*
    if (mole == 1) arduboy.drawBitmap(0, 50, life, 9, 5);
    if (mole == 2) arduboy.drawBitmap(25, 50, life, 9, 5);
    if (mole == 3) {arduboy.drawBitmap(50, 50, life, 9, 5); gamelevel = 0; beep3 = 8000;} //end game */
    arduboy.print(score);
    arduboy.display();
  }
}

void drawHump(int16_t x, int16_t y, const uint8_t * bitmap, uint8_t w, unsigned char frame) {
  arduboy.drawBitmap(x, y, bitmap, w, 8);
  arduboy.fillRect(x, y, w, 9 - frame * 3, BLACK);
}

void drawGround() {
  unsigned int line = MOLE_LINE + 6;
  arduboy.drawLine(0, line, 22, line);
  //GAP 1 = 12
  arduboy.drawLine(34, line, 40, line);
  //GAP 2 = 12
  arduboy.drawLine(52, line, 58, line);
  //GAP 3 = 11
  arduboy.drawLine(69, line, 75, line);
  //GAP 4 = 12
  arduboy.drawLine(87, line, 93, line);
  //GAP 5 = 12
  arduboy.drawLine(105, line, 128, line);
}

void drawMan(int pos, int state) {
  switch (pos) {
    case 0:
      switch (state) {
        case 0:
          arduboy.drawBitmap(MAN_1_X, MAN_Y, man_1u, 84, 20);
          break;
        case 1:
          arduboy.drawBitmap(MAN_1_X, MAN_Y, man_1l, 84, 20);
          break;
        case 2:
          arduboy.drawBitmap(MAN_1_X, MAN_Y, man_1r, 84, 20);
          break;
      }
      break;
    case 1:
      switch (state) {
        case 0:
          arduboy.drawBitmap(MAN_1_X, MAN_Y, man_2u, 84, 20);
          break;
        case 1:
          arduboy.drawBitmap(MAN_1_X, MAN_Y, man_2l, 84, 20);
          break;
        case 2:
        arduboy.drawBitmap(MAN_1_X, MAN_Y, man_2r, 84, 20);
          break;
      }
      break;
    case 2:
    switch (state) {
        case 0:
          arduboy.drawBitmap(MAN_1_X, MAN_Y, man_3u, 84, 20);
          break;
        case 1:
          arduboy.drawBitmap(MAN_1_X, MAN_Y, man_3l, 84, 20);
          break;
        case 2:
          arduboy.drawBitmap(MAN_1_X, MAN_Y, man_3r, 84, 20);
          break;  
      }
      break;
  }
}

boolean leftPressed() { 
  return (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(A_BUTTON));
}

boolean rightPressed() {
  return (arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed(B_BUTTON));
}

unsigned char random1() {
    return pgm_read_byte_near(randomtable1 + prand++);
}

unsigned char random2() {
    return pgm_read_byte_near(randomtable2 + prand++);
}

unsigned char random3() {
    return pgm_read_byte_near(randomtable3 + prand++);
}

byte getOffset(unsigned int s) {
  if (s > 9999) { return 24; }
  if (s > 999) { return 18; }
  if (s > 99) { return 12; }
  if (s > 9) { return 6; }
  return 0;
}



/*
   ()()()()()()()
   ()[][][][][]()
   ()[][][][][]()
   ()[][][][][]()
   ()[][][][][]()
   ()[][][][][]()
   ()[][][][][]()
   ()[][][][][]()
   ()()()()()()()

   [][][]

     []
   [][]

     []
   [][][]

   [][]
   [][]
*/

#define OFF 0x00
#define ON 0xFF

// buffer width and height
#define TET_WIDTH 7
#define TET_HEIGHT 9
// visible buffer width and height
#define TET_VWIDTH 5
#define TET_VHEIGHT 7

// macros to convert linear array into X and Y coords
#define  buf2x( k ) ((k)%(TET_WIDTH))
#define  buf2y( k ) ((k)/(TET_WIDTH))
#define vbuf2x( k ) // TODO
#define vbuf2y( k )

static uint8_t dbuf[TET_WIDTH*TET_HEIGHT] = { OFF };
static uint8_t  buf[TET_WIDTH*TET_HEIGHT] = 
{
	ON,0,0,0,0,0,ON,
	ON,0,0,0,0,0,ON,
	ON,0,0,0,0,0,ON,
	ON,0,0,0,0,0,ON,
	ON,0,0,0,0,0,ON,
	ON,0,0,0,0,0,ON,
	ON,0,0,0,0,0,ON,
	ON,0,0,0,0,0,ON,
	ON,ON,ON,ON,ON,ON,ON
};

void newShape()
{
	switch( random(4) )
	{
		default:
		case 0: // ...
			dbuf[2]=ON;dbuf[3]=ON;dbuf[4]=ON;
			break;
		case 1: // .:
			dbuf[2]=ON;dbuf[3]=ON;dbuf[10]=ON;
			break;
		case 2: // .:.
			dbuf[2]=ON;dbuf[3]=ON;dbuf[4]=ON;dbuf[10]=ON;
			break;
		case 3: // ::
			dbuf[2]=ON;dbuf[3]=ON;dbuf[9]=ON;dbuf[10]=ON;
			break;
	}
}

void dbufFreeze()
{
	// OR dbuf into buf
	for ( uint8_t i = 0; i < TET_WIDTH*TET_HEIGHT; i++ )
	{
		buf[i] |= dbuf[i];
	}
	// clear dbuf, ready for the next shape
	memset( dbuf, OFF, TET_WIDTH*TET_HEIGHT*sizeof(uint8_t) );
}

void dbufAdvance()
{
	for ( uint8_t i = TET_WIDTH*(TET_HEIGHT-1)-1; i >= 0; i-- )
	{
		if ( dbuf[i] )
		{
			if ( !buf[i+TET_WIDTH] ) // the space directly below is clear
			{
				dbuf[i] = OFF;
				dbuf[i+TET_WIDTH] = ON; // move the block down one row
			}
			else
			{
				dbufFreeze();
				newShape();
			}
		}
	}
}

/* Implementation stuff follows */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();
void setup() {matrix.begin(0x70);}
void loop()
{
	matrix.clear();
	for ( uint8_t i = 0; i < TET_WIDTH * TET_HEIGHT; i++ )
	{
		if ( buf[i] || dbuf[i] )
		{
			matrix.drawPixel( buf2x(i), buf2y(i), LED_YELLOW );
		}
	}
	matrix.writeDisplay();
	delay(100);
	dbufAdvance();
}

















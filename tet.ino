
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
*/,

#define OFF 0
#define ON ~0

// buffer width and height
#define WIDTH 7
#define HEIGHT 9
// visible buffer width and height
#define VWIDTH 5
#define VHEIGHT 7

// macros to convert linear array into X and Y coords
#define buf2x( k ) ((k)%(WIDTH))
#define buf2y( k ) ((k)/(WIDTH))

static unsigned byte dbuf[WIDTH*HEIGHT] = { OFF };
static unsigned byte  buf[WIDTH*HEIGHT] = 
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
	// first, clear dbuf
	memset( dbuf, OFF, WIDTH*HEIGHT*sizeof(byte) );
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

bool isGameOver()
{
	// check if the top row, excluding the first and last, has any ON
	for ( unsigned byte i = 1; i < WIDTH - 1; i++ )
		if ( i = ON ) return true;
	return false;
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
	for ( unsigned byte i = 0; i < WIDTH * HEIGHT; i++ )
	{
		if ( buf[i] || dbuf[i] )
		{
			matrix.drawPixel( buf2x(i), buf2y(i), LED_YELLOW );
		}
	}
	matrix.writeDisplay();
	delay(100);
}

















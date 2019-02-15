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

// constant frame delay, in milliseconds
#define TET_DELAY_CONST 200

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
#define vbuf2y( k ) // TODO

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
	for ( int8_t i = TET_WIDTH*(TET_HEIGHT-1)-1; i >= 0; i-- )
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

/* Arduino-style setup() and loop() being used for now */
#if 1
void setup()
{
	matrix.begin(0x70);
	randomSeed(analogRead(0));
	newShape();
}
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
	delay(TET_DELAY_CONST);
	dbufAdvance();
}

/* AVR/timer code that will eventually replace the above setup() and loop() */
#endif
#if 1-1
#include <avr/interrupt.h>

void interruptSetup()
{
	cli(); // stop interrupts

	TCCR1A = 0; // zero TCCR1A register
	TCCR2A = 0; // zero TCCR2B register
	TCNT1 = 0;  // initialize counter to 0
	
	OCR1A = 64000; // set compare match register for 250 Hz increments
	// 64000 = 16000000 / (1 * 250 - 1)

	TCCR1B |= ( 1 << WGM12 ); // turn on CTC
	TCCR1B |= ( 0 << CS12 ) | ( 0 << CS11 ) | ( 1 << CS10 ); // set bits for x1 prescaler
	TIMSK1 |= ( 1 << OCIE1A ); // enable timer compare interrupt

	sei(); // allow interrupts
}

void main()
{
	matrix.begin( 0x70 );
	randomSeed( analogRead( 0 ) );
	newShape();
	interruptSetup();
}

ISR ( TIMER1_COMPA_vect )
{
	dbufAdvance();
	matrix.clear();
	for ( uint8_t i = 0; i < TET_WIDTH * TET_HEIGHT; i++ )
	{
		if ( buf[i] || dbuf[i] )
		{
			matrix.drawPixel( buf2x(i), buf2y(i), LED_YELLOW );
		}
	}
	matrix.writeDisplay();
}
#endif














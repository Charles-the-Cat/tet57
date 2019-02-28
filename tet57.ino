#define OFF 0x00
#define ON 0xFF

#define TET_PIN_LEFT  2
#define TET_PIN_RIGHT 4
#define TET_PIN_ROT   3

// number of overflows to pass before moving on to next tick
// 32 will give you about a half second per tick
#define TET_TIME_OVFS 32

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

uint8_t assertGameOver()
{
	// if anything in the highest visible line of buf is filled, 
	// between the initialized barriers, game over!
	for ( uint8_t i = 8; i < 8 + TET_VWIDTH; i++ )
	{
		if ( buf[i] )
		{
			return true;
		}
	}
	return false;
}

void moveLeft()
{

	for ( int8_t i = TET_WIDTH*(TET_HEIGHT-1)-1; i >= 0; i-- )
	{
		if ( dbuf[i] )
		{
			if ( !buf[i-1] ) // the space to the left is clear
			{
				dbuf[i] = OFF;
				dbuf[i-1] = ON;
			}
		}
	}
}

void moveRight()
{

	for ( int8_t i = TET_WIDTH*(TET_HEIGHT-1)-1; i >= 0; i-- )
	{
		if ( dbuf[i] )
		{
			if ( !buf[i+1] ) // the space to the right is clear
			{
				dbuf[i] = OFF;
				dbuf[i+1] = ON;
			}
		}
	}
}



/* Implementation stuff follows */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <avr/interrupt.h>

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

/* increments with each overflow interrupt.
   at 32, the next game tick begins and its value is reset to 0. */
volatile uint8_t ovfcnt = 0; 
void interruptSetup()
{
	cli(); // stop interrupts

	TCCR2A = 0; // clear timer control register A. We want this to remain zero.
	
	/* clear and set timer control register B to use prescale mode Clk/1024 */
	TCCR2B = ( 1 << CS20 ) | ( 1 << CS21 ) | ( 1 << CS22 );

	TIMSK2 = ( 1 << TOIE2 ); // clear and enable timer overflow interrupt

	TIFR2 = 0; // clear timer overflow flag

	TCNT2 = 0; // clear timer register so it can begin counting from zero

	sei(); // allow interrupts
}

void setup()
{
	matrix.begin( 0x70 );
	pinMode( TET_PIN_LEFT , OUTPUT );
	pinMode( TET_PIN_RIGHT, OUTPUT );
	pinMode( TET_PIN_ROT  , OUTPUT );
	randomSeed( analogRead( 0 ) );
	newShape();
	interruptSetup();
}

ISR ( TIMER2_OVF_vect )
{
	ovfcnt++;
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
	dbufAdvance();
	if ( assertGameOver() )
	{
		memset( dbuf, ON, TET_WIDTH*TET_HEIGHT*sizeof(uint8_t) );
	}

	// wait for the remainder of the tick, then reset ovfcnt and begin next tick
	while ( ovfcnt < TET_TIME_OVFS ) 
	{
		if ( digitalRead( TET_PIN_LEFT ) )
		{
			moveLeft();
		}
		if ( digitalRead( TET_PIN_RIGHT ) )
		{
			moveRight();
		}
	}
	ovfcnt = 0;
}













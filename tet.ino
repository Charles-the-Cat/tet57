
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

static byte dbuf[WIDTH*HEIGHT] = { OFF };
static byte  buf[WIDTH*HEIGHT] = 
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
	for ( byte i = 1; i < WIDTH - 1; i++ )
		if ( i = ON ) return true;
	return false;
}

void setup() {}
void loop()  {}


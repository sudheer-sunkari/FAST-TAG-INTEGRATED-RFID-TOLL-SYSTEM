#define SETBIT(WORD,BIT)(WORD|=1<<BIT)                         // Sets the specified bit of WORD to 1

#define SSETBIT(WORD,BIT)(WORD=1<<BIT)                        // Assigns WORD with a value having only the specified bit set to 1

#define CLRBIT(WORD,BIT)(WORD&=~(1<<BIT))                     // Clears the specified bit of WORD by making it 0

#define SCLRBIT(WORD,BIT)(WORD=1<<BIT)                        // Assigns WORD with a value having only the specified bit position set

#define CPLBIT(WORD,BIT)(WORD^=1<<BIT)                        // Toggles the specified bit of WORD

#define READBIT(WORD,BIT)((WORD>>BIT)&1)                      // Reads the specified bit of WORD and returns either 0 or 1

#define WRITEBIT(WORD,BIT,BITLEVEL)WORD=((WORD&~(1<<BIT))|(BITLEVEL<<BIT)) // Clears the specified bit and writes the given bit level to it

#define WRITENIBBLE(WORD,NIBBLESTARTBIT,NIBBLE)\
WORD=((WORD&~(15<<NIBBLESTARTBIT))|(NIBBLE<<NIBBLESTARTBIT)) // Clears 4 bits starting at NIBBLESTARTBIT and writes the given nibble

#define READNIBBLE(WORD,NIBBLESTARTBIT)\
((WORD>>NIBBLESTARTBIT)&15)                                  // Reads 4 bits starting at NIBBLESTARTBIT and returns the nibble value

#define WRITEBYTE(WORD,BYTESTARTBIT,BYTE)\
 WORD=((WORD&~(255<<BYTESTARTBIT))|(BYTE<<BYTESTARTBIT))      // Clears 8 bits starting at BYTESTARTBIT and writes the given byte

#define READBYTE(WORD,BYTESTARTBIT)\
((WORD>>BYTESTARTBIT)&255)                                   // Reads 8 bits starting at BYTESTARTBIT and returns the byte value
								  
#define READWRITEBIT(DWORD,DBIT,SWORD,SBIT)\
 DWORD=((DWORD&~(1<<DBIT))|(((SWORD>>SBIT)&1)<<DBIT))         // Reads one bit from SWORD and writes that bit into the specified bit position of DWORD

  

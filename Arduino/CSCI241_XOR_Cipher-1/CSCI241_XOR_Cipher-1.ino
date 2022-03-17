// Programmer: Wayne Wall
// Description: Encrypt/Decrypt plain text using a simple XOR cipher key.

#include <Arduino.h>        // Arduino standard library.
#include <CSCI_Console.h>   // Console utility library.

// This program accepts a line of plain text from the console, encrypts each character
// using a multi-byte encryption key applied via a bitwise XOR operation.  After
// displaying the encrypted cipher text, the cipher text is then encrypted again using
// the same key, which should yield the original text back again.

// Bytes to use as the encryption key.

const uint8_t KeyBytes[] = { 0b00101010, 0b00111011, 0b10101110, 0b00111100 };
const size_t NumKeyBytes = sizeof(KeyBytes) / sizeof(KeyBytes[0]);

// *** Called once at program start up ***

void setup()
{
  ConsoleSetup();  
}

// *** Called repeatedly until a "reset" is performed ***

void loop()
{
  DisplayNewline();
  DisplayString("Encryption key bytes:");
  DisplayNewline();

  for ( size_t pos = 0; pos < NumKeyBytes; pos++ )
  {
    DisplayString("Key[");
    DisplayInteger(pos);
    DisplayString("] = ");
    DisplayAsBinary(KeyBytes[pos]);
    DisplayNewline();
  }
  
  DisplayNewline();
  DisplayString("Enter message text to encrypt:");
  DisplayNewline();
  
  // Get a line of input from the keyboard.
  // Does NOT contain any end-of-line designator.

  String plainText = KeyboardGetline();

  DisplayNewline();
  DisplayString("Message text is: ");
  DisplayNewline();
  DisplayCipherText(plainText);

  // Encrypt the plain text using the key.

  String cipherText(plainText);

  EncryptString(cipherText, KeyBytes, NumKeyBytes);

  DisplayNewline();
  DisplayNewline();
  DisplayString("Cipher text is: ");
  DisplayNewline();
  DisplayCipherText(cipherText);

  // Encrypt the cipher text using the same key.

  EncryptString(cipherText, KeyBytes, NumKeyBytes);

  DisplayNewline();
  DisplayNewline();
  DisplayString("Encrypted cipher text is: ");
  DisplayNewline();
  DisplayCipherText(cipherText);  
}

// ---------------------------------------------------------
// Routine to display the passed cipher text, character by character,
// one per line.  If a character is printable, display the ASCII
// character inside single quote marks ('...'), otherwise display '?'.
// Then follow with the binary representation of the character.

void DisplayCipherText(const String& text)
{
  unsigned int length = text.length();

  for ( unsigned int pos = 0; pos < length; pos++ )
  {
    char charCode = text.charAt(pos);

    if ( !isPrintable(charCode) )
    {
      charCode = '?';
    }

    String charMsg("'");
    charMsg += String(charCode) + "' ";

    DisplayString(charMsg);
    DisplayAsBinary(text.charAt(pos));
    DisplayNewline();
  }
}

// ---------------------------------------------------------
// Routine to encrypt the text string passed in using the
// specified 8-bit key byte array.  The key bytes are applied
// in round-robin fashion to each byte of the text string.

void EncryptString(String& text, const uint8_t keyByte[], size_t numKeyBytes)
{
  size_t textLength = text.length();

  // Start at first key byte.

  size_t keyPos = 0;

  // For each text character...

  for ( size_t pos = 0; pos < textLength; pos++ )
  {
    // Use key byte to encrypt the text character.
    
    char cipherCode = text.charAt(pos) ^ keyByte[keyPos++];
    text.setCharAt(pos, cipherCode);

    // If we've used all the key bytes, start over.

    if ( keyPos >= numKeyBytes )
    {
      keyPos = 0;
    }
  }     
}

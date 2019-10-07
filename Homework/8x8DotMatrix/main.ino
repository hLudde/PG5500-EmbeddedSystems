#include "LedControl.h"

LedControl lc = LedControl(12,11,10,1);
byte Palette[] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
byte* LEDDisplayText = (byte *) malloc(16 * sizeof(byte));
int LEDScrollPosition = 0;
int textLength = 0;
unsigned long delayTime = 100;
char* input;
char endMarker = '\n';
const byte maxChars = 32;
char receivedChars[maxChars] = {'H','E','L','L','O', ' ','W','O','R','L','D'};
bool newData = false;

byte A[] = {B00000000,B00111100,B01100110,B01100110,B01111110,B01100110,B01100110,B01100110};
byte B[] = {B01111000,B01001000,B01001000,B01110000,B01001000,B01000100,B01000100,B01111100};
byte C[] = {B00000000,B00011110,B00100000,B01000000,B01000000,B01000000,B00100000,B00011110};
byte D[] = {B00000000,B00111000,B00100100,B00100010,B00100010,B00100100,B00111000,B00000000};
byte E[] = {B00000000,B00111100,B00100000,B00111000,B00100000,B00100000,B00111100,B00000000};
byte F[] = {B00000000,B00111100,B00100000,B00111000,B00100000,B00100000,B00100000,B00000000};
byte G[] = {B00000000,B00111110,B00100000,B00100000,B00101110,B00100010,B00111110,B00000000};
byte H[] = {B00000000,B00100100,B00100100,B00111100,B00100100,B00100100,B00100100,B00000000};
byte I[] = {B00000000,B00111000,B00010000,B00010000,B00010000,B00010000,B00111000,B00000000};
byte J[] = {B00000000,B00011100,B00001000,B00001000,B00001000,B00101000,B00111000,B00000000};
byte K[] = {B00000000,B00100100,B00101000,B00110000,B00101000,B00100100,B00100100,B00000000};
byte L[] = {B00000000,B00100000,B00100000,B00100000,B00100000,B00100000,B00111100,B00000000};
byte M[] = {B00000000,B00000000,B01000100,B10101010,B10010010,B10000010,B10000010,B00000000};
byte N[] = {B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000,B00000000};
byte O[] = {B00000000,B00111100,B01000010,B01000010,B01000010,B01000010,B00111100,B00000000};
byte P[] = {B00000000,B00111000,B00100100,B00100100,B00111000,B00100000,B00100000,B00000000};
byte Q[] = {B00000000,B00111100,B01000010,B01000010,B01000010,B01000110,B00111110,B00000001};
byte R[] = {B00000000,B00111000,B00100100,B00100100,B00111000,B00100100,B00100100,B00000000};
byte S[] = {B00000000,B00111100,B00100000,B00111100,B00000100,B00000100,B00111100,B00000000};
byte T[] = {B00000000,B01111100,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000};
byte U[] = {B00000000,B01000010,B01000010,B01000010,B01000010,B00100100,B00011000,B00000000};
byte V[] = {B00000000,B00100010,B00100010,B00100010,B00010100,B00010100,B00001000,B00000000};
byte W[] = {B00000000,B10000010,B10010010,B01010100,B01010100,B00101000,B00000000,B00000000};
byte X[] = {B00000000,B01000010,B00100100,B00011000,B00011000,B00100100,B01000010,B00000000};
byte Y[] = {B00000000,B01000100,B00101000,B00010000,B00010000,B00010000,B00010000,B00000000};
byte Z[] = {B00000000,B00111100,B00000100,B00001000,B00010000,B00100000,B00111100,B00000000};
byte Space[] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
byte* Alphabet[] = {A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z};

void setup(){
    Serial.begin(9600);
    lc.shutdown(0, false);
    lc.setIntensity(0, 15);
    lc.clearDisplay(0);
    updateLedDisplayText(receivedChars);
}

void serialInput() {
    byte position = 0;
    char recievedChar;
    while (Serial.available() > 0) {
        newData = true;
        recievedChar = Serial.read();
        if (recievedChar != endMarker) {
            if(!filterInput(recievedChar)){
                continue;
            }
            receivedChars[position] = charToUpperCase(recievedChar);
            position++;
            if (position > maxChars) {
                position = maxChars - 1;
            }
        } else {
            receivedChars[position] = '\0';
        }
    }
    if(newData){
        updateLedDisplayText(receivedChars);
    }
}

void updateLedDisplayText(char text[]){
    textLength = getSizeOfString(text);
    free(LEDDisplayText);
    LEDDisplayText = (byte *)malloc((textLength+2)*sizeof(byte));

    memcpy(LEDDisplayText, Space, 8*sizeof(byte));
    for(int i = 1; i <= textLength; i++){
        int letterIndex = (int)text[i-1]-65;
        if(letterIndex<0){
            memcpy(LEDDisplayText+(8*i), Space, 8*sizeof(byte));
        }else{
            memcpy(LEDDisplayText+(8*i), Alphabet[letterIndex], 8*sizeof(byte));
        }
    }
    memcpy(LEDDisplayText+((textLength+1)*8), Space, 8*sizeof(byte));
}

int getSizeOfString(char text[]){
    for(int i = 0; i < maxChars; i++){
        if(text[i]=='\0'){
            return i;
        }
    }
    return maxChars;
}

void scrollTextLCD(){
    if(newData || LEDScrollPosition >= textLength*8 + 8){
        LEDScrollPosition = 0;
    }
    for(int i = 0; i < 8; i++){
        lc.setRow(0, i, LEDDisplayText[LEDScrollPosition+8-i]);
    }
    LEDScrollPosition++;
}

bool filterInput(char character){
    if((character >= 65 && character <= 90) || (character >= 97 && character <= 122) || character == 32){
        return true;
    }
    return false;
}

char charToUpperCase(char character){
    if(character == 32){
        return character;
    }
    return character & ~32;
}

void loop(){
    serialInput();
    scrollTextLCD();
    delay(delayTime);
    if(newData){
        newData = false;
    }
}
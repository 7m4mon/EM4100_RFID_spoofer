/***********************************************************
 * EM4100_RIFD_spoofer
 * Quote source: https://github.com/Einstein2150/EM4100_RFID_spoofer
 * modified by: 7M4MON, 26 Feb 2020
 * 
 * I added a function that convert from numeric Version and ID to send bit array with parity.
 ***********************************************************/

//Pin to connect to the circuit
//Setting the pin LOW will tune the coil
//meaning it will respond as a high signal to the reader
//Setting the pin to HIGH will detune coil
//meaning the reader will see it as a low signal


#define PIN_COIL 9
#define DAT_SIZE (8+32)             // (ver 8bit + id 32bit)
#define NIB_SIZE (DAT_SIZE/4)       // NIBBLE
#define BIT_SIZE (9+(NIB_SIZE+1)*5) // HEADER_BITS 1x9 + (NIBBLE+PARITY) + COLUM_PARITY + LAST_ZEROFunction addition

uint8_t   ver_num = 0x01;
uint32_t  id_num  = 0x23456789;
uint8_t   send_bit[BIT_SIZE];

void set_send_bit(uint64_t raw_num){
    uint8_t nib_num, colum_parity;
    uint8_t nib_data[NIB_SIZE+1];        //including column parity
    int8_t i,j,k,b,p;

    /***  Set nibble data and column parity ***/
    colum_parity = 0;
    for (i = NIB_SIZE; i > 0; i--){
        nib_num = raw_num & 0x0f;
        nib_data[i-1] = nib_num;
        colum_parity ^= nib_num;
        raw_num >>= 4;
    }
    nib_data[NIB_SIZE] = colum_parity;    // last row is column parity

    /*** Convert nibble data to send_bit data. ***/
    // Set Header bits 9's 1
    i = 0;
    while(i<9){
        send_bit[i] = 1;
        i++;
    }
    for(k=0; k<NIB_SIZE+1; k++){
        p = 0;                          // clear parity bit
        nib_num = nib_data[k];
        for (j=0; j<4; j++){
            b =(nib_num & 0x8) ? 1:0;   // MSB first
            send_bit[i]= b;
            p ^= b;
            nib_num <<= 1;
            i++;
        }
        send_bit[i] = p;                // 5th bit is parity
        i++;
    }
    send_bit[BIT_SIZE-1] = 0;    // LAST BIT is zero

    // for debug
    for(i=0;i<BIT_SIZE;i++){
       Serial.print(send_bit[i], HEX);
       Serial.print(",");
    }
}

void setup(){
    uint64_t raw_num = 0;
    
    Serial.begin(9600);

    // Marge version and id
    raw_num = ((uint64_t)ver_num << 32) + (uint64_t)id_num;
    set_send_bit(raw_num);
    
    
    //Set pin as output
    pinMode(PIN_COIL, OUTPUT);
    //Start it as low
    digitalWrite(PIN_COIL, LOW);
}

//Does manchester encoding for signal and sets pins.
//Needs clock and signal to do encoding
void set_pin_manchester(int clock_half, int signal)
{
  //manchester encoding is xoring the clock with the signal
  int man_encoded = clock_half ^ signal;
  
  //if it's 1, set the pin LOW (this will tune the antenna and the reader sees this as a high signal)
  //if it's 0, set the pin to HIGH  (this will detune the antenna and the reader sees this as a low signal)
  if(man_encoded == 1){
     digitalWrite(PIN_COIL, LOW);
  }
  else{
    digitalWrite(PIN_COIL, HIGH);
  }
}

void loop(){
    int8_t i;
    for(i = 0; i < BIT_SIZE; i++){
        set_pin_manchester(1, send_bit[i]);
        delayMicroseconds(256);
        
        set_pin_manchester(0, send_bit[i]);
        delayMicroseconds(256); 
    }
}

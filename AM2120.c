static inline int32_t asm_ccount(void) {
    int32_t r;
    asm volatile ("rsr %0, ccount" : "=r"(r));
    return r;
}

void setupAM2120() {
  pinMode(D7, OUTPUT);
  digitalWrite(D7, HIGH);
  pinMode(D8, OUTPUT);
  digitalWrite(D8, LOW);
  
}
int readAM2120(float* fhumidity, float* ftemp) {
  // returns 0 on success, nonzero on error.
  pinMode(D7, OUTPUT);
  digitalWrite(D7, LOW);
  
  delay(5);
  uint8_t data[5];
  
  for (int i=0; i<5; i++)
    data[i]=0;

  uint32_t start_time = asm_ccount();

  pinMode(D7, INPUT_PULLUP);
  delayMicroseconds(35+80+80);   // skip the first pulse

  for (int i=0; i<5; i++)
    for (int j=0; j<8; j++) {
      // Wait till data line goes high
      while (!digitalRead(D7))
        if (asm_ccount()-start_time>1000000) return 1;  // timeout waiting for start of data bit.
      
      delayMicroseconds(40);
      data[i] = (data[i]<<1) + (digitalRead(D7)?1:0);

      // Wait for data line to go low
      while (digitalRead(D7))
        if (asm_ccount()-start_time>1000000) return 2;  // Timeout waiting for end of data bit

  }

  if ((uint8_t)(data[0]+data[1]+data[2]+data[3]) != data[4]) return 3;   // checksum failure.

  uint16_t humidity = ((int)data[0] << 8) + ((int)data[1]);
  uint16_t temp = ((int)data[2] << 8) + ((int)data[3]);
  
  *fhumidity = (float)(humidity)/10.0f;
  *ftemp = (float)(temp&0x7FFF)/10.0f;
  
  if (temp & 0x8000) *ftemp = -*ftemp; 
  return 0;
  
}

void setup() {
    Serial.begin(115200);
}

void loop() {
    runTest();
    delay(5000);
}

int AddrChecker(){
  int test = 0;
  int test2 = 0;
  __asm__(
    "mov %0, $a0\n" // need to use a0
    "addi $a0, $a0, 0" 
    : "=r" (test) 
    : "r" (test2)
  );
  return test;
  //printf("\nADDRESS: 0x%x \n", AddrChecker());
}

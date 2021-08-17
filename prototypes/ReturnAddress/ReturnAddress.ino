void setup() {
  Serial.begin(115200); 
}
void loop() {
  // put your main code here, to run repeatedly:
  someFunction();
  someFunction();
  someFunction();
  someFunction();
  delay(10000);
}
// -2146626680 -> -0x7FF2EC78
// read a0 see call0 
void someFunction(){
  int test = 0;
  int test2 = 0;
  __asm__(
    "mov %0, $a0\n" // need to use a0
    "addi $a0, $a0, 3" 
    : "=r" (test) 
    : "r" (test2)
  );
  printf("0x%x \n",test);
}

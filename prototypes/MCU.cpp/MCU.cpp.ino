#include <setjmp.h>
jmp_buf buf; 
void setup()
{
  Serial.begin(115200);
  Serial.print("init");
  funcA();
}
void loop() {}

// This outputs "initAB"
// Microcontroller Context-Switching Prototype
// remove serial, thats because I was working in the Arduino IDE
void funcA(){
  if (setjmp(buf))
  {
    Serial.print("B"); // Print "B"
  }
  else
  {
    funcB();
  }
}
void funcB(){
  Serial.print("A");
  longjmp(buf,1); 
  Serial.print("C");
}

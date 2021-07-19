int i=0;
int A = 0;
void setup(){
  Serial.begin(115200);
  delay(2000);
  A = 4; 
  char task1Param[12] = "task1Param"; 
  char task2Param[12] = "task2Param"; 
  xTaskCreate( 
    task1,              // Function name
    "task1",            // Task Name
    10000,              // Stack Size
    (void*)task1Param,  // Task Parameter 
    1,                  // Priority 
    NULL                // Task Handle 
   );
   xTaskCreate(
    task2, 
    "task2", 
    10000, 
    (void*)task2Param,
    1,
    NULL
   );
}
void loop(){}

void task1(void * parameter)
{
  int i2 = 0;
  Serial.println((char *)parameter);
  delay(250); // delay before loop
  while(A != 5){
    mutexTSLAtomic();
    crital(&i2, &i, 1);
    unlocker();
  }
  vTaskDelete(NULL);
}
void task2(void * parameter)
{
  int i2 = 0;
  Serial.println((char *)parameter);
  delay(250); // delay before loop
  while(A != 5){
    mutexTSLAtomic();
    crital(&i2, &i, 2);
    unlocker();
  }
  vTaskDelete(NULL);
}

void crital(int *localCount, int *GlobalCount, int Task){
  if ((*GlobalCount)>=50)
  {
    A = 5;
  }
  else if(A!=5){
    (*localCount)++;
    (*GlobalCount)++;
    Serial.print(*GlobalCount);
    Serial.print("\t Task");
    Serial.print(Task);
    Serial.print(": ");
    Serial.print(*localCount);
    Serial.println();
  }
}

volatile int lock = 0;
void mutexTSLAtomic() {
    while (__atomic_test_and_set(&lock, 0) == 1){}  // & or *? order? 1 or 0? 
//    i++; // moved to critical
}
void unlocker(){
  lock = 0;
}

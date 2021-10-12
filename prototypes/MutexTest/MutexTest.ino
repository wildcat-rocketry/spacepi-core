int mainCount = 0;        // Global Count
int Flag = 0;             // Flag variable
void setup() {            // Setup (run at startup)
    Serial.begin(115200); // Start serial
    delay(2000);          // Wait a bit for ESP32
    Flag = 4;             // Set flag to 4

    /*  Task creation section
     *    Creates task parameters
     *    Creates tasks
     */
    char task1Param[12] = "task1Param"; // Set task 1 parameter
    char task2Param[12] = "task2Param"; // Set task 2 parameter
    xTaskCreate(                        // Create task 1
        task1,                          // Function name to be called
        "task1",                        // Task Name
        10000,                          // Stack Size
        (void *) task1Param,            // Task Parameter
        1,                              // Priority
        NULL                            // Task Handle
    );
    xTaskCreate(             // Create task 2
        task2,               // Function name to be called
        "task2",             // Task Name
        10000,               // Stack Size
        (void *) task2Param, // Task Parameter
        1,                   // Priority
        NULL                 // Task Handle
    );
    // Watchdogs are disabled, as they time out in the while loop
    disableCore0WDT(); //   Disable watchdog timer
    disableCore1WDT(); //   Disable watchdog timer
}

/*    Task functions (task1 and task2):
 *    Keeps track of task's count.
 *    When the stop flag is not set, calls mutex code, executes critical, the unlocks
 *    Delete themselves when finished.
 *
 *    task2 seems to call critical far less than task1
 */
void task1(void *parameter) {              //  Function of task 1
    int taskCount = 0;                     //  Local Count var
    Serial.println((char *) parameter);    //  Print when task initilizes
    delay(250);                            //  delay before loop
    while (Flag != 5) {                    //  While flag is not set, continue
        mutexTSLAtomic();                  //  Mutex locker
        crital(&taskCount, &mainCount, 1); //  Execute critical code; pass local & glabal counts, task ID
        unlocker();                        //  Unlock mutex after critical section is over
    }
    vTaskDelete(NULL); //  Task deletes itself when the count ends
}
void task2(void *parameter) {              //  Function of task 2
    int taskCount = 0;                     //  Local Count var
    Serial.println((char *) parameter);    //  Print when task initilizes
    delay(250);                            //  delay before loop
    while (Flag != 5) {                    //  While flag is not set, continue
        mutexTSLAtomic();                  //  Mutex locker
        crital(&taskCount, &mainCount, 2); //  Execute critical code; pass local & glabal counts, task ID
        unlocker();                        //  Unlock mutex after critical section is over
    }
    vTaskDelete(NULL); //  Task deletes itself when the count ends
}

/*    Critical: Takes task ID, local and global counts.
 *    This is to be run while mutex is active
 *    Checks if stopping condition is met, if so, end the count.
 *    If it isn't, it increments the local and global counts, then prints information.
 */
void crital(int *localCount, int *globalCount, int Task) {
    if ((*globalCount) >= 5000) { //  When GlobalCount exceeds 50
        Flag = 5;                 //   Set the flag to stop the count
    } else if (Flag != 5) {       //  If the flag is not set
        (*localCount)++;          //    Increment local count
        (*globalCount)++;         //    Increment glabal count

        Serial.print(*globalCount); //    Print off the information
        Serial.print("\t Task");    //    (globalCount)   (Task ID): (localCount)
        Serial.print(Task);         //    Prints off the count for the system, along
        Serial.print(": ");         //    with the count for the task the
        Serial.print(*localCount);  //    request came from.
        Serial.println();           //    new line
    }
}

/*  MUTEX SECTION:
 *    lock:           Glabal variable that keeps track of status of mutex
 *    mutexTSLAtomic: Locks mutex while in use, called before critical code is executed.
 *      while loop: executes no code.
 *      atomic TSL: Takes lock variable and an "memOrder" int. Order doesn't seem to matter
 *    unlocker: unlocks the lock, to be called after critical code is execured
 */
volatile int lock = 0; // Lock variable for mutex
void mutexTSLAtomic() {
    while (__atomic_test_and_set(&lock, 0) == 1) { // Atomic TSL
    }
    // Critical code is executed by task functions
}
void unlocker() {
    lock = 0; // Unlock the lock
}
void loop() {
} // Loop does nothing, needs to exist.

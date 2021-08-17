Context contexts[3];
bool testValid;
int lastTestPoint;

void testPoint(int n) {
    Serial.print("Test Point #");
    Serial.println(n);
    if (n != lastTestPoint + 1) {
        testValid = false;
    }
    lastTestPoint = n;
}

void functionA(void *dummy) {
    testPoint(1);
    Context::initAndSwitch(contexts[0], contexts[1], functionB, nullptr);
    // Store functionA in contexts[0], jump to functionB in contexts[1]

    testPoint(4);
    Context::contextSwitch(contexts[0], contexts[2]);
    // Store functionA in contexts[0], jump back to contexts[2] (in functionC)

    testPoint(7);
    // Return out of context switching test
}

void functionB(void *dummy) {
    testPoint(2);
    Context::initAndSwitch(contexts[1], contexts[2], functionC, nullptr);
    // Store functionB in contexts[1], jump to functionC in contexts[2]

    testPoint(6);
    Context::contextSwitch(contexts[1], contexts[0]);
    // Store functionB in contexts[1], jump back to contexts[0] (in functionA)
}

void functionC(void *dummy) {
    testPoint(3);
    Context::contextSwitch(contexts[2], contexts[0]);
    // Store functionC in contexts[2], jump back to contexts[0] (in functionA)

    testPoint(5);
    Context::contextSwitch(contexts[2], contexts[1]);
    // Store functionC in contexts[2], jump back to contexts[1] (in functionB)
}

void myMemset(char *buf, char c, int n) {
  // Using actual memset() makes the IDE complain
  for (int i = 0; i < n; ++i) {
    buf[i] = c;
  }
}

void runTest() {
    myMemset((char *) contexts, 0, sizeof(contexts));
    testValid = true;
    lastTestPoint = 0;
    Serial.println("Beginning test...");
    functionA(nullptr);
    if (testValid) {
        Serial.println("Test complete.");
    } else {
        Serial.println("Test failed!");
    }
}

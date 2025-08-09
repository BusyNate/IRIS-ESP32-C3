#include <Arduino.h>
#include <unity.h>

void test_smoke(void) {
    TEST_ASSERT_TRUE(true);
}

void setup() {
    delay(100); // give serial a moment (safe even without monitor)
    UNITY_BEGIN();
    RUN_TEST(test_smoke);
    UNITY_END();
}

void loop() {
    // not used in tests
}
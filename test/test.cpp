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
void time_test(){
    for (int i = 0; i < 20; i++){
        unsigned long timestamp = millis();
        std::cout << "Timestamp" << timestamp << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

}
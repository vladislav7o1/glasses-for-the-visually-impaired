#include <Arduino.h>

// ===============================
// PINOUT
// ===============================

// LEFT HC-SR04
constexpr uint8_t LEFT_TRIG = 4;
constexpr uint8_t LEFT_ECHO = 5;

// RIGHT HC-SR04
constexpr uint8_t RIGHT_TRIG = 6;
constexpr uint8_t RIGHT_ECHO = 7;

// VIBRATION MOTORS
constexpr uint8_t LEFT_MOTOR = 8;
constexpr uint8_t RIGHT_MOTOR = 10;


// Максимальное расстояние,
// после которого вибрации нет
constexpr float MAX_WARNING_DISTANCE = 200.0;


// ====================================
// Чтение HC-SR04
// ====================================

float readDistanceCM(uint8_t trigPin, uint8_t echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(3);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long duration =
        pulseIn(echoPin, HIGH, 25000UL);

    if (duration == 0)
    {
        return 999.0;
    }

    float distance =
        duration * 0.0343 / 2.0;

    return distance;
}


// ====================================
// Сила вибрации
// 0   = выключено
// 255 = максимум
// ====================================

uint8_t getMotorStrength(float distance)
{
    if (distance > MAX_WARNING_DISTANCE)
    {
        return 0;
    }

    if (distance > 120)
    {
        return 70;
    }

    if (distance > 60)
    {
        return 130;
    }

    if (distance > 30)
    {
        return 190;
    }

    return 255;
}


// ====================================
// SETUP
// ====================================

void setup()
{
    Serial.begin(115200);

    pinMode(LEFT_TRIG, OUTPUT);
    pinMode(LEFT_ECHO, INPUT);

    pinMode(RIGHT_TRIG, OUTPUT);
    pinMode(RIGHT_ECHO, INPUT);

    pinMode(LEFT_MOTOR, OUTPUT);
    pinMode(RIGHT_MOTOR, OUTPUT);

    digitalWrite(LEFT_TRIG, LOW);
    digitalWrite(RIGHT_TRIG, LOW);

    analogWrite(LEFT_MOTOR, 0);
    analogWrite(RIGHT_MOTOR, 0);

    Serial.println();
    Serial.println("Smart Vision Assistant");
    Serial.println("System started");
}


// ====================================
// MAIN LOOP
// ====================================

void loop()
{
    float leftDistance =
        readDistanceCM(LEFT_TRIG, LEFT_ECHO);

    uint8_t leftPower =
        getMotorStrength(leftDistance);

    analogWrite(LEFT_MOTOR, leftPower);

    delay(60);

    float rightDistance =
        readDistanceCM(RIGHT_TRIG, RIGHT_ECHO);

    uint8_t rightPower =
        getMotorStrength(rightDistance);

    analogWrite(RIGHT_MOTOR, rightPower);

    delay(60);

    Serial.print("LEFT: ");

    if (leftDistance > 400)
    {
        Serial.print("OUT");
    }
    else
    {
        Serial.print(leftDistance, 1);
        Serial.print(" cm");
    }

    Serial.print("   Motor: ");
    Serial.print(leftPower);

    Serial.print(" | RIGHT: ");

    if (rightDistance > 400)
    {
        Serial.print("OUT");
    }
    else
    {
        Serial.print(rightDistance, 1);
        Serial.print(" cm");
    }

    Serial.print("   Motor: ");
    Serial.println(rightPower);
}

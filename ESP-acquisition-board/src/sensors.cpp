#include <sensors.hpp>

Adafruit_BMP280 bmp_sensors[BMP_SENS_NUM];
static sensors_data_t sensors_data;
static bool acquire_data_flag = false;
static bool critical_data_flag = false;
static bool can_transmit_data = false;

void setup_sensors()
{
    analogReadResolution(ANALOG_RES);
    pinMode(RAIN_SENS_PIN, INPUT);
    pinMode(LIGHT_SENS_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    for (int i = 0; i < BMP_SENS_NUM; i++) {
        if (!bmp_sensors[i].begin(i % 2 ? BMP_ADDR_2 : BMP_ADDR_1)) {
            Serial.println(bmp_sensors[i].sensorID());
        }
    }

    alloc_sensors_data();
}

uint16_t retrieve_dist()
{
    float duration, distance;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration * SOUND_SPEED) / 2;

    return (uint16_t)floor(distance);
}

uint16_t retrieve_rain()
{
    return analogRead(RAIN_SENS_PIN);
}

uint16_t retrieve_light()
{
    return analogRead(LIGHT_SENS_PIN);
}

void alloc_sensors_data()
{
    sensors_data.bmp_num = BMP_SENS_NUM;
    sensors_data.bmp_data = (bmp_sensor_data_t *)malloc(sensors_data.bmp_num * sizeof(bmp_sensor_data_t));
}

int populate_sensors_data()
{
    if (sensors_data.bmp_num && !sensors_data.bmp_data) {
        return -1;
    }

    sensors_data.external.dist = retrieve_dist();
    sensors_data.external.rain = retrieve_rain();
    sensors_data.external.light = retrieve_light();

    for (int i = 0; i < BMP_SENS_NUM; i++) {
        sensors_data.bmp_data[i].sensor_id = i;
        sensors_data.bmp_data[i].temp = bmp_sensors[i].readTemperature();
        sensors_data.bmp_data[i].pres = bmp_sensors[i].readPressure();

        if (sensors_data.bmp_data[i].temp > 30
            || sensors_data.bmp_data[i].pres > 200000) {
            critical_data_flag = true;
        }
    }

    return 0;
}

sensors_data_t *get_current_sensors_data()
{
    sensors_data_t *sensors_data_copy = (sensors_data_t *)malloc(sizeof(*sensors_data_copy));
    memcpy(sensors_data_copy, &sensors_data, sizeof(sensors_data));

    return sensors_data_copy;
}

void set_acquire_data_flag()
{
    acquire_data_flag = true;
}

bool get_acquire_data_flag()
{
    return acquire_data_flag;
}

void reset_critical_data_flag()
{
    critical_data_flag = false;
}

bool get_critical_data_flag()
{
    return critical_data_flag;
}

void set_can_transmit_data()
{
    can_transmit_data = true;
}

void reset_can_transmit_data()
{
    can_transmit_data = false;
}

bool get_can_transmit_data()
{
    return can_transmit_data;
}
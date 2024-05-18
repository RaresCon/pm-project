#include <sensors.hpp>

Adafruit_BMP280 bmp_sensors[BMP_SENS_NUM];

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

int populate_sensors_data(sensors_data_t *data)
{
    if (!data || (data->bmp_num && !data->bmp_data)) {
        return -1;
    }

    data->external.dist = retrieve_dist();
    data->external.rain = retrieve_rain();
    data->external.light = retrieve_light();

    for (int i = 0; i < BMP_SENS_NUM; i++) {
        data->bmp_data[i].sensor_id = i;
        data->bmp_data[i].temp = bmp_sensors[i].readTemperature();
        data->bmp_data[i].pres = bmp_sensors[i].readPressure();
    }

    return 0;
}

sensors_data_t *alloc_sensors_data()
{
    sensors_data_t *data = (sensors_data_t *)malloc(sizeof(*data));
    data->bmp_num = BMP_SENS_NUM;
    data->bmp_data = (bmp_sensor_data_t *)malloc(data->bmp_num * sizeof(bmp_sensor_data_t));

    return data;
}

void free_sensors_data(sensors_data_t **data)
{
    if ((*data)->bmp_num) {
        free((*data)->bmp_data);
    }

    free(*data);
}
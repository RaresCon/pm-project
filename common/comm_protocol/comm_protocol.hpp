#include <stdint.h>

typedef enum R_SRDS {
    SRDS_OK,
    SRDS_BUFFER_TOO_SMALL,
    SRDS_BAD_ARGS
} R_SRDS;

typedef enum CONN_TYPE {
    FETCH = 1,
    GOTCHA,
    CRITICAL,
} CONN_TYPE;

typedef struct __attribute__((packed)) tr_data {
    uint8_t type;
    uint32_t data_len;
    uint8_t *data;
} tr_data_t;

typedef struct __attribute__((packed)) bmp_sensor_data {
    uint8_t sensor_id;
    float temp;
    float pres;
} bmp_sensor_data_t;

typedef struct __attribute__((packed)) external_sensors_data {
    uint16_t rain;
    uint16_t light;
    uint16_t dist;
} external_sensors_data_t;

typedef struct __attribute__((packed)) sensors_data {
    external_sensors_data_t external;
    uint8_t bmp_num;
    bmp_sensor_data_t *bmp_data;
} sensors_data_t;

R_SRDS serialize_sensor_info(sensors_data_t *s_data, uint8_t *dest, uint32_t *dest_len);
R_SRDS deserialize_sensor_info(uint8_t *src, sensors_data_t *s_data);
R_SRDS serialize_tr_data(tr_data_t *tr_data, uint8_t *dest, uint32_t *dest_len);
R_SRDS deserialize_tr_data(uint8_t *src, tr_data_t *tr_data);

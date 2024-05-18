#include "comm_protocol.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int get_tr_data_len(tr_data_t *tr_data)
{
    if (!tr_data) {
        return -1;
    }

    return sizeof(tr_data->type) +
           sizeof(tr_data->data_len) +
           tr_data->data_len;
}

static int get_sensors_data_len(sensors_data_t *s_data)
{
    if (!s_data) {
        return -1;
    }

    return sizeof(s_data->external) +
           sizeof(s_data->bmp_num) +
           sizeof(bmp_sensor_data_t) * s_data->bmp_num;
}

R_SRDS serialize_sensor_info(sensors_data_t *s_data, uint8_t *dest, uint32_t *dest_len)
{
    int s_data_len = get_sensors_data_len(s_data), offset = 0;

    if (s_data_len <= 0 || !s_data->bmp_data || !dest_len) {
        return SRDS_BAD_ARGS;
    } else if (!dest) {
        *dest_len = s_data_len;
        return SRDS_OK;
    } else if (s_data_len > *dest_len) {
        return SRDS_BUFFER_TOO_SMALL;
    }

    memcpy(dest, &s_data->external, sizeof(s_data->external));
    offset += sizeof(s_data->external);
    memcpy(dest + offset, &s_data->bmp_num, sizeof(s_data->bmp_num));
    offset += sizeof(s_data->bmp_num);

    if (s_data->bmp_num) {
        memcpy(dest + offset, s_data->bmp_data, s_data->bmp_num * sizeof(bmp_sensor_data_t));
    }
    return SRDS_OK;
}

R_SRDS deserialize_sensor_info(uint8_t *src, sensors_data_t *s_data)
{
    int offset = 0;

    if (!src || !s_data) {
        return SRDS_BAD_ARGS;
    }

    memcpy((uint8_t *)s_data, src, sizeof(s_data->external));
    offset += sizeof(s_data->external);
    memcpy((uint8_t *)s_data + offset, src + offset, sizeof(s_data->bmp_num));
    offset += sizeof(s_data->bmp_num);

    s_data->bmp_data = NULL;
    if (s_data->bmp_num) {
        s_data->bmp_data = (bmp_sensor_data *)malloc(s_data->bmp_num * sizeof(bmp_sensor_data_t));
        memcpy(s_data->bmp_data, src + offset, s_data->bmp_num * sizeof(bmp_sensor_data_t));
    }
    return SRDS_OK;
}

R_SRDS serialize_tr_data(tr_data_t *tr_data, uint8_t *dest, uint32_t *dest_len)
{
    int tr_data_len = get_tr_data_len(tr_data), offset = 0;

    if (tr_data_len <= 0 || !dest_len) {
        return SRDS_BAD_ARGS;
    } else if (!dest) {
        *dest_len = tr_data_len;
        return SRDS_OK;
    } else if (tr_data_len > *dest_len) {
        return SRDS_BUFFER_TOO_SMALL;
    }

    memcpy(dest, &tr_data->type, sizeof(tr_data->type));
    offset += sizeof(tr_data->type);
    memcpy(dest + offset, &tr_data->data_len, sizeof(tr_data->data_len));
    offset += sizeof(tr_data->data_len);

    if (tr_data->data_len) {
        memcpy(dest + offset, tr_data->data, tr_data->data_len);
    }
    return SRDS_OK;
}

R_SRDS deserialize_tr_data(uint8_t *src, tr_data_t *tr_data)
{
    int offset = 0;

    if (!src || !tr_data) {
        return SRDS_BAD_ARGS;
    }

    memcpy((uint8_t *)tr_data, src, sizeof(tr_data->type));
    offset += sizeof(tr_data->type);
    memcpy((uint8_t *)tr_data + offset , src + offset, sizeof(tr_data->data_len));
    offset += sizeof(tr_data->data_len);

    tr_data->data = NULL;
    if (tr_data->data_len) {
        tr_data->data = (uint8_t *)malloc(tr_data->data_len);
        memcpy(tr_data->data, src + offset, tr_data->data_len);
    }
    return SRDS_OK;
}
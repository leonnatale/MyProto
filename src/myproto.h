#pragma once

#include <stddef.h>
#include <stdint.h>

#define STR(x) #x
#define STR2(x) STR(x)

#define MP_VERSION STR2(MP_V_MAJOR) "." STR2(MP_V_MINOR) "." STR2(MP_V_PATCH)

#define MP_MET_CLOSE  0
#define MP_MET_FETCH  1
#define MP_MET_DEFINE 2

#define MP_CMD_ADD_FIELD 0
#define MP_CMD_SET_FIELD 1


typedef struct {
    uint8_t major, minor, patch;
} myproto_version;

typedef struct {
    char key[257];
    char* value;
} myproto_entry;

typedef struct {
    uint8_t method;
    myproto_version version;
    myproto_entry fields[256];
} myproto_data;

typedef struct {
    uint8_t* data_buffer;
    size_t buffer_size, data_length;
} myproto_client;

int myproto_parse_bytes(myproto_data* data, uint8_t* bytes, size_t size);

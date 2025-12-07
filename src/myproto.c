#include "myproto.h"

int myproto_parse_bytes(myproto_data* data, uint8_t* bytes, size_t size) {
    if (size < 4) return -1;
    data->method = bytes[3];
    data->version = (myproto_version) {
        .major = bytes[0],
        .minor = bytes[1],
        .patch = bytes[2]
    };
    return 0;
}

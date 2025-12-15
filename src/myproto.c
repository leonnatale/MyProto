#include "myproto.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int myproto_parse_bytes(myproto_data* data, uint8_t* bytes, size_t size) {
    if (size < 4) return -1;
    data->method = bytes[3];
    data->version = (myproto_version) {
        .major = bytes[0],
        .minor = bytes[1],
        .patch = bytes[2]
    };
    size_t index = 4;
    size_t field_index = 0;
    if (index > size) return 0;
    while (index < size) {
        uint8_t current_byte = bytes[index];
        if (current_byte == MP_CMD_ADD_FIELD) {
            char field_name[257] = "\0";
            strcpy(field_name, (char*)&bytes[++index]);
            printf("{%s}\n", field_name);
            index += strlen(field_name) + 1;

            if (strlen(field_name) > 0) {
                strcpy(data->fields[field_index].key, field_name);
                break;
            }
        }

        // if (current_byte == MP_CMD_SET_FIELD) {
        //     if (data->fields[field_index].key == NULL) return -1;
        //     char field_value[256 * 1024] = "\0";

        //     while (bytes[index] != 0) {
        //         strcat(field_value, (char*)&bytes[index]);
        //         index++;
        //     }

        //     if (strlen(field_value) > 0) data->fields[field_index].value = field_value;
        // }
    }
    return 0;
}

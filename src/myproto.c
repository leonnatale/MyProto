#include "myproto.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int myproto_parse_bytes(myproto_data* data, uint8_t* bytes, size_t size) {
    if (size < 4) return -1;
    data->method = bytes[3];
    data->used_fields = 0;
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
            index += strlen(field_name) + 1;

            if (data->used_fields > 0)
                for (size_t i = 0; i < data->used_fields; i++)
                    if (!strcmp(data->fields[i].key, field_name)) return -3;

            if (strlen(field_name) > 0) {
                strcpy(data->fields[field_index].key, field_name);
                data->used_fields++;
            }
        } else if (current_byte == MP_CMD_SET_FIELD) {
            if (strlen(data->fields[field_index].key) == 0) return -2;
            size_t field_size = 0;
            size_t field_temp_index = index;

            while (bytes[field_temp_index] != 0) {
                field_size++;
                field_temp_index++;
            }

            char* field_value = (char*)malloc(sizeof(char) * field_size);

            strcpy(field_value, (char*)&bytes[++index]);
            index += strlen(field_value) + 1;

            if (strlen(field_value) > 0) {
                if (data->fields[field_index].value == NULL)
                    free(data->fields[field_index].value);
                data->fields[field_index].value = (char*)malloc(sizeof(char) * field_size);
                strcpy(data->fields[field_index].value, field_value);
                free(field_value);
                field_index++;
            }
        } else {
            index++;
        }
    }
    return 0;
}

void myproto_free_data(myproto_data* data) {
    if (data->used_fields == 0) return;
   for (size_t i = 0; i < data->used_fields; i++) {
       if (data->fields[i].value == NULL) continue;
      free(data->fields[i].value);
   }
   data->used_fields = 0;
}

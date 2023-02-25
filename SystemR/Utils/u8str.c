#include "u8str.h"

#define CHAR_IS_NUM(c) (c >= '0' && c <= '9')

U8Size u8_string_size(U8String text) {
    if (text == NULL) return 0;
    U8Size len = 0;
    while (text[len] != '\0') {
        len ++;
    }
    return len;
}

U8Size u8_string_group_size(U8StringGroup text_group) {
    if (text_group == NULL) return 0;
    U8StringGroup pointer = text_group;
    U8Size len = 0;
    while (*pointer != '\0') {
        while (*pointer != '\0') {
            pointer ++;
        }
        pointer ++;
        len ++;
    }
    return len;
}

U8Size u8_string_group_list_size(U8StringGroupList text_group_list) {
    if (text_group_list == NULL) return 0;
    U8StringGroupList pointer = text_group_list;
    U8Size len = 0;
    while (*pointer != '\0') {
        while (*pointer != '\0') {
            while (*pointer != '\0') {
                pointer ++;
            }
            pointer ++;
        }
        pointer ++;
        len ++;
    }
    return len;
}

U8String u8_string_group_get(U8StringGroup text_group, U8Size index) {
    if (text_group == NULL) return NULL;
    U8StringGroup pointer = text_group;
    U8Size len = 0;
    while (*pointer != '\0') {
        if (index == len) {
            return pointer;
        }
        while (*pointer != '\0') {
            pointer ++;
        }
        pointer ++;
        len ++;
    }
    return NULL;
}

U8StringGroup u8_string_group_list_get(U8StringGroupList text_group, U8Size index) {
    if (text_group == NULL) return NULL;
    U8StringGroupList pointer = text_group;
    U8Size len = 0;
    while (*pointer != '\0') {
        if (index == len) {
            return pointer;
        }
        while (*pointer != '\0') {
            while (*pointer != '\0') {
                pointer ++;
            }
            pointer ++;
        }
        pointer ++;
        len ++;
    }
    return NULL;
}

void i32_to_u8str(int32_t value, char *text_buffer) {
    uint8_t pos = 0;
    if (value < 0) {
        text_buffer[pos] = '-';
        pos += 1;
    }
    if (value == 0) {
        text_buffer[pos] = '0';
        pos += 1;
    } else {
        uint64_t scales = 10;
        uint64_t vals = (value < 0) ? ((uint64_t)(-(int64_t)value)) : ((uint64_t)value);
        while (vals / scales > 0) {
            scales *= 10;
        }
        scales /= 10;
        while (scales > 0) {
            text_buffer[pos] = '0' + ((vals / scales) % 10);
            scales /= 10;
            pos ++;
        }
    }
    text_buffer[pos] = '\0';
}

uint8_t u8str_to_i32(U8String text, int32_t *value_buffer) {
    U8Size length = u8_string_size(text);
    uint8_t pos = 0;
    if (length <= 0) {
        return 0;
    }
    uint8_t is_signed = (text[0] == '-') ? 1: 0;
    if (is_signed) pos ++;
    if (is_signed && length <= 1) return 0;
    int64_t value = 0;
    while (pos < length && pos <= u8_MAX_NUMBER_STRING_LENGTH) {
        char ch = text[pos];
        if (!CHAR_IS_NUM(ch)) {
            return 0;
        }
        value *= 10;
        value += ch - '0';
        pos ++;
    }
    if (is_signed) value = -value;
    if (value <= INT32_MAX && value >= INT32_MIN) {
        *value_buffer = value;
        return 1;
    }
    return 0;
}

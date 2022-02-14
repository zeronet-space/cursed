#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char* BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void _base64_encode_triple(unsigned char triple[3], char result[4]) {
    int tripleValue, i;

    tripleValue = triple[0];
    tripleValue *= 256;
    tripleValue += triple[1];
    tripleValue *= 256;
    tripleValue += triple[2];

    for (i = 0; i < 4; i++) {
        result[3 - i] = BASE64_CHARS[tripleValue % 64];
        tripleValue /= 64;
    }
}

int base64_encode(unsigned char* source, size_t sourcelen, char* target, size_t targetlen) {
    if ((sourcelen + 2) / 3 * 4 > targetlen - 1) return 0;

    while (sourcelen >= 3) {
        _base64_encode_triple(source, target);
        sourcelen -= 3;
        source += 3;
        target += 4;
    }

    if (sourcelen > 0) {
        unsigned char temp[3];
        memset(temp, 0, sizeof(temp));
        memcpy(temp, source, sourcelen);
        _base64_encode_triple(temp, target);

        target[3] = '=';
        if (sourcelen == 1) target[2] = '=';
        target += 4;
    }

    target[0] = 0;

    return 1;
}

int _base64_char_value(char base64char) {
    if (base64char >= 'A' && base64char <= 'Z')
        return base64char - 'A';
    if (base64char >= 'a' && base64char <= 'z')
        return base64char - 'a' + 26;
    if (base64char >= '0' && base64char <= '9')
        return base64char - '0' + 2 * 26;
    if (base64char == '+')
        return 2 * 26 + 10;
    if (base64char == '/')
        return 2 * 26 + 11;
    return -1;
}

int _base64_decode_triple(char quadruple[4], unsigned char* result) {
    int i, triple_value, bytes_to_decode = 3, only_equals_yet = 1;
    int char_value[4];

    for (i = 0; i < 4; i++)
        char_value[i] = _base64_char_value(quadruple[i]);

    for (i = 3; i >= 0; i--) {
        if (char_value[i] < 0) {
            if (only_equals_yet && quadruple[i] == '=') {
                char_value[i] = 0;
                bytes_to_decode--;
                continue;
            }
            return 0;
        }
        only_equals_yet = 0;
    }

    if (bytes_to_decode < 0) bytes_to_decode = 0;

    triple_value = char_value[0];
    triple_value *= 64;
    triple_value += char_value[1];
    triple_value *= 64;
    triple_value += char_value[2];
    triple_value *= 64;
    triple_value += char_value[3];

    for (i = bytes_to_decode; i < 3; i++) triple_value /= 256;
    for (i = bytes_to_decode - 1; i >= 0; i--) {
        result[i] = triple_value % 256;
        triple_value /= 256;
    }

    return bytes_to_decode;
}

size_t base64_decode(char* source, unsigned char* target, size_t targetlen) {
    char* src, * tmpptr;
    char quadruple[4];
    unsigned char tmpresult[3];
    int i, tmplen = 3;
    size_t converted = 0;

    src = (char*)malloc(strlen(source) + 5);
    if (src == NULL) return -1;

    strcpy(src, source);
    strcat(src, "====");

    tmpptr = src;

    while (tmplen == 3) {
        for (i = 0; i < 4; i++) {
            while (*tmpptr != '=' && _base64_char_value(*tmpptr) < 0)
                tmpptr++;

            quadruple[i] = *(tmpptr++);
        }

        tmplen = _base64_decode_triple(quadruple, tmpresult);

        if (targetlen < tmplen) {
            free(src);
            return -1;
        }

        memcpy(target, tmpresult, tmplen);
        target += tmplen;
        targetlen -= tmplen;
        converted += tmplen;
    }

    free(src);
    return converted;
}

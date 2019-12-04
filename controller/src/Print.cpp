#include "Print.hpp"
#include <Arduino.h>

void init_print()
{
    Serial.begin(115200);
    while (!Serial) { }

    println("\r\n\r\n========================================");
}

void println() { Serial.print("\r\n"); }

int println(const char* format, ...)
{
    int i, j, count = 0;

    va_list argv;
    va_start(argv, format);
    for (i = 0, j = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            count++;

            Serial.write(reinterpret_cast<const uint8_t*>(format + j), i - j);

            switch (format[++i]) {
            case 'd':
                Serial.print(va_arg(argv, int));
                break;
            case 'u':
                Serial.print(va_arg(argv, unsigned int));
                break;
            case 'l':
                Serial.print(va_arg(argv, long));
                break;
            case 'f':
                Serial.print(va_arg(argv, double));
                break;
            case 'c':
                Serial.print((char)va_arg(argv, int));
                break;
            case 's':
                Serial.print(va_arg(argv, char*));
                break;
            case '%':
                Serial.print("%");
                break;
            default:;
            };

            j = i + 1;
        }
    };
    va_end(argv);

    if (i > j) {
        Serial.write(reinterpret_cast<const uint8_t*>(format + j), i - j);
    }

    Serial.print("\r\n");
    return count;
}

int print(const char* format, ...)
{
    int i, j, count = 0;

    va_list argv;
    va_start(argv, format);
    for (i = 0, j = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            count++;

            Serial.write(reinterpret_cast<const uint8_t*>(format + j), i - j);

            switch (format[++i]) {
            case 'd':
                Serial.print(va_arg(argv, int));
                break;
            case 'u':
                Serial.print(va_arg(argv, unsigned int));
                break;
            case 'l':
                Serial.print(va_arg(argv, long));
                break;
            case 'f':
                Serial.print(va_arg(argv, double));
                break;
            case 'c':
                Serial.print((char)va_arg(argv, int));
                break;
            case 's':
                Serial.print(va_arg(argv, char*));
                break;
            case '%':
                Serial.print("%");
                break;
            default:;
            };

            j = i + 1;
        }
    };
    va_end(argv);

    if (i > j) {
        Serial.write(reinterpret_cast<const uint8_t*>(format + j), i - j);
    }

    return count;
}

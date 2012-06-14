#ifndef _OFFSETS_H
#define _OFFSETS_H

#include <bus.h>

struct bus_hdr* get_bus_header(const char* data);
struct bus_hello* get_bus_hello(const char* data);
struct bus_hello_reply* get_bus_hello_reply(const char* data);
struct bus_event_hdr* get_bus_event_header(const char* data);
struct bus_set_motor_driver* get_bus_set_motor_driver(const char* data);
struct bus_set_outputs* get_bus_set_outputs(const char* data);
struct bus_motor_sensors_event* get_bus_motor_sensors_event(const char* data);

#endif /* offsets.h */

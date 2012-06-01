#ifndef _OFFSETS_H
#define _OFFSETS_H

#include <bus.h>

struct bus_hdr* get_bus_header(const char* data);
struct bus_hello* get_bus_hello(const char* data);
struct bus_hello_reply* get_bus_hello_reply(const char* data);
struct bus_event_hdr* get_bus_event_header(const char* data);

#endif /* offsets.h */

#include "offsets.h"

struct bus_hdr* get_bus_header(const char* data)
{
        return (struct bus_hdr*)(data);
}

struct bus_hello* get_bus_hello(const char* data)
{
        return (struct bus_hello*)(((char*)get_bus_header(data)) + sizeof(struct bus_hdr));
}

struct bus_hello_reply* get_bus_hello_reply(const char* data)
{
        return (struct bus_hello_reply*)(((char*)get_bus_header(data)) + sizeof(struct bus_hdr));
}

struct bus_event_hdr* get_bus_event_header(const char* data)
{
        return (struct bus_event_hdr*)(((char*)get_bus_header(data)) + sizeof(struct bus_hdr));
}
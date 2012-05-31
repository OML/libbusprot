#ifndef _OFFSETS_H
#define _OFFSETS_H


static inline struct bus_hdr* get_bus_header(const char* data)
{
        return (struct bus_hdr*)(data);
}

static inline struct bus_hello* get_bus_hello(const char* data)
{
        return (struct bus_hello*)(((char*)get_bus_header(data)) + sizeof(struct bus_hdr));
}

static inline struct bus_hello_reply* get_bus_hello_reply(const char* data)
{
        return (struct bus_hello_reply*)(((char*)get_bus_header(data)) + sizeof(struct bus_hdr));
}


#endif /* offsets.h */

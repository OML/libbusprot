#ifndef _BUSPROT_INTERNAL_H
#define _BUSPROT_INTERNAL_H

struct bus_descriptor;

void bus_init(struct bus_descriptor* bus);

void forward_packet(char* data, size_t len);

size_t uart_write(struct uart_descriptor* uart, const char* data, size_t len);
size_t uart_read(struct uart_descriptor* uart, char* data, size_t len);
void uart_init(struct uart_descriptor* uart, int nth_uart);


void bus_write(struct bus_descriptor* bus, const char* data, size_t len);
void bus_read(struct bus_descriptor* bus, char* buffer, size_t len);



#endif /* busprot_internal.h */

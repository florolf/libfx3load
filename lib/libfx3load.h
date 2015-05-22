#ifndef _LIBFX3LOAD_H
#define _LIBFX3LOAD_H

#include <stdint.h>

typedef struct fx3 *fx3_t;

fx3_t fx3load_open(uint16_t vid, uint16_t pid);
void fx3load_close(fx3_t fx3);

int fx3load_write(fx3_t fx3, uint32_t addr, uint16_t len, uint8_t *data);
int fx3load_read(fx3_t fx3, uint32_t addr, uint16_t len, uint8_t *data);
int fx3load_exec(fx3_t fx3, uint32_t addr);

int fx3load_from_ihex(fx3_t fx3, const char *path);

const char *fx3load_error(fx3_t fx3);

#endif /* _LIBFX3LOAD_H */

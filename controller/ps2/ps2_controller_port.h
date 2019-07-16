#include <stdint.h>

typedef struct
{
    int (*init)(void *vp);
    void (*send)(const uint8_t *pb, uint8_t len);
    void (*send_then_recv)(const uint8_t *pb_send, uint8_t len_send, uint8_t *pb_recv, uint8_t len_recv);
    void (*transfer)(const uint8_t *pb_send, uint8_t *pb_recv, uint8_t len);
}hal_ps2_port_t;


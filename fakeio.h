#include "IOboard.h"

#define ID_MAX 4
#define DATA_WORD_MAX 4
#define DATA_BYTE_MAX 0x10

#define IO_MODEL 0x2

#define MASK_SEQ 0x0f
#define FLAG_UPLOAD 0x01
#define FLAG_DOWNLOAD 0x10
#define MASK_UPLOAD 0x0f
#define MASK_DOWNLOAD 0xf0
#define OF_UPLOAD 0
#define OF_DOWNLOAD 4

#define IO_STATUS_MAX 5

struct iobuf {
	uint32_t first;
        uint32_t second;
        uint32_t third;
        uint32_t forth;
}

struct ioboard {
	uint32_t idseq;
	uint32_t updown;
	struct iobuf *buftohps;
	struct iobuf *buffromhps;
	struct iobuf *data;
}

static struct ioboard *pio;

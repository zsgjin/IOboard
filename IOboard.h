#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define IO_DATAFRAME_MAX 5
struct IODeviceData {
	uint8_t	id;
	uint8_t enable;
	uint8_t verify;
	uint8_t model;
	unsigned char input[IO_DATAFRAME_MAX];
	unsigned char output[IO_DATAFRAME_MAX];
};

#define IOERROR (-1)

#define IO_BASE 0xC1013000
#define LENGTH 0x1000
#define ADDR_FLAG_WRITE 0x30
#define ADDR_FLAG_READ 0x38
#define ADDR_DATA_WRITE 0x50
#define ADDR_DATA_READ 0x40

#define IO_DATA_WORD 4
#define IO_DATA_BYTE ((IO_DATA_BYTE) * 4)

#define OF_ID 0x4
#define OF_ENABLE 0x11
#define OF_VERIFY 0x10
#define OF_MODEL 0x0

#define OF_FRAME_INPUT 0x4
#define OF_FRAME_OUTPUT 0xB

#define MASK_ID 0x000000f0
#define MASK_ENABLE	0x00020000
#define MASK_VERIFY 0x00010000
#define MASK_MODEL 0x0000000f
int ioInit(uint8_t fake);

int ioSetIdSeq(uint8_t idseq);

int ioGetSeq(uint8_t *seq);

int ioWriteDownload(struct IODeviceData *idd);

int ioReadUpload(struct IODeviceData *idd);

#ifdef __cplusplus
}
#endif

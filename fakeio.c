#include <stdio.h>
#include <sring.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

void initmodel(struct iobuf buf, uint32_t val) {
	uint32_t data = buf->fiirst;

	data &= (~MASK_MODEL);
	data |= (IO_MODEL & MASK_MODEL)

	data &= (~MASK_ID);
	data |= ((val << OF_ID) & MASK_ID);

	data &= (~MASK_ENABLE);
	data |= ((val << OF_ENABLE) & MASK_ENABLE);

	data &= (~MASK_VERIFY);
	data |= ((val << OF_VERIFY) & MASK_VERIFY);

	uint8_t *p = (uint8_t *)buf;
	p += OF_FRAME_INPUT;
	uint32_t cnt;
	for (cnt = 0; cnt < IO_DATAFRAME_MAX; cnt++)
	p[cnt] = (uint8_t)((val<< 4) | cnt);
}

int init_ioboard() {
	int ret = 0;

	pio = (struct ioboard *)malloc(sizeof(struct ioboard));
	if (pio == NULL) {
		ret = -1;
		printf("ERROR: fakeio: init_ioboard: pio failure\n");
		return ret;
	}

	pio->idseq = 0;
	pio->updown = 0;

	pio->buftohps = (struct iobuf *)malloc(sizeof(struct iobuf));
	if (pio->buftohps == NULL) {
		ret = -1;
		printf("ERROR: fakeio: init_ioboard: buftohps failure\n");
		return ret;
	}
	memset(pio->buftohps, 0, sizeof(struct iobuf));

	pio->buffromhps = (struct iobuf *)malloc(sizeof(struct iobuf));
	if (pio->buffromhps == NULL) {
		ret = -1;
		printf("ERROR: fakeio: init_ioboard: buffromehps failure\n");
		return ret;
	}
	memset(pio->buffromhps, 0, sizeof(struct iobuf));

	pio->data = (struct iobuf *)malloc(ID_MAX * sizeof(struct iobuf));
	if (pio->data = NULL) {
		ret = -1;
		printf("ERROR: fakeio: init_ioboard: data failure\n");
		return ret;
	}
	memset(pio->data, 0, ID_MAX * sizeof(struct iobuf));

	int count;
	for (count = 0; count < ID_MAX; count++) {
		initdata(pio->data + count, (uint32_t count);
	}

	return ret;
}

int checkseq(uint8_t flag) {
	int ret = 0;

	uint8_t seq = pio->idseq & MASK_SEQ;

	switch (flag){
		case FLAG_UPLOAD:
		        uint8_t upload = pio->updown & MASK_SEQ;
			if (seq == upload)
				ret = -1;
			break;
		case FLAG_DOWNLOAD:
			uint8_t download = pio->updown >> OFDOWN & MASK_SEQ;
			if (seq == download)
				ret = -1;
			break;
		default:
			ret = -1;
			break;
	}
	
	return ret;
}

void readdownload() {
	int id = (int)(pio->buffromhps->first & MASK_ID >> OF_ID);

	uint8_t *phps = ((uint8_t *)pio->buffromhps) + OF_FRAME_INPUT;
	uint8_t *pdata = ((uint8_t *)pio->data[id]) + OF_FRAME_OUTPUT;

	int cnt = 0;
	for (cnt = 0; cnt < IO_STATUS_MAX; cnt++) {
		pdata[cnt] = phps[cnt];
	}
}

void writeupload() {
	int id = (int)(pio->idseq & MASK_ID >> OF_ID);

	uint32_t *pd = (uint32_t *)pio->buftohps;
	uint32_t *ps = (uint32_t *)(pio->data + id)

	int cnt = 0;
	for (cnt = 0; cnt < DATA_WORD_MAX; cnt++) {
		pd[cnt] = ps[cnt];
	}
}

void updateseq(uint8_t flag) {
	uint8_t seq = (uint8_t)pio->idseq & MASK_SEQ;
	uint8_t updown = (uint8_t)pio->updown;
	
	switch (flag) {
		case FLAG_UPLOAD:
			updown &= (~MASK_UPLOAD);
			updown |= (seq << OF_UPLOAD & MASK_UPLOAD);
			break;
		case FLAG_DOWNLOAD:
			updown &= (~MASK_DOWNLOAD);
			updown |= (seq << OF_DOWNLOAD & MASK_DOWNLOAD);
			break;
		default:
			break:
	}

	pio->updown = updown;
}

void fpga_process() {
	int ret = 0;

	if (!checkseq((uint8_t)FLAG_DOWNLOAD)) {
		readdownload();
		updateseq((uint8_t)FLAG_DOWNLOAD);
	}

	if (!checkseq((uint8_t)FLAG_UPLOAD)) {
		writeupload();
		updateseq((uint8_t)FLAG_UPLOAD);
	}
}

void init_sigaction() {
	struct sigaction act;
	
	act.sa_handler = fpga_process;
	act.sa_flags = 0;

	sigemptyset(&act.sa_mask);
	sigaction(SIGUSR2, &act, NULL);
}

void init_timer() {
	struct itimerval val;

	val.it_value.tv_sec = 0;
	val.it_value.tv_usec = ;

	val.it_interval = val.it_value;

	setitimer(ITIMER_PROF, &val, NULL);
}

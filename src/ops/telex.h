#ifndef _OPS_TELEX_H_
#define _OPS_TELEX_H_

#include "ops/op.h"

// operators
// TXo Operators
extern const tele_op_t op_TO_TR;
extern const tele_op_t op_TO_TR_TOG;
extern const tele_op_t op_TO_TR_PULSE;
extern const tele_op_t op_TO_TR_TIME;
extern const tele_op_t op_TO_TR_TIMES;
extern const tele_op_t op_TO_TR_TIMEM;
extern const tele_op_t op_TO_TR_POL;

extern const tele_op_t op_TO_KILL;
extern const tele_op_t op_TO_CV;
extern const tele_op_t op_TO_CV_SLEW;
extern const tele_op_t op_TO_CV_SLEWS;
extern const tele_op_t op_TO_CV_SLEWM;
extern const tele_op_t op_TO_CV_SET;
extern const tele_op_t op_TO_CV_OFF;
extern const tele_op_t op_TO_CV_QT;
extern const tele_op_t op_TO_CV_QT_SET;
extern const tele_op_t op_TO_CV_N;
extern const tele_op_t op_TO_CV_N_SET;
extern const tele_op_t op_TO_CV_SCALE;

extern const tele_op_t op_TO_OSC;
extern const tele_op_t op_TO_OSC_SET;
extern const tele_op_t op_TO_OSC_QT;
extern const tele_op_t op_TO_OSC_QT_SET;
extern const tele_op_t op_TO_OSC_FQ;
extern const tele_op_t op_TO_OSC_FQ_SET;
extern const tele_op_t op_TO_OSC_N;
extern const tele_op_t op_TO_OSC_N_SET;

extern const tele_op_t op_TO_OSC_LFO;
extern const tele_op_t op_TO_OSC_LFO_SET;
extern const tele_op_t op_TO_OSC_WAVE;
extern const tele_op_t op_TO_OSC_SYNC;
extern const tele_op_t op_TO_OSC_WIDTH;
extern const tele_op_t op_TO_OSC_RECT;
extern const tele_op_t op_TO_OSC_SLEW;
extern const tele_op_t op_TO_OSC_SLEWS;
extern const tele_op_t op_TO_OSC_SLEWM;
extern const tele_op_t op_TO_OSC_SCALE;

extern const tele_op_t op_TO_ENV_ACT;
extern const tele_op_t op_TO_ENV_ATT;
extern const tele_op_t op_TO_ENV_ATTS;
extern const tele_op_t op_TO_ENV_ATTM;
extern const tele_op_t op_TO_ENV_DEC;
extern const tele_op_t op_TO_ENV_DECS;
extern const tele_op_t op_TO_ENV_DECM;
extern const tele_op_t op_TO_ENV_TRIG;

// TXi Operators
extern const tele_op_t op_TI_PARAM;
extern const tele_op_t op_TI_IN;
extern const tele_op_t op_TI_PARAM_QT;
extern const tele_op_t op_TI_IN_QT;
extern const tele_op_t op_TI_PARAM_N;
extern const tele_op_t op_TI_IN_N;
extern const tele_op_t op_TI_PARAM_SCALE;
extern const tele_op_t op_TI_IN_SCALE;
extern const tele_op_t op_TI_PARAM_CALIBRATE;
extern const tele_op_t op_TI_IN_CALIBRATE;
extern const tele_op_t op_TI_STORE;
extern const tele_op_t op_TI_RESET;

// helpers
void TXSend(uint8_t model, uint8_t command, uint8_t output, int16_t value,
            bool set);
void TXCmd(uint8_t model, uint8_t command, uint8_t output);
void TXSet(uint8_t model, uint8_t command, command_state_t *cs);
void TXReceive(uint8_t model, command_state_t *cs, uint8_t mode, bool shift);

// constants

// TELEXo

#define TO 0x60
#define TO_0 0x60
#define TO_1 0x61
#define TO_2 0x62
#define TO_3 0x63
#define TO_4 0x64
#define TO_5 0x65
#define TO_6 0x66
#define TO_7 0x67

// TXo Commands

#define TO_TR 0x00
#define TO_TR_TOG 0x01
#define TO_TR_TIME 0x02
#define TO_TR_TIMES 0x03
#define TO_TR_TIMEM 0x04
#define TO_TR_PULSE 0x05
#define TO_TR_POL 0x06
#define TO_TR_M 0x07
#define TO_TR_M_ACT 0x08

#define TO_CV 0x10
#define TO_CV_SET 0x11
#define TO_CV_SLEW 0x12
#define TO_CV_SLEWS 0x13
#define TO_CV_SLEWM 0x14
#define TO_CV_OFF 0x15

#define TO_KILL 0x20
#define TO_RESET 0x21

#define TO_CV_QT 0x30
#define TO_CV_QT_SET 0x31
#define TO_CV_N 0x32
#define TO_CV_N_SET 0x33
#define TO_CV_SCALE 0x34

#define TO_OSC 0x40
#define TO_OSC_SET 0x41
#define TO_OSC_QT 0x42
#define TO_OSC_QT_SET 0x43
#define TO_OSC_FQ 0x44
#define TO_OSC_FQ_SET 0x45
#define TO_OSC_N 0x46
#define TO_OSC_N_SET 0x47
#define TO_OSC_LFO 0x48
#define TO_OSC_LFO_SET 0x49
#define TO_OSC_WAVE 0x4A
#define TO_OSC_SYNC 0x4B
#define TO_OSC_WIDTH 0x4C
#define TO_OSC_RECT 0x4D
#define TO_OSC_SCALE 0x4E
#define TO_OSC_SLEW 0x4F
#define TO_OSC_SLEWS 0x50
#define TO_OSC_SLEWM 0x51
#define TO_OSC_TR_ACT 0x52

#define TO_ENV_ACT 0x60
#define TO_ENV_ATT 0x61
#define TO_ENV_ATTS 0x62
#define TO_ENV_ATTM 0x63
#define TO_ENV_DEC 0x64
#define TO_ENV_DECS 0x65
#define TO_ENV_DECM 0x66
#define TO_ENV_TRIG 0x67
#define TO_ENV_CYC 0x68
#define TO_ENV_BPOL 0x69

// TELEXi

#define TI 0x68
#define TI_0 0x68
#define TI_1 0x69
#define TI_2 0x6A
#define TI_3 0x6B
#define TI_4 0x6C
#define TI_5 0x6D
#define TI_6 0x6E
#define TI_7 0x6F

// TXi Commands

#define TI_IN 0x00
#define TI_IN_QT 0x01
#define TI_IN_N 0x02
#define TI_IN_SCALE 0x03

#define TI_PARAM 0x10
#define TI_PARAM_QT 0x11
#define TI_PARAM_N 0x12
#define TI_PARAM_SCALE 0x13

#define TI_IN_CALIBRATE 0x20
#define TI_PARAM_CALIBRATE 0x21
#define TI_STORE 0x22
#define TI_RESET 0x23

#endif
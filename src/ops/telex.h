#ifndef _OPS_TELEX_H_
#define _OPS_TELEX_H_

#include "ops/op.h"

// operators
// TXo Operators
extern const tele_op_t op_TO_TR;
extern const tele_op_t op_TO_TR_PULSE;

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
#define TO_TR_PULSE 0x05

#endif
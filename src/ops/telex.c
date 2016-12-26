// Operators for the TELEXo (TXo) and the TELEXi (TXi) Modules
#include "ops/telex.h"

#include "helpers.h"
#include "ii.h"
#include "teletype.h"
#include "teletype_io.h"

// TXo Methods
static void op_TO_TR_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_TO_TR_PULSE_get(const void *data, scene_state_t *ss,
                               exec_state_t *es, command_state_t *cs);

const tele_op_t op_TO_TR              = MAKE_GET_OP(TO.TR               , op_TO_TR_get              , 2, false);
const tele_op_t op_TO_TR_PULSE 	      = MAKE_GET_OP(TO.TR.PULSE         , op_TO_TR_PULSE_get	      , 1, false);

// telex helpers
void TXSend(uint8_t model, uint8_t command, uint8_t output, int16_t value,
            bool set) {
    // zero-index the output
    output -= 1;
    // convert the output to the device and the port
    uint8_t port = output % 4;
    uint8_t device = output >> 2;
    uint8_t address = model + device;
    // init and fill the buffer	(make the buffer smaller if we are not sending a
    // payload)
    uint8_t buffer[set ? 4 : 2];
    buffer[0] = command;
    buffer[1] = port;
    if (set) {
        uint16_t temp = (uint16_t)value;
        buffer[2] = temp >> 8;
        buffer[3] = temp & 0xff;
    }
    tele_ii_tx(address, buffer, set ? 4 : 2);
}
void TXCmd(uint8_t model, uint8_t command, uint8_t output) {
    TXSend(model, command, output, 0, false);
}
void TXSet(uint8_t model, uint8_t command, command_state_t *cs) {
	uint8_t output = cs_pop(cs);
	int16_t value = cs_pop(cs);
    TXSend(model, command, output, value, true);
}
void TXReceive(uint8_t model, command_state_t *cs, uint8_t mode, bool shift) {
    // zero-index the output
    uint8_t input = cs_pop(cs) - 1;
    // send the port, device and address
    uint8_t port = input % 4;
    uint8_t device = input >> 2;
    uint8_t address = model + device;
    // inputs are numbered 0-7 for each device - shift is for the second half
    // mode pushes it up so it can read quantized values and note numbers
    port += (shift ? 4 : 0) + (mode * 8);
    // tell the device what value you are going to query
    uint8_t buffer[2];
    buffer[0] = port;
    tele_ii_tx(address, buffer, 1);
    // now read the vaule
    buffer[0] = 0;
    buffer[1] = 0;
    tele_ii_rx(address, buffer, 2);
    int16_t value = (buffer[0] << 8) + buffer[1];
    cs_push(cs, value);
}

// TELEX get and set methods
// TXo
static void op_TO_TR_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    TXSet(TO, TO_TR, cs);
}
static void op_TO_TR_PULSE_get(const void *NOTUSED(data), scene_state_t *ss,
                               exec_state_t *NOTUSED(es), command_state_t *cs) {
    TXCmd(TO, TO_TR_PULSE, cs_pop(cs));
}
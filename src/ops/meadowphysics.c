#include "meadowphysics.h"

#include "ii.h"

const tele_op_t op_MP_PRESET = MAKE_SIMPLE_I2C_OP(MP.PRESET, MP_PRESET);
const tele_op_t op_MP_RESET = MAKE_SIMPLE_I2C_OP(MP.RESET, MP_RESET);
const tele_op_t op_MP_SYNC = MAKE_SIMPLE_I2C_OP(MP.SYNC, MP_SYNC);
const tele_op_t op_MP_MUTE = MAKE_SIMPLE_I2C_OP(MP.MUTE, MP_MUTE);
const tele_op_t op_MP_UNMUTE = MAKE_SIMPLE_I2C_OP(MP.UNMUTE, MP_UNMUTE);
const tele_op_t op_MP_FREEZE = MAKE_SIMPLE_I2C_OP(MP.FREEZE, MP_FREEZE);
const tele_op_t op_MP_UNFREEZE = MAKE_SIMPLE_I2C_OP(MP.UNFREEZE, MP_UNFREEZE);
const tele_op_t op_MP_STOP = MAKE_SIMPLE_I2C_OP(MP.STOP, MP_STOP);

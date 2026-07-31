#include "chip8_core.h"


#define X(optype, opcode, executor, decoder)                \
    if (!( _opcode - ( _opcode & opcode )))                 \
        return optype;

OpType chip8_get_optype(const addr _opcode){
    CHIP8_INSTRUCTION_TABLE(X)
    return RAW;
}

#undef X

#define X(optype, opcode, executor, decoder)                \
    case optype:                                            \
        return executor;                                    \
        break;

const ExecutorFunc chip8_get_executor(const OpType optype){
    switch (optype)
    {
    CHIP8_INSTRUCTION_TABLE(X)
    default:
        return NULL;    
    }
}

#undef X

#define X(optype, opcode, executor, decoder)          \
    case optype:                                      \
        return decoder;

const DecoderFunc chip8_get_decoder(DecodedOpcode* decoded_opcode){
    switch (decoded_opcode->optype)
    {
        CHIP8_INSTRUCTION_TABLE(X)
        default:
            return decode_raw;
    }
}

#undef X
#include "match_token.h"

#include <ctype.h>   // isdigit
#include <stdlib.h>  // rand, strtol

#include <stdio.h>

#include "ops/op.h"
#include "ops/op_enum.h"

%%{
    machine match_token; # declare our ragel machine

    number = '-'? digit+;

    main := |*
        # NUMBERS
        number        => { MATCH_NUMBER() };

        # OPS
        # variables
        "A"           => { MATCH_OP(E_OP_A); };
        "B"           => { MATCH_OP(E_OP_B); };
        "C"           => { MATCH_OP(E_OP_C); };
        "D"           => { MATCH_OP(E_OP_D); };
        "DRUNK"       => { MATCH_OP(E_OP_DRUNK); };
        "DRUNK.MAX"   => { MATCH_OP(E_OP_DRUNK_MAX); };
        "DRUNK.MIN"   => { MATCH_OP(E_OP_DRUNK_MIN); };
        "DRUNK.WRAP"  => { MATCH_OP(E_OP_DRUNK_WRAP); };
        "FLIP"        => { MATCH_OP(E_OP_FLIP); };
        "I"           => { MATCH_OP(E_OP_I); };
        "IN"          => { MATCH_OP(E_OP_IN); };
        "O"           => { MATCH_OP(E_OP_O); };
        "O.INC"       => { MATCH_OP(E_OP_O_INC); };
        "O.MAX"       => { MATCH_OP(E_OP_O_MAX); };
        "O.MIN"       => { MATCH_OP(E_OP_O_MIN); };
        "O.WRAP"      => { MATCH_OP(E_OP_O_WRAP); };
        "PARAM"       => { MATCH_OP(E_OP_PARAM); };
        "PRM"         => { MATCH_OP(E_OP_PRM); };
        "T"           => { MATCH_OP(E_OP_T); };
        "TIME"        => { MATCH_OP(E_OP_TIME); };
        "TIME.ACT"    => { MATCH_OP(E_OP_TIME_ACT); };
        "X"           => { MATCH_OP(E_OP_X); };
        "Y"           => { MATCH_OP(E_OP_Y); };
        "Z"           => { MATCH_OP(E_OP_Z); };

        # metronome
        "M"           => { MATCH_OP(E_OP_M); };
        "M.ACT"       => { MATCH_OP(E_OP_M_ACT); };
        "M.RESET"     => { MATCH_OP(E_OP_M_RESET); };

        # patterns
        "P.N"         => { MATCH_OP(E_OP_P_N); };
        "P"           => { MATCH_OP(E_OP_P); };
        "PN"          => { MATCH_OP(E_OP_PN); };
        "P.L"         => { MATCH_OP(E_OP_P_L); };
        "PN.L"        => { MATCH_OP(E_OP_PN_L); };
        "P.WRAP"      => { MATCH_OP(E_OP_P_WRAP); };
        "PN.WRAP"     => { MATCH_OP(E_OP_PN_WRAP); };
        "P.START"     => { MATCH_OP(E_OP_P_START); };
        "PN.START"    => { MATCH_OP(E_OP_PN_START); };
        "P.END"       => { MATCH_OP(E_OP_P_END); };
        "PN.END"      => { MATCH_OP(E_OP_PN_END); };
        "P.I"         => { MATCH_OP(E_OP_P_I); };
        "PN.I"        => { MATCH_OP(E_OP_PN_I); };
        "P.HERE"      => { MATCH_OP(E_OP_P_HERE); };
        "PN.HERE"     => { MATCH_OP(E_OP_PN_HERE); };
        "P.NEXT"      => { MATCH_OP(E_OP_P_NEXT); };
        "PN.NEXT"     => { MATCH_OP(E_OP_PN_NEXT); };
        "P.PREV"      => { MATCH_OP(E_OP_P_PREV); };
        "PN.PREV"     => { MATCH_OP(E_OP_PN_PREV); };
        "P.INS"       => { MATCH_OP(E_OP_P_INS); };
        "PN.INS"      => { MATCH_OP(E_OP_PN_INS); };
        "P.RM"        => { MATCH_OP(E_OP_P_RM); };
        "PN.RM"       => { MATCH_OP(E_OP_PN_RM); };
        "P.PUSH"      => { MATCH_OP(E_OP_P_PUSH); };
        "PN.PUSH"     => { MATCH_OP(E_OP_PN_PUSH); };
        "P.POP"       => { MATCH_OP(E_OP_P_POP); };
        "PN.POP"      => { MATCH_OP(E_OP_PN_POP); };

        # queue
        "Q"           => { MATCH_OP(E_OP_Q); };
        "Q.AVG"       => { MATCH_OP(E_OP_Q_AVG); };
        "Q.N"         => { MATCH_OP(E_OP_Q_N); };

        # hardware
        "CV"          => { MATCH_OP(E_OP_CV); };
        "CV.OFF"      => { MATCH_OP(E_OP_CV_OFF); };
        "CV.SLEW"     => { MATCH_OP(E_OP_CV_SLEW); };
        "TR"          => { MATCH_OP(E_OP_TR); };
        "TR.POL"      => { MATCH_OP(E_OP_TR_POL); };
        "TR.TIME"     => { MATCH_OP(E_OP_TR_TIME); };
        "TR.TOG"      => { MATCH_OP(E_OP_TR_TOG); };
        "TR.PULSE"    => { MATCH_OP(E_OP_TR_PULSE); };
        "TR.P"        => { MATCH_OP(E_OP_TR_P); };
        "II"          => { MATCH_OP(E_OP_II); };
        "CV.SET"      => { MATCH_OP(E_OP_CV_SET); };
        "MUTE"        => { MATCH_OP(E_OP_MUTE); };
        "UNMUTE"      => { MATCH_OP(E_OP_UNMUTE); };
        "STATE"       => { MATCH_OP(E_OP_STATE); };
        "JF.TR"       => { MATCH_OP(E_OP_JF_TR); };
        "JF.RMODE"    => { MATCH_OP(E_OP_JF_RMODE); };
        "JF.RUN"      => { MATCH_OP(E_OP_JF_RUN); };
        "JF.SHIFT"    => { MATCH_OP(E_OP_JF_SHIFT); };
        "JF.VTR"      => { MATCH_OP(E_OP_JF_VTR); };
        "JF.MODE"     => { MATCH_OP(E_OP_JF_MODE); };
        "JF.TICK"     => { MATCH_OP(E_OP_JF_TICK); };
        "JF.VOX"      => { MATCH_OP(E_OP_JF_VOX); };
        "JF.NOTE"     => { MATCH_OP(E_OP_JF_NOTE); };
        "JF.GOD"      => { MATCH_OP(E_OP_JF_GOD); };
        "JF.TUNE"     => { MATCH_OP(E_OP_JF_TUNE); };
        "JF.QT"       => { MATCH_OP(E_OP_JF_QT); };
        "KR.PRE"      => { MATCH_OP(E_OP_KR_PRESET); };
        "KR.PAT"      => { MATCH_OP(E_OP_KR_PATTERN); };
        "KR.SCALE"    => { MATCH_OP(E_OP_KR_SCALE); };
        "KR.PERIOD"   => { MATCH_OP(E_OP_KR_PERIOD); };
        "KR.POS"      => { MATCH_OP(E_OP_KR_POS); };
        "KR.L.ST"     => { MATCH_OP(E_OP_KR_LOOP_ST); };
        "KR.L.LEN"    => { MATCH_OP(E_OP_KR_LOOP_LEN); };
        "KR.RES"      => { MATCH_OP(E_OP_KR_RESET); };
        "MP.PRE"      => { MATCH_OP(E_OP_MP_PRESET1); };
        "MP.RES"      => { MATCH_OP(E_OP_MP_RESET1); };
        "MP.OFF"      => { MATCH_OP(E_OP_MP_STOP1); };
        "MP.SCALE"    => { MATCH_OP(E_OP_MP_SCALE); };
        "MP.PERIOD"   => { MATCH_OP(E_OP_MP_PERIOD); };
        "LV.PRE"      => { MATCH_OP(E_OP_LV_PRESET); };
        "LV.RES"      => { MATCH_OP(E_OP_LV_RESET); };
        "LV.PAT"      => { MATCH_OP(E_OP_LV_PATTERN); };
        "LV.L.ST"     => { MATCH_OP(E_OP_LV_LOOP_ST); };
        "LV.L.LEN"    => { MATCH_OP(E_OP_LV_LOOP_LEN); };
        "LV.POS"      => { MATCH_OP(E_OP_LV_POS); };
        "CY.PRE"      => { MATCH_OP(E_OP_CY_PRESET); };
        "CY.RES"      => { MATCH_OP(E_OP_CY_RESET); };
        "CY.POS"      => { MATCH_OP(E_OP_CY_POS); };
        "CY.REV"      => { MATCH_OP(E_OP_CY_REV); };
        "CY.BRAKE"    => { MATCH_OP(E_OP_CY_BRAKE); };
        "CY.ACCEL"    => { MATCH_OP(E_OP_CY_ACCEL); };
        "CY.RANGE"    => { MATCH_OP(E_OP_CY_RANGE); };

        # maths
        "ADD"         => { MATCH_OP(E_OP_ADD); };
        "SUB"         => { MATCH_OP(E_OP_SUB); };
        "MUL"         => { MATCH_OP(E_OP_MUL); };
        "DIV"         => { MATCH_OP(E_OP_DIV); };
        "MOD"         => { MATCH_OP(E_OP_MOD); };
        "RAND"        => { MATCH_OP(E_OP_RAND); };
        "RRAND"       => { MATCH_OP(E_OP_RRAND); };
        "TOSS"        => { MATCH_OP(E_OP_TOSS); };
        "MIN"         => { MATCH_OP(E_OP_MIN); };
        "MAX"         => { MATCH_OP(E_OP_MAX); };
        "LIM"         => { MATCH_OP(E_OP_LIM); };
        "WRAP"        => { MATCH_OP(E_OP_WRAP); };
        "QT"          => { MATCH_OP(E_OP_QT); };
        "AVG"         => { MATCH_OP(E_OP_AVG); };
        "EQ"          => { MATCH_OP(E_OP_EQ); };
        "NE"          => { MATCH_OP(E_OP_NE); };
        "LT"          => { MATCH_OP(E_OP_LT); };
        "GT"          => { MATCH_OP(E_OP_GT); };
        "NZ"          => { MATCH_OP(E_OP_NZ); };
        "EZ"          => { MATCH_OP(E_OP_EZ); };
        "RSH"         => { MATCH_OP(E_OP_RSH); };
        "LSH"         => { MATCH_OP(E_OP_LSH); };
        "EXP"         => { MATCH_OP(E_OP_EXP); };
        "ABS"         => { MATCH_OP(E_OP_ABS); };
        "AND"         => { MATCH_OP(E_OP_AND); };
        "OR"          => { MATCH_OP(E_OP_OR); };
        "XOR"         => { MATCH_OP(E_OP_XOR); };
        "JI"          => { MATCH_OP(E_OP_JI); };
        "SCALE"       => { MATCH_OP(E_OP_SCALE); };
        "N"           => { MATCH_OP(E_OP_N); };
        "V"           => { MATCH_OP(E_OP_V); };
        "VV"          => { MATCH_OP(E_OP_VV); };
        "ER"          => { MATCH_OP(E_OP_ER); };
        "+"           => { MATCH_OP(E_OP_SYM_PLUS); };
        "-"           => { MATCH_OP(E_OP_SYM_DASH); };
        "*"           => { MATCH_OP(E_OP_SYM_STAR); };
        "/"           => { MATCH_OP(E_OP_SYM_FORWARD_SLASH); };
        "%"           => { MATCH_OP(E_OP_SYM_PERCENTAGE); };
        "=="          => { MATCH_OP(E_OP_SYM_EQUAL_x2); };
        "!="          => { MATCH_OP(E_OP_SYM_EXCLAMATION_EQUAL); };
        "<"           => { MATCH_OP(E_OP_SYM_LEFT_ANGLED); };
        ">"           => { MATCH_OP(E_OP_SYM_RIGHT_ANGLED); };
        "<="          => { MATCH_OP(E_OP_SYM_LEFT_ANGLED_EQUAL); };
        ">="          => { MATCH_OP(E_OP_SYM_RIGHT_ANGLED_EQUAL); };
        "<<"          => { MATCH_OP(E_OP_SYM_LEFT_ANGLED_x2); };
        ">>"          => { MATCH_OP(E_OP_SYM_RIGHT_ANGLED_x2); };
        "!"           => { MATCH_OP(E_OP_SYM_EXCLAMATION); };
        "&&"          => { MATCH_OP(E_OP_AMPERSAND_x2); };
        "||"          => { MATCH_OP(E_OP_PIPE_x2); };

        # stack
        "S.ALL"       => { MATCH_OP(E_OP_S_ALL); };
        "S.POP"       => { MATCH_OP(E_OP_S_POP); };
        "S.CLR"       => { MATCH_OP(E_OP_S_CLR); };
        "S.L"         => { MATCH_OP(E_OP_S_L); };

        # controlflow
        "SCRIPT"      => { MATCH_OP(E_OP_SCRIPT); };
        "KILL"        => { MATCH_OP(E_OP_KILL); };
        "SCENE"       => { MATCH_OP(E_OP_SCENE); };

        # delay
        "DEL.CLR"     => { MATCH_OP(E_OP_DEL_CLR); };

        # constants
        "WW.PRESET"   => { MATCH_OP(E_OP_WW_PRESET); };
        "WW.POS"      => { MATCH_OP(E_OP_WW_POS); };
        "WW.SYNC"     => { MATCH_OP(E_OP_WW_SYNC); };
        "WW.START"    => { MATCH_OP(E_OP_WW_START); };
        "WW.END"      => { MATCH_OP(E_OP_WW_END); };
        "WW.PMODE"    => { MATCH_OP(E_OP_WW_PMODE); };
        "WW.PATTERN"  => { MATCH_OP(E_OP_WW_PATTERN); };
        "WW.QPATTERN" => { MATCH_OP(E_OP_WW_QPATTERN); };
        "WW.MUTE1"    => { MATCH_OP(E_OP_WW_MUTE1); };
        "WW.MUTE2"    => { MATCH_OP(E_OP_WW_MUTE2); };
        "WW.MUTE3"    => { MATCH_OP(E_OP_WW_MUTE3); };
        "WW.MUTE4"    => { MATCH_OP(E_OP_WW_MUTE4); };
        "WW.MUTEA"    => { MATCH_OP(E_OP_WW_MUTEA); };
        "WW.MUTEB"    => { MATCH_OP(E_OP_WW_MUTEB); };
        "MP.PRESET"   => { MATCH_OP(E_OP_MP_PRESET); };
        "MP.RESET"    => { MATCH_OP(E_OP_MP_RESET); };
        "MP.SYNC"     => { MATCH_OP(E_OP_MP_SYNC); };
        "MP.MUTE"     => { MATCH_OP(E_OP_MP_MUTE); };
        "MP.UNMUTE"   => { MATCH_OP(E_OP_MP_UNMUTE); };
        "MP.FREEZE"   => { MATCH_OP(E_OP_MP_FREEZE); };
        "MP.UNFREEZE" => { MATCH_OP(E_OP_MP_UNFREEZE); };
        "MP.STOP"     => { MATCH_OP(E_OP_MP_STOP); };
        "ES.PRESET"   => { MATCH_OP(E_OP_ES_PRESET); };
        "ES.MODE"     => { MATCH_OP(E_OP_ES_MODE); };
        "ES.CLOCK"    => { MATCH_OP(E_OP_ES_CLOCK); };
        "ES.RESET"    => { MATCH_OP(E_OP_ES_RESET); };
        "ES.PATTERN"  => { MATCH_OP(E_OP_ES_PATTERN); };
        "ES.TRANS"    => { MATCH_OP(E_OP_ES_TRANS); };
        "ES.STOP"     => { MATCH_OP(E_OP_ES_STOP); };
        "ES.TRIPLE"   => { MATCH_OP(E_OP_ES_TRIPLE); };
        "ES.MAGIC"    => { MATCH_OP(E_OP_ES_MAGIC); };
        "OR.TRK"      => { MATCH_OP(E_OP_OR_TRK); };
        "OR.CLK"      => { MATCH_OP(E_OP_OR_CLK); };
        "OR.DIV"      => { MATCH_OP(E_OP_OR_DIV); };
        "OR.PHASE"    => { MATCH_OP(E_OP_OR_PHASE); };
        "OR.RST"      => { MATCH_OP(E_OP_OR_RST); };
        "OR.WGT"      => { MATCH_OP(E_OP_OR_WGT); };
        "OR.MUTE"     => { MATCH_OP(E_OP_OR_MUTE); };
        "OR.SCALE"    => { MATCH_OP(E_OP_OR_SCALE); };
        "OR.BANK"     => { MATCH_OP(E_OP_OR_BANK); };
        "OR.PRESET"   => { MATCH_OP(E_OP_OR_PRESET); };
        "OR.RELOAD"   => { MATCH_OP(E_OP_OR_RELOAD); };
        "OR.ROTS"     => { MATCH_OP(E_OP_OR_ROTS); };
        "OR.ROTW"     => { MATCH_OP(E_OP_OR_ROTW); };
        "OR.GRST"     => { MATCH_OP(E_OP_OR_GRST); };
        "OR.CVA"      => { MATCH_OP(E_OP_OR_CVA); };
        "OR.CVB"      => { MATCH_OP(E_OP_OR_CVB); };

        # MODS
        # controlflow
        "IF"          => { MATCH_MOD(E_MOD_IF); };
        "ELIF"        => { MATCH_MOD(E_MOD_ELIF); };
        "ELSE"        => { MATCH_MOD(E_MOD_ELSE); };
        "L"           => { MATCH_MOD(E_MOD_L); };

        # delay
        "PROB"        => { MATCH_MOD(E_MOD_PROB); };
        "DEL"         => { MATCH_MOD(E_MOD_DEL); };

        # stack
        "S"           => { MATCH_MOD(E_MOD_S); };
    *|;

    write data;          # write any ragel data here
}%%

// these are our macros that are inserted into the code when Ragel finds a match
#define MATCH_OP(op) { out->tag = OP; out->value = op; no_of_tokens++; }
#define MATCH_MOD(mod) { out->tag = MOD; out->value = mod; no_of_tokens++; }
#define MATCH_NUMBER()                       \
    {                                        \
        out->tag = NUMBER;                   \
        out->value = strtol(token, NULL, 0); \
        no_of_tokens++;                      \
    }

// matches a single token, out contains the token, return value indicates
// success or failure
bool match_token(const char *token, const size_t len, tele_data_t *out) {
    // required ragel declarations
    int cs;                       // machine state
    int act;                      // used with '=>'
    const char* ts;               // token start
    const char* te;               // token end

    const char* p = token;        // pointer to data
    const char* pe = token + len; // pointer to end of data
    const char* eof = pe;         // pointer to eof
    (void)match_token_en_main;    // fix unused variable warning

    int no_of_tokens = 0;

    %%{
        write init; # write initialisation
        write exec; # run the machine
    }%%

    // Ragel errors
    if (cs == match_token_error || cs < match_token_first_final) {
        return false;
    }
    // only 1 token!
    else if (no_of_tokens != 1) {
        return false;
    }
    else {
        return true;
    }

}

#ifndef INSTRUCTION_DEFINITION_H
#define INSTRUCTION_DEFINITION_H

#include <stdbool.h>
#include "instruction.h"
#include "expression_parser/expression_parser.h"

#define MAX_OPERANDS 2
#define MAX_FORMS    7

typedef struct{
    OperandKind operands[MAX_OPERANDS];
    int count;
    
    uint8_t opcode;
    bool use_register_src;
    bool use_register_or_flag_dest;
    bool is_opcode_2byte;
} OperandForm;

typedef struct
{
    int form_count;
    OperandForm forms[MAX_FORMS];
}InstructionDefinition;

const InstructionDefinition instruction_definition[] =
{
    // arithmetic instructions
    [ADD_mn] = {
        .form_count = 5,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .opcode                    = ADDr
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ADDm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ADDm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ADDm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ADDm | IMM_ADDR << 6
            }
        }
    },
    [AND_mn] = {
        .form_count = 5,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .opcode                    = ANDr
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ANDm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ANDm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ANDm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ANDm | IMM_ADDR << 6
            }
        }
    },
    [XOR_mn] = {
        .form_count = 5,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .opcode                    = XORr
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = XORm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = XORm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = XORm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = XORm | IMM_ADDR << 6
            }
        }
    },
    [OR_mn] = {
        .form_count = 5,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .opcode                    = ORr
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ORm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ORm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ORm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ORm | IMM_ADDR << 6
            }
        }
    },

    // arithmetic register instructions
    [ADDr_mn] = {
        .form_count = 1,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .opcode                    = ADDr
            }
        }
    },
    [ANDr_mn] = {
        .form_count = 1,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .opcode                    = ANDr
            }
        }
    },
    [XORr_mn] = {
        .form_count = 1,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .opcode                    = XORr
            }
        }
    },
    [ORr_mn]  = {
        .form_count = 1,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .opcode                    = ORr
            }
        }
    },

    // arithmetic memory instructions
    [ADDm_mn] = {
        .form_count = 4,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ADDm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ADDm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ADDm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ADDm | IMM_ADDR << 6
            }
        }
    },
    [ANDm_mn] = {
        .form_count = 4,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ANDm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ANDm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ANDm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = ANDm | IMM_ADDR << 6
            }
        }
    },
    [XORm_mn] = {
        .form_count = 4,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = XORm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = XORm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = XORm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = XORm | IMM_ADDR << 6
            }
        }
    },
    [ORm_mn]  = {
        .form_count = 4,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = ORm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = ORm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = ORm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = ORm | IMM_ADDR << 6
            }
        }
    },

    [SHL_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = SHL
            }
        }
    },
    [SHR_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = SHR
            }
        }
    },
    [ROL_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = ROL
            }
        }
    },
    [ROR_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = ROR
            }
        }
    },

    [NOT_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = NOT
            }
        }
    },
    [INC_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = INC
            }
        }
    },
    [DEC_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = DEC
            }
        }
    },
    [NEG_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode                    = NEG
            }
        }
    },

    // branch instructions
    [JMP_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = JMP << DIRECT_16
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = JMP << DIRECT_8
            }
        }
    },
    [BCS_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = BCS << DIRECT_16
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = BCS << DIRECT_8
            }
        }
    },
    [BZS_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode                    = BZS | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode                    = BZS | DIRECT_8 << 6
            }
        }
    },
    [BVS_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode                    = BSS | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode                    = BSS | DIRECT_8 << 6
            }
        }
    },
    [BCC_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode                    = BCC | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode                    = BCC | DIRECT_8 << 6
            }
        }
    },
    [BZC_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode                    = BZC | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode                    = BZC | DIRECT_8 << 6
            }
        }
    },
    [BVC_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = BSC << DIRECT_16
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = BSC << DIRECT_8
            }
        }
    },

    // function flow instructions
    [CALL_mn]  = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = CALL
            },
            {
                .operands = {OPERAND_FLAG, OPERAND_ADDR_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = CALL
            }
        }
    },
    [CALLn_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = CALLn
            },
            {
                .operands = {OPERAND_FLAG, OPERAND_ADDR_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = CALLn
            }
        }
    },
    [RET_mn] = {
        .form_count = 2,
        .forms = {
            {
                .count = 0,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = RET
            },
            {
                .operands = {OPERAND_FLAG},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = RET
            }
        }
    },
    [RETn_mn] = {
        .form_count = 2,
        .forms = {
            {
                .count = 0,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = RETn
            },
            {
                .operands = {OPERAND_FLAG},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = RETn
            }
        }
    },

    // flag manipulation
    [CLF_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_FLAG},
                .count = 1, 
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = CLF
            }
        }
    },
    [SEF_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_FLAG},
                .count = 1, 
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = SEF
            }
        }
    },

    [CCF_mn] = {
        .form_count = 1,
        .forms = {
            {
                .count = 0,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = CCF
            }
        }
    },
    [CZF_mn] = {
        .form_count = 1,
        .forms = {
            {
                .count = 0,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = CZF
            }
        }
    },
    [CVF_mn] = {
        .form_count = 1,
        .forms = {
            {
                .count = 0,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = CSF
            }
        }
    },
    [SCF_mn] = {
        .form_count = 1,
        .forms = {
            {
                .count = 0,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = SCF
            }
        }
    },
    [SZF_mn] = {
        .form_count = 1,
        .forms = {
            {
                .count = 0,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode               = SZF
            }
        }
    },
    [SVF_mn] = {
        .form_count = 1,
        .forms = {
            {
                .count = 0,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .opcode                    = SSF
            }
        }
    },

    [ST_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = ST | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_MAR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = ST | MAR << 6
            }
        }
    },
    [LD_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = LD | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_MAR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = LD | MAR << 6
            }
        }
    },
    [POP_mn] = {
        .form_count = 3,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = POP
            },
            // TODO:
            // {
            //     .operands = {OPERAND_REGISTER_8},
            //     .count = 1,
            //     
            // },
            // {
            //     .operands = {OPERAND_REGISTER_16},
            //     .addr_mode = NO_MODE,
            //     .count = 1
            // }
        }
    },
    [PUSH_mn] = {
        .form_count = 3,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .opcode               = PUSH
            },
            // {
            //     .operands = {OPERAND_REGISTER_8},
            //     .addr_mode = NO_MODE,
            //     .count = 1
            // },
            // {
            //     .operands = {OPERAND_REGISTER_16},
            //     .addr_mode = NO_MODE,
            //     .count = 1
            // }
        }
    },

    [MOV_mn] = {
        .form_count = 7,
        .forms = {
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .opcode                    = MOVr
            },
            {
                .operands = {OPERAND_GPR, OPERAND_REGISTER_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b00 << 4 | 0b0 << 6
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b00 << 4 | 0b0 << 6
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_REGISTER_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b00 << 4 | 0b0 << 6
            },
            {
                .operands = {OPERAND_REGISTER_16, OPERAND_REGISTER_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b00 << 4 | 0b1 << 6
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_IMM_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b11 << 4 | 0b0 << 6
            },
            {
                .operands = {OPERAND_REGISTER_16, OPERAND_IMM_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b11 << 4 | 0b1 << 6
            }
        }
    },
    [MOVr_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .is_opcode_2byte           = false,
                .opcode                    = MOVr
            }
        }
    },
    [MOVe_mn] = {
        .form_count = 7,
        .forms = {
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b00 << 4 | 0b0 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_REGISTER_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b00 << 4 | 0b0 << 6
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_GPR},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b00 << 4 | 0b0 << 6
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_REGISTER_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b00 << 4 | 0b0 << 6
            },
            {
                .operands = {OPERAND_REGISTER_16, OPERAND_REGISTER_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = true,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b00 << 4 | 0b1 << 6
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_IMM_8},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b11 << 4 | 0b0 << 6
            },
            {
                .operands = {OPERAND_REGISTER_16, OPERAND_IMM_16},
                .count = 2,
                .use_register_or_flag_dest = true,
                .use_register_src          = false,
                .is_opcode_2byte           = true,
                .opcode                    = MOVe | 0b11 << 4 | 0b1 << 6
            }
        }
    },

    [NOP_mn] = {
        .form_count = 1,
        .forms = {
            {
                .count = 0,
                .use_register_or_flag_dest = false,
                .use_register_src          = false,
                .is_opcode_2byte           = false,
                .opcode                    = MOVr // MOVr r0 r0
            }
        }
    },
};

#endif // INSTRUCTION_DEFINITION_H
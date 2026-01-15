typedef enum
{
    // arithmetic register
    ADDr = 0b0000,
    ANDr = 0b0001,
    ORr  = 0b0010,
    XORr = 0b0011,

    // arithmetic memory
    ADDm = 0b0100,
    ANDm = 0b0101,
    XORm = 0b0110,
    ORm  = 0b0111,

    // one operand instruction
    OOI = 0x8,

    // shift
    SH = 0x9,

    // memory
    LD = 0xa,
    ST = 0xb,

    // control flow
    JMP  = 0xc,
    JMPn = 0xd,

    // data transfer
    MOVr = 0xe,
    MOVe = 0xf,

    // edit flags
    EF = 0b10001111
} Opcodes;

// pseudo instructions
typedef enum
{   
    // arithmetic
	ADD_mn,
    ADDm_mn,
    ADDr_mn,
    
    AND_mn,
    ANDm_mn,
    ANDr_mn,
    
    OR_mn,
    ORm_mn,
    ORr_mn,

    XOR_mn,
    XORm_mn,
    XORr_mn,

    ROL_mn,
    ROR_mn,
    SHL_mn,
    SHR_mn,
    
    DEC_mn,
    INC_mn,
    NEG_mn,
    NOT_mn,

    
    // branch
    BCC_mn,
    BCS_mn,
    
    BSC_mn,
    BSS_mn,
    
    BZC_mn,
    BZS_mn,

    JMP_mn,

    // function calls
    CALL_mn,
    CALLn_mn,
    RET_mn,
    RETn_mn,
    
    // memory
    LD_mn,
    ST_mn,
    
    // stack
    POP_mn,
    PUSH_mn,

    // flag
    SEF_mn,
    CEF_mn,
    
    SCF_mn,
    CCF_mn,

    SZF_mn,
    CZF_mn,
    
    SSF_mn,
    CSF_mn,    

    // data transfer
    MOV_mn,
    MOVe_mn,
    MOVr_mn,
    
    NOP_mn,
} PseudoInstructions;

// sorted alphabeticly so binary sort can be used.
// sort with sort.c
typedef enum
{
	ADD_mn, ADDm_mn, ADDr_mn, AND_mn, ANDm_mn, ANDr_mn, BCC_mn, BCS_mn, BVC_mn, BVS_mn, BZC_mn, BZS_mn, CALL_mn, CALLn_mn, CCF_mn, CLF_mn, CVF_mn, CZF_mn, DEC_mn, INC_mn, JMP_mn, LD_mn, MOV_mn, MOVe_mn, MOVr_mn, NEG_mn, NOP_mn, NOT_mn, OR_mn, ORm_mn, ORr_mn, POP_mn, PUSH_mn, RET_mn, RETn_mn, ROL_mn, ROR_mn, SCF_mn, SEF_mn, SHL_mn, SHR_mn, ST_mn, SVF_mn, SZF_mn, XOR_mn, XORm_mn, XORr_mn
} PseudoInstructionsSorted;

typedef enum
{
    // register
    OPERAND_REGISTER,
    OPERAND_MAR,
    OPERAND_MBR,
    OPERAND_SP,
    OPERAND_FLAG_REG, // A flag and the flag register is very different -> look up the isa

    // addresses
    OPERAND_ADDR_8,
    OPERAND_ADDR_16,
    OPERAND_LABEL, // a label is a address where the size is unkown
    
    // flag is used for some branch and indirect modification of the flag register
    OPERAND_FLAG,

    OPERAND_IMM_8,
} OperandKind;

typedef enum
{

} AddressModes;
#pragma once

//#define CPU_DEBUG
//#define PPU_DEBUG
//#define PPU_WRITE_LOG

const int ADC_Imm = 0x69;
const int ADC_Zp = 0x65;
const int ADC_Zpx = 0x75;
const int ADC_Abs = 0x6D;
const int ADC_Absx = 0x7D;
const int ADC_Absy = 0x79;
const int ADC_Indx = 0x61;
const int ADC_Indy = 0x71;

const int AND_Imm = 0x29;
const int AND_Zp = 0x25;
const int AND_Zpx = 0x35;
const int AND_Abs = 0x2D;
const int AND_Absx = 0x3D;
const int AND_Absy = 0x39;
const int AND_Indx = 0x21;
const int AND_Indy = 0x31;

const int ASL_A = 0x0A;
const int ASL_Zp = 0x06;
const int ASL_Zpx = 0x16;
const int ASL_Abs = 0x0E;
const int ASL_Absx = 0x1E;

const int BCC = 0x90;
const int BCS = 0xB0;
const int BEQ = 0xF0;
const int BMI = 0x30;
const int BNE = 0xD0;
const int BPL = 0x10;
const int BVC = 0x50;
const int BVS = 0x70;

const int BIT_Abs = 0x2C;
const int BIT_Zp = 0x24;

const int BRK = 0x00;
const int CLC = 0x18;
const int CLD = 0xD8;
const int CLI = 0x58;
const int CLV = 0xB8;

const int CMP_Imm = 0xC9;
const int CMP_Zp = 0xC5;
const int CMP_Zpx = 0xD5;
const int CMP_Abs = 0xCD;
const int CMP_Absx = 0xDD;
const int CMP_Absy = 0xD9;
const int CMP_Indx = 0xC1;
const int CMP_Indy = 0xD1;

const int CPX_Imm = 0xE0;
const int CPX_Zp = 0xE4;
const int CPX_Abs = 0xEC;

const int CPY_Imm = 0xC0;
const int CPY_Zp = 0xC4;
const int CPY_Abs = 0xCC;

const int DCP_Zp = 0xC7;
const int DCP_Zpx = 0xD7;
const int DCP_Abs = 0xCF;
const int DCP_Absx = 0xDF;
const int DCP_Absy = 0xDB;
const int DCP_Indx = 0xC3;
const int DCP_Indy = 0xD3;

const int DEC_Zp = 0xC6;
const int DEC_Zpx = 0xD6;
const int DEC_Abs = 0xCE;
const int DEC_Absx = 0xDE;

const int DEX = 0xCA;
const int DEY = 0x88;

const int EOR_Imm = 0x49;
const int EOR_Zp = 0x45;
const int EOR_Zpx = 0x55;
const int EOR_Abs = 0x4D;
const int EOR_Absx = 0x5D;
const int EOR_Absy = 0x59;
const int EOR_Indx = 0x41;
const int EOR_Indy = 0x51;

const int INC_Zp = 0xE6;
const int INC_Zpx = 0xF6;
const int INC_Abs = 0xEE;
const int INC_Absx = 0xFE;

const int INX = 0xE8;
const int INY = 0xC8;

const int ISB_Zp = 0xE7;
const int ISB_Zpx = 0xF7;
const int ISB_Abs = 0xEF;
const int ISB_Absx = 0xFF;
const int ISB_Absy = 0xFB;
const int ISB_Indx = 0xE3;
const int ISB_Indy = 0xF3;

const int JMP_Abs = 0x4C;
const int JMP_Ind = 0x6C;

const int JSR = 0x20;

const int LAX_Zp = 0xA7;
const int LAX_Zpy = 0xB7;
const int LAX_Abs = 0xAF;
const int LAX_Absy = 0xBF;
const int LAX_Indx = 0xA3;
const int LAX_Indy = 0xB3;

const int LDA_Imm = 0xA9;
const int LDA_Zp = 0xA5;
const int LDA_Zpx = 0xB5;
const int LDA_Abs = 0xAD;
const int LDA_Absx = 0xBD;
const int LDA_Absy = 0xB9;
const int LDA_Indx = 0xA1;
const int LDA_Indy = 0xB1;

const int LDX_Imm = 0xA2;
const int LDX_Zp = 0xA6;
const int LDX_Zpy = 0xB6;
const int LDX_Abs = 0xAE;
const int LDX_Absy = 0xBE;

const int LDY_Imm = 0xA0;
const int LDY_Zp = 0xA4;
const int LDY_Zpx = 0xB4;
const int LDY_Abs = 0xAC;
const int LDY_Absx = 0xBC;

const int LSR_A = 0x4A;
const int LSR_Zp = 0x46;
const int LSR_Zpx = 0x56;
const int LSR_Abs = 0x4E;
const int LSR_Absx = 0x5E;

const int NOP = 0xEA;
const int nNOP_ONE_BYTE = 7;
const int NOP_ONE_BYTE[nNOP_ONE_BYTE] =
                             {0x1A,
			      0x3A,
			      0x5A,
			      0x7A,
			      0xDA,
			      0xEA,
			      0xFA};
const int nNOP_TWO_BYTES = 14;
const int NOP_TWO_BYTES[nNOP_TWO_BYTES] =
                             {0x04,
			      0x14,
			      0x34,
			      0x44,
			      0x54,
			      0x64,
			      0x74,
			      0x80,
			      0x82,
			      0x89,
			      0xC2,
			      0xD4,
			      0xE2,
			      0xF4};
const int nNOP_THREE_BYTES = 7;
const int NOP_THREE_BYTES[nNOP_THREE_BYTES] =
                             {0x0C,
			      0x1C,
			      0x3C,
			      0x5C,
			      0x7C,
			      0xDC,
			      0xFC};

const int ORA_Imm = 0x09;
const int ORA_Zp = 0x05;
const int ORA_Zpx = 0x15;
const int ORA_Abs = 0x0D;
const int ORA_Absx = 0x1D;
const int ORA_Absy = 0x19;
const int ORA_Indx = 0x01;
const int ORA_Indy = 0x11;

const int PHA = 0x48;
const int PHP = 0x08;
const int PLA = 0x68;
const int PLP = 0x28;

const int ROL_A = 0x2A;
const int ROL_Zp = 0x26;
const int ROL_Zpx = 0x36;
const int ROL_Abs = 0x2E;
const int ROL_Absx = 0x3E;

const int ROR_A = 0x6A;
const int ROR_Zp = 0x66;
const int ROR_Zpx = 0x76;
const int ROR_Abs = 0x6E;
const int ROR_Absx = 0x7E;

const int RTI = 0x40;
const int RTS = 0x60;

const int RLA_Zp = 0x27;
const int RLA_Zpx = 0x37;
const int RLA_Abs = 0x2F;
const int RLA_Absx = 0x3F;
const int RLA_Absy = 0x3B;
const int RLA_Indx = 0x23;
const int RLA_Indy = 0x33;

const int RRA_Zp = 0x67;
const int RRA_Zpx = 0x77;
const int RRA_Abs = 0x6F;
const int RRA_Absx = 0x7F;
const int RRA_Absy = 0x7B;
const int RRA_Indx = 0x63;
const int RRA_Indy = 0x73;

const int SAX_Zp = 0x87;
const int SAX_Zpy = 0x97;
const int SAX_Indx = 0x83;
const int SAX_Abs = 0x8F;

const int SBC_Imm = 0xE9;
const int SBC_Imm2 = 0xEB;
const int SBC_Zp = 0xE5;
const int SBC_Zpx = 0xF5;
const int SBC_Abs = 0xED;
const int SBC_Absx = 0xFD;
const int SBC_Absy = 0xF9;
const int SBC_Indx = 0xE1;
const int SBC_Indy = 0xF1;

const int SEC = 0x38;
const int SED = 0xF8;
const int SEI = 0x78;

const int SLO_Zp = 0x07;
const int SLO_Zpx = 0x17;
const int SLO_Abs = 0x0F;
const int SLO_Absx = 0x1F;
const int SLO_Absy = 0x1B;
const int SLO_Indx = 0x03;
const int SLO_Indy = 0x13;

const int SRE_Zp = 0x47;
const int SRE_Zpx = 0x57;
const int SRE_Abs = 0x4F;
const int SRE_Absx = 0x5F;
const int SRE_Absy = 0x5B;
const int SRE_Indx = 0x43;
const int SRE_Indy = 0x53;

const int STA_Zp = 0x85;
const int STA_Zpx = 0x95;
const int STA_Abs = 0x8D;
const int STA_Absx = 0x9D;
const int STA_Absy = 0x99;
const int STA_Indx = 0x81;
const int STA_Indy = 0x91;

const int STX_Zp = 0x86;
const int STX_Zpy = 0x96;
const int STX_Abs = 0x8E;

const int STY_Zp = 0x84;
const int STY_Zpx = 0x94;
const int STY_Abs = 0x8C;

const int SYA = 0x9C;

const int TAX = 0xAA;
const int TAY = 0xA8;
const int TSX = 0xBA;
const int TXA = 0x8A;
const int TXS = 0x9A;
const int TYA = 0x98;

// Interrupt Vectors

const int VECTOR_NMI = 0xFFFA;
const int VECTOR_RESET = 0xFFFC;
const int VECTOR_BRK = 0xFFFE;

// Size
const int KB = 1024;

// Cycles per opcode
// Doesn't take branches/page boudaries into count
const int cycleMap[] = {7, 8, 0, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			6, 6, 0, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			6, 6, 0, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
			2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			6, 6, 0, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
			2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
			2, 6, 0, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
			2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
			2, 5, 0, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
			2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7};

const int MIRRORING_HORIZONTAL = 0;
const int MIRRORING_VERTICAL = 1;
const int MIRRORING_LOWER_BANK = 2;
const int MIRRORING_UPPER_BANK = 3;

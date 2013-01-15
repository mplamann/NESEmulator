#include "CpuV2.h"
#include <iostream>
using namespace std;

//------------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------------

inline void setNZ(CpuV2* cpu, int value)
{
  value &= 0xFF;
  cpu->N = ((value & 0x80) != 0);
  cpu->Z = (value == 0);
}

//------------------------------------------------------------------------------
// Load / Store Operations
//------------------------------------------------------------------------------

void TAX(CpuV2* cpu, int argument)
{
  cpu->X = cpu->A;
  setNZ(cpu, cpu->X);
}

void TAY(CpuV2* cpu, int argument)
{
  cpu->Y = cpu->A;
  setNZ(cpu, cpu->Y);
}

void TSX(CpuV2* cpu, int argument)
{
  cpu->X = cpu->S;
  setNZ(cpu, cpu->X);
}

void TXS(CpuV2* cpu, int argument)
{
  cpu->S = cpu->X;
}

void TXA(CpuV2* cpu, int argument)
{
  cpu->A = cpu->X;
  setNZ(cpu, cpu->A);
}

void TYA(CpuV2* cpu, int argument)
{
  cpu->A = cpu->Y;
  setNZ(cpu, cpu->A);
}

void LAX(CpuV2* cpu, int argument)
{
  cpu->A = argument;
  cpu->X = argument;
  setNZ(cpu, cpu->A);
}

void LDA(CpuV2* cpu, int argument)
{
  cpu->A = argument;
  setNZ(cpu, cpu->A);
}

void LDX(CpuV2* cpu, int argument)
{
  cpu->X = argument;
  setNZ(cpu, cpu->X);
}

void LDY(CpuV2* cpu, int argument)
{
  cpu->Y = argument;
  setNZ(cpu, cpu->Y);
}

void STA(CpuV2* cpu, int argument)
{
  cpu->memory->writeByteTo(argument, cpu->A);
}

void STX(CpuV2* cpu, int argument)
{
  cpu->memory->writeByteTo(argument, cpu->X);
}

void STY(CpuV2* cpu, int argument)
{
  cpu->memory->writeByteTo(argument, cpu->Y);
}

//------------------------------------------------------------------------------
// Flag Operations
//------------------------------------------------------------------------------

void CLC(CpuV2* cpu, int argument) { cpu->C = false; }
void SEC(CpuV2* cpu, int argument) { cpu->C = true; }
void CLI(CpuV2* cpu, int argument) { cpu->I = false; }
void SEI(CpuV2* cpu, int argument) { cpu->I = true; }
void CLV(CpuV2* cpu, int argument) { cpu->V = false; }
void CLD(CpuV2* cpu, int argument) { cpu->D = false; }
void SED(CpuV2* cpu, int argument) { cpu->D = true; }

//------------------------------------------------------------------------------
// Stack Operations
//------------------------------------------------------------------------------

void PHA(CpuV2* cpu, int argument)
{
  pushToStack(cpu->A);
}

void PLA(CpuV2* cpu, int argument)
{
}

void PHP(CpuV2* cpu, int argument)
{
}

void PLP(CpuV2* cpu, int argument)
{
}

//------------------------------------------------------------------------------
// Arithmetic Operations
//------------------------------------------------------------------------------

// Helper functions
//-----------------
void compareWithRegister(CpuV2* cpu, int value, int reg)
{
  unsigned int temp = reg - value;
  cpu->C = (temp < 0x100);
  cpu->N = ((temp & 0x80) != 0);
  cpu->Z = (temp == 0);
}

void modMemory(CpuV2* cpu, int address, int dValue)
{
  int value = cpu->memory->readByteFrom(address);
  value += dValue;
  cpu->memory->writeByteTo(address, value);
  setNZ(cpu, value);
}

// Opcodes
//--------
void ADC(CpuV2* cpu, int argument)
{
  unsigned int temp = argument + cpu->A + (cpu->C ? 1 : 0);
  cpu->C = (temp > 0xFF);
  cpu->V = (!((cpu->A ^ argument) & 0x80) && ((cpu->A ^ temp) & 0x80));
  cpu->A = (temp & 0xFF);
  cpu->N = ((cpu->A & 0x80) != 0);
  cpu->Z = (cpu->A == 0);
}

void SBC(CpuV2* cpu, int argument)
{
  unsigned int temp = cpu->A - argument - (cpu->C ? 0 : 1);
  cpu->C = (temp < 0x100);
  cpu->V = (((cpu->A ^ argument) & 0x80) && ((cpu->A ^ temp) & 0x80));
  cpu->A = temp & 0xFF;
  cpu->N = ((cpu->A & 0x80) != 0);
  cpu->Z = (cpu->A == 0);
}

void CMP(CpuV2* cpu, int argument)
{
  compareWithRegister(cpu, argument, cpu->A);
}

void CPX(CpuV2* cpu, int argument)
{
  compareWithRegister(cpu, argument, cpu->X);
}

void CPY(CpuV2* cpu, int argument)
{
  compareWithRegister(cpu, argument, cpu->Y);
}

void DCP(CpuV2* cpu, int argument)
{
  modMemory(cpu, argument, -1);
  compareWithRegister(cpu, cpu->memory->readByteFrom(argument),cpu->A);
}

void DEC(CpuV2* cpu, int argument)
{
  modMemory(cpu, argument, -1);
}

void DEX(CpuV2* cpu, int argument)
{
  cpu->X--;
  cpu->X &= 0xFF;
  setNZ(cpu, cpu->X);
}

void DEY(CpuV2* cpu, int argument)
{
  cpu->Y--;
  cpu->Y &= 0xFF;
  setNZ(cpu, cpu->Y);
}

void INC(CpuV2* cpu, int argument)
{
  modMemory(cpu, argument, 1);
}

void INX(CpuV2* cpu, int argument)
{
  cpu->X++;
  cpu->X &= 0xFF;
  setNZ(cpu, cpu->X);
}

void INY(CpuV2* cpu, int argument)
{
  cpu->Y++;
  cpu->Y &= 0xFF;
  setNZ(cpu, cpu->Y);
}

void ISB(CpuV2* cpu, int argument)
{
  INC(cpu, argument);
  SBC(cpu, cpu->memory->readByteFrom(argument));
}

void RRA(CpuV2* cpu, int argument)
{
  cout << "RRA Unimplemented!\n"; // TODO: FIXME
}


//------------------------------------------------------------------------------
// ??????????
//------------------------------------------------------------------------------

void AND(CpuV2* cpu, int argument)
{
}

void ASL(CpuV2* cpu, int argument)
{
}

void BIT(CpuV2* cpu, int argument)
{
}

void BPL(CpuV2* cpu, int argument)
{
}

void BMI(CpuV2* cpu, int argument)
{
}

void BVC(CpuV2* cpu, int argument)
{
}

void BVS(CpuV2* cpu, int argument)
{
}

void BCC(CpuV2* cpu, int argument)
{
}

void BCS(CpuV2* cpu, int argument)
{
}

void BNE(CpuV2* cpu, int argument)
{
}

void BEQ(CpuV2* cpu, int argument)
{
}

void BRK(CpuV2* cpu, int argument)
{
}

void EOR(CpuV2* cpu, int argument)
{
}

void JMP(CpuV2* cpu, int argument)
{
}

void JSR(CpuV2* cpu, int argument)
{
}

void LSR(CpuV2* cpu, int argument)
{
}


void ORA(CpuV2* cpu, int argument)
{
}

void ROL(CpuV2* cpu, int argument)
{
}

void ROR(CpuV2* cpu, int argument)
{
}

void RTI(CpuV2* cpu, int argument)
{
}

void RTS(CpuV2* cpu, int argument)
{
}

void SLO(CpuV2* cpu, int argument)
{
}

void ANC(CpuV2* cpu, int argument)
{
}

void RLA(CpuV2* cpu, int argument)
{
}

void SRE(CpuV2* cpu, int argument)
{
}

void ALR(CpuV2* cpu, int argument)
{
}

void ARR(CpuV2* cpu, int argument)
{
}

void SAX(CpuV2* cpu, int argument)
{
}

void XAA(CpuV2* cpu, int argument)
{
}

void AHX(CpuV2* cpu, int argument)
{
}

void TAS(CpuV2* cpu, int argument)
{
}

void SHY(CpuV2* cpu, int argument)
{
}

void SHX(CpuV2* cpu, int argument)
{
}

void LAS(CpuV2* cpu, int argument)
{
}

void AXS(CpuV2* cpu, int argument)
{
}

void KIL(CpuV2*, int) {}
void NOP(CpuV2*, int) {}

//------------------------------------------------------------------------------
// Addressing Modes
// CAPS = returns address referred to by arguments
// lower = returns memory pointed to by that address
//------------------------------------------------------------------------------

inline int pageBoundaryCycles(int address, int offset)
{
  int newAddr = address + offset;
  if ((newAddr & 0xFF00) != (address & 0xFF00))
    return 1;
  return 0;
}

int imp(CpuV2* cpu, int, int) // Implied - doesn't take an argument
{
  cpu->PC += 1;
  return 0;
}

int imm(CpuV2* cpu, int arg1, int) // Immediate - argument is passed in as arg1
{
  cpu->PC += 2;
  return arg1;
}

int ZP(CpuV2* cpu, int arg1, int)
{
  cpu->PC += 2;
  return arg1;
}
int zp(CpuV2* cpu, int arg1, int arg2) {return cpu->memory->readByteFrom(ZP(cpu,arg1,arg2));}

int ZPX(CpuV2* cpu, int arg1, int)
{
  cpu->PC += 2;
  return (arg1 + cpu->X) & 0xFF;
}
int zpx(CpuV2* cpu, int arg1, int arg2) {return cpu->memory->readByteFrom(ZPX(cpu,arg1,arg2));}

int ZPY(CpuV2* cpu, int arg1, int)
{
  cpu->PC += 2;
  return (arg1 + cpu->Y) & 0xFF;
}
int zpy(CpuV2* cpu, int arg1, int arg2) {return cpu->memory->readByteFrom(ZPY(cpu,arg1,arg2));}

int jab(CpuV2* cpu, int arg1, int arg2) // JMP_Abs uses just the abs mem addr, not the value stored there.
{                                       // Otherwise, same as abs
  int addr = arg1 + (arg2 << 8);
  cpu->PC += 3;
  return addr;
}

int ABS(CpuV2* cpu, int arg1, int arg2)
{
  int addr = arg1 + (arg2 << 8);
  cpu->PC += 3;
  return addr;
}
int abs(CpuV2* cpu, int arg1, int arg2) {return cpu->memory->readByteFrom(ABS(cpu,arg1,arg2));}

int ABX(CpuV2* cpu, int arg1, int arg2)
{
  int addr = arg1 + (arg2 << 8);
  cpu->PC += 3;
  cpu->cycles += pageBoundaryCycles(addr, cpu->X);
  return (addr + cpu->X) & 0xFFFF;
}
int abx(CpuV2* cpu, int arg1, int arg2) {return cpu->memory->readByteFrom(ABX(cpu,arg1,arg2));}

int ABY(CpuV2* cpu, int arg1, int arg2)
{
  int addr = arg1 + (arg2 << 8);
  cpu->PC += 3;
  cpu->cycles += pageBoundaryCycles(addr, cpu->Y);
  return (addr + cpu->Y) & 0xFFFF;
}
int aby(CpuV2* cpu, int arg1, int arg2) {return cpu->memory->readByteFrom(ABY(cpu,arg1,arg2));}

int ind(CpuV2* cpu, int arg1, int arg2) // Used only in JMP_Ind
{
  int addrLSB = arg1 + (arg2 << 8);
  int addrMSB = ((arg1 + 1) & 0xFF) + (arg2 << 8);
  cpu->PC += 3;
  return cpu->memory->readByteFrom(addrLSB) + (cpu->memory->readByteFrom(addrMSB) << 8);
}

int INX(CpuV2* cpu, int arg1, int)
{
  int zpAddress = (arg1 + cpu->X) & 0xFF;
  int indirectAddress = cpu->memory->readByteFrom(zpAddress) + (cpu->memory->readByteFrom((zpAddress+1)%0x100)<<8);
  cpu->PC += 2;
  return indirectAddress;
}
int inx(CpuV2* cpu, int arg1, int arg2) {return cpu->memory->readByteFrom(INX(cpu,arg1,arg2));}

int INY(CpuV2* cpu, int arg1, int)
{
  int indirectAddress = cpu->memory->readByteFrom(arg1) + (cpu->memory->readByteFrom((arg1 + 1) & 0xFF) << 8);
  cpu->cycles += pageBoundaryCycles(indirectAddress, cpu->Y);
  cpu->PC += 2;
  return (indirectAddress + cpu->Y) & 0xFFFF;
}
int iny(CpuV2* cpu, int arg1, int arg2) {return cpu->memory->readByteFrom(INY(cpu,arg1,arg2));}

int acc(CpuV2* cpu, int, int)
{
  cpu->PC += 1;
  return cpu->A;
}
int rel(CpuV2* cpu, int arg1, int)
{
  cpu->PC += 2;
  int offset = arg1;
  if ((offset & 0x7F) != offset)
    offset = (0x7F & offset) - 0x80;
  return offset;
}

int (*addressingModes[256]) (CpuV2* cpu, int arg1, int arg2) =
{imp, inx, imp, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, abs, abs, abs, abs,
 rel, iny, imp, iny, zpx, zpx, zpx, zpx, imp, aby, imp, aby, abx, abx, abx, abx,
 abs, inx, imp, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, abs, abs, abs, abs,
 rel, iny, imp, iny, zpx, zpx, zpx, zpx, imp, aby, imp, aby, abx, abx, abx, abx,
 imp, inx, imp, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, jab, abs, abs, abs,
 rel, iny, imp, iny, zpx, zpx, zpx, zpx, imp, aby, imp, aby, abx, abx, abx, abx,
 imp, inx, imp, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, ind, ABS, abs, abs,
 rel, iny, imp, iny, zpx, ZPX, zpx, zpx, imp, ABY, imp, aby, abx, ABX, abx, abx,
 imm, INX, imm, inx, ZP,  ZP,  ZP,  zp,  imp, imm, acc, imm, ABS, abs, ABS, abs,
 rel, INY, imp, iny, ZPX, zpx, ZPY, zpy, imp, aby, acc, aby, abx, abx, aby, aby,
 imm, inx, imm, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, abs, abs, abs, abs,
 rel, iny, imp, iny, zpx, zpx, zpy, zpy, imp, aby, acc, aby, abx, abx, aby, aby,
 imm, inx, imm, INX, zp,  zp,  ZP,  ZP,  imp, imm, acc, imm, abs, abs, ABS, ABS,
 rel, iny, imp, INY, zpx, zpx, ZPX, ZPX, imp, aby, imp, ABY, abx, abx, ABX, ABX,
 imm, inx, imm, INX, zp,  zp,  ZP,  ZP,  imp, imm, imp, imm, abs, abs, ABS, ABS,
 rel, iny, imp, INY, zpx, zpx, ZPX, ZPX, imp, aby, imp, ABY, abx, abx, ABX, ABX};

void (*opcodes[256]) (CpuV2* cpu, int argument) =
{BRK, ORA, KIL, SLO, NOP, ORA, ASL, SLO, PHP, ORA, ASL, ANC, NOP, ORA, ASL, SLO,
 BPL, ORA, KIL, SLO, NOP, ORA, ASL, SLO, CLC, ORA, NOP, SLO, NOP, ORA, ASL, SLO,
 JSR, AND, KIL, RLA, BIT, AND, ROL, RLA, PLP, AND, ROL, ANC, BIT, AND, ROL, RLA,
 BMI, AND, KIL, RLA, NOP, AND, ROL, RLA, SEC, AND, NOP, RLA, NOP, AND, ROL, RLA,
 RTI, EOR, KIL, SRE, NOP, EOR, LSR, SRE, PHA, EOR, LSR, ALR, JMP, EOR, LSR, SRE,
 BVC, EOR, KIL, SRE, NOP, EOR, LSR, SRE, CLI, EOR, NOP, SRE, NOP, EOR, LSR, SRE,
 RTS, ADC, KIL, RRA, NOP, ADC, ROR, RRA, PLA, ADC, ROR, ARR, JMP, ADC, ROR, RRA,
 BVS, ADC, KIL, RRA, NOP, ADC, ROR, RRA, SEI, ADC, NOP, RRA, NOP, ADC, ROR, RRA,
 NOP, STA, NOP, SAX, STY, STA, STX, SAX, DEY, NOP, TXA, XAA, STY, STA, STX, SAX,
 BCC, STA, KIL, AHX, STY, STA, STX, SAX, TYA, STA, TXS, TAS, SHY, STA, SHX, AHX,
 LDY, LDA, LDX, LAX, LDY, LDA, LDX, LAX, TAY, LDA, TAX, LAX, LDY, LDA, LDX, LAX,
 BCS, LDA, KIL, LAX, LDY, LDA, LDX, LAX, CLV, LDA, TSX, LAS, LDY, LDA, LDX, LAX,
 CPY, CMP, NOP, DCP, CPY, CMP, DEC, DCP, INY, CMP, DEX, AXS, CPY, CMP, DEC, DCP,
 BNE, CMP, KIL, DCP, NOP, CMP, DEC, DCP, CLD, CMP, NOP, DCP, NOP, CMP, DEC, DCP,
 CPX, SBC, NOP, ISB, CPX, SBC, INC, ISB, INX, SBC, NOP, SBC, CPX, SBC, INC, ISB,
 BEQ, SBC, KIL, ISB, NOP, SBC, INC, ISB, SED, SBC, NOP, ISB, NOP, SBC, INC, ISB};

const int cycleMap[] =
  {7, 8, 0, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
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

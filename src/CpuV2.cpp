#include "CpuV2.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

const int VECTOR_NMI = 0xFFFA;
const int VECTOR_RESET = 0xFFFC;
const int VECTOR_BRK = 0xFFFE;

//------------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------------

inline void setNZ(CpuV2* cpu, int value)
{
  value &= 0xFF;
  cpu->N = ((value & 0x80) != 0);
  cpu->Z = (value == 0);
}

inline int pageBoundaryCycles(int address, int offset)
{
  int newAddr = address + offset;
  if ((newAddr & 0xFF00) != (address & 0xFF00))
    return 1;
  return 0;
}

//------------------------------------------------------------------------------
// Load / Store Operations
//------------------------------------------------------------------------------

void TAX(CpuV2* cpu, int)
{
  cpu->X = cpu->A;
  setNZ(cpu, cpu->X);
}

void TAY(CpuV2* cpu, int)
{
  cpu->Y = cpu->A;
  setNZ(cpu, cpu->Y);
}

void TSX(CpuV2* cpu, int)
{
  cpu->X = cpu->S;
  setNZ(cpu, cpu->X);
}

void TXS(CpuV2* cpu, int)
{
  cpu->S = cpu->X;
}

void TXA(CpuV2* cpu, int)
{
  cpu->A = cpu->X;
  setNZ(cpu, cpu->A);
}

void TYA(CpuV2* cpu, int)
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

void CLC(CpuV2* cpu, int) { cpu->C = false; }
void SEC(CpuV2* cpu, int) { cpu->C = true; }
void CLI(CpuV2* cpu, int) { cpu->I = false; }
void SEI(CpuV2* cpu, int) { cpu->I = true; }
void CLV(CpuV2* cpu, int) { cpu->V = false; }
void CLD(CpuV2* cpu, int) { cpu->D = false; }
void SED(CpuV2* cpu, int) { cpu->D = true; }

//------------------------------------------------------------------------------
// Boolean Operations
//------------------------------------------------------------------------------

void AND(CpuV2* cpu, int argument)
{
  cpu->A &= argument;
  setNZ(cpu, cpu->A);
}

void SAX(CpuV2* cpu, int argument)
{
  cpu->memory->writeByteTo(argument, cpu->A & cpu->X);
}

void EOR(CpuV2* cpu, int argument)
{
  cpu->A ^= argument;
  setNZ(cpu, cpu->A);
}

void ORA(CpuV2* cpu, int argument)
{
  cpu->A |= argument;
  setNZ(cpu, cpu->A);
}

void ASL(CpuV2* cpu, int argument)
{
  int value;
  if (argument == -1)
    value = cpu->A << 1;
  else
    value = cpu->memory->readByteFrom(argument) << 1;
  cpu->C = (value & 0x100);
  value &= 0xFF;
  if (argument == -1)
    cpu->A = value;
  else
    cpu->memory->writeByteTo(argument, value);
  setNZ(cpu, value);
}

void LSR(CpuV2* cpu, int argument)
{
  int value;
  if (argument == -1)
    value = cpu->A;
  else
    value = cpu->memory->readByteFrom(argument);
  cpu->C = value & 0x01;
  value = value >> 1;
  value &= 0x7F;
  if (argument == -1)
    cpu->A = value;
  else
    cpu->memory->writeByteTo(argument, value);
  setNZ(cpu, value);
}

void ROL(CpuV2* cpu, int argument)
{
  int value;
  if (argument == -1)
    value = cpu->A << 1;
  else
    value = cpu->memory->readByteFrom(argument) << 1;
  value += (int)cpu->C;
  cpu->C = (value > 0xFF);
  value &= 0xFF;
  if (argument == -1)
    cpu->A = value;
  else
    cpu->memory->writeByteTo(argument, value);
  setNZ(cpu, value);
}

void ROR(CpuV2* cpu, int argument)
{
  int value;
  if (argument == -1)
    value = cpu->A;
  else
    value = cpu->memory->readByteFrom(argument);
  if (cpu->C)
    value |= 0x100;
  cpu->C = (value & 0x01);
  value = value >> 1;
  value &= 0xFF;
  if (argument == -1)
    cpu->A = value;
  else
    cpu->memory->writeByteTo(argument, value);
  setNZ(cpu, value);
}

void SLO(CpuV2* cpu, int argument)
{
  ASL(cpu, argument);
  ORA(cpu, cpu->memory->readByteFrom(argument));
}

void RLA(CpuV2* cpu, int argument)
{
  ROL(cpu, argument);
  AND(cpu, cpu->memory->readByteFrom(argument));
}

void SRE(CpuV2* cpu, int argument)
{
  LSR(cpu, argument);
  EOR(cpu, cpu->memory->readByteFrom(argument));
}

void BIT(CpuV2* cpu, int argument)
{
  cpu->Z = ((cpu->A & argument) == 0);
  cpu->V = (0x40 & argument);
  cpu->N = (0x80 & argument);
}

//------------------------------------------------------------------------------
// Stack Operations
//------------------------------------------------------------------------------

// Helper functions
//-----------------

void pushToStack(CpuV2* cpu, int value)
{
  cpu->memory->writeByteTo(0x100 | (cpu->S-- & 0xFF), value);
}

int popFromStack(CpuV2* cpu)
{
  return cpu->memory->readByteFrom(0x100 | (++cpu->S & 0xFF));
}

// Opcodes
//--------

void PHA(CpuV2* cpu, int)
{
  pushToStack(cpu, cpu->A);
}

void PLA(CpuV2* cpu, int)
{
  cpu->A = popFromStack(cpu);
  setNZ(cpu, cpu->A);
}

void PHP(CpuV2* cpu, int)
{
  pushToStack(cpu, cpu->getP() | (1<<4));
}

void PLP(CpuV2* cpu, int)
{
  cpu->setP(popFromStack(cpu));
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

void DEX(CpuV2* cpu, int)
{
  cpu->X--;
  cpu->X &= 0xFF;
  setNZ(cpu, cpu->X);
}

void DEY(CpuV2* cpu, int)
{
  cpu->Y--;
  cpu->Y &= 0xFF;
  setNZ(cpu, cpu->Y);
}

void INC(CpuV2* cpu, int argument)
{
  modMemory(cpu, argument, 1);
}

void INX(CpuV2* cpu, int)
{
  cpu->X++;
  cpu->X &= 0xFF;
  setNZ(cpu, cpu->X);
}

void INY(CpuV2* cpu, int)
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

void RRA(CpuV2*, int)
{
  cout << "RRA Unimplemented!\n"; // TODO: FIXME
}

//------------------------------------------------------------------------------
// Branches and Jumps
//------------------------------------------------------------------------------

// Helper Functions
//-----------------

void doBranch(CpuV2* cpu, bool condition, int offset)
{
  if (!condition)
    return;
  cpu->cycles += pageBoundaryCycles(cpu->PC, offset);
  cpu->cycles++;
  cpu->PC += offset;
}

// Opcodes
//--------

void JMP(CpuV2* cpu, int argument)
{
  cpu->PC = argument;
}

void JSR(CpuV2* cpu, int argument)
{
  pushToStack(cpu, ((cpu->PC-1) >> 8) & 0xFF);
  pushToStack(cpu, (cpu->PC-1) & 0xFF);
  cpu->PC = argument;
}

void RTI(CpuV2* cpu, int)
{
  cpu->setP(popFromStack(cpu));
  cpu->PC = popFromStack(cpu) + (popFromStack(cpu) << 8);
}

void RTS(CpuV2* cpu, int)
{
  cpu->PC = popFromStack(cpu);
  cpu->PC += (popFromStack(cpu) << 8);
  cpu->PC++;
}

void BPL(CpuV2* cpu, int argument) { doBranch(cpu, !cpu->N, argument); }
void BMI(CpuV2* cpu, int argument) { doBranch(cpu, cpu->N, argument); }
void BVC(CpuV2* cpu, int argument) { doBranch(cpu, !cpu->V, argument); }
void BVS(CpuV2* cpu, int argument) { doBranch(cpu, cpu->V, argument); }
void BCC(CpuV2* cpu, int argument) { doBranch(cpu, !cpu->C, argument); }
void BCS(CpuV2* cpu, int argument) { doBranch(cpu, cpu->C, argument); }
void BNE(CpuV2* cpu, int argument) { doBranch(cpu, !cpu->Z, argument); }
void BEQ(CpuV2* cpu, int argument) { doBranch(cpu, cpu->Z, argument); }

//------------------------------------------------------------------------------
// Interrupt Handling
//------------------------------------------------------------------------------

// Helper functions
//-----------------

void processInterrupt(CpuV2* cpu, int vector)
{
  pushToStack(cpu, (cpu->PC >> 8) & 0xFF);
  pushToStack(cpu, cpu->PC & 0xFF);
  pushToStack(cpu, cpu->getP());
  cpu->I = true;
  cpu->PC = vector;
}

// Opcodes
//--------

void BRK(CpuV2* cpu, int)
{
  int vector = cpu->memory->readByteFrom(VECTOR_BRK) + (cpu->memory->readByteFrom(VECTOR_BRK+1) << 8);
  processInterrupt(cpu, vector);
}

//------------------------------------------------------------------------------
// ??????????
//------------------------------------------------------------------------------

void ANC(CpuV2*, int)
{
  cout << "ANC unimplemented.\n";
}

void ALR(CpuV2*, int)
{
  cout << "ALR unimplemented.\n";
}

void ARR(CpuV2*, int)
{
  cout << "ARR unimplemented.\n";
}

void XAA(CpuV2*, int)
{
  cout << "XAA unimplemented.\n";
}

void AHX(CpuV2*, int)
{
  cout << "AHX unimplemented.\n";
}

void TAS(CpuV2*, int)
{
  cout << "TAS unimplemented.\n";
}

void SHY(CpuV2*, int)
{
  cout << "SHY unimplemented.\n";
}

void SHX(CpuV2*, int)
{
  cout << "SHX unimplemented.\n";
}

void LAS(CpuV2*, int)
{
  cout << "LAS unimplemented.\n";
}

void AXS(CpuV2*, int)
{
  cout << "AXS unimplemented.\n";
}

void KIL(CpuV2*, int) { cout << "KIL called. Is that really what you want?\n"; }
void NOP(CpuV2*, int) {}

//------------------------------------------------------------------------------
// Addressing Modes
// CAPS = returns address referred to by arguments
// lower = returns memory pointed to by that address
//------------------------------------------------------------------------------

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

int ACC(CpuV2* cpu, int, int) { cpu->PC += 1; return -1; } // -1 is signal to use accumulator as memory address
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
{imp, inx, imp, INX, zp,  zp,  ZP,  ZP,  imp, imm, ACC, imm, abs, abs, ABS, ABS,
 rel, iny, imp, INY, zpx, zpx, ZPX, ZPX, imp, aby, imp, ABY, abx, abx, ABX, ABX,
 ABS, inx, imp, INX, zp,  zp,  ZP,  ZP,  imp, imm, ACC, imm, abs, abs, ABS, ABS,
 rel, iny, imp, INY, zpx, zpx, ZPX, ZPX, imp, aby, imp, ABY, abx, abx, ABX, ABX,
 imp, inx, imp, INX, zp,  zp,  ZP,  ZP,  imp, imm, ACC, imm, jab, abs, ABS, ABS,
 rel, iny, imp, INY, zpx, zpx, ZPX, ZPX, imp, aby, imp, ABY, abx, abx, ABX, ABX,
 imp, inx, imp, inx, zp,  zp,  ZP,  zp,  imp, imm, ACC, imm, ind, abs, ABS, abs,
 rel, iny, imp, iny, zpx, zpx, ZPX, zpx, imp, aby, imp, aby, abx, abx, ABX, abx,
 imm, INX, imm, INX, ZP,  ZP,  ZP,  ZP,  imp, imm, acc, imm, ABS, ABS, ABS, ABS,
 rel, INY, imp, iny, ZPX, ZPX, ZPY, ZPY, imp, ABY, acc, aby, abx, ABX, aby, aby,
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

const char* opcodeStrings[256] = 
{"BRK", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO", "PHP", "ORA", "ASL", "ANC", "NOP", "ORA", "ASL", "SLO",
 "BPL", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO", "CLC", "ORA", "NOP", "SLO", "NOP", "ORA", "ASL", "SLO",
 "JSR", "AND", "KIL", "RLA", "BIT", "AND", "ROL", "RLA", "PLP", "AND", "ROL", "ANC", "BIT", "AND", "ROL", "RLA",
 "BMI", "AND", "KIL", "RLA", "NOP", "AND", "ROL", "RLA", "SEC", "AND", "NOP", "RLA", "NOP", "AND", "ROL", "RLA",
 "RTI", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE", "PHA", "EOR", "LSR", "ALR", "JMP", "EOR", "LSR", "SRE",
 "BVC", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE", "CLI", "EOR", "NOP", "SRE", "NOP", "EOR", "LSR", "SRE",
 "RTS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA", "PLA", "ADC", "ROR", "ARR", "JMP", "ADC", "ROR", "RRA",
 "BVS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA", "SEI", "ADC", "NOP", "RRA", "NOP", "ADC", "ROR", "RRA",
 "NOP", "STA", "NOP", "SAX", "STY", "STA", "STX", "SAX", "DEY", "NOP", "TXA", "XAA", "STY", "STA", "STX", "SAX",
 "BCC", "STA", "KIL", "AHX", "STY", "STA", "STX", "SAX", "TYA", "STA", "TXS", "TAS", "SHY", "STA", "SHX", "AHX",
 "LDY", "LDA", "LDX", "LAX", "LDY", "LDA", "LDX", "LAX", "TAY", "LDA", "TAX", "LAX", "LDY", "LDA", "LDX", "LAX",
 "BCS", "LDA", "KIL", "LAX", "LDY", "LDA", "LDX", "LAX", "CLV", "LDA", "TSX", "LAS", "LDY", "LDA", "LDX", "LAX",
 "CPY", "CMP", "NOP", "DCP", "CPY", "CMP", "DEC", "DCP", "INY", "CMP", "DEX", "AXS", "CPY", "CMP", "DEC", "DCP",
 "BNE", "CMP", "KIL", "DCP", "NOP", "CMP", "DEC", "DCP", "CLD", "CMP", "NOP", "DCP", "NOP", "CMP", "DEC", "DCP",
 "CPX", "SBC", "NOP", "ISB", "CPX", "SBC", "INC", "ISB", "INX", "SBC", "NOP", "SBC", "CPX", "SBC", "INC", "ISB",
 "BEQ", "SBC", "KIL", "ISB", "NOP", "SBC", "INC", "ISB", "SED", "SBC", "NOP", "ISB", "NOP", "SBC", "INC", "ISB"};

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


void CpuV2::setP(int P)
{
  C = ((P & 0x01) != 0);
  Z = ((P & 0x02) != 0);
  I = ((P & 0x04) != 0);
  D = ((P & 0x08) != 0);
  V = ((P & 0x40) != 0);
  N = ((P & 0x80) != 0);
}

int CpuV2::getP()
{
  int P = 0x20;
  if (C)
    P |= 0x01;
  if (Z)
    P |= 0x02;
  if (I)
    P |= 0x04;
  if (B)
    P |= 0x10;
  if (V)
    P |= 0x40;
  if (N)
    P |= 0x80;
  if (D)
    P |= 0x08;
  return P;
}

void CpuV2::RunInstruction()
{
  int opcode = memory->readByteFrom(PC);
  int arg1 = memory->readByteFrom(PC+1);
  int arg2 = memory->readByteFrom(PC+2);

  //cout << setw(4) << PC << "  " << setw(2) << opcode << " " << setw(2) << arg1 << " " << setw(2) << arg2 << "  " << opcodeStrings[opcode] << "                             ";
  //cout << "A:" << setw(2) << A << " X:" << setw(2) << X << " Y:" << setw(2) << Y << " P:" << setw(2) << getP() << " SP:" << setw(2) << S << " 0x0678: " << memory->readByteFrom(0x0678);

  int argument = addressingModes[opcode](this, arg1, arg2);
  cycles += cycleMap[opcode];
  opcodes[opcode](this, argument);

  //cout << "\n";
}

void CpuV2::RunForCycles(float cycle_count, int)
{
  double fpart, ipart;
  fpart = modf(cycle_count, &ipart);
  accumulator += fpart;

  while (accumulator > 1)
    {
      accumulator--;
      cycle_count++;
    }
      
  total_cycles += cycle_count;
  cycles_remain += cycle_count;
  
  int lastCycle = cycles;
  while (cycles_remain > 0)
    {
      RunInstruction();
      int dCycles = cycles-lastCycle;
      cycles_remain -= dCycles;
      lastCycle = cycles;
    }
}

void CpuV2::doRESET()
{
  cycles += 6;
  I = true;
  PC = memory->readByteFrom(VECTOR_RESET) + (memory->readByteFrom(VECTOR_RESET+1) << 8);
}

void CpuV2::doNMI()
{
  cycles += 7;
  int vector = memory->readByteFrom(VECTOR_NMI) + (memory->readByteFrom(VECTOR_NMI+1) << 8);
  processInterrupt(this, vector);
}

void CpuV2::doBRK()
{
  cycles += 7;
  BRK(this, 0);
}

void CpuV2::doIRQ()
{
  cycles += 7;
  I = true;
  BRK(this, 0);
}

int CpuV2::elapsed()
{
  return total_cycles - cycles_remain;
}

char* CpuV2::stateData(size_t* size)
{
  int sizeOfRegs = sizeof(int) * 5;  // A,X,Y,S,PC
  int sizeOfFlags = sizeof(int) * 7; // N,C,Z,I,D,V,B
  int sizeOfCycleData = sizeof(int)*2 + sizeof(long); // cycles, total_cycles, cycles_remain
  int sizeOfAccumulator = sizeof(float);
  
  *size = sizeOfRegs + sizeOfFlags + sizeOfCycleData + sizeOfAccumulator;
  char* buffer = (char*)malloc(sizeof(char)*(*size));
  int bufferIndex = 0;
  
  int regs[5] = {A,X,Y,S,PC};
  int flags[7] = {N,C,Z,I,D,V,B};
  memcpy(buffer+bufferIndex, regs, sizeOfRegs);
  bufferIndex += sizeOfRegs;
  memcpy(buffer+bufferIndex, flags, sizeOfFlags);
  bufferIndex += sizeOfFlags;
  memcpy(buffer+bufferIndex, &cycles, sizeof(long));
  bufferIndex += sizeof(long);
  memcpy(buffer+bufferIndex, &total_cycles, sizeof(int));
  bufferIndex += sizeof(int);
  memcpy(buffer+bufferIndex, &cycles_remain, sizeof(int));
  bufferIndex += sizeof(int);
  memcpy(buffer+bufferIndex, &accumulator, sizeOfAccumulator);
  
  return buffer;
}

void CpuV2::loadState(char* buffer, size_t size)
{
  int sizeOfRegs = sizeof(int) * 5;  // A,X,Y,S,PC
  int sizeOfFlags = sizeof(int) * 7; // N,C,Z,I,D,V,B
  int sizeOfAccumulator = sizeof(float);
  int bufferIndex = 0;
  int regs[5];
  int flags[7];
  memcpy(regs, buffer+bufferIndex, sizeOfRegs);
  bufferIndex += sizeOfRegs;
  memcpy(flags, buffer+bufferIndex, sizeOfFlags);
  bufferIndex += sizeOfFlags;
  memcpy(&cycles, buffer+bufferIndex, sizeof(long));
  bufferIndex += sizeof(long);
  memcpy(&total_cycles, buffer+bufferIndex, sizeof(int));
  bufferIndex += sizeof(int);
  memcpy(&cycles_remain, buffer+bufferIndex, sizeof(int));
  bufferIndex += sizeof(int);
  memcpy(&accumulator, buffer+bufferIndex, sizeOfAccumulator);
  N = flags[0];
  C = flags[1];
  Z = flags[2];
  I = flags[3];
  D = flags[4];
  V = flags[5];
  B = flags[6];
  A = regs[0];
  X = regs[1];
  Y = regs[2];
  S = regs[3];
  PC = regs[4];
}

CpuV2::CpuV2(void)
{
  A = 0;
  X = 0;
  Y = 0;
  S = 0xFF;
  PC = 0;
  cycles = 0;
  total_cycles = 0;
  cycles_remain = 0;

  C = false;
  Z = false;
  I = true;
  D = false;
  B = false;
  V = false;
  N = false;
}

CpuV2::~CpuV2(void) {}

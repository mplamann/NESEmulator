#include "CpuV2.h"

//------------------------------------------------------------------------------
// Arithmetic Operations
//------------------------------------------------------------------------------
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

void compareWithRegister(CpuV2* cpu, int value, int reg)
{
  unsigned int temp = reg - value;
  cpu->C = (temp < 0x100);
  cpu->N = ((temp & 0x80) != 0);
  cpu->Z = (temp == 0);
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

void CMP(CpuV2* cpu, int argument)
{
}

void CPX(CpuV2* cpu, int argument)
{
}

void CPY(CpuV2* cpu, int argument)
{
}

void DEC(CpuV2* cpu, int argument)
{
}

void EOR(CpuV2* cpu, int argument)
{
}

void CLC(CpuV2* cpu, int argument)
{
}

void SEC(CpuV2* cpu, int argument)
{
}

void CLI(CpuV2* cpu, int argument)
{
}

void SEI(CpuV2* cpu, int argument)
{
}

void CLV(CpuV2* cpu, int argument)
{
}

void CLD(CpuV2* cpu, int argument)
{
}

void SED(CpuV2* cpu, int argument)
{
}

void INC(CpuV2* cpu, int argument)
{
}

void JMP(CpuV2* cpu, int argument)
{
}

void JSR(CpuV2* cpu, int argument)
{
}

void LDA(CpuV2* cpu, int argument)
{
}

void LDX(CpuV2* cpu, int argument)
{
}

void LDY(CpuV2* cpu, int argument)
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

void STA(CpuV2* cpu, int argument)
{
}

void TXS(CpuV2* cpu, int argument)
{
}

void TSX(CpuV2* cpu, int argument)
{
}

void PHA(CpuV2* cpu, int argument)
{
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

void STX(CpuV2* cpu, int argument)
{
}

void STY(CpuV2* cpu, int argument)
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

void RRA(CpuV2* cpu, int argument)
{
}

void ARR(CpuV2* cpu, int argument)
{
}

void SAX(CpuV2* cpu, int argument)
{
}

void DEY(CpuV2* cpu, int argument)
{
}

void TXA(CpuV2* cpu, int argument)
{
}

void XAA(CpuV2* cpu, int argument)
{
}

void AHX(CpuV2* cpu, int argument)
{
}

void TYA(CpuV2* cpu, int argument)
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

void LAX(CpuV2* cpu, int argument)
{
}

void TAY(CpuV2* cpu, int argument)
{
}

void TAX(CpuV2* cpu, int argument)
{
}

void LAS(CpuV2* cpu, int argument)
{
}

void INY(CpuV2* cpu, int argument)
{
}

void DEX(CpuV2* cpu, int argument)
{
}

void AXS(CpuV2* cpu, int argument)
{
}

void DCP(CpuV2* cpu, int argument)
{
}

void ISC(CpuV2* cpu, int argument)
{
}

void INX(CpuV2* cpu, int argument)
{
}

void KIL(CpuV2*, int) {}
void NOP(CpuV2*, int) {}

int imp(int arg1, int arg2);
int imm(int arg1, int arg2);
int zp(int arg1, int arg2);
int zpx(int arg1, int arg2);
int zpy(int arg1, int arg2);
int abs(int arg1, int arg2);
int abx(int arg1, int arg2);
int aby(int arg1, int arg2);
int ind(int arg1, int arg2);
int inx(int arg1, int arg2);
int iny(int arg1, int arg2);
int acc(int arg1, int arg2);
int rel(int arg1, int arg2);

int (*addressingModes[256]) (int arg1, int arg2) = {imp, inx, imp, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, abs, abs, abs, abs,
						    rel, iny, imp, iny, zpx, zpx, zpx, zpx, imp, aby, imp, aby, abx, abx, abx, abx,
						    abs, inx, imp, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, abs, abs, abs, abs,
						    rel, iny, imp, iny, zpx, zpx, zpx, zpx, imp, aby, imp, aby, abx, abx, abx, abx,
						    imp, inx, imp, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, abs, abs, abs, abs,
						    rel, iny, imp, iny, zpx, zpx, zpx, zpx, imp, aby, imp, aby, abx, abx, abx, abx,
						    imp, inx, imp, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, ind, abs, abs, abs,
						    rel, iny, imp, iny, zpx, zpx, zpx, zpx, imp, aby, imp, aby, abx, abx, abx, abx,
						    imm, inx, imm, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, abs, abs, abs, abs,
						    rel, iny, imp, iny, zpx, zpx, zpy, zpy, imp, aby, acc, aby, abx, abx, aby, aby,
						    imm, inx, imm, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, abs, abs, abs, abs,
						    rel, iny, imp, iny, zpx, zpx, zpy, zpy, imp, aby, acc, aby, abx, abx, aby, aby,
						    imm, inx, imm, inx, zp,  zp,  zp,  zp,  imp, imm, acc, imm, abs, abs, abs, abs,
						    rel, iny, imp, iny, zpx, zpx, zpx, zpx, imp, aby, imp, aby, abx, abx, abx, abx,
						    imm, inx, imm, inx, zp,  zp,  zp,  zp,  imp, imm, imp, imm, abs, abs, abs, abs,
						    rel, iny, imp, iny, zpx, zpx, zpx, zpx, imp, aby, imp, aby, abx, abx, abx, abx};

void (*opcodes[256]) (CpuV2* cpu, int argument) = {BRK, ORA, KIL, SLO, NOP, ORA, ASL, SLO, PHP, ORA, ASL, ANC, NOP, ORA, ASL, SLO,
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
						   CPX, SBC, NOP, ISC, CPX, SBC, INC, ISC, INX, SBC, NOP, SBC, CPX, SBC, INC, ISC,
						   BEQ, SBC, KIL, ISC, NOP, SBC, INC, ISC, SED, SBC, NOP, ISC, NOP, SBC, INC, ISC};

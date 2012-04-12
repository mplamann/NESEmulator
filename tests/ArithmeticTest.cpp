#include "CppUnitLite/TestHarness.h"
#include "CpuArithmetic.h"
#include "Util.h"

// ADC Instruction

TEST (CpuArithmetic, ADC_Imm)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Imm);
  mem->writeByteTo(1,0x38);
  cpu->setA(0x27);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x5F);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == false);
  CHECK(cpu->getV() == false);
}

TEST (CpuArithmetic, ADC_Zp)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Zp);
  mem->writeByteTo(1,0x38);
  mem->writeByteTo(0x38,0xF7);
  cpu->setA(0x27);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x1E);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == false);
}

TEST (CpuArithmetic, ADC_Zpx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Zpx);
  mem->writeByteTo(1,0x37);
  mem->writeByteTo(0x38,0xF6);
  cpu->setA(0x27);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x1D);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == false);
}

TEST (CpuArithmetic, ADC_Abs)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Abs);
  mem->writeByteTo(1,0x37);
  mem->writeByteTo(2,0x13);
  mem->writeByteTo(0x1337,0x26);
  cpu->setA(0x11);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x37);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == false);
  CHECK(cpu->getV() == false);
}

TEST (CpuArithmetic, ADC_Absx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Absx);
  mem->writeByteTo(1,0x37);
  mem->writeByteTo(2,0x13);
  mem->writeByteTo(0x1338,0xCB);
  cpu->setA(0x6E);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x39);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == false);
}

TEST (CpuArithmetic, ADC_Absy)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Absy);
  mem->writeByteTo(1,0x37);
  mem->writeByteTo(2,0x13);
  mem->writeByteTo(0x1338,0xFF);
  cpu->setA(0x1);
  cpu->setY(0x1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x00);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == true);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == false);
}

TEST (CpuArithmetic, ADC_Indx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Indx);
  mem->writeByteTo(1,0x36);
  mem->writeByteTo(0x37,0x25);
  mem->writeByteTo(0x38,0x15);
  mem->writeByteTo(0x1525,0xFF);
  cpu->setA(0x80);
  cpu->setX(0x1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x7F);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == true);
}

TEST (CpuArithmetic, ADC_Indy)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Indy);
  mem->writeByteTo(1,0x37);
  mem->writeByteTo(0x37,0x24);
  mem->writeByteTo(0x38,0x15);
  mem->writeByteTo(0x1525,127);
  cpu->setA(127);
  cpu->setY(0x1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 254);
  CHECK(cpu->getN() == true);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == false);
  CHECK(cpu->getV() == true);
}

// SBC Instruction

TEST (CpuArithmetic, SBC_Imm)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,SBC_Imm);
  mem->writeByteTo(2,0x36);
  cpu->setA(0x80);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4A);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == true);
}

TEST (CpuArithmetic, SBC_Zp)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,SBC_Zp);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(0x36,0x36);
  cpu->setA(0x80);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4A);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == true);
}

TEST (CpuArithmetic, SBC_Zpx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,SBC_Zpx);
  mem->writeByteTo(2,0x32);
  mem->writeByteTo(0x36,0x36);
  cpu->setA(0x80);
  cpu->setX(4);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4A);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == true);
}

TEST (CpuArithmetic, SBC_Abs)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,SBC_Abs);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(3,0x23);
  mem->writeByteTo(0x2336,0x36);
  cpu->setA(0x80);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4A);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == true);
}

TEST (CpuArithmetic, SBC_Absx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,SBC_Absx);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(3,0x23);
  mem->writeByteTo(0x2338,0x36);
  cpu->setA(0x80);
  cpu->setX(0x2);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4A);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == true);
}

TEST (CpuArithmetic, SBC_Absy)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,SBC_Absy);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(3,0x23);
  mem->writeByteTo(0x2369,0x36);
  cpu->setA(0x80);
  cpu->setY(0x33);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4A);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == true);
}

TEST (CpuArithmetic, SBC_Indx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,SBC_Indx);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(0x38,0x36);
  mem->writeByteTo(0x39,0x23);
  mem->writeByteTo(0x2336,0x36);
  cpu->setA(0x80);
  cpu->setX(0x2);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4A);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == true);
}

TEST (CpuArithmetic, SBC_Indy)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,SBC_Indy);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(0x36,0x23);
  mem->writeByteTo(0x37,0x22);
  mem->writeByteTo(0x2226,0x36);
  cpu->setA(0x80);
  cpu->setY(0x3);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4A);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getV() == true);
}

// DEC Instruction

TEST (CpuArithmetic, DEC_Zp)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,DEC_Zp);
  mem->writeByteTo(1,134);
  mem->writeByteTo(134,30);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(134) == 29);
  CHECK(!cpu->getN());
  CHECK(!cpu->getZ());
}

TEST (CpuArithmetic, DEC_Zpx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,DEC_Zpx);
  mem->writeByteTo(1,134);
  mem->writeByteTo(343,179);
  cpu->setX(209);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(343) == 178);
  CHECK(cpu->getN());
  CHECK(!cpu->getZ());
}

TEST (CpuArithmetic, DEC_Abs)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,DEC_Abs);
  mem->writeByteTo(1,0x3B);
  mem->writeByteTo(2,0xB9);
  mem->writeByteTo(0xB93B,0x3C);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0xB93B) == 0x3B);
  CHECK(!cpu->getN());
  CHECK(!cpu->getZ());
}

TEST (CpuArithmetic, DEC_Absx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,DEC_Absx);
  mem->writeByteTo(1,0x02);
  mem->writeByteTo(2,0x20);
  mem->writeByteTo(0x200B,1);
  cpu->setX(9);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x200B) == 0);
  CHECK(!cpu->getN());
  CHECK(cpu->getZ());
}

// DEX Instruction

TEST (CpuArithmetic, DEX)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,DEX);
  cpu->setX(23);
  cpu->RunInstruction();
  CHECK(cpu->getX() == 22);
  CHECK(!cpu->getN());
  CHECK(!cpu->getZ());
}

// DEY Instruction

TEST (CpuArithmetic, DEY)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,DEY);
  cpu->setY(0);
  cpu->RunInstruction();
  CHECK(cpu->getY() == 255);
  CHECK(cpu->getN());
  CHECK(!cpu->getZ());
}

// INC Instruction

TEST (CpuArithmetic, INC_Zp)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,INC_Zp);
  mem->writeByteTo(1,134);
  mem->writeByteTo(134,30);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(134) == 31);
  CHECK(!cpu->getN());
  CHECK(!cpu->getZ());
}

TEST (CpuArithmetic, INC_Zpx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,INC_Zpx);
  mem->writeByteTo(1,134);
  mem->writeByteTo(343,179);
  cpu->setX(209);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(343) == 180);
  CHECK(cpu->getN());
  CHECK(!cpu->getZ());
}

TEST (CpuArithmetic, INC_Abs)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,INC_Abs);
  mem->writeByteTo(1,0x3B);
  mem->writeByteTo(2,0xB9);
  mem->writeByteTo(0xB93B,0x7F);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0xB93B) == 0x80);
  CHECK(cpu->getN());
  CHECK(!cpu->getZ());
}

TEST (CpuArithmetic, INC_Absx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,INC_Absx);
  mem->writeByteTo(1,0x02);
  mem->writeByteTo(2,0x20);
  mem->writeByteTo(0x2007,255);
  cpu->setX(5);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x2007) == 0);
  CHECK(!cpu->getN());
  CHECK(cpu->getZ());
}

// INX Instruction

TEST (CpuArithmetic, INX)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,INX);
  cpu->setX(23);
  cpu->RunInstruction();
  CHECK(cpu->getX() == 24);
  CHECK(!cpu->getN());
  CHECK(!cpu->getZ());
}

// INY Instruction

TEST (CpuArithmetic, INY)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,INY);
  cpu->setY(255);
  cpu->RunInstruction();
  CHECK(cpu->getY() == 0);
  CHECK(!cpu->getN());
  CHECK(cpu->getZ());
}

// CMP Instruction
// I'm just going to declare that this works for CPX and CPY since they share the same code

TEST (CpuArithmetic, CMP_Imm)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,CMP_Imm);
  mem->writeByteTo(2,0x36);
  cpu->setA(0x80);
  cpu->RunInstruction();
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
}

TEST (CpuArithmetic, CMP_Zp)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,CMP_Zp);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(0x36,0x36);
  cpu->setA(0x80);
  cpu->RunInstruction();
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
}

TEST (CpuArithmetic, CMP_Zpx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,CMP_Zpx);
  mem->writeByteTo(2,0x32);
  mem->writeByteTo(0x36,0x36);
  cpu->setA(0x80);
  cpu->setX(4);
  cpu->RunInstruction();
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
}

TEST (CpuArithmetic, CMP_Abs)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,CMP_Abs);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(3,0x23);
  mem->writeByteTo(0x2336,0x36);
  cpu->setA(0x80);
  cpu->RunInstruction();
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
}

TEST (CpuArithmetic, CMP_Absx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,CMP_Absx);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(3,0x23);
  mem->writeByteTo(0x2338,0x36);
  cpu->setA(0x80);
  cpu->setX(0x2);
  cpu->RunInstruction();
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
}

TEST (CpuArithmetic, CMP_Absy)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,CMP_Absy);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(3,0x23);
  mem->writeByteTo(0x2369,0x36);
  cpu->setA(0x80);
  cpu->setY(0x33);
  cpu->RunInstruction();
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
}

TEST (CpuArithmetic, CMP_Indx)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,CMP_Indx);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(0x38,0x36);
  mem->writeByteTo(0x39,0x23);
  mem->writeByteTo(0x2336,0x36);
  cpu->setA(0x80);
  cpu->setX(0x2);
  cpu->RunInstruction();
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
}

TEST (CpuArithmetic, CMP_Indy)
{
  CpuArithmetic* cpu = new CpuArithmetic();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction(); // Clear the carry flag
  mem->writeByteTo(1,CMP_Indy);
  mem->writeByteTo(2,0x36);
  mem->writeByteTo(0x36,0x23);
  mem->writeByteTo(0x37,0x22);
  mem->writeByteTo(0x2226,0x36);
  cpu->setA(0x80);
  cpu->setY(0x3);
  cpu->RunInstruction();
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == true);
}

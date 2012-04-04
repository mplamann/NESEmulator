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

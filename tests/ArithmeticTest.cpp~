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
  mem->writeByteTo(0,ADC_Imm);
  mem->writeByteTo(1,0x38);
  mem->writeByteTo(0x38,0xF7);
  cpu->setA(0x27);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x1E);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
  CHECK(cpu->getC() == false);
  CHECK(cpu->getV() == false);
}

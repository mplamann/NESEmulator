#include "CppUnitLite/TestHarness.h"
#include "CpuRegisters.h"
#include "Util.h"

TEST(CpuRegisters,LDA_Imm)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDA_Imm);
  cpu->getMemory()->writeByteTo(1,42);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 42);
}

TEST(CpuRegisters,LDA_Zp)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDA_Zp);
  cpu->getMemory()->writeByteTo(1,42);
  cpu->getMemory()->writeByteTo(42,17);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 17);
}

TEST(CpuRegisters,LDA_Zpx)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDA_Zpx);
  cpu->getMemory()->writeByteTo(1,42);
  cpu->getMemory()->writeByteTo(43,17);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 17);
}

TEST(CpuRegisters,LDA_Abs)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDA_Abs);
  cpu->getMemory()->writeByteTo(1,0x39);
  cpu->getMemory()->writeByteTo(2,0x01);
  cpu->getMemory()->writeByteTo(0x0139,99);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 99);
}

TEST(CpuRegisters,LDA_Absx)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Absx);
  mem->writeByteTo(1,0x39);
  mem->writeByteTo(2,0x01);
  mem->writeByteTo(0x013A,99);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 99);
}

TEST(CpuRegisters,LDA_Absy)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Absy);
  mem->writeByteTo(1,0x29);
  mem->writeByteTo(2,0x01);
  mem->writeByteTo(0x012B,99);
  cpu->setY(2);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 99);
}

TEST(CpuRegisters,LDA_Indx)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Indx);
  mem->writeByteTo(1,0x17);
  mem->writeByteTo(0x19,0x45);
  mem->writeByteTo(0x20,0x01);
  mem->writeByteTo(0x0145,74);
  cpu->setX(2);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 74);
}

int main()
{
  TestResult tr;
  TestRegistry::runAllTests(tr);
  
  return 0;
}

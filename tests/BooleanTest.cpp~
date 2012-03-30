#include "CppUnitLite/TestHarness.h"
#include "CpuBoolean.h"
#include "Util.h"

TEST (BooleanTest,AND_Imm)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Imm);
  mem->writeByteTo(1,0xA);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Zp)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Zp);
  mem->writeByteTo(1,0x12);
  mem->writeByteTo(0x12,0xA);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Zpx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Zpx);
  mem->writeByteTo(1,0x11);
  mem->writeByteTo(0x12,0xA);
  cpu->setA(0xE);
  cpu->setX(0x01);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Abs)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Abs);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x01);
  mem->writeByteTo(0x010A, 0xF);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xE);
}

TEST (BooleanTest,AND_Absx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Absx);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x01);
  mem->writeByteTo(0x011A,0xA);
  cpu->setA(0xE);
  cpu->setX(0x10);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Absy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Absy);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x01);
  mem->writeByteTo(0x011A,0xA);
  cpu->setA(0xE);
  cpu->setY(0x10);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Indx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Indx);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(0xB,0xA);
  mem->writeByteTo(0xC,0x01);
  mem->writeByteTo(0x010A,0xA);
  cpu->setA(0xE);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Indy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Indy);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(0xA,0x01);
  mem->writeByteTo(0xB,0x02);
  mem->writeByteTo(0x0211,0xA);
  cpu->setA(0xE);
  cpu->setY(0x10);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}





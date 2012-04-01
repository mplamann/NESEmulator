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

TEST (BooleanTest,EOR_Imm)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Imm);
  mem->writeByteTo(1,0xA);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Zp)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Zp);
  mem->writeByteTo(1,0xAA);
  mem->writeByteTo(0xAA,0xA);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Zpx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Zpx);
  mem->writeByteTo(1,0xA9);
  mem->writeByteTo(0xAA,0xA);
  cpu->setA(0xE);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Abs)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Abs);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x41);
  mem->writeByteTo(0x410A, 0xA);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Absx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Absx);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x12);
  cpu->setA(0xE);
  cpu->setX(0xFF);
  mem->writeByteTo(0x1309,0xA);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Absy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Absy);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x12);
  cpu->setA(0xE);
  cpu->setY(0xFF);
  mem->writeByteTo(0x1309,0xA);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Indx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Indx);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(0xA,0x12);
  mem->writeByteTo(0xB,0x11);
  mem->writeByteTo(0xC,0x10);
  mem->writeByteTo(0x1011,0xA);
  cpu->setA(0xE);
  cpu->setX(0x1);
  mem->writeByteTo(0x1309,0xA);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Indy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Indy);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(0xA,0x12);
  mem->writeByteTo(0xB,0x17);
  mem->writeByteTo(0x1713,0xA);
  cpu->setA(0xE);
  cpu->setY(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest, ORA_Imm)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Imm);
  mem->writeByteTo(1,0xD);
  cpu->setA(3);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xF);
}

TEST (BooleanTest, ORA_Zp)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Zp);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(0xD,0x9);
  cpu->setA(3);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Zpx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Zpx);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(0xE,0x9);
  cpu->setA(3);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Abs)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Abs);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(2,0x42);
  mem->writeByteTo(0x420D,0x9);
  cpu->setA(3);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Absx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Absx);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(2,0x42);
  mem->writeByteTo(0x420F,0x9);
  cpu->setA(3);
  cpu->setX(2);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Absy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Absy);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(2,0x42);
  mem->writeByteTo(0x420E,0x9);
  cpu->setA(3);
  cpu->setY(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Indx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Indx);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(0xE,0xD);
  mem->writeByteTo(0xF,0x42);
  mem->writeByteTo(0x420D,0x9);
  cpu->setA(3);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Indy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Indy);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(0xD,0x0C);
  mem->writeByteTo(0xE,0x42);
  mem->writeByteTo(0x420D,0x9);
  cpu->setA(3);
  cpu->setY(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}



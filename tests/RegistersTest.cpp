#include "CppUnitLite/TestHarness.h"
#include "CpuRegisters.h"
#include "Util.h"

#include <iostream>
using namespace std;

// LDA Instructions

TEST(CpuRegisters,LDA_Imm)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDA_Imm);
  cpu->getMemory()->writeByteTo(1,0xF1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xF1);
  CHECK(cpu->getN());
  CHECK(!cpu->getZ());
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
  mem->writeByteTo(1,0x3E);
  mem->writeByteTo(0x43,0x15);
  mem->writeByteTo(0x44,0x24);
  mem->writeByteTo(0x2415,74);
  cpu->setX(5);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 74);
}

TEST (CpuRegisters,LDA_Indy)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Indy);
  mem->writeByteTo(1,0xF2);
  mem->writeByteTo(0xF2,0x11);
  mem->writeByteTo(0XF3,0x22);
  mem->writeByteTo(0x2222,23);
  cpu->setY(0x2222-0x2211);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 23);
}

// LDX Instructions

TEST(CpuRegisters,LDX_Imm)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDX_Imm);
  cpu->getMemory()->writeByteTo(1,0xF1);
  cpu->RunInstruction();
  CHECK(cpu->getX() == 0xF1);
  CHECK(cpu->getN());
  CHECK(!cpu->getZ());
}

TEST(CpuRegisters,LDX_Zp)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDX_Zp);
  cpu->getMemory()->writeByteTo(1,42);
  cpu->getMemory()->writeByteTo(42,17);
  cpu->RunInstruction();
  CHECK(cpu->getX() == 17);
}

TEST(CpuRegisters,LDX_Zpy)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDX_Zpy);
  cpu->getMemory()->writeByteTo(1,42);
  cpu->getMemory()->writeByteTo(43,17);
  cpu->setY(1);
  cpu->RunInstruction();
  CHECK(cpu->getX() == 17);
}

TEST(CpuRegisters,LDX_Abs)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDX_Abs);
  cpu->getMemory()->writeByteTo(1,0x39);
  cpu->getMemory()->writeByteTo(2,0x01);
  cpu->getMemory()->writeByteTo(0x0139,99);
  cpu->RunInstruction();
  CHECK(cpu->getX() == 99);
}

TEST(CpuRegisters,LDX_Absy)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDX_Absy);
  mem->writeByteTo(1,0x29);
  mem->writeByteTo(2,0x01);
  mem->writeByteTo(0x012B,99);
  cpu->setY(2);
  cpu->RunInstruction();
  CHECK(cpu->getX() == 99);
}

// LDX Instructions

TEST(CpuRegisters,LDY_Imm)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDY_Imm);
  cpu->getMemory()->writeByteTo(1,0xF1);
  cpu->RunInstruction();
  CHECK(cpu->getY() == 0xF1);
  CHECK(cpu->getN());
  CHECK(!cpu->getZ());
}

TEST(CpuRegisters,LDY_Zp)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDY_Zp);
  cpu->getMemory()->writeByteTo(1,42);
  cpu->getMemory()->writeByteTo(42,17);
  cpu->RunInstruction();
  CHECK(cpu->getY() == 17);
}

TEST(CpuRegisters,LDY_Zpy)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDY_Zpx);
  cpu->getMemory()->writeByteTo(1,42);
  cpu->getMemory()->writeByteTo(43,17);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getY() == 17);
}

TEST(CpuRegisters,LDY_Abs)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,LDY_Abs);
  cpu->getMemory()->writeByteTo(1,0x39);
  cpu->getMemory()->writeByteTo(2,0x01);
  cpu->getMemory()->writeByteTo(0x0139,99);
  cpu->RunInstruction();
  CHECK(cpu->getY() == 99);
}

TEST(CpuRegisters,LDY_Absy)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDY_Absx);
  mem->writeByteTo(1,0x29);
  mem->writeByteTo(2,0x01);
  mem->writeByteTo(0x012B,99);
  cpu->setX(2);
  cpu->RunInstruction();
  CHECK(cpu->getY() == 99);
}


// Transfer Instructions

TEST (CpuRegisters,TAX)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,TAX);
  cpu->setA(45);
  cpu->RunInstruction();
  CHECK(cpu->getX() == 45);
}

TEST (CpuRegisters,TAY)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,TAY);
  cpu->setA(45);
  cpu->RunInstruction();
  CHECK(cpu->getY() == 45);
}

TEST (CpuRegisters,TSX)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,TSX);
  cpu->setS(45);
  cpu->RunInstruction();
  CHECK(cpu->getX() == 45);
}

TEST (CpuRegisters,TXA)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,TXA);
  cpu->setX(245);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 245);
}

TEST (CpuRegisters,TXS)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,TXS);
  cpu->setX(245);
  cpu->RunInstruction();
  CHECK(cpu->getS() == 245);
}

TEST (CpuRegisters,TYA)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,TYA);
  cpu->setY(245);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 245);
}

// STA Instructions

TEST (CpuRegisters,STA_Zp)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,STA_Zp);
  mem->writeByteTo(1,0x47);
  cpu->setA(95);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x47) == 95);
}

TEST (CpuRegisters,STA_Zpx)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,STA_Zpx);
  mem->writeByteTo(1,0x47);
  cpu->setA(95);
  cpu->setX(0x10);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x57) == 95);
}

TEST (CpuRegisters,STA_Abs)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,STA_Abs);
  mem->writeByteTo(1,0x47);
  mem->writeByteTo(2,0x03);
  cpu->setA(84);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x0347) == 84);
}

TEST (CpuRegisters,STA_Absx)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,STA_Absx);
  mem->writeByteTo(1,0x47);
  mem->writeByteTo(2,0x05);
  cpu->setA(95);
  cpu->setX(0x03);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x054A) == 95);
}

TEST (CpuRegisters,STA_Absy)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,STA_Absy);
  mem->writeByteTo(1,0x47);
  mem->writeByteTo(2,0x15);
  cpu->setA(42);
  cpu->setY(12);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x1547+12) == 42);
}

TEST (CpuRegisters,STA_Indx)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,STA_Indx);
  mem->writeByteTo(1,0x47);
  mem->writeByteTo(0x48,0x12);
  mem->writeByteTo(0x49,0x09);
  cpu->setA(95);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x0912) == 95);
}

TEST (CpuRegisters,STA_Indy)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,STA_Indy);
  mem->writeByteTo(1,0x47);
  mem->writeByteTo(0x47,0x15);
  mem->writeByteTo(0x48,0x03);
  cpu->setA(95);
  cpu->setY(12);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x0315+12) == 95);
}

// STX Instructions

TEST (CpuRegisters,STX_Zp)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,STX_Zp);
  mem->writeByteTo(1,42);
  cpu->setX(121);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(42) == 121);
}

TEST (CpuRegisters,STX_Zpy)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,STX_Zpy);
  mem->writeByteTo(1,42);
  cpu->setX(121);
  cpu->setY(22);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(64) == 121);
}

TEST (CpuRegisters,STX_Abs)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,STX_Abs);
  mem->writeByteTo(1,0x42);
  mem->writeByteTo(2,0x11);
  cpu->setX(121);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x1142) == 121);
}

// Flag Instructions

TEST (CpuRegisters,SEC)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,SEC);
  cpu->RunInstruction();
  CHECK(cpu->getC());
}

TEST (CpuRegisters,SED)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,SED);
  cpu->RunInstruction();
  CHECK(cpu->getD());
}

TEST (CpuRegisters,SEI)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,SEI);
  cpu->RunInstruction();
  CHECK(cpu->getI());
}

TEST (CpuRegisters,CLC)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,CLC);
  cpu->RunInstruction();
  CHECK(!cpu->getC());
}

TEST (CpuRegisters,CLD)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,CLD);
  cpu->RunInstruction();
  CHECK(!cpu->getD());
}

TEST (CpuRegisters,CLI)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,CLI);
  cpu->RunInstruction();
  CHECK(!cpu->getI());
}

TEST (CpuRegisters,CLV)
{
  CpuRegisters* cpu = new CpuRegisters();
  cpu->getMemory()->writeByteTo(0,CLV);
  cpu->RunInstruction();
  CHECK(!cpu->getV());
}

TEST (CpuRegisters,PHA)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,PHA);
  cpu->setA(23);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x01FF) == 23);
}

TEST (CpuRegisters,PHP)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,PHP);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x01FF) == 0x20);
}

TEST (CpuRegisters,PLA)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,PLA);
  mem->writeByteTo(0x01FF,23);
  cpu->setS(0x01FE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 23);
}

TEST (CpuRegisters,PLP)
{
  CpuRegisters* cpu = new CpuRegisters();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,PLP);
  mem->writeByteTo(0x01FF,0xF0);
  cpu->setS(0x01FE);
  cpu->RunInstruction();
  CHECK(cpu->getN());
  CHECK(cpu->getV());
  CHECK(cpu->getB());
}

int main()
{
  TestResult tr;
  TestRegistry::runAllTests(tr);
  return 0;
}

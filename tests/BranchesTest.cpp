#include "CppUnitLite/TestHarness.h"
#include "CpuBranches.h"
#include "Util.h"

// BCC Instruction

TEST (CpuBranches, BCCNotTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC); // Set C to true
  cpu->RunInstruction();
  mem->writeByteTo(1,BCC);
  mem->writeByteTo(2,0x10);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 3);
}

TEST (CpuBranches, BCCTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,CLC); // Set C to false
  cpu->RunInstruction();
  mem->writeByteTo(1,BCC);
  mem->writeByteTo(2,0x10);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x13);
}

// BCS Instruction
// Second verse, same as the first! (But flipped around)

TEST (CpuBranches, BCSNotTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,CLC);
  cpu->RunInstruction();
  mem->writeByteTo(1,BCS);
  mem->writeByteTo(2,0x10);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 3);
}

TEST (CpuBranches, BCSTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,SEC);
  cpu->RunInstruction();
  mem->writeByteTo(1,BCS);
  mem->writeByteTo(2,0x10);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x13);
}

// BEQ Instruction

TEST (CpuBranches, BEQTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Imm);
  mem->writeByteTo(1,0); // Gotta set the zero flag somehow, right?
  cpu->RunInstruction();
  mem->writeByteTo(2,BEQ);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0);
}

TEST (CpuBranches, BEQNotTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Imm);
  mem->writeByteTo(1,47);
  cpu->RunInstruction();
  mem->writeByteTo(2,BEQ);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x4);
}

#include "CppUnitLite/TestHarness.h"
#include "CpuBranches.h"
#include "Util.h"

#include <iostream>
using namespace std;

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

// BMI Insturction

TEST (CpuBranches, BMITaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Imm);
  mem->writeByteTo(1,0xF2); // Gotta set the zero flag somehow, right?
  cpu->RunInstruction();
  mem->writeByteTo(2,BMI);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0);
}

TEST (CpuBranches, BMINotTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Imm);
  mem->writeByteTo(1,47);
  cpu->RunInstruction();
  mem->writeByteTo(2,BMI);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x4);
}

// BNE Insturction

TEST (CpuBranches, BNETaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Imm);
  mem->writeByteTo(1,47); // Gotta set the zero flag somehow, right?
  cpu->RunInstruction();
  mem->writeByteTo(2,BNE);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0);
}

TEST (CpuBranches, BNENotTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Imm);
  mem->writeByteTo(1,0);
  cpu->RunInstruction();
  mem->writeByteTo(2,BNE);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x4);
}

// BPL Insturction

TEST (CpuBranches, BPLTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Imm);
  mem->writeByteTo(1,023); // Gotta set the zero flag somehow, right?
  cpu->RunInstruction();
  mem->writeByteTo(2,BPL);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0);
}

TEST (CpuBranches, BPLNotTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LDA_Imm);
  mem->writeByteTo(1,0x85);
  cpu->RunInstruction();
  mem->writeByteTo(2,BPL);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x4);
}

// BVC Insturction

TEST (CpuBranches, BVCTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Imm);
  mem->writeByteTo(1,0x0F); // Gotta set the zero flag somehow, right?
  cpu->RunInstruction();
  mem->writeByteTo(2,BVC);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0);
}

TEST (CpuBranches, BVCNotTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Imm);
  mem->writeByteTo(1,0x7F);
  cpu->setA(0x7F);
  cpu->RunInstruction();
  mem->writeByteTo(2,BVC);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x4);
}

// BVS Instruction
TEST (CpuBranches, BVSNotTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Imm);
  mem->writeByteTo(1,0x0F); // Gotta set the zero flag somehow, right?
  cpu->RunInstruction();
  mem->writeByteTo(2,BVS);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x4);
}

TEST (CpuBranches, BVSTaken)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ADC_Imm);
  mem->writeByteTo(1,0x7F);
  cpu->setA(0x7F);
  cpu->RunInstruction();
  mem->writeByteTo(2,BVS);
  mem->writeByteTo(3,0xFC);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x0);
}

// JMP Instruction

TEST (BranchesTest, JMP_Abs)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,JMP_Abs);
  mem->writeByteTo(1,0x27);
  mem->writeByteTo(2,0x42);
  mem->writeByteTo(0x4227,0x21);
  mem->writeByteTo(0x4228,0x07);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x4227);
}

TEST (BranchesTest, JMP_Ind)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,JMP_Ind);
  mem->writeByteTo(1,0x27);
  mem->writeByteTo(2,0x42);
  mem->writeByteTo(0x4227,0x21);
  mem->writeByteTo(0x4228,0x07);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x0721);
}

// JSR Instruction

TEST (BranchesTest, JSR)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,JSR);
  mem->writeByteTo(1,0x27);
  mem->writeByteTo(2,0x42);
  mem->writeByteTo(0x4227,0x21);
  mem->writeByteTo(0x4228,0x07);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x4227);
  CHECK(mem->readByteFrom(0x01FE) == 2);
}

// RTS Instruction

TEST (BranchesTest, RTS)
{
  CpuBranches* cpu = new CpuBranches();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,RTS);
  mem->writeByteTo(0x01FF,0x07);
  mem->writeByteTo(0x01FE,0x23);
  cpu->setS(0x01FD);
  cpu->RunInstruction();
  CHECK(cpu->getPC() == 0x0724);
}

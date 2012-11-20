#include "CpuState.h"

#define CPU_DEBUG

#include <iostream>
#include <iomanip>
using namespace std;

CpuState::CpuState(void)
{
}


CpuState::~CpuState(void)
{
}

const char* nameForOpcode(int opcode)
{
  switch (opcode)
    {
    case ADC_Imm:
    case ADC_Zp:
    case ADC_Zpx:
    case ADC_Abs:
    case ADC_Absx:
    case ADC_Absy:
    case ADC_Indx:
    case ADC_Indy:
      return "ADC";
    case AND_Imm:
    case AND_Zp:
    case AND_Zpx:
    case AND_Abs:
    case AND_Absx:
    case AND_Absy:
    case AND_Indx:
    case AND_Indy:
      return "AND";
      
    case ASL_A:
    case ASL_Zp:
    case ASL_Zpx:
    case ASL_Abs:
    case ASL_Absx:
      return "ASL";

    case BCC:
      return "BCC";
    case BCS:
      return "BCS";
    case BEQ:
      return "BEQ";
    case BMI:
      return "BMI";
    case BNE:
      return "BNE";
    case BPL:
      return "BPL";
    case BVC:
      return "BVC";
    case BVS:
      return "BVS";

    case BIT_Abs:
    case BIT_Zp:
      return "BIT";

    case BRK:
      return "BRK";
    case CLC:
      return "CLC";
    case CLD:
      return "CLD";
    case CLI:
      return "CLI";
    case CLV:
      return "CLV";

    case CMP_Imm:
    case CMP_Zp:
    case CMP_Zpx:
    case CMP_Abs:
    case CMP_Absx:
    case CMP_Absy:
    case CMP_Indx:
    case CMP_Indy:
      return "CMP";

    case CPX_Imm:
    case CPX_Zp:
    case CPX_Abs:
      return "CPX";

    case CPY_Imm:
    case CPY_Zp:
    case CPY_Abs:
      return "CPY";

    case DCP_Zp:
    case DCP_Zpx:
    case DCP_Abs:
    case DCP_Absx:
    case DCP_Absy:
    case DCP_Indx:
    case DCP_Indy:
      return "DCP";
      
    case DEC_Zp:
    case DEC_Zpx:
    case DEC_Abs:
    case DEC_Absx:
      return "DEC";

    case DEX:
      return "DEX";
    case DEY:
      return "DEY";

    case EOR_Imm:
    case EOR_Zp:
    case EOR_Zpx:
    case EOR_Abs:
    case EOR_Absx:
    case EOR_Absy:
    case EOR_Indx:
    case EOR_Indy:
      return "EOR";

    case INC_Zp:
    case INC_Zpx:
    case INC_Abs:
    case INC_Absx:
      return "INC";

    case INX:
      return "INX";
    case INY:
      return "INY";

    case ISB_Zp:
    case ISB_Zpx:
    case ISB_Abs:
    case ISB_Absx:
    case ISB_Absy:
    case ISB_Indx:
    case ISB_Indy:
      return "ISB";
      
    case JMP_Abs:
    case JMP_Ind:
      return "JMP";

    case JSR:
      return "JSR";

    case LAX_Zp:
    case LAX_Zpy:
    case LAX_Abs:
    case LAX_Absy:
    case LAX_Indx:
    case LAX_Indy:
      return "LAX";
      
    case LDA_Imm:
    case LDA_Zp:
    case LDA_Zpx:
    case LDA_Abs:
    case LDA_Absx:
    case LDA_Absy:
    case LDA_Indx:
    case LDA_Indy:
      return "LDA";

    case LDX_Imm:
    case LDX_Zp:
    case LDX_Zpy:
    case LDX_Abs:
    case LDX_Absy:
      return "LDX";

    case LDY_Imm:
    case LDY_Zp:
    case LDY_Zpx:
    case LDY_Abs:
    case LDY_Absx:
      return "LDY";

    case LSR_A:
    case LSR_Zp:
    case LSR_Zpx:
    case LSR_Abs:
    case LSR_Absx:
      return "LSR";

    case NOP:
      return "NOP";

    case ORA_Imm:
    case ORA_Zp:
    case ORA_Zpx:
    case ORA_Abs:
    case ORA_Absx:
    case ORA_Absy:
    case ORA_Indx:
    case ORA_Indy:
      return "ORA";

    case PHA:
      return "PHA";
    case PHP:
      return "PHP";
    case PLA:
      return "PLA";
    case PLP:
      return "PLP";

    case ROL_A:
    case ROL_Zp:
    case ROL_Zpx:
    case ROL_Abs:
    case ROL_Absx:
      return "ROL";

    case ROR_A:
    case ROR_Zp:
    case ROR_Zpx:
    case ROR_Abs:
    case ROR_Absx:
      return "ROR";

    case RTI:
      return "RTI";
    case RTS:
      return "RTS";

    case RLA_Zp:
    case RLA_Zpx:
    case RLA_Abs:
    case RLA_Absx:
    case RLA_Absy:
    case RLA_Indx:
    case RLA_Indy:
      return "RLA";

    case RRA_Zp:
    case RRA_Zpx:
    case RRA_Abs:
    case RRA_Absx:
    case RRA_Absy:
    case RRA_Indx:
    case RRA_Indy:
      return "RRA";

    case SAX_Zp:
    case SAX_Zpy:
    case SAX_Indx:
    case SAX_Abs:
      return "SAX";

    case SBC_Imm:
    case SBC_Imm2:
    case SBC_Zp:
    case SBC_Zpx:
    case SBC_Abs:
    case SBC_Absx:
    case SBC_Absy:
    case SBC_Indx:
    case SBC_Indy:
      return "SBC";

    case SEC:
      return "SEC";
    case SED:
      return "SED";
    case SEI:
      return "SEI";

    case SLO_Zp:
    case SLO_Zpx:
    case SLO_Abs:
    case SLO_Absx:
    case SLO_Absy:
    case SLO_Indx:
    case SLO_Indy:
      return "SLO";

    case SRE_Zp:
    case SRE_Zpx:
    case SRE_Abs:
    case SRE_Absx:
    case SRE_Absy:
    case SRE_Indx:
    case SRE_Indy:
      return "SRE";
      
    case STA_Zp:
    case STA_Zpx:
    case STA_Abs:
    case STA_Absx:
    case STA_Absy:
    case STA_Indx:
    case STA_Indy:
      return "STA";

    case STX_Zp:
    case STX_Zpy:
    case STX_Abs:
      return "STX";

    case STY_Zp:
    case STY_Zpx:
    case STY_Abs:
      return "STY";

      //    case SYA:
      //      return "SYA";  Turns out that I don't need to implement this to get Megaman running
      
    case TAX:
      return "TAX";
    case TAY:
      return "TAY";
    case TSX:
      return "TSX";
    case TXA:
      return "TXA";
    case TXS:
      return "TXS";
    case TYA:
      return "TYA";
    }
  return "???";
}

#ifdef CPU_DEBUG
void printVariable(char* text, int value)
{
  cout << " " << text << ": " << setw(2) << value;
}
#endif

bool CpuState::RunInstruction()
{
  bool success = false;
#ifdef CPU_DEBUG
  int opcode = memory->readByteFrom(PC);
  int arg1 = memory->readByteFrom(PC+1);
  int arg2 = memory->readByteFrom(PC+2);

  int lastPC = PC;

  cout << setw(4) << PC << "  " << setw(2) << opcode << " " << setw(2) << arg1 << " " << setw(2) << arg2 << "  " << nameForOpcode(opcode) << "                             ";
  cout << "A:" << setw(2) << A << " X:" << setw(2) << X << " Y:" << setw(2) << Y << " P:" << setw(2) << getP() << " SP:" << setw(2) << S;

  cout << "\n";

#endif
  if (super::RunInstruction())
    success = true;
#ifdef CPU_DEBUG
  if (success == false)
    {
      //      cout << "Opcode " << opcode << " unimplemented.";
      PC++; // Roll with it
    }
#endif
  if (!success)
    {
      cout << "Error - unknown opcode.\n";
    }
  return success;
}

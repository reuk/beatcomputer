#include "instruction_manager.h"

#include "instruction_structs.h"

#include <memory>

using namespace std;

InstructionManager::InstructionManager()
    : r_instructions({
          make_shared<Instruction__ADD>(), make_shared<Instruction__AND>(),
          make_shared<Instruction__DIV>(), make_shared<Instruction__JR>(),
          make_shared<Instruction__MFHI>(), make_shared<Instruction__MFLO>(),
          make_shared<Instruction__MULT>(), make_shared<Instruction__NOR>(),
          make_shared<Instruction__XOR>(), make_shared<Instruction__OR>(),
          make_shared<Instruction__SLT>(), make_shared<Instruction__SLL>(),
          make_shared<Instruction__SRL>(), make_shared<Instruction__SUB>(),
      })
    , i_instructions({
          make_shared<Instruction__ADDI>(), make_shared<Instruction__ANDI>(),
          make_shared<Instruction__BEQ>(), make_shared<Instruction__BNE>(),
          make_shared<Instruction__LW>(), make_shared<Instruction__LUI>(),
          make_shared<Instruction__ORI>(), make_shared<Instruction__SLTI>(),
          make_shared<Instruction__SW>(),
      })
    , j_instructions({
          make_shared<Instruction__J>(), make_shared<Instruction__JAL>(),
      }) {
}

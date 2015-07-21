#include "instruction_manager.h"

#include "instruction_set.h"

#include <memory>

using namespace std;

InstructionManager::InstructionManager()
    : fu_instructions({
        make_shared<NOP    >(),
        make_shared<ADD_R  >(),
        make_shared<SUB_R  >(),
        make_shared<MUL_R  >(),
        make_shared<DIV_R  >(),
        make_shared<BAND_R >(),
        make_shared<BOR_R  >(),
        make_shared<BNOT_R >(),
        make_shared<BXOR_R >(),
        make_shared<AND_R  >(),
        make_shared<OR_R   >(),
        make_shared<NOT_R  >(),
        make_shared<MOVE_R >(),
        make_shared<OSC_R  >(),
        make_shared<RND    >(),
      })
    , op_instructions({
        make_shared<ADD_I  >(),
        make_shared<SUB_I  >(),
        make_shared<MUL_I  >(),
        make_shared<DIV_I  >(),
        make_shared<BAND_I >(),
        make_shared<BOR_I  >(),
        make_shared<BNOT_I >(),
        make_shared<BXOR_I >(),
        make_shared<AND_I  >(),
        make_shared<OR_I   >(),
        make_shared<NOT_I  >(),
        make_shared<MOVE_I >(),
        make_shared<OSC_I  >(),
        make_shared<LOAD   >(),
        make_shared<SAVE   >(),
        make_shared<JUMP   >(),
        make_shared<JE     >(),
        make_shared<JNE    >(),
        make_shared<JL     >(),
        make_shared<JLE    >(),
        make_shared<JG     >(),
        make_shared<JGE    >(),
      }) {
}



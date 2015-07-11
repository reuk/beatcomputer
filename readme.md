Assembly Live Coding Machine
============================

Todo
----

* random number generation
* ncurses mode needs an editor
* immediate-mode instructions should be generated for instructions that use
  constants
    * shouldn't have to specify immediate mode, it should be deduced
* simplify instruction set
* write special parse functions where they're needed
* unit tests!
    * check that math instructions work properly
        * they might do just unsigned math whereas I want just signed probably
* add float support
* enable/disable tooltip mode
* labels (symbol table)
* hook up osc or midi support
* add syscall or special register for note output
* tempo sync

Done
----

* some centralised way of adding instructions (parse rules, opcodes, execution)
    * to add an instruction, add a specialisation of
      `Instruction[RIJ]Descriptor` (defined in instruction_descriptor.h), then
      register it in the constructor of `InstructionManager` (defined in
      instruction_manager.cpp).
* disassembler
* ncurses interface with visualiser for registers
    * each core should have
        * directly editable
            * raw hex instruction column
            * labels column
            * mnemonic instructions column
        * static
            * tooltips column (can be disabled/enabled I guess)
            * register status column
    * each core gets maybe 64 words of memory

Maybe
-----

* limit number of usable registers
* overflow trap
* comments - end of line only

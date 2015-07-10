Assembly Live Coding Machine
============================

Todo
----

* disassembler
* unit tests!
    * check that math instructions work properly
        * they might do just unsigned math whereas I want just signed probably
* add float support
* ncurses interface with visualiser for registers
* labels (symbol table)
* comments
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

Maybe
-----

* limit number of usable registers
* overflow trap

Assembly Live Coding Machine
============================

Todo
----

* ncurses mode needs an editor
* finish writing unit tests!
* synch instructions
* modulo instruction?
* extra tooling
    * linting
    * static analysis `scan-build`
    * fuzzing? `afl-fuzz`
* NOP invalid instructions
* re-highlight executed lines

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
* hook up osc or midi support
* tempo sync
* random number generation
* immediate-mode instructions should be generated for instructions that use
  constants
    * shouldn't have to specify immediate mode, it should be deduced
* simplify instruction set
* add syscall or special register for note output
* add ability for playing notes through midi/os
* reimplement tooltips
* write special parse functions where they're needed
* write special disassemble functions where they're needed
* keypresses over osc
* some 'execute' callback handler that changes highlighted row
* uninitialized memory (from program file) should be zeroed/NOPed
* display memory in editor
* move cursor properly at line-ends

Maybe
-----

* limit number of usable registers
* overflow trap
* comments - end of line only
* add float support
* labels (symbol table)
* enable/disable tooltip mode


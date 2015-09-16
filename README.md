# gbdisasm
Gameboy Disassembler

Disassembles Gameboy ROM files into their LR35902 opcodes and corresponding immediates. Doesn't distinquish between op-codes and data.

Based on: http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html

$ make
$ ./gbdisasm <your_gb_file>.gb > asm.output

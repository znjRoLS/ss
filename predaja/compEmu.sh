echo "Compiling..."
#bin/compiler tests/badOperands.asm tests/badOperands.o
#bin/compiler tests/badInstruction.asm tests/badInstruction.o
#bin/compiler tests/badFormat2.asm tests/badFormat2.o
#bin/compiler tests/badFormat.asm tests/badFormat.o
#bin/compiler tests/badDirective.asm tests/badDirective.o
#bin/compiler tests/testRelocation1.asm tests/testRelocation1.o
#bin/compiler tests/testRelocation2.asm tests/testRelocation2.o
#bin/compiler tests/testScript.asm tests/testScript.o
bin/compiler tests/testIntr.asm tests/testIntr.o
#bin/compiler tests/testEmu1.asm tests/testEmu1.o
#bin/compiler tests/testEmu2.asm tests/testEmu2.o
#bin/compiler tests/testEmu3.asm tests/testEmu3.o
echo "Compiled"
echo "Emulating..."
#bin/emulator tests/testEmpty.script tests/testRelocation1.o tests/testRelocation2.o
#bin/emulator tests/testScript.script tests/testScript.o
bin/emulator tests/testEmpty.script tests/testIntr.o
#bin/emulator tests/testEmpty.script tests/testEmu1.o tests/testEmu2.o tests/testEmu3.o
# CHIP-8_emulator

## About the project

**My first emulator!** I am trying to organize my projects better and use unit tests to feel more confident when adding features to my code, so this is my first project with this level of organization.

## How to build it

You just need to install CMake and GCC. After that, follow these instructions:

1. Give executable permission to the script: `chmod +x build_src.sh`
2. Run the script: `./build_src.sh`
3. When you run this script, you can choose to build the tests or the program. And voilà, you have built my project!

## How to run it

To run the emulator, use the following base command:

```bash
./chip8Emulator <rom bin> <op per sec>
```

What is `<op per sec>`? This parameter controls how many instructions the emulator will execute per second. If you leave this field blank, the default configuration will be used (700 operations per second).

## What I still want to do

[ ] Implement a keypad visualizer
[ ] Add debug options (breakpoints and pause execution)


## References 

Those are some references that i use to create my project, if you want to do a CHIP-8 emulator too, see thoses articles and pages.

- To understand how a CHIP-8 emulator works, see this [guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/).
- To get a deeper understanding of the functionality of certain instructions, see this [page](https://chip8.gulrak.net/)!
- As a bonus, although it is not specifically about CHIP-8, this [article](https://cliutils.gitlab.io/modern-cmake/) teaches you how to use CMake (it helped me a lot).
- This online [CHIP-8 emulator](https://chipo.ber.gp/) shows the disassembly of a ROM, which helps me debug my code.
- To test my emulator, these [ROMs](https://github.com/Timendus/chip8-test-suite) by Timendus were very useful.

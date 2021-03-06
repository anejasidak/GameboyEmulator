#ifndef CONFIG_H_
#define CONFIG_H_

#define WINDOW_TITLE "Gameboy Emulator"

// View Information

#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 144
#define DISPLAY_SCALE 4

// Memory Information
#define CARTRIDGE_SIZE 0x200000
#define MEMORY_SIZE 0x10000

#define ROM_BANK_0_START 0x0000
#define ROM_BANK_0_END 0x3FFF

#define ROM_BANK_1_START 0x4000
#define ROM_BANK_1_END 0x7FFF

#define WRAM_BANK_0_START 0xC000
#define WRAM_BANK_0_END 0xCFFF

#define WRAM_BANK_1_START 0xD000
#define WRAM_BANK_1_END 0xDFFF

#define ECHO_MEM_START 0xE000
#define ECHO_MEM_END 0xFDFF

#define RESTRICTED_RAM_START 0xFEA0
#define RESTRICTED_RAM_END 0xFEFF

// Cpu information
#define FLAGS_ZERO (1 << 7)
#define FLAGS_NEGATIVE (1 << 6)
#define FLAGS_HALFCARRY (1 << 5)
#define FLAGS_CARRY (1 << 4)

// Header information
#define HEADER_START_ADDR 0x0104
#define HEADER_END_ADDR 0x0133

#endif
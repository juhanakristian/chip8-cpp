#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdint.h>

void disassemble_chip8(uint8_t *codebuffer, int pc) {
  uint8_t *code = &codebuffer[pc];

  uint8_t firstbyte = code[0];
  uint8_t lastbyte = code[1];

  uint8_t firstword = (firstbyte >> 4);

  std::cout << std::hex << std::setfill('0') << std::setw(4) << pc << " "
            << std::setw(2) << static_cast<int>(firstbyte) << " " << std::setw(2)
            << static_cast<int>(lastbyte) << " ";


  switch (firstword) {
  case 0x00:
    switch (lastbyte) {
    case 0xE0: {
      // 00E0
      // CLS
      // Clear the screen
      std::cout << "CLS";
    } break;
    case 0xEE: {
      // 00EE
      // RTS
      // Return from subroutine
      std::cout << "RTS";
    } break;
    }
    break;
  case 0x01: {
    // JUMP $NNN
    // Jump to address NNN
    uint8_t addresshi = firstbyte & 0x0F;
    std::cout << "JUMP #$" << std::setw(1) << static_cast<int>(addresshi)
              << std::setw(2) << static_cast<int>(lastbyte);
  } break;
  case 0x02: {
    // CALL $NNN
    // Jump to subroutine at NNN
    uint8_t addresshi = firstbyte & 0x0F;
    std::cout << "CALL #$" << std::setw(1) << static_cast<int>(addresshi)
              << std::setw(2) << static_cast<int>(lastbyte);
  } break;
  case 0x03: {
    // SKIP.EQ VX, #$NN
    // Skip the next instruction if VX equals NN
    uint8_t addresshi = firstbyte & 0x0F;
    std::cout << "SKIP.EQ V" << std::setw(1) << static_cast<int>(addresshi)
              << ", #$" << std::setw(2) << static_cast<int>(lastbyte);
  } break;
  case 0x04: {
    // SKIP.NQ VX, #$NN
    // Skip the next instruction if VX doesn't equal NN
    uint8_t addresshi = firstbyte & 0x0F;
    std::cout << "SKIP.NQ V" << std::setw(1) << static_cast<int>(addresshi)
              << ", #$" << std::setw(2) << static_cast<int>(lastbyte);
  } break;
  case 0x05: {
    // SKIP.EQ VX, VY
    // Skip the next instruction if VX equals VY
    uint8_t firstaddress = firstbyte & 0x0F;
    uint8_t secondaddress = (lastbyte & 0xF0) >> 4;
    std::cout << "SKIP.EQ V" << std::setw(1) << static_cast<int>(firstaddress)
              << ", V" << std::setw(1) << static_cast<int>(secondaddress);
  } break;
  case 0x06: {
    // MVI VX,NN
    // Sets register VX to NN
    uint8_t reg = firstbyte & 0x0F;
    std::cout << "MVI V" << std::setw(1) << static_cast<int>(reg) << ",#$"
              << std::setw(2) << static_cast<int>(lastbyte);
  } break;
  case 0x07: {
    // ADD VX, #$NN
    // Add NN to value in register VX
    uint8_t reg = firstbyte & 0x0F;
    std::cout << "ADD V" << std::setw(1) << static_cast<int>(reg) << ",#$"
              << std::setw(2) << static_cast<int>(lastbyte);
  } break;
  case 0x08: {
    // 8XYN have 9 different operations using two registers
    uint8_t operation = lastbyte & 0x0F;
    uint8_t regx = firstbyte & 0x0F;
    uint8_t regy = (lastbyte & 0xF0) >> 4;
    switch (operation) {
    case 0x00: {
      // MOV VX, VY
      // Set VX to the value of VY
      std::cout << "MOV V" << std::setw(1) << static_cast<int>(regx) << ", V"
                << std::setw(1) << static_cast<int>(regy);
    } break;
    case 0x01: {
      // OR VX, VY
      // Set VX to VX OR VY
      std::cout << "OR V" << std::setw(1) << static_cast<int>(regx) << ", V"
                << std::setw(1) << static_cast<int>(regy);
    } break;
    case 0x02: {
      // AND VX, VY
      // Set VX to VX AND VY
      std::cout << "AND V" << std::setw(1) << static_cast<int>(regx) << ", V"
                << std::setw(1) << static_cast<int>(regy);
    } break;
    case 0x03: {
      // XOR VX, VY
      // Set VX to VX XOR VY
      std::cout << "XOR V" << std::setw(1) << static_cast<int>(regx) << ", V"
                << std::setw(1) << static_cast<int>(regy);
    } break;
    case 0x04: {
      // ADD. VX, VY
      // Add VX to VY, VF will be set to 1 if there's a carry and to 0 if
      // there's not
      std::cout << "ADD. V" << std::setw(1) << static_cast<int>(regx) << ", V"
                << std::setw(1) << static_cast<int>(regy);
    } break;
    case 0x05: {
      // SUB. VX, VY
      // Substract VY from VX. VF is set to 0 if there's a borrow, and to 1 if
      // there's not.
      std::cout << "SUB. V" << std::setw(1) << static_cast<int>(regx) << ", V"
                << std::setw(1) << static_cast<int>(regy);
    } break;
    case 0x06: {
      // SHR. VX
      // Shift VX right by one. VF is set to the value of th least significant
      // bit of VX before the shift.
      std::cout << "SHR. V" << std::setw(1) << static_cast<int>(regx);
    } break;
    case 0x07: {
      // 8XY7
      // SUBB. VX, VY
      // Substract VY from VX. VF is set to the mos significant bit of VX before
      // shift.
      std::cout << "SUBB. V" << static_cast<int>(regx) << ", V"
                << static_cast<int>(regy);
    } break;
    case 0x0e: {
      // 8XYe
      // SHL. VX
      // Shifts VX left by one. VF is set to the value of the most significant
      // bit of VX before shift.
      std::cout << "SHL. V" << static_cast<int>(regx);
    } break;
    }
  } break;
  case 0x09: {
    // 9XY0
    // SKIP.NQ VX, VY
    // Skip the next instruction if VX doesn't equal VY
    uint8_t firstaddress = firstbyte & 0x0F;
    uint8_t secondaddress = (lastbyte & 0xF0) >> 4;
    std::cout << "SKIP.NQ V" << std::setw(1) << static_cast<int>(firstaddress)
              << ", V" << std::setw(1) << static_cast<int>(secondaddress);
  }
  case 0x0a: {
    // ANNN
    // MVI I, #$NNN
    // Sets I (index register) to the adress of NNN
    uint8_t addresshi = firstbyte & 0x0F;
    std::cout << "MVI I,#$" << std::setw(1) << static_cast<int>(addresshi)
              << std::setw(2) << static_cast<int>(lastbyte);

  } break;
  case 0x0b: {
    // BNNN
    // JUMP $NNN(V0)
    // Jumps to address NNN plus value o V0
    uint8_t addresshi = firstbyte & 0x0F;
    std::cout << "JUMP $" << std::setw(1) << static_cast<int>(addresshi)
              << std::setw(2) << static_cast<int>(lastbyte) << "(V0)";

  } break;
  case 0x0c: {
    // CXNN
    // RAND VX, #$NN
    // Sets VX to a random value plus NN
    uint8_t addresshi = firstbyte & 0x0F;
    std::cout << "RAND V" << std::setw(1) << static_cast<int>(addresshi)
              << " #$" << std::setw(2) << static_cast<int>(lastbyte);

  } break;
  case 0x0d: {
    // DXYN
    // SPRITE VX, VY, #$N
    // Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and
    // height of N pixels starting from memory location I (index register).
    uint8_t height = lastbyte & 0x0F;
    uint8_t regx = firstbyte & 0x0F;
    uint8_t regy = (lastbyte & 0xF0) >> 4;
    std::cout << "SPRITE V" << static_cast<int>(regx) << " V"
              << static_cast<int>(regy) << ", #$" << static_cast<int>(height);

  } break;
  case 0x0e: {
    // Two instructions for key input handling
    uint8_t operation = lastbyte & 0xFF;
    uint8_t reg = lastbyte & 0x0F;
    switch (operation) {
    case 0x9E:
      // EX9E
      // SKIP.KEY VX
      // Skip the next instruction if key stored in VX is pressed
      std::cout << "SKIP.KEY V" << static_cast<int>(reg);
      break;
    case 0xA1:
      // EXA1
      // SKIP.NOKEY VX
      // Skip the next instruction if key stored in VX is not pressed
      std::cout << "SKIP.NOKEY V" << static_cast<int>(reg);
      break;
    }
  } break;
  case 0x0f: {
    uint8_t operation = lastbyte & 0xFF;
    uint8_t reg = firstbyte & 0x0F;
    switch (operation) {
    case 0x07:
      // FX07
      // MOV VX, DELAY
      // Set VX to the value of the delay timer
      std::cout << "MOV V" << static_cast<int>(reg) << ", DELAY";
      break;
    case 0x0A:
      // FX07
      // WAITKEY VX
      // Wait for key stored in VX to be pressed
      std::cout << "WAITKEY V" << static_cast<int>(reg);
      break;
    case 0x15:
      // FX15
      // MOV DELAY, VX
      // Set delay timer to the value of VX
      std::cout << "MOV DELAY, V" << static_cast<int>(reg);
      break;
    case 0x18:
      // FX18
      // MOV SOUND, VX
      // Set sound timer to the value of VX
      std::cout << "MOV SOUND, V" << static_cast<int>(reg);
      break;
    case 0x1E:
      // FX1E
      // ADD I, VX
      // Add value of VX to I (index register)
      std::cout << "ADD I, V" << static_cast<int>(reg);
      break;
    case 0x29:
      // FX29
      // SPRITECHAR VX
      // Set I (index register) to the location of the sprite for character
      // stored in VX
      std::cout << "SPRITECHAR V" << static_cast<int>(reg);
      break;
    case 0x33:
      // FX33
      // MOVBCD VX
      // Stores Binary-coded decimal representation of VX at I (index register)
      std::cout << "SPRITECHAR V" << static_cast<int>(reg);
      break;
    case 0x55:
      // FX55
      // MOVM (I), V0-VX
      // Stores V0 to VX in memory starting at I (index register)
      std::cout << "MOVM (I), V0-V" << static_cast<int>(reg);
      break;
    case 0x65:
      // FX65
      // MOVM V0-VX, (I)
      // Fills V0 to VX with values from memory starting at address I
      std::cout << "MOVM V0-V" << static_cast<int>(reg) << ", (I)";
      break;
    }
  }
  }
}

int main(int argc, char **argv) {
  std::ifstream rom(argv[1], std::ios::in | std::ios::binary | std::ios::ate);

  if (!rom.is_open()) {
    std::cout << "Couldn't open file!" << std::endl;
    return 1;
  }

  long size = rom.tellg();
  unsigned char *buffer = new unsigned char[size + 0x200];

  rom.seekg(0, std::ios::beg);
  rom.read((char *)buffer + 0x200, size);
  rom.close();

  int pc = 0x200;
  while (pc < (size + 0x200)) {
    disassemble_chip8(buffer, pc);
    pc += 2;
    std::cout << std::endl;
  }

  return 0;
}

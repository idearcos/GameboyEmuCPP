#include "Z80.h"

Z80::InstructionMap Z80::FillBitInstructionMap()
{
	InstructionMap bit_instructions;

#pragma region Bit instructions 0x00 - 0x0F
	// RLC B
	bit_instructions[0x00] = [this](){ return this->Rlc(Register8bit::B); };
	// RLC C
	bit_instructions[0x01] = [this](){ return this->Rlc(Register8bit::C); };
	// RLC D
	bit_instructions[0x02] = [this](){ return this->Rlc(Register8bit::D); };
	// RLC E
	bit_instructions[0x03] = [this](){ return this->Rlc(Register8bit::E); };
	// RLC H
	bit_instructions[0x04] = [this](){ return this->Rlc(Register8bit::H); };
	// RLC L
	bit_instructions[0x05] = [this](){ return this->Rlc(Register8bit::L); };
	// RLC (HL)
	bit_instructions[0x06] = [this](){ return this->Rlc(Register16bit::HL); };
	// RLC A
	bit_instructions[0x07] = [this](){ return this->Rlc(Register8bit::A); };
	// RRC B
	bit_instructions[0x08] = [this](){ return this->Rrc(Register8bit::B); };
	// RRC C
	bit_instructions[0x09] = [this](){ return this->Rrc(Register8bit::C); };
	// RRC D
	bit_instructions[0x0A] = [this](){ return this->Rrc(Register8bit::D); };
	// RRC E
	bit_instructions[0x0B] = [this](){ return this->Rrc(Register8bit::E); };
	// RRC H
	bit_instructions[0x0C] = [this](){ return this->Rrc(Register8bit::H); };
	// RRC L
	bit_instructions[0x0D] = [this](){ return this->Rrc(Register8bit::L); };
	// RRC (HL)
	bit_instructions[0x0E] = [this](){ return this->Rrc(Register16bit::HL); };
	// RRC A
	bit_instructions[0x0F] = [this](){ return this->Rrc(Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x10 - 0x1F
	// RL B
	bit_instructions[0x10] = [this](){ return this->Rl(Register8bit::B); };
	// RL C
	bit_instructions[0x11] = [this](){ return this->Rl(Register8bit::C); };
	// RL D
	bit_instructions[0x12] = [this](){ return this->Rl(Register8bit::D); };
	// RL E
	bit_instructions[0x13] = [this](){ return this->Rl(Register8bit::E); };
	// RL H
	bit_instructions[0x14] = [this](){ return this->Rl(Register8bit::H); };
	// RL L
	bit_instructions[0x15] = [this](){ return this->Rl(Register8bit::L); };
	// RL (HL)
	bit_instructions[0x16] = [this](){ return this->Rl(Register16bit::HL); };
	// RL A
	bit_instructions[0x17] = [this](){ return this->Rl(Register8bit::A); };
	// RR B
	bit_instructions[0x18] = [this](){ return this->Rr(Register8bit::B); };
	// RR C
	bit_instructions[0x19] = [this](){ return this->Rr(Register8bit::C); };
	// RR D
	bit_instructions[0x1A] = [this](){ return this->Rr(Register8bit::D); };
	// RR E
	bit_instructions[0x1B] = [this](){ return this->Rr(Register8bit::E); };
	// RR H
	bit_instructions[0x1C] = [this](){ return this->Rr(Register8bit::H); };
	// RR L
	bit_instructions[0x1D] = [this](){ return this->Rr(Register8bit::L); };
	// RR (HL)
	bit_instructions[0x1E] = [this](){ return this->Rr(Register16bit::HL); };
	// RR A
	bit_instructions[0x1F] = [this](){ return this->Rr(Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x20 - 0x2F
	// SLA B
	bit_instructions[0x20] = [this](){ return this->Sla(Register8bit::B); };
	// SLA C
	bit_instructions[0x21] = [this](){ return this->Sla(Register8bit::C); };
	// SLA D
	bit_instructions[0x22] = [this](){ return this->Sla(Register8bit::D); };
	// SLA E
	bit_instructions[0x23] = [this](){ return this->Sla(Register8bit::E); };
	// SLA H
	bit_instructions[0x24] = [this](){ return this->Sla(Register8bit::H); };
	// SLA L
	bit_instructions[0x25] = [this](){ return this->Sla(Register8bit::L); };
	// SLA (HL)
	bit_instructions[0x26] = [this](){ return this->Sla(Register16bit::HL); };
	// SLA A
	bit_instructions[0x27] = [this](){ return this->Sla(Register8bit::A); };
	// SRA B
	bit_instructions[0x28] = [this](){ return this->Sra(Register8bit::B); };
	// SRA C
	bit_instructions[0x29] = [this](){ return this->Sra(Register8bit::C); };
	// SRA D
	bit_instructions[0x2A] = [this](){ return this->Sra(Register8bit::D); };
	// SRA E
	bit_instructions[0x2B] = [this](){ return this->Sra(Register8bit::E); };
	// SRA H
	bit_instructions[0x2C] = [this](){ return this->Sra(Register8bit::H); };
	// SRA L
	bit_instructions[0x2D] = [this](){ return this->Sra(Register8bit::L); };
	// SRA (HL)
	bit_instructions[0x2E] = [this](){ return this->Sra(Register16bit::HL); };
	// SRA A
	bit_instructions[0x2F] = [this](){ return this->Sra(Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x30 - 0x3F
	// SWAP B
	bit_instructions[0x30] = [this](){ return this->Swap(Register8bit::B); };
	// SWAP C
	bit_instructions[0x31] = [this](){ return this->Swap(Register8bit::C); };
	// SWAP D
	bit_instructions[0x32] = [this](){ return this->Swap(Register8bit::D); };
	// SWAP E
	bit_instructions[0x33] = [this](){ return this->Swap(Register8bit::E); };
	// SWAP H
	bit_instructions[0x34] = [this](){ return this->Swap(Register8bit::H); };
	// SWAP L
	bit_instructions[0x35] = [this](){ return this->Swap(Register8bit::L); };
	// SWAP (HL)
	bit_instructions[0x36] = [this](){ return this->Swap(Register16bit::HL); };
	// SWAP A
	bit_instructions[0x37] = [this](){ return this->Swap(Register8bit::A); };
	// SRL B
	bit_instructions[0x38] = [this](){ return this->Srl(Register8bit::B); };
	// SRL C
	bit_instructions[0x39] = [this](){ return this->Srl(Register8bit::C); };
	// SRL D
	bit_instructions[0x3A] = [this](){ return this->Srl(Register8bit::D); };
	// SRL E
	bit_instructions[0x3B] = [this](){ return this->Srl(Register8bit::E); };
	// SRL H
	bit_instructions[0x3C] = [this](){ return this->Srl(Register8bit::H); };
	// SRL L
	bit_instructions[0x3D] = [this](){ return this->Srl(Register8bit::L); };
	// SRL (HL)
	bit_instructions[0x3E] = [this](){ return this->Srl(Register16bit::HL); };
	// SRL A
	bit_instructions[0x3F] = [this](){ return this->Srl(Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x40 - 0x4F
	// BIT 0, B
	bit_instructions[0x40] = [this](){ return this->TestBit(0, Register8bit::B); };
	// BIT 0, C
	bit_instructions[0x41] = [this](){ return this->TestBit(0, Register8bit::C); };
	// BIT 0, D
	bit_instructions[0x42] = [this](){ return this->TestBit(0, Register8bit::D); };
	// BIT 0, E
	bit_instructions[0x43] = [this](){ return this->TestBit(0, Register8bit::E); };
	// BIT 0, H
	bit_instructions[0x44] = [this](){ return this->TestBit(0, Register8bit::H); };
	// BIT 0, L
	bit_instructions[0x45] = [this](){ return this->TestBit(0, Register8bit::L); };
	// BIT 0, (HL)
	bit_instructions[0x46] = [this](){ return this->TestBit(0, Register16bit::HL); };
	// BIT 0, A
	bit_instructions[0x47] = [this](){ return this->TestBit(0, Register8bit::A); };
	// BIT 1, B
	bit_instructions[0x48] = [this](){ return this->TestBit(1, Register8bit::B); };
	// BIT 1, C
	bit_instructions[0x49] = [this](){ return this->TestBit(1, Register8bit::C); };
	// BIT 1, D
	bit_instructions[0x4A] = [this](){ return this->TestBit(1, Register8bit::D); };
	// BIT 1, E
	bit_instructions[0x4B] = [this](){ return this->TestBit(1, Register8bit::E); };
	// BIT 1, H
	bit_instructions[0x4C] = [this](){ return this->TestBit(1, Register8bit::H); };
	// BIT 1, L
	bit_instructions[0x4D] = [this](){ return this->TestBit(1, Register8bit::L); };
	// BIT 1, (HL)
	bit_instructions[0x4E] = [this](){ return this->TestBit(1, Register16bit::HL); };
	// BIT 1, A
	bit_instructions[0x4F] = [this](){ return this->TestBit(1, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x50 - 0x5F
	// BIT 2, B
	bit_instructions[0x50] = [this](){ return this->TestBit(2, Register8bit::B); };
	// BIT 2, C
	bit_instructions[0x51] = [this](){ return this->TestBit(2, Register8bit::C); };
	// BIT 2, D
	bit_instructions[0x52] = [this](){ return this->TestBit(2, Register8bit::D); };
	// BIT 2, E
	bit_instructions[0x53] = [this](){ return this->TestBit(2, Register8bit::E); };
	// BIT 2, H
	bit_instructions[0x54] = [this](){ return this->TestBit(2, Register8bit::H); };
	// BIT 2, L
	bit_instructions[0x55] = [this](){ return this->TestBit(2, Register8bit::L); };
	// BIT 2, (HL)
	bit_instructions[0x56] = [this](){ return this->TestBit(2, Register16bit::HL); };
	// BIT 2, A
	bit_instructions[0x57] = [this](){ return this->TestBit(2, Register8bit::A); };
	// BIT 3, B
	bit_instructions[0x58] = [this](){ return this->TestBit(3, Register8bit::B); };
	// BIT 3, C
	bit_instructions[0x59] = [this](){ return this->TestBit(3, Register8bit::C); };
	// BIT 3, D
	bit_instructions[0x5A] = [this](){ return this->TestBit(3, Register8bit::D); };
	// BIT 3, E
	bit_instructions[0x5B] = [this](){ return this->TestBit(3, Register8bit::E); };
	// BIT 3, H
	bit_instructions[0x5C] = [this](){ return this->TestBit(3, Register8bit::H); };
	// BIT 3, L
	bit_instructions[0x5D] = [this](){ return this->TestBit(3, Register8bit::L); };
	// BIT 3, (HL)
	bit_instructions[0x5E] = [this](){ return this->TestBit(3, Register16bit::HL); };
	// BIT 3, A
	bit_instructions[0x5F] = [this](){ return this->TestBit(3, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x60 - 0x6F
	// BIT 4, B
	bit_instructions[0x60] = [this](){ return this->TestBit(4, Register8bit::B); };
	// BIT 4, C
	bit_instructions[0x61] = [this](){ return this->TestBit(4, Register8bit::C); };
	// BIT 4, D
	bit_instructions[0x62] = [this](){ return this->TestBit(4, Register8bit::D); };
	// BIT 4, E
	bit_instructions[0x63] = [this](){ return this->TestBit(4, Register8bit::E); };
	// BIT 4, H
	bit_instructions[0x64] = [this](){ return this->TestBit(4, Register8bit::H); };
	// BIT 4, L
	bit_instructions[0x65] = [this](){ return this->TestBit(4, Register8bit::L); };
	// BIT 4, (HL)
	bit_instructions[0x66] = [this](){ return this->TestBit(4, Register16bit::HL); };
	// BIT 4, A
	bit_instructions[0x67] = [this](){ return this->TestBit(4, Register8bit::A); };
	// BIT 5, B
	bit_instructions[0x68] = [this](){ return this->TestBit(5, Register8bit::B); };
	// BIT 5, C
	bit_instructions[0x69] = [this](){ return this->TestBit(5, Register8bit::C); };
	// BIT 5, D
	bit_instructions[0x6A] = [this](){ return this->TestBit(5, Register8bit::D); };
	// BIT 5, E
	bit_instructions[0x6B] = [this](){ return this->TestBit(5, Register8bit::E); };
	// BIT 5, H
	bit_instructions[0x6C] = [this](){ return this->TestBit(5, Register8bit::H); };
	// BIT 5, L
	bit_instructions[0x6D] = [this](){ return this->TestBit(5, Register8bit::L); };
	// BIT 5, (HL)
	bit_instructions[0x6E] = [this](){ return this->TestBit(5, Register16bit::HL); };
	// BIT 5, A
	bit_instructions[0x6F] = [this](){ return this->TestBit(5, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x70 - 0x7F
	// BIT 6, B
	bit_instructions[0x70] = [this](){ return this->TestBit(6, Register8bit::B); };
	// BIT 6, C
	bit_instructions[0x71] = [this](){ return this->TestBit(6, Register8bit::C); };
	// BIT 6, D
	bit_instructions[0x72] = [this](){ return this->TestBit(6, Register8bit::D); };
	// BIT 6, E
	bit_instructions[0x73] = [this](){ return this->TestBit(6, Register8bit::E); };
	// BIT 6, H
	bit_instructions[0x74] = [this](){ return this->TestBit(6, Register8bit::H); };
	// BIT 6, L
	bit_instructions[0x75] = [this](){ return this->TestBit(6, Register8bit::L); };
	// BIT 6, (HL)
	bit_instructions[0x76] = [this](){ return this->TestBit(6, Register16bit::HL); };
	// BIT 6, A
	bit_instructions[0x77] = [this](){ return this->TestBit(6, Register8bit::A); };
	// BIT 7, B
	bit_instructions[0x78] = [this](){ return this->TestBit(7, Register8bit::B); };
	// BIT 7, C
	bit_instructions[0x79] = [this](){ return this->TestBit(7, Register8bit::C); };
	// BIT 7, D
	bit_instructions[0x7A] = [this](){ return this->TestBit(7, Register8bit::D); };
	// BIT 7, E
	bit_instructions[0x7B] = [this](){ return this->TestBit(7, Register8bit::E); };
	// BIT 7, H
	bit_instructions[0x7C] = [this](){ return this->TestBit(7, Register8bit::H); };
	// BIT 7, L
	bit_instructions[0x7D] = [this](){ return this->TestBit(7, Register8bit::L); };
	// BIT 7, (HL)
	bit_instructions[0x7E] = [this](){ return this->TestBit(7, Register16bit::HL); };
	// BIT 7, A
	bit_instructions[0x7F] = [this](){ return this->TestBit(7, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x80 - 0x8F
	// RES 0, B
	bit_instructions[0x80] = [this](){ return this->ResetBit(0, Register8bit::B); };
	// RES 0, C
	bit_instructions[0x81] = [this](){ return this->ResetBit(0, Register8bit::C); };
	// RES 0, D
	bit_instructions[0x82] = [this](){ return this->ResetBit(0, Register8bit::D); };
	// RES 0, E
	bit_instructions[0x83] = [this](){ return this->ResetBit(0, Register8bit::E); };
	// RES 0, H
	bit_instructions[0x84] = [this](){ return this->ResetBit(0, Register8bit::H); };
	// RES 0, L
	bit_instructions[0x85] = [this](){ return this->ResetBit(0, Register8bit::L); };
	// RES 0, (HL)
	bit_instructions[0x86] = [this](){ return this->ResetBit(0, Register16bit::HL); };
	// RES 0, A
	bit_instructions[0x87] = [this](){ return this->ResetBit(0, Register8bit::A); };
	// RES 1, B
	bit_instructions[0x88] = [this](){ return this->ResetBit(1, Register8bit::B); };
	// RES 1, C
	bit_instructions[0x89] = [this](){ return this->ResetBit(1, Register8bit::C); };
	// RES 1, D
	bit_instructions[0x8A] = [this](){ return this->ResetBit(1, Register8bit::D); };
	// RES 1, E
	bit_instructions[0x8B] = [this](){ return this->ResetBit(1, Register8bit::E); };
	// RES 1, H
	bit_instructions[0x8C] = [this](){ return this->ResetBit(1, Register8bit::H); };
	// RES 1, L
	bit_instructions[0x8D] = [this](){ return this->ResetBit(1, Register8bit::L); };
	// RES 1, (HL)
	bit_instructions[0x8E] = [this](){ return this->ResetBit(1, Register16bit::HL); };
	// RES 1, A
	bit_instructions[0x8F] = [this](){ return this->ResetBit(1, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x90 - 0x9F
	// RES 2, B
	bit_instructions[0x90] = [this](){ return this->ResetBit(2, Register8bit::B); };
	// RES 2, C
	bit_instructions[0x91] = [this](){ return this->ResetBit(2, Register8bit::C); };
	// RES 2, D
	bit_instructions[0x92] = [this](){ return this->ResetBit(2, Register8bit::D); };
	// RES 2, E
	bit_instructions[0x93] = [this](){ return this->ResetBit(2, Register8bit::E); };
	// RES 2, H
	bit_instructions[0x94] = [this](){ return this->ResetBit(2, Register8bit::H); };
	// RES 2, L
	bit_instructions[0x95] = [this](){ return this->ResetBit(2, Register8bit::L); };
	// RES 2, (HL)
	bit_instructions[0x96] = [this](){ return this->ResetBit(2, Register16bit::HL); };
	// RES 2, A
	bit_instructions[0x97] = [this](){ return this->ResetBit(2, Register8bit::A); };
	// RES 3, B
	bit_instructions[0x98] = [this](){ return this->ResetBit(3, Register8bit::B); };
	// RES 3, C
	bit_instructions[0x99] = [this](){ return this->ResetBit(3, Register8bit::C); };
	// RES 3, D
	bit_instructions[0x9A] = [this](){ return this->ResetBit(3, Register8bit::D); };
	// RES 3, E
	bit_instructions[0x9B] = [this](){ return this->ResetBit(3, Register8bit::E); };
	// RES 3, H
	bit_instructions[0x9C] = [this](){ return this->ResetBit(3, Register8bit::H); };
	// RES 3, L
	bit_instructions[0x9D] = [this](){ return this->ResetBit(3, Register8bit::L); };
	// RES 3, (HL)
	bit_instructions[0x9E] = [this](){ return this->ResetBit(3, Register16bit::HL); };
	// RES 3, A
	bit_instructions[0x9F] = [this](){ return this->ResetBit(3, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xA0 - 0xAF
	// RES 4, B
	bit_instructions[0xA0] = [this](){ return this->ResetBit(4, Register8bit::B); };
	// RES 4, C
	bit_instructions[0xA1] = [this](){ return this->ResetBit(4, Register8bit::C); };
	// RES 4, D
	bit_instructions[0xA2] = [this](){ return this->ResetBit(4, Register8bit::D); };
	// RES 4, E
	bit_instructions[0xA3] = [this](){ return this->ResetBit(4, Register8bit::E); };
	// RES 4, H
	bit_instructions[0xA4] = [this](){ return this->ResetBit(4, Register8bit::H); };
	// RES 4, L
	bit_instructions[0xA5] = [this](){ return this->ResetBit(4, Register8bit::L); };
	// RES 4, (HL)
	bit_instructions[0xA6] = [this](){ return this->ResetBit(4, Register16bit::HL); };
	// RES 4, A
	bit_instructions[0xA7] = [this](){ return this->ResetBit(4, Register8bit::A); };
	// RES 5, B
	bit_instructions[0xA8] = [this](){ return this->ResetBit(5, Register8bit::B); };
	// RES 5, C
	bit_instructions[0xA9] = [this](){ return this->ResetBit(5, Register8bit::C); };
	// RES 5, D
	bit_instructions[0xAA] = [this](){ return this->ResetBit(5, Register8bit::D); };
	// RES 5, E
	bit_instructions[0xAB] = [this](){ return this->ResetBit(5, Register8bit::E); };
	// RES 5, H
	bit_instructions[0xAC] = [this](){ return this->ResetBit(5, Register8bit::H); };
	// RES 5, L
	bit_instructions[0xAD] = [this](){ return this->ResetBit(5, Register8bit::L); };
	// RES 5, (HL)
	bit_instructions[0xAE] = [this](){ return this->ResetBit(5, Register16bit::HL); };
	// RES 5, A
	bit_instructions[0xAF] = [this](){ return this->ResetBit(5, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xB0 - 0xBF
	// RES 6, B
	bit_instructions[0xB0] = [this](){ return this->ResetBit(6, Register8bit::B); };
	// RES 6, C
	bit_instructions[0xB1] = [this](){ return this->ResetBit(6, Register8bit::C); };
	// RES 6, D
	bit_instructions[0xB2] = [this](){ return this->ResetBit(6, Register8bit::D); };
	// RES 6, E
	bit_instructions[0xB3] = [this](){ return this->ResetBit(6, Register8bit::E); };
	// RES 6, H
	bit_instructions[0xB4] = [this](){ return this->ResetBit(6, Register8bit::H); };
	// RES 6, L
	bit_instructions[0xB5] = [this](){ return this->ResetBit(6, Register8bit::L); };
	// RES 6, (HL)
	bit_instructions[0xB6] = [this](){ return this->ResetBit(6, Register16bit::HL); };
	// RES 6, A
	bit_instructions[0xB7] = [this](){ return this->ResetBit(6, Register8bit::A); };
	// RES 7, B
	bit_instructions[0xB8] = [this](){ return this->ResetBit(7, Register8bit::B); };
	// RES 7, C
	bit_instructions[0xB9] = [this](){ return this->ResetBit(7, Register8bit::C); };
	// RES 7, D
	bit_instructions[0xBA] = [this](){ return this->ResetBit(7, Register8bit::D); };
	// RES 7, E
	bit_instructions[0xBB] = [this](){ return this->ResetBit(7, Register8bit::E); };
	// RES 7, H
	bit_instructions[0xBC] = [this](){ return this->ResetBit(7, Register8bit::H); };
	// RES 7, L
	bit_instructions[0xBD] = [this](){ return this->ResetBit(7, Register8bit::L); };
	// RES 7, (HL)
	bit_instructions[0xBE] = [this](){ return this->ResetBit(7, Register16bit::HL); };
	// RES 7, A
	bit_instructions[0xBF] = [this](){ return this->ResetBit(7, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xC0 - 0xCF
	// SET 0, B
	bit_instructions[0xC0] = [this](){ return this->SetBit(0, Register8bit::B); };
	// SET 0, C
	bit_instructions[0xC1] = [this](){ return this->SetBit(0, Register8bit::C); };
	// SET 0, D
	bit_instructions[0xC2] = [this](){ return this->SetBit(0, Register8bit::D); };
	// SET 0, E
	bit_instructions[0xC3] = [this](){ return this->SetBit(0, Register8bit::E); };
	// SET 0, H
	bit_instructions[0xC4] = [this](){ return this->SetBit(0, Register8bit::H); };
	// SET 0, L
	bit_instructions[0xC5] = [this](){ return this->SetBit(0, Register8bit::L); };
	// SET 0, (HL)
	bit_instructions[0xC6] = [this](){ return this->SetBit(0, Register16bit::HL); };
	// SET 0, A
	bit_instructions[0xC7] = [this](){ return this->SetBit(0, Register8bit::A); };
	// SET 1, B
	bit_instructions[0xC8] = [this](){ return this->SetBit(1, Register8bit::B); };
	// SET 1, C
	bit_instructions[0xC9] = [this](){ return this->SetBit(1, Register8bit::C); };
	// SET 1, D
	bit_instructions[0xCA] = [this](){ return this->SetBit(1, Register8bit::D); };
	// SET 1, E
	bit_instructions[0xCB] = [this](){ return this->SetBit(1, Register8bit::E); };
	// SET 1, H
	bit_instructions[0xCC] = [this](){ return this->SetBit(1, Register8bit::H); };
	// SET 1, L
	bit_instructions[0xCD] = [this](){ return this->SetBit(1, Register8bit::L); };
	// SET 1, (HL)
	bit_instructions[0xCE] = [this](){ return this->SetBit(1, Register16bit::HL); };
	// SET 1, A
	bit_instructions[0xCF] = [this](){ return this->SetBit(1, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xD0 - 0xDF
	// SET 2, B
	bit_instructions[0xD0] = [this](){ return this->SetBit(2, Register8bit::B); };
	// SET 2, C
	bit_instructions[0xD1] = [this](){ return this->SetBit(2, Register8bit::C); };
	// SET 2, D
	bit_instructions[0xD2] = [this](){ return this->SetBit(2, Register8bit::D); };
	// SET 2, E
	bit_instructions[0xD3] = [this](){ return this->SetBit(2, Register8bit::E); };
	// SET 2, H
	bit_instructions[0xD4] = [this](){ return this->SetBit(2, Register8bit::H); };
	// SET 2, L
	bit_instructions[0xD5] = [this](){ return this->SetBit(2, Register8bit::L); };
	// SET 2, (HL)
	bit_instructions[0xD6] = [this](){ return this->SetBit(2, Register16bit::HL); };
	// SET 2, A
	bit_instructions[0xD7] = [this](){ return this->SetBit(2, Register8bit::A); };
	// SET 3, B
	bit_instructions[0xD8] = [this](){ return this->SetBit(3, Register8bit::B); };
	// SET 3, C
	bit_instructions[0xD9] = [this](){ return this->SetBit(3, Register8bit::C); };
	// SET 3, D
	bit_instructions[0xDA] = [this](){ return this->SetBit(3, Register8bit::D); };
	// SET 3, E
	bit_instructions[0xDB] = [this](){ return this->SetBit(3, Register8bit::E); };
	// SET 3, H
	bit_instructions[0xDC] = [this](){ return this->SetBit(3, Register8bit::H); };
	// SET 3, L
	bit_instructions[0xDD] = [this](){ return this->SetBit(3, Register8bit::L); };
	// SET 3, (HL)
	bit_instructions[0xDE] = [this](){ return this->SetBit(3, Register16bit::HL); };
	// SET 3, A
	bit_instructions[0xDF] = [this](){ return this->SetBit(3, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xE0 - 0xEF
	// SET 4, B
	bit_instructions[0xE0] = [this](){ return this->SetBit(4, Register8bit::B); };
	// SET 4, C
	bit_instructions[0xE1] = [this](){ return this->SetBit(4, Register8bit::C); };
	// SET 4, D
	bit_instructions[0xE2] = [this](){ return this->SetBit(4, Register8bit::D); };
	// SET 4, E
	bit_instructions[0xE3] = [this](){ return this->SetBit(4, Register8bit::E); };
	// SET 4, H
	bit_instructions[0xE4] = [this](){ return this->SetBit(4, Register8bit::H); };
	// SET 4, L
	bit_instructions[0xE5] = [this](){ return this->SetBit(4, Register8bit::L); };
	// SET 4, (HL)
	bit_instructions[0xE6] = [this](){ return this->SetBit(4, Register16bit::HL); };
	// SET 4, A
	bit_instructions[0xE7] = [this](){ return this->SetBit(4, Register8bit::A); };
	// SET 5, B
	bit_instructions[0xE8] = [this](){ return this->SetBit(5, Register8bit::B); };
	// SET 5, C
	bit_instructions[0xE9] = [this](){ return this->SetBit(5, Register8bit::C); };
	// SET 5, D
	bit_instructions[0xEA] = [this](){ return this->SetBit(5, Register8bit::D); };
	// SET 5, E
	bit_instructions[0xEB] = [this](){ return this->SetBit(5, Register8bit::E); };
	// SET 5, H
	bit_instructions[0xEC] = [this](){ return this->SetBit(5, Register8bit::H); };
	// SET 5, L
	bit_instructions[0xED] = [this](){ return this->SetBit(5, Register8bit::L); };
	// SET 5, (HL)
	bit_instructions[0xEE] = [this](){ return this->SetBit(5, Register16bit::HL); };
	// SET 5, A
	bit_instructions[0xEF] = [this](){ return this->SetBit(5, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xF0 - 0xFF
	// SET 6, B
	bit_instructions[0xF0] = [this](){ return this->SetBit(6, Register8bit::B); };
	// SET 6, C
	bit_instructions[0xF1] = [this](){ return this->SetBit(6, Register8bit::C); };
	// SET 6, D
	bit_instructions[0xF2] = [this](){ return this->SetBit(6, Register8bit::D); };
	// SET 6, E
	bit_instructions[0xF3] = [this](){ return this->SetBit(6, Register8bit::E); };
	// SET 6, H
	bit_instructions[0xF4] = [this](){ return this->SetBit(6, Register8bit::H); };
	// SET 6, L
	bit_instructions[0xF5] = [this](){ return this->SetBit(6, Register8bit::L); };
	// SET 6, (HL)
	bit_instructions[0xF6] = [this](){ return this->SetBit(6, Register16bit::HL); };
	// SET 6, A
	bit_instructions[0xF7] = [this](){ return this->SetBit(6, Register8bit::A); };
	// SET 7, B
	bit_instructions[0xF8] = [this](){ return this->SetBit(7, Register8bit::B); };
	// SET 7, C
	bit_instructions[0xF9] = [this](){ return this->SetBit(7, Register8bit::C); };
	// SET 7, D
	bit_instructions[0xFA] = [this](){ return this->SetBit(7, Register8bit::D); };
	// SET 7, E
	bit_instructions[0xFB] = [this](){ return this->SetBit(7, Register8bit::E); };
	// SET 7, H
	bit_instructions[0xFC] = [this](){ return this->SetBit(7, Register8bit::H); };
	// SET 7, L
	bit_instructions[0xFD] = [this](){ return this->SetBit(7, Register8bit::L); };
	// SET 7, (HL)
	bit_instructions[0xFE] = [this](){ return this->SetBit(7, Register16bit::HL); };
	// SET 7, A
	bit_instructions[0xFF] = [this](){ return this->SetBit(7, Register8bit::A); };
#pragma endregion

	return bit_instructions;
}

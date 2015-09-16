#include "Z80.h"

#include <iostream>

Z80::Z80()
{
#pragma region 0x00 - 0x0F
	// NOP
	operations_[0x00] = [this](){ return this->NoOperation(); };
	// LD BC, nn
	operations_[0x01] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::BC, value); };
	// LD (BC), A
	operations_[0x02] = [this](){ return this->LoadAddressFromRegister(Register16bit::BC, Register8bit::A); };
	operations_[0x03] = [this](){ return this->WrongOpCode(); };
	operations_[0x04] = [this](){ return this->WrongOpCode(); };
	operations_[0x05] = [this](){ return this->WrongOpCode(); };
	// LD B, n
	operations_[0x06] = [this](){ return this->LoadRegister(Register8bit::B, this->FetchByte()); };
	operations_[0x07] = [this](){ return this->WrongOpCode(); };
	operations_[0x08] = [this](){ return this->WrongOpCode(); };
	operations_[0x09] = [this](){ return this->WrongOpCode(); };
	// LD A, (BC)
	operations_[0x0A] = [this](){ return this->LoadRegisterFromAddress(Register8bit::A, Register16bit::BC); };
	operations_[0x0B] = [this](){ return this->WrongOpCode(); };
	operations_[0x0C] = [this](){ return this->WrongOpCode(); };
	operations_[0x0D] = [this](){ return this->WrongOpCode(); };
	// LD C, n
	operations_[0x0E] = [this](){ return this->LoadRegister(Register8bit::C, this->FetchByte()); };
	operations_[0x0F] = [this](){ return this->WrongOpCode(); };
#pragma endregion

#pragma region 0x10 - 0x1F
	operations_[0x10] = [this](){ return this->WrongOpCode(); };
	// LD DE, nn
	operations_[0x11] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::DE, value); };
	// LD (DE), A
	operations_[0x12] = [this](){ return this->LoadAddressFromRegister(Register16bit::DE, Register8bit::A); };
	operations_[0x13] = [this](){ return this->WrongOpCode(); };
	operations_[0x14] = [this](){ return this->WrongOpCode(); };
	operations_[0x15] = [this](){ return this->WrongOpCode(); };
	// LD D, n
	operations_[0x16] = [this](){ return this->LoadRegister(Register8bit::D, this->FetchByte()); };
	operations_[0x17] = [this](){ return this->WrongOpCode(); };
	operations_[0x18] = [this](){ return this->WrongOpCode(); };
	operations_[0x19] = [this](){ return this->WrongOpCode(); };
	// LD A, (DE)
	operations_[0x1A] = [this](){ return this->LoadRegisterFromAddress(Register8bit::A, Register16bit::DE); };
	operations_[0x1B] = [this](){ return this->WrongOpCode(); };
	operations_[0x1C] = [this](){ return this->WrongOpCode(); };
	operations_[0x1D] = [this](){ return this->WrongOpCode(); };
	// LD E, n
	operations_[0x1E] = [this](){ return this->LoadRegister(Register8bit::E, this->FetchByte()); };
	operations_[0x1F] = [this](){ return this->WrongOpCode(); };
#pragma endregion

#pragma region 0x20 - 0x2F
	operations_[0x20] = [this](){ return this->WrongOpCode(); };
	// LD HL, nn
	operations_[0x21] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::HL, value); };
	operations_[0x22] = [this](){ return this->WrongOpCode(); };
	operations_[0x23] = [this](){ return this->WrongOpCode(); };
	operations_[0x24] = [this](){ return this->WrongOpCode(); };
	operations_[0x25] = [this](){ return this->WrongOpCode(); };
	// LD H, n
	operations_[0x26] = [this](){ return this->LoadRegister(Register8bit::H, this->FetchByte()); };
	operations_[0x27] = [this](){ return this->WrongOpCode(); };
	operations_[0x28] = [this](){ return this->WrongOpCode(); };
	operations_[0x29] = [this](){ return this->WrongOpCode(); };
	operations_[0x2A] = [this](){ return this->WrongOpCode(); };
	operations_[0x2B] = [this](){ return this->WrongOpCode(); };
	operations_[0x2C] = [this](){ return this->WrongOpCode(); };
	operations_[0x2D] = [this](){ return this->WrongOpCode(); };
	// LD L, n
	operations_[0x2E] = [this](){ return this->LoadRegister(Register8bit::L, this->FetchByte()); };
	operations_[0x2F] = [this](){ return this->WrongOpCode(); };
#pragma endregion

#pragma region 0x30 - 0x3F
	operations_[0x30] = [this](){ return this->WrongOpCode(); };
	// LD SP, nn
	operations_[0x31] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::SP, value); };
	// LD (nn), A
	operations_[0x32] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadAddressFromRegister(address, Register8bit::A); };
	operations_[0x33] = [this](){ return this->WrongOpCode(); };
	operations_[0x34] = [this](){ return this->WrongOpCode(); };
	operations_[0x35] = [this](){ return this->WrongOpCode(); };
	// LD (HL), n
	operations_[0x36] = [this](){ return this->LoadAddress(Register16bit::HL, this->FetchByte()); };
	operations_[0x37] = [this](){ return this->WrongOpCode(); };
	operations_[0x38] = [this](){ return this->WrongOpCode(); };
	operations_[0x39] = [this](){ return this->WrongOpCode(); };
	// LD A, (nn)
	operations_[0x3A] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegisterFromAddress(Register8bit::A, address); };
	operations_[0x3B] = [this](){ return this->WrongOpCode(); };
	operations_[0x3C] = [this](){ return this->WrongOpCode(); };
	operations_[0x3D] = [this](){ return this->WrongOpCode(); };
	// LD A, n
	operations_[0x3E] = [this](){ return this->LoadRegister(Register8bit::A, this->FetchByte()); };
	operations_[0x3F] = [this](){ return this->WrongOpCode(); };
#pragma endregion

#pragma region 0x40 - 0x4F
	// LD B, B
	operations_[0x40] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::B); };
	// LD B, C
	operations_[0x41] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::C); };
	// LD B, D
	operations_[0x42] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::D); };
	// LD B, E
	operations_[0x43] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::E); };
	// LD B, H
	operations_[0x44] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::H); };
	// LD B, L
	operations_[0x45] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::L); };
	// LD B, (HL)
	operations_[0x46] = [this](){ return this->LoadRegisterFromAddress(Register8bit::B, Register16bit::HL); };
	// LD B, A
	operations_[0x47] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::A); };
	// LD C, B
	operations_[0x48] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::B); };
	// LD C, C
	operations_[0x49] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::C); };
	// LD C, D
	operations_[0x4A] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::D); };
	// LD C, E
	operations_[0x4B] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::E); };
	// LD C, H
	operations_[0x4C] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::H); };
	// LD C, L
	operations_[0x4D] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::L); };
	// LD C, (HL)
	operations_[0x4E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::C, Register16bit::HL); };
	// LD C, A
	operations_[0x4F] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::A); };
#pragma endregion

#pragma region 0x50 - 0x5F
	// LD D, B
	operations_[0x50] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::B); };
	// LD D, C
	operations_[0x51] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::C); };
	// LD D, D
	operations_[0x52] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::D); };
	// LD D, E
	operations_[0x53] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::E); };
	// LD D, H
	operations_[0x54] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::H); };
	// LD D, L
	operations_[0x55] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::L); };
	// LD D, (HL)
	operations_[0x56] = [this](){ return this->LoadRegisterFromAddress(Register8bit::D, Register16bit::HL); };
	// LD D, A
	operations_[0x57] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::A); };
	// LD E, B
	operations_[0x58] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::B); };
	// LD E, C
	operations_[0x59] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::C); };
	// LD E, D
	operations_[0x5A] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::D); };
	// LD E, E
	operations_[0x5B] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::E); };
	// LD E, H
	operations_[0x5C] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::H); };
	// LD E, L
	operations_[0x5D] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::L); };
	// LD E, (HL)
	operations_[0x5E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::E, Register16bit::HL); };
	// LD E, A
	operations_[0x5F] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::A); };
#pragma endregion

#pragma region 0x60 - 0x6F
	// LD H, B
	operations_[0x60] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::B); };
	// LD H, C
	operations_[0x61] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::C); };
	// LD H, D
	operations_[0x62] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::D); };
	// LD H, E
	operations_[0x63] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::E); };
	// LD H, H
	operations_[0x64] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::H); };
	// LD H, L
	operations_[0x65] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::L); };
	// LD H, (HL)
	operations_[0x66] = [this](){ return this->LoadRegisterFromAddress(Register8bit::H, Register16bit::HL); };
	// LD H, A
	operations_[0x67] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::A); };
	// LD L, B
	operations_[0x68] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::B); };
	// LD L, C
	operations_[0x69] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::C); };
	// LD L, D
	operations_[0x6A] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::D); };
	// LD L, E
	operations_[0x6B] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::E); };
	// LD L, H
	operations_[0x6C] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::H); };
	// LD L, L
	operations_[0x6D] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::L); };
	// LD L, (HL)
	operations_[0x6E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::L, Register16bit::HL); };
	// LD L, A
	operations_[0x6F] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::A); };
#pragma endregion

#pragma region 0x70 - 0x7F
	// LD (HC), B
	operations_[0x70] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::B); };
	// LD (HC), C
	operations_[0x71] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::C); };
	// LD (HC), D
	operations_[0x72] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::D); };
	// LD (HC), E
	operations_[0x73] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::E); };
	// LD (HC), H
	operations_[0x74] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::H); };
	// LD (HC), L
	operations_[0x75] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::L); };
	operations_[0x76] = [this](){ return this->WrongOpCode(); };
	// LD (HC), A
	operations_[0x77] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::A); };
	// LD A, B
	operations_[0x78] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::B); };
	// LD A, C
	operations_[0x79] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::C); };
	// LD A, D
	operations_[0x7A] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::D); };
	// LD A, E
	operations_[0x7B] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::E); };
	// LD A, H
	operations_[0x7C] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::H); };
	// LD A, L
	operations_[0x7D] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::L); };
	// LD A, (HL)
	operations_[0x7E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::A, Register16bit::HL); };
	// LD A, A
	operations_[0x7F] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::A); };
#pragma endregion

#pragma region 0x80 - 0x8F
	// ADD A, B
	operations_[0x80] = [this](){ return this->Add(Register8bit::B); };
	// ADD A, C
	operations_[0x81] = [this](){ return this->Add(Register8bit::C); };
	// ADD A, D
	operations_[0x82] = [this](){ return this->Add(Register8bit::D); };
	// ADD A, E
	operations_[0x83] = [this](){ return this->Add(Register8bit::E); };
	// ADD A, H
	operations_[0x84] = [this](){ return this->Add(Register8bit::H); };
	// ADD A, L
	operations_[0x85] = [this](){ return this->Add(Register8bit::L); };
	// ADD A, (HL)
	operations_[0x86] = [this](){ return this->Add(Register8bit::); };
	// ADD A, A
	operations_[0x87] = [this](){ return this->Add(Register8bit::A); };
	// ADC A, B
	operations_[0x88] = [this](){ return this->AddPlusCarry(Register8bit::B); };
	// ADC A, C
	operations_[0x89] = [this](){ return this->AddPlusCarry(Register8bit::C); };
	// ADC A, D
	operations_[0x8A] = [this](){ return this->AddPlusCarry(Register8bit::D); };
	// ADC A, E
	operations_[0x8B] = [this](){ return this->AddPlusCarry(Register8bit::E); };
	// ADC A, H
	operations_[0x8C] = [this](){ return this->AddPlusCarry(Register8bit::H); };
	// ADC A, L
	operations_[0x8D] = [this](){ return this->AddPlusCarry(Register8bit::L); };
	// ADC A, (HL)
	operations_[0x8E] = [this](){ return this->AddPlusCarry(Register8bit::); };
	// ADC A, A
	operations_[0x8F] = [this](){ return this->AddPlusCarry(Register8bit::A); };
#pragma endregion

#pragma region 0x90 - 0x9F
	// SUB A, B
	operations_[0x90] = [this](){ return this->Sub(Register8bit::B); };
	// SUB A, C
	operations_[0x91] = [this](){ return this->Sub(Register8bit::C); };
	// SUB A, D
	operations_[0x92] = [this](){ return this->Sub(Register8bit::D); };
	// SUB A, E
	operations_[0x93] = [this](){ return this->Sub(Register8bit::E); };
	// SUB A, H
	operations_[0x94] = [this](){ return this->Sub(Register8bit::H); };
	// SUB A, L
	operations_[0x95] = [this](){ return this->Sub(Register8bit::L); };
	// SUB A, (HL)
	operations_[0x96] = [this](){ return this->Sub(Register8bit::); };
	// SUB A, A
	operations_[0x97] = [this](){ return this->Sub(Register8bit::A); };
	// SBC A, B
	operations_[0x98] = [this](){ return this->SubMinusCarry(Register8bit::B); };
	// SBC A, C
	operations_[0x99] = [this](){ return this->SubMinusCarry(Register8bit::C); };
	// SBC A, D
	operations_[0x9A] = [this](){ return this->SubMinusCarry(Register8bit::D); };
	// SBC A, E
	operations_[0x9B] = [this](){ return this->SubMinusCarry(Register8bit::E); };
	// SBC A, H
	operations_[0x9C] = [this](){ return this->SubMinusCarry(Register8bit::H); };
	// SBC A, L
	operations_[0x9D] = [this](){ return this->SubMinusCarry(Register8bit::L); };
	// SBC A, (HL)
	operations_[0x9E] = [this](){ return this->SubMinusCarry(Register8bit::); };
	// SBC A, A
	operations_[0x9F] = [this](){ return this->SubMinusCarry(Register8bit::A); };
#pragma endregion

#pragma region 0xA0 - 0xAF
	// AND B
	operations_[0xA0] = [this](){ return this->And(Register8bit::B); };
	// AND C
	operations_[0xA1] = [this](){ return this->And(Register8bit::C); };
	// AND D
	operations_[0xA2] = [this](){ return this->And(Register8bit::D); };
	// AND E
	operations_[0xA3] = [this](){ return this->And(Register8bit::E); };
	// AND H
	operations_[0xA4] = [this](){ return this->And(Register8bit::H); };
	// AND L
	operations_[0xA5] = [this](){ return this->And(Register8bit::L); };
	// AND (HL)
	operations_[0xA6] = [this](){ return this->And(Register8bit::); };
	// AND A
	operations_[0xA7] = [this](){ return this->And(Register8bit::A); };
	// XOR B
	operations_[0xA8] = [this](){ return this->Xor(Register8bit::B); };
	// XOR C
	operations_[0xA9] = [this](){ return this->Xor(Register8bit::C); };
	// XOR D
	operations_[0xAA] = [this](){ return this->Xor(Register8bit::D); };
	// XOR E
	operations_[0xAB] = [this](){ return this->Xor(Register8bit::E); };
	// XOR H
	operations_[0xAC] = [this](){ return this->Xor(Register8bit::H); };
	// XOR L
	operations_[0xAD] = [this](){ return this->Xor(Register8bit::L); };
	// XOR (HL)
	operations_[0xAE] = [this](){ return this->Xor(Register8bit::); };
	// XOR A
	operations_[0xAF] = [this](){ return this->Xor(Register8bit::A); };
#pragma endregion

#pragma region 0xB0 - 0xBF
	// OR B
	operations_[0xB0] = [this](){ return this->Or(Register8bit::B); };
	// OR C
	operations_[0xB1] = [this](){ return this->Or(Register8bit::C); };
	// OR D
	operations_[0xB2] = [this](){ return this->Or(Register8bit::D); };
	// OR E
	operations_[0xB3] = [this](){ return this->Or(Register8bit::E); };
	// OR H
	operations_[0xB4] = [this](){ return this->Or(Register8bit::H); };
	// OR L
	operations_[0xB5] = [this](){ return this->Or(Register8bit::L); };
	// OR (HL)
	operations_[0xB6] = [this](){ return this->Or(Register8bit::); };
	// OR A
	operations_[0xB7] = [this](){ return this->Or(Register8bit::A); };
	// CP B
	operations_[0xB8] = [this](){ return this->Compare(Register8bit::B); };
	// CP C
	operations_[0xB9] = [this](){ return this->Compare(Register8bit::C); };
	// CP D
	operations_[0xBA] = [this](){ return this->Compare(Register8bit::D); };
	// CP E
	operations_[0xBB] = [this](){ return this->Compare(Register8bit::E); };
	// CP H
	operations_[0xBC] = [this](){ return this->Compare(Register8bit::H); };
	// CP L
	operations_[0xBD] = [this](){ return this->Compare(Register8bit::L); };
	// CP (HL)
	operations_[0xBE] = [this](){ return this->Compare(Register8bit::); };
	// CP A
	operations_[0xBF] = [this](){ return this->Compare(Register8bit::A); };
#pragma endregion

#pragma region 0xC0 - 0xCF
	operations_[0xC0] = [this](){ return this->WrongOpCode(); };
	// POP BC
	operations_[0xC1] = [this](){ return this->PopFromStack(Register16bit::BC); };
	operations_[0xC2] = [this](){ return this->WrongOpCode(); };
	operations_[0xC3] = [this](){ return this->WrongOpCode(); };
	operations_[0xC4] = [this](){ return this->WrongOpCode(); };
	// PUSH BC
	operations_[0xC5] = [this](){ return this->PushToStack(Register16bit::BC); };
	// ADD A, n
	operations_[0xC6] = [this](){ return this->WrongOpCode(); };
	operations_[0xC7] = [this](){ return this->WrongOpCode(); };
	operations_[0xC8] = [this](){ return this->WrongOpCode(); };
	operations_[0xC9] = [this](){ return this->WrongOpCode(); };
	operations_[0xCA] = [this](){ return this->WrongOpCode(); };
	operations_[0xCB] = [this](){ return this->WrongOpCode(); };
	operations_[0xCC] = [this](){ return this->WrongOpCode(); };
	operations_[0xCD] = [this](){ return this->WrongOpCode(); };
	// ADC A, n
	operations_[0xCE] = [this](){ return this->WrongOpCode(); };
	operations_[0xCF] = [this](){ return this->WrongOpCode(); };
#pragma endregion

#pragma region 0xD0 - 0xDF
	operations_[0xD0] = [this](){ return this->WrongOpCode(); };
	// POP DE
	operations_[0xD1] = [this](){ return this->PopFromStack(Register16bit::DE); };
	operations_[0xD2] = [this](){ return this->WrongOpCode(); };
	operations_[0xD3] = [this](){ return this->WrongOpCode(); };
	operations_[0xD4] = [this](){ return this->WrongOpCode(); };
	// PUSH DE
	operations_[0xD5] = [this](){ return this->PushToStack(Register16bit::DE); };
	// SUB A, n
	operations_[0xD6] = [this](){ return this->WrongOpCode(); };
	operations_[0xD7] = [this](){ return this->WrongOpCode(); };
	operations_[0xD8] = [this](){ return this->WrongOpCode(); };
	operations_[0xD9] = [this](){ return this->WrongOpCode(); };
	operations_[0xDA] = [this](){ return this->WrongOpCode(); };
	operations_[0xDB] = [this](){ return this->WrongOpCode(); };
	operations_[0xDC] = [this](){ return this->WrongOpCode(); };
	operations_[0xDD] = [this](){ return this->WrongOpCode(); };
	// SBC A, n
	operations_[0xDE] = [this](){ return this->WrongOpCode(); };
	operations_[0xDF] = [this](){ return this->WrongOpCode(); };
#pragma endregion

#pragma region 0xE0 - 0xEF
	operations_[0xE0] = [this](){ return this->WrongOpCode(); };
	// POP HL
	operations_[0xE1] = [this](){ return this->PopFromStack(Register16bit::HL); };
	operations_[0xE2] = [this](){ return this->WrongOpCode(); };
	operations_[0xE3] = [this](){ return this->WrongOpCode(); };
	operations_[0xE4] = [this](){ return this->WrongOpCode(); };
	// PUSH HL
	operations_[0xE5] = [this](){ return this->PushToStack(Register16bit::HL); };
	// AND n
	operations_[0xE6] = [this](){ return this->WrongOpCode(); };
	operations_[0xE7] = [this](){ return this->WrongOpCode(); };
	operations_[0xE8] = [this](){ return this->WrongOpCode(); };
	operations_[0xE9] = [this](){ return this->WrongOpCode(); };
	operations_[0xEA] = [this](){ return this->WrongOpCode(); };
	operations_[0xEB] = [this](){ return this->WrongOpCode(); };
	operations_[0xEC] = [this](){ return this->WrongOpCode(); };
	operations_[0xED] = [this](){ return this->WrongOpCode(); };
	// XOR n
	operations_[0xEE] = [this](){ return this->WrongOpCode(); };
	operations_[0xEF] = [this](){ return this->WrongOpCode(); };
#pragma endregion

#pragma region 0xF0 - 0xFF
	operations_[0xF0] = [this](){ return this->WrongOpCode(); };
	// POP AF
	operations_[0xF1] = [this](){ return this->PopFromStack(Register16bit::AF); };
	operations_[0xF2] = [this](){ return this->WrongOpCode(); };
	operations_[0xF3] = [this](){ return this->WrongOpCode(); };
	operations_[0xF4] = [this](){ return this->WrongOpCode(); };
	// PUSH AF
	operations_[0xF5] = [this](){ return this->PushToStack(Register16bit::AF); };
	// OR n
	operations_[0xF6] = [this](){ return this->WrongOpCode(); };
	operations_[0xF7] = [this](){ return this->WrongOpCode(); };
	operations_[0xF8] = [this](){ return this->WrongOpCode(); };
	// LD SP, HL
	operations_[0xF9] = [this](){ return this->LoadRegister(Register16bit::SP, Register16bit::HL); };
	operations_[0xFA] = [this](){ return this->WrongOpCode(); };
	operations_[0xFB] = [this](){ return this->WrongOpCode(); };
	operations_[0xFC] = [this](){ return this->WrongOpCode(); };
	operations_[0xFD] = [this](){ return this->WrongOpCode(); };
	// CP n
	operations_[0xFE] = [this](){ return this->WrongOpCode(); };
	operations_[0xFF] = [this](){ return this->WrongOpCode(); };
#pragma endregion
}

void Z80::DispatchLoopFunction()
{
	while (exit_loop_.load() == false)
	{
		const auto opcode = FetchByte();
		try
		{
			const auto op_duration = operations_.at(opcode)();
			clock += op_duration;
			//TODO Lapse GPU
		}
		catch (std::out_of_range &)
		{
			std::cout << "Exception caught: No instruction for op code " << std::hex << static_cast<uint32_t>(opcode) << std::endl;
			break;
		}
		catch (std::exception &exc)
		{
			std::cout << "Exception caught: " << exc.what() << std::endl;
			break;
		}
	}
}

uint8_t Z80::FetchByte()
{
	const auto opcode = mmu.Read8bitFromMemory(registers.Read(Register16bit::PC));
	registers.Increment(Register16bit::PC);
	return opcode;
}

Clock Z80::WrongOpCode()
{
	throw std::runtime_error("Wrong operation code");
}

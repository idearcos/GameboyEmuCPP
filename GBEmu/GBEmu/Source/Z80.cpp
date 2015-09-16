#include "Z80.h"

#include <iostream>

Z80::Z80()
{
#pragma region Instructions 0x00 - 0x0F
	// NOP
	instructions_[0x00] = [this](){ return this->NoOperation(); };
	// LD BC, nn
	instructions_[0x01] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::BC, value); };
	// LD (BC), A
	instructions_[0x02] = [this](){ return this->LoadAddressFromRegister(Register16bit::BC, Register8bit::A); };
	// INC BC
	instructions_[0x03] = [this](){ return this->Increment(Register16bit::BC); };
	// INC B
	instructions_[0x04] = [this](){ return this->Increment(Register8bit::B); };
	// DEC B
	instructions_[0x05] = [this](){ return this->Decrement(Register8bit::B); };
	// LD B, n
	instructions_[0x06] = [this](){ return this->LoadRegister(Register8bit::B, this->FetchByte()); };
	// RLCA
	instructions_[0x07] = [this](){ return this->Rlca(); };
	instructions_[0x08] = [this](){ return this->WrongOpCode(); };
	// ADD HL, BC
	instructions_[0x09] = [this](){ return this->Add(Register16bit::HL, Register16bit::BC); };
	// LD A, (BC)
	instructions_[0x0A] = [this](){ return this->LoadRegisterFromAddress(Register8bit::A, Register16bit::BC); };
	// DEC BC
	instructions_[0x0B] = [this](){ return this->Decrement(Register16bit::BC); };
	// INC C
	instructions_[0x0C] = [this](){ return this->Increment(Register8bit::C); };
	// DEC C
	instructions_[0x0D] = [this](){ return this->Decrement(Register8bit::C); };
	// LD C, n
	instructions_[0x0E] = [this](){ return this->LoadRegister(Register8bit::C, this->FetchByte()); };
	// RRCA
	instructions_[0x0F] = [this](){ return this->Rrca(); };
#pragma endregion

#pragma region Instructions 0x10 - 0x1F
	instructions_[0x10] = [this](){ return this->WrongOpCode(); };
	// LD DE, nn
	instructions_[0x11] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::DE, value); };
	// LD (DE), A
	instructions_[0x12] = [this](){ return this->LoadAddressFromRegister(Register16bit::DE, Register8bit::A); };
	// INC DE
	instructions_[0x13] = [this](){ return this->Increment(Register16bit::DE); };
	// INC D
	instructions_[0x14] = [this](){ return this->Increment(Register8bit::D); };
	// DEC D
	instructions_[0x15] = [this](){ return this->Decrement(Register8bit::D); };
	// LD D, n
	instructions_[0x16] = [this](){ return this->LoadRegister(Register8bit::D, this->FetchByte()); };
	// RLA
	instructions_[0x17] = [this](){ return this->Rla(); };
	// JR n
	instructions_[0x18] = [this](){
		return this->Jump(static_cast<int8_t>(this->FetchByte())); };
	// ADD HL, DE
	instructions_[0x19] = [this](){ return this->Add(Register16bit::HL, Register16bit::DE); };
	// LD A, (DE)
	instructions_[0x1A] = [this](){ return this->LoadRegisterFromAddress(Register8bit::A, Register16bit::DE); };
	// DEC DE
	instructions_[0x1B] = [this](){ return this->Decrement(Register16bit::DE); };
	// INC E
	instructions_[0x1C] = [this](){ return this->Increment(Register8bit::E); };
	// DEC E
	instructions_[0x1D] = [this](){ return this->Decrement(Register8bit::E); };
	// LD E, n
	instructions_[0x1E] = [this](){ return this->LoadRegister(Register8bit::E, this->FetchByte()); };
	// RRA
	instructions_[0x1F] = [this](){ return this->Rra(); };
#pragma endregion

#pragma region Instructions 0x20 - 0x2F
	// JR NZ, n
	instructions_[0x20] = [this](){
		return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Zero, false); };
	// LD HL, nn
	instructions_[0x21] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::HL, value); };
	instructions_[0x22] = [this](){ return this->WrongOpCode(); };
	// INC HL
	instructions_[0x23] = [this](){ return this->Increment(Register16bit::HL); };
	// INC H
	instructions_[0x24] = [this](){ return this->Increment(Register8bit::H); };
	// DEC H
	instructions_[0x25] = [this](){ return this->Decrement(Register8bit::H); };
	// LD H, n
	instructions_[0x26] = [this](){ return this->LoadRegister(Register8bit::H, this->FetchByte()); };
	// DAA
	instructions_[0x27] = [this](){ return this->DAA(); };
	// JR Z, n
	instructions_[0x28] = [this](){
		return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Zero, true); };
	// ADD HL, HL
	instructions_[0x29] = [this](){ return this->Add(Register16bit::HL, Register16bit::HL); };
	instructions_[0x2A] = [this](){ return this->WrongOpCode(); };
	// DEC HL
	instructions_[0x2B] = [this](){ return this->Decrement(Register16bit::HL); };
	// INC L
	instructions_[0x2C] = [this](){ return this->Increment(Register8bit::L); };
	// DEC L
	instructions_[0x2D] = [this](){ return this->Decrement(Register8bit::L); };
	// LD L, n
	instructions_[0x2E] = [this](){ return this->LoadRegister(Register8bit::L, this->FetchByte()); };
	// CPL
	instructions_[0x2F] = [this](){ return this->InvertAccumulator(); };
#pragma endregion

#pragma region Instructions 0x30 - 0x3F
	// JR NC, n
	instructions_[0x30] = [this](){
		return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Carry, false); };
	// LD SP, nn
	instructions_[0x31] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::SP, value); };
	// LD (nn), A
	instructions_[0x32] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadAddressFromRegister(address, Register8bit::A); };
	// INC SP
	instructions_[0x33] = [this](){ return this->Increment(Register16bit::SP); };
	// INC (HL)
	instructions_[0x34] = [this](){ return this->IncrementInAddress(Register16bit::HL); };
	// DEC (HL)
	instructions_[0x35] = [this](){ return this->DecrementInAddress(Register16bit::HL); };
	// LD (HL), n
	instructions_[0x36] = [this](){ return this->LoadAddress(Register16bit::HL, this->FetchByte()); };
	// SCF
	instructions_[0x37] = [this](){ return this->SetCarryFlag(); };
	// JR C, n
	instructions_[0x38] = [this](){
		return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Carry, true); };
	// ADD HL, SP
	instructions_[0x39] = [this](){ return this->Add(Register16bit::HL, Register16bit::SP); };
	// LD A, (nn)
	instructions_[0x3A] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegisterFromAddress(Register8bit::A, address); };
	// DEC SP
	instructions_[0x3B] = [this](){ return this->Decrement(Register16bit::SP); };
	// INC A
	instructions_[0x3C] = [this](){ return this->Increment(Register8bit::A); };
	// DEC A
	instructions_[0x3D] = [this](){ return this->Decrement(Register8bit::A); };
	// LD A, n
	instructions_[0x3E] = [this](){ return this->LoadRegister(Register8bit::A, this->FetchByte()); };
	// CCF
	instructions_[0x3F] = [this](){ return this->InvertCarryFlag(); };
#pragma endregion

#pragma region Instructions 0x40 - 0x4F
	// LD B, B
	instructions_[0x40] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::B); };
	// LD B, C
	instructions_[0x41] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::C); };
	// LD B, D
	instructions_[0x42] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::D); };
	// LD B, E
	instructions_[0x43] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::E); };
	// LD B, H
	instructions_[0x44] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::H); };
	// LD B, L
	instructions_[0x45] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::L); };
	// LD B, (HL)
	instructions_[0x46] = [this](){ return this->LoadRegisterFromAddress(Register8bit::B, Register16bit::HL); };
	// LD B, A
	instructions_[0x47] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::A); };
	// LD C, B
	instructions_[0x48] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::B); };
	// LD C, C
	instructions_[0x49] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::C); };
	// LD C, D
	instructions_[0x4A] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::D); };
	// LD C, E
	instructions_[0x4B] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::E); };
	// LD C, H
	instructions_[0x4C] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::H); };
	// LD C, L
	instructions_[0x4D] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::L); };
	// LD C, (HL)
	instructions_[0x4E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::C, Register16bit::HL); };
	// LD C, A
	instructions_[0x4F] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::A); };
#pragma endregion

#pragma region Instructions 0x50 - 0x5F
	// LD D, B
	instructions_[0x50] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::B); };
	// LD D, C
	instructions_[0x51] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::C); };
	// LD D, D
	instructions_[0x52] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::D); };
	// LD D, E
	instructions_[0x53] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::E); };
	// LD D, H
	instructions_[0x54] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::H); };
	// LD D, L
	instructions_[0x55] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::L); };
	// LD D, (HL)
	instructions_[0x56] = [this](){ return this->LoadRegisterFromAddress(Register8bit::D, Register16bit::HL); };
	// LD D, A
	instructions_[0x57] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::A); };
	// LD E, B
	instructions_[0x58] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::B); };
	// LD E, C
	instructions_[0x59] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::C); };
	// LD E, D
	instructions_[0x5A] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::D); };
	// LD E, E
	instructions_[0x5B] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::E); };
	// LD E, H
	instructions_[0x5C] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::H); };
	// LD E, L
	instructions_[0x5D] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::L); };
	// LD E, (HL)
	instructions_[0x5E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::E, Register16bit::HL); };
	// LD E, A
	instructions_[0x5F] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::A); };
#pragma endregion

#pragma region Instructions 0x60 - 0x6F
	// LD H, B
	instructions_[0x60] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::B); };
	// LD H, C
	instructions_[0x61] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::C); };
	// LD H, D
	instructions_[0x62] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::D); };
	// LD H, E
	instructions_[0x63] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::E); };
	// LD H, H
	instructions_[0x64] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::H); };
	// LD H, L
	instructions_[0x65] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::L); };
	// LD H, (HL)
	instructions_[0x66] = [this](){ return this->LoadRegisterFromAddress(Register8bit::H, Register16bit::HL); };
	// LD H, A
	instructions_[0x67] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::A); };
	// LD L, B
	instructions_[0x68] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::B); };
	// LD L, C
	instructions_[0x69] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::C); };
	// LD L, D
	instructions_[0x6A] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::D); };
	// LD L, E
	instructions_[0x6B] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::E); };
	// LD L, H
	instructions_[0x6C] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::H); };
	// LD L, L
	instructions_[0x6D] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::L); };
	// LD L, (HL)
	instructions_[0x6E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::L, Register16bit::HL); };
	// LD L, A
	instructions_[0x6F] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::A); };
#pragma endregion

#pragma region Instructions 0x70 - 0x7F
	// LD (HC), B
	instructions_[0x70] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::B); };
	// LD (HC), C
	instructions_[0x71] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::C); };
	// LD (HC), D
	instructions_[0x72] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::D); };
	// LD (HC), E
	instructions_[0x73] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::E); };
	// LD (HC), H
	instructions_[0x74] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::H); };
	// LD (HC), L
	instructions_[0x75] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::L); };
	// HALT
	instructions_[0x76] = [this](){ return this->Halt(); };
	// LD (HC), A
	instructions_[0x77] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::A); };
	// LD A, B
	instructions_[0x78] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::B); };
	// LD A, C
	instructions_[0x79] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::C); };
	// LD A, D
	instructions_[0x7A] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::D); };
	// LD A, E
	instructions_[0x7B] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::E); };
	// LD A, H
	instructions_[0x7C] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::H); };
	// LD A, L
	instructions_[0x7D] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::L); };
	// LD A, (HL)
	instructions_[0x7E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::A, Register16bit::HL); };
	// LD A, A
	instructions_[0x7F] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::A); };
#pragma endregion

#pragma region Instructions 0x80 - 0x8F
	// ADD A, B
	instructions_[0x80] = [this](){ return this->Add(Register8bit::B); };
	// ADD A, C
	instructions_[0x81] = [this](){ return this->Add(Register8bit::C); };
	// ADD A, D
	instructions_[0x82] = [this](){ return this->Add(Register8bit::D); };
	// ADD A, E
	instructions_[0x83] = [this](){ return this->Add(Register8bit::E); };
	// ADD A, H
	instructions_[0x84] = [this](){ return this->Add(Register8bit::H); };
	// ADD A, L
	instructions_[0x85] = [this](){ return this->Add(Register8bit::L); };
	// ADD A, (HL)
	instructions_[0x86] = [this](){ return this->Add(Register16bit::HL); };
	// ADD A, A
	instructions_[0x87] = [this](){ return this->Add(Register8bit::A); };
	// ADC A, B
	instructions_[0x88] = [this](){ return this->AddPlusCarry(Register8bit::B); };
	// ADC A, C
	instructions_[0x89] = [this](){ return this->AddPlusCarry(Register8bit::C); };
	// ADC A, D
	instructions_[0x8A] = [this](){ return this->AddPlusCarry(Register8bit::D); };
	// ADC A, E
	instructions_[0x8B] = [this](){ return this->AddPlusCarry(Register8bit::E); };
	// ADC A, H
	instructions_[0x8C] = [this](){ return this->AddPlusCarry(Register8bit::H); };
	// ADC A, L
	instructions_[0x8D] = [this](){ return this->AddPlusCarry(Register8bit::L); };
	// ADC A, (HL)
	instructions_[0x8E] = [this](){ return this->AddPlusCarry(Register16bit::HL); };
	// ADC A, A
	instructions_[0x8F] = [this](){ return this->AddPlusCarry(Register8bit::A); };
#pragma endregion

#pragma region Instructions 0x90 - 0x9F
	// SUB A, B
	instructions_[0x90] = [this](){ return this->Sub(Register8bit::B); };
	// SUB A, C
	instructions_[0x91] = [this](){ return this->Sub(Register8bit::C); };
	// SUB A, D
	instructions_[0x92] = [this](){ return this->Sub(Register8bit::D); };
	// SUB A, E
	instructions_[0x93] = [this](){ return this->Sub(Register8bit::E); };
	// SUB A, H
	instructions_[0x94] = [this](){ return this->Sub(Register8bit::H); };
	// SUB A, L
	instructions_[0x95] = [this](){ return this->Sub(Register8bit::L); };
	// SUB A, (HL)
	instructions_[0x96] = [this](){ return this->Sub(Register16bit::HL); };
	// SUB A, A
	instructions_[0x97] = [this](){ return this->Sub(Register8bit::A); };
	// SBC A, B
	instructions_[0x98] = [this](){ return this->SubMinusCarry(Register8bit::B); };
	// SBC A, C
	instructions_[0x99] = [this](){ return this->SubMinusCarry(Register8bit::C); };
	// SBC A, D
	instructions_[0x9A] = [this](){ return this->SubMinusCarry(Register8bit::D); };
	// SBC A, E
	instructions_[0x9B] = [this](){ return this->SubMinusCarry(Register8bit::E); };
	// SBC A, H
	instructions_[0x9C] = [this](){ return this->SubMinusCarry(Register8bit::H); };
	// SBC A, L
	instructions_[0x9D] = [this](){ return this->SubMinusCarry(Register8bit::L); };
	// SBC A, (HL)
	instructions_[0x9E] = [this](){ return this->SubMinusCarry(Register16bit::HL); };
	// SBC A, A
	instructions_[0x9F] = [this](){ return this->SubMinusCarry(Register8bit::A); };
#pragma endregion

#pragma region Instructions 0xA0 - 0xAF
	// AND B
	instructions_[0xA0] = [this](){ return this->And(Register8bit::B); };
	// AND C
	instructions_[0xA1] = [this](){ return this->And(Register8bit::C); };
	// AND D
	instructions_[0xA2] = [this](){ return this->And(Register8bit::D); };
	// AND E
	instructions_[0xA3] = [this](){ return this->And(Register8bit::E); };
	// AND H
	instructions_[0xA4] = [this](){ return this->And(Register8bit::H); };
	// AND L
	instructions_[0xA5] = [this](){ return this->And(Register8bit::L); };
	// AND (HL)
	instructions_[0xA6] = [this](){ return this->And(Register16bit::HL); };
	// AND A
	instructions_[0xA7] = [this](){ return this->And(Register8bit::A); };
	// XOR B
	instructions_[0xA8] = [this](){ return this->Xor(Register8bit::B); };
	// XOR C
	instructions_[0xA9] = [this](){ return this->Xor(Register8bit::C); };
	// XOR D
	instructions_[0xAA] = [this](){ return this->Xor(Register8bit::D); };
	// XOR E
	instructions_[0xAB] = [this](){ return this->Xor(Register8bit::E); };
	// XOR H
	instructions_[0xAC] = [this](){ return this->Xor(Register8bit::H); };
	// XOR L
	instructions_[0xAD] = [this](){ return this->Xor(Register8bit::L); };
	// XOR (HL)
	instructions_[0xAE] = [this](){ return this->Xor(Register16bit::HL); };
	// XOR A
	instructions_[0xAF] = [this](){ return this->Xor(Register8bit::A); };
#pragma endregion

#pragma region Instructions 0xB0 - 0xBF
	// OR B
	instructions_[0xB0] = [this](){ return this->Or(Register8bit::B); };
	// OR C
	instructions_[0xB1] = [this](){ return this->Or(Register8bit::C); };
	// OR D
	instructions_[0xB2] = [this](){ return this->Or(Register8bit::D); };
	// OR E
	instructions_[0xB3] = [this](){ return this->Or(Register8bit::E); };
	// OR H
	instructions_[0xB4] = [this](){ return this->Or(Register8bit::H); };
	// OR L
	instructions_[0xB5] = [this](){ return this->Or(Register8bit::L); };
	// OR (HL)
	instructions_[0xB6] = [this](){ return this->Or(Register16bit::HL); };
	// OR A
	instructions_[0xB7] = [this](){ return this->Or(Register8bit::A); };
	// CP B
	instructions_[0xB8] = [this](){ return this->Compare(Register8bit::B); };
	// CP C
	instructions_[0xB9] = [this](){ return this->Compare(Register8bit::C); };
	// CP D
	instructions_[0xBA] = [this](){ return this->Compare(Register8bit::D); };
	// CP E
	instructions_[0xBB] = [this](){ return this->Compare(Register8bit::E); };
	// CP H
	instructions_[0xBC] = [this](){ return this->Compare(Register8bit::H); };
	// CP L
	instructions_[0xBD] = [this](){ return this->Compare(Register8bit::L); };
	// CP (HL)
	instructions_[0xBE] = [this](){ return this->Compare(Register16bit::HL); };
	// CP A
	instructions_[0xBF] = [this](){ return this->Compare(Register8bit::A); };
#pragma endregion

#pragma region Instructions 0xC0 - 0xCF
	instructions_[0xC0] = [this](){ return this->WrongOpCode(); };
	// POP BC
	instructions_[0xC1] = [this](){ return this->PopFromStack(Register16bit::BC); };
	// JP NZ, nn
	instructions_[0xC2] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->JumpIf(address, Flags::Zero, false); };
	// JP nn
	instructions_[0xC3] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->Jump(address); };
	instructions_[0xC4] = [this](){ return this->WrongOpCode(); };
	// PUSH BC
	instructions_[0xC5] = [this](){ return this->PushToStack(Register16bit::BC); };
	// ADD A, n
	instructions_[0xC6] = [this](){ return this->Add(this->FetchByte()); };
	instructions_[0xC7] = [this](){ return this->WrongOpCode(); };
	instructions_[0xC8] = [this](){ return this->WrongOpCode(); };
	instructions_[0xC9] = [this](){ return this->WrongOpCode(); };
	// JP Z, nn
	instructions_[0xCA] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->JumpIf(address, Flags::Zero, true); };
	// BITS
	instructions_[0xCB] = [this](){ return this->bit_instructions_.at(this->FetchByte())(); };
	instructions_[0xCC] = [this](){ return this->WrongOpCode(); };
	instructions_[0xCD] = [this](){ return this->WrongOpCode(); };
	// ADC A, n
	instructions_[0xCE] = [this](){ return this->AddPlusCarry(this->FetchByte()); };
	instructions_[0xCF] = [this](){ return this->WrongOpCode(); };
#pragma endregion

#pragma region Instructions 0xD0 - 0xDF
	instructions_[0xD0] = [this](){ return this->WrongOpCode(); };
	// POP DE
	instructions_[0xD1] = [this](){ return this->PopFromStack(Register16bit::DE); };
	// JP NC, nn
	instructions_[0xD2] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->JumpIf(address, Flags::Carry, false); };
	instructions_[0xD3] = [this](){ return this->WrongOpCode(); };
	instructions_[0xD4] = [this](){ return this->WrongOpCode(); };
	// PUSH DE
	instructions_[0xD5] = [this](){ return this->PushToStack(Register16bit::DE); };
	// SUB A, n
	instructions_[0xD6] = [this](){ return this->Sub(this->FetchByte()); };
	instructions_[0xD7] = [this](){ return this->WrongOpCode(); };
	instructions_[0xD8] = [this](){ return this->WrongOpCode(); };
	instructions_[0xD9] = [this](){ return this->WrongOpCode(); };
	// JP C, nn
	instructions_[0xDA] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->JumpIf(address, Flags::Carry, true); };
	instructions_[0xDB] = [this](){ return this->WrongOpCode(); };
	instructions_[0xDC] = [this](){ return this->WrongOpCode(); };
	instructions_[0xDD] = [this](){ return this->WrongOpCode(); };
	// SBC A, n
	instructions_[0xDE] = [this](){ return this->SubMinusCarry(this->FetchByte()); };
	instructions_[0xDF] = [this](){ return this->WrongOpCode(); };
#pragma endregion

#pragma region Instructions 0xE0 - 0xEF
	instructions_[0xE0] = [this](){ return this->WrongOpCode(); };
	// POP HL
	instructions_[0xE1] = [this](){ return this->PopFromStack(Register16bit::HL); };
	instructions_[0xE2] = [this](){ return this->WrongOpCode(); };
	instructions_[0xE3] = [this](){ return this->WrongOpCode(); };
	instructions_[0xE4] = [this](){ return this->WrongOpCode(); };
	// PUSH HL
	instructions_[0xE5] = [this](){ return this->PushToStack(Register16bit::HL); };
	// AND n
	instructions_[0xE6] = [this](){ return this->And(this->FetchByte()); };
	instructions_[0xE7] = [this](){ return this->WrongOpCode(); };
	instructions_[0xE8] = [this](){ return this->WrongOpCode(); };
	// JP (HL)
	instructions_[0xE9] = [this](){ return this->Jump(Register16bit::HL); };
	instructions_[0xEA] = [this](){ return this->WrongOpCode(); };
	instructions_[0xEB] = [this](){ return this->WrongOpCode(); };
	instructions_[0xEC] = [this](){ return this->WrongOpCode(); };
	instructions_[0xED] = [this](){ return this->WrongOpCode(); };
	// XOR n
	instructions_[0xEE] = [this](){ return this->Xor(this->FetchByte()); };
	instructions_[0xEF] = [this](){ return this->WrongOpCode(); };
#pragma endregion

#pragma region Instructions 0xF0 - 0xFF
	instructions_[0xF0] = [this](){ return this->WrongOpCode(); };
	// POP AF
	instructions_[0xF1] = [this](){ return this->PopFromStack(Register16bit::AF); };
	instructions_[0xF2] = [this](){ return this->WrongOpCode(); };
	// DI
	instructions_[0xF3] = [this](){ return this->DisableInterrupts(); };
	instructions_[0xF4] = [this](){ return this->WrongOpCode(); };
	// PUSH AF
	instructions_[0xF5] = [this](){ return this->PushToStack(Register16bit::AF); };
	// OR n
	instructions_[0xF6] = [this](){ return this->Or(this->FetchByte()); };
	instructions_[0xF7] = [this](){ return this->WrongOpCode(); };
	instructions_[0xF8] = [this](){ return this->WrongOpCode(); };
	// LD SP, HL
	instructions_[0xF9] = [this](){ return this->LoadRegister(Register16bit::SP, Register16bit::HL); };
	instructions_[0xFA] = [this](){ return this->WrongOpCode(); };
	// EI
	instructions_[0xFB] = [this](){ return this->EnableInterrupts(); };
	instructions_[0xFC] = [this](){ return this->WrongOpCode(); };
	instructions_[0xFD] = [this](){ return this->WrongOpCode(); };
	// CP n
	instructions_[0xFE] = [this](){ return this->Compare(this->FetchByte()); };
	instructions_[0xFF] = [this](){ return this->WrongOpCode(); };
#pragma endregion

#pragma region Bit instructions 0x00 - 0x0F
	bit_instructions_[0x00] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x01] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x02] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x03] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x04] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x05] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x06] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x07] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x08] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x09] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x0A] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x0B] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x0C] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x0D] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x0E] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x0F] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0x10 - 0x1F
	bit_instructions_[0x10] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x11] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x12] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x13] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x14] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x15] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x16] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x17] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x18] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x19] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x1A] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x1B] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x1C] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x1D] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x1E] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x1F] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0x20 - 0x2F
	bit_instructions_[0x20] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x21] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x22] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x23] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x24] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x25] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x26] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x27] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x28] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x29] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x2A] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x2B] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x2C] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x2D] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x2E] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x2F] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0x30 - 0x3F
	bit_instructions_[0x30] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x31] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x32] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x33] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x34] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x35] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x36] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x37] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x38] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x39] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x3A] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x3B] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x3C] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x3D] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x3E] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x3F] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0x40 - 0x4F
	bit_instructions_[0x40] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x41] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x42] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x43] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x44] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x45] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x46] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x47] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x48] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x49] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x4A] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x4B] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x4C] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x4D] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x4E] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x4F] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0x50 - 0x5F
	bit_instructions_[0x50] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x51] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x52] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x53] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x54] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x55] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x56] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x57] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x58] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x59] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x5A] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x5B] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x5C] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x5D] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x5E] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x5F] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0x60 - 0x6F
	bit_instructions_[0x60] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x61] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x62] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x63] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x64] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x65] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x66] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x67] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x68] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x69] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x6A] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x6B] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x6C] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x6D] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x6E] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x6F] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0x70 - 0x7F
	bit_instructions_[0x70] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x71] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x72] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x73] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x74] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x75] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x76] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x77] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x78] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x79] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x7A] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x7B] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x7C] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x7D] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x7E] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x7F] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0x80 - 0x8F
	bit_instructions_[0x80] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x81] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x82] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x83] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x84] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x85] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x86] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x87] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x88] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x89] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x8A] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x8B] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x8C] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x8D] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x8E] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x8F] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0x90 - 0x9F
	bit_instructions_[0x90] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x91] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x92] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x93] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x94] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x95] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x96] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x97] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x98] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x99] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x9A] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x9B] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x9C] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x9D] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x9E] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0x9F] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0xA0 - 0xAF
	bit_instructions_[0xA0] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xA1] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xA2] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xA3] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xA4] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xA5] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xA6] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xA7] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xA8] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xA9] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xAA] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xAB] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xAC] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xAD] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xAE] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xAF] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0xB0 - 0xBF
	bit_instructions_[0xB0] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xB1] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xB2] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xB3] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xB4] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xB5] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xB6] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xB7] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xB8] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xB9] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xBA] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xBB] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xBC] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xBD] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xBE] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xBF] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0xC0 - 0xCF
	bit_instructions_[0xC0] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xC1] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xC2] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xC3] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xC4] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xC5] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xC6] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xC7] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xC8] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xC9] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xCA] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xCB] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xCC] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xCD] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xCE] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xCF] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0xD0 - 0xDF
	bit_instructions_[0xD0] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xD1] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xD2] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xD3] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xD4] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xD5] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xD6] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xD7] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xD8] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xD9] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xDA] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xDB] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xDC] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xDD] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xDE] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xDF] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0xE0 - 0xEF
	bit_instructions_[0xE0] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xE1] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xE2] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xE3] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xE4] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xE5] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xE6] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xE7] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xE8] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xE9] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xEA] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xEB] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xEC] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xED] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xEE] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xEF] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion

#pragma region Bit instructions 0xF0 - 0xFF
	bit_instructions_[0xF0] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xF1] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xF2] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xF3] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xF4] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xF5] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xF6] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xF7] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xF8] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xF9] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xFA] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xFB] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xFC] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xFD] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xFE] = [this](){ return this->WrongBitOpCode(); };
	bit_instructions_[0xFF] = [this](){ return this->WrongBitOpCode(); };
#pragma endregion
}

void Z80::DispatchLoopFunction()
{
	while (exit_loop_.load() == false)
	{
		const auto opcode = FetchByte();
		try
		{
			const auto op_duration = instructions_.at(opcode)();
			clock_ += op_duration;
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
	const auto opcode = mmu_.Read8bitFromMemory(registers_.Read(Register16bit::PC));
	registers_.Increment(Register16bit::PC);
	return opcode;
}

Clock Z80::WrongOpCode()
{
	throw std::runtime_error("Wrong operation code");
}

Clock Z80::WrongBitOpCode()
{
	throw std::runtime_error("Wrong bit operation code");
}

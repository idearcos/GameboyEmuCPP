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
	// LD (nn), SP
	instructions_[0x08] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8; 
		return this->LoadAddressFromRegister(value, Register16bit::SP); };
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
	//TODO STOP
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
	instructions_[0x18] = [this](){ return this->Jump(static_cast<int8_t>(this->FetchByte())); };
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
	instructions_[0x20] = [this](){ return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Zero, false); };
	// LD HL, nn
	instructions_[0x21] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::HL, value); };
	// LD (HL+), A
	instructions_[0x22] = [this](){ return this->LoadAddressAndIncrement(Register16bit::HL, Register8bit::A); };
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
	instructions_[0x28] = [this](){ return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Zero, true); };
	// ADD HL, HL
	instructions_[0x29] = [this](){ return this->Add(Register16bit::HL, Register16bit::HL); };
	// LD A, (HL+)
	instructions_[0x2A] = [this](){ return this->LoadRegisterAndIncrement(Register8bit::A, Register16bit::HL); };
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
	instructions_[0x30] = [this](){ return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Carry, false); };
	// LD SP, nn
	instructions_[0x31] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::SP, value); };
	// LD (HL-), A
	instructions_[0x32] = [this](){ return this->LoadAddressAndDecrement(Register16bit::HL, Register8bit::A); };
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
	instructions_[0x38] = [this](){ return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Carry, true); };
	// ADD HL, SP
	instructions_[0x39] = [this](){ return this->Add(Register16bit::HL, Register16bit::SP); };
	// LD A, (HL-)
	instructions_[0x3A] = [this](){ return this->LoadRegisterAndDecrement(Register8bit::A, Register16bit::HL); };
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
	// RET NZ
	instructions_[0xC0] = [this](){ return this->ReturnIf(Flags::Zero, false); };
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
	// CALL NZ, nn
	instructions_[0xC4] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->CallIf(address, Flags::Zero, false); };
	// PUSH BC
	instructions_[0xC5] = [this](){ return this->PushToStack(Register16bit::BC); };
	// ADD A, n
	instructions_[0xC6] = [this](){ return this->Add(this->FetchByte()); };
	// RST 00h
	instructions_[0xC7] = [this](){ return this->Restart(0x00); };
	// RET Z
	instructions_[0xC8] = [this](){ return this->ReturnIf(Flags::Zero, true); };
	// RET
	instructions_[0xC9] = [this](){ return this->Return(); };
	// JP Z, nn
	instructions_[0xCA] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->JumpIf(address, Flags::Zero, true); };
	// BITS
	instructions_[0xCB] = [this](){ return this->bit_instructions_.at(this->FetchByte())(); };
	// CALL Z, nn
	instructions_[0xCC] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->CallIf(address, Flags::Zero, true); };
	// CALL nn
	instructions_[0xCD] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->Call(address); };
	// ADC A, n
	instructions_[0xCE] = [this](){ return this->AddPlusCarry(this->FetchByte()); };
	// RST 08h
	instructions_[0xCF] = [this](){ return this->Restart(0x08); };
#pragma endregion

#pragma region Instructions 0xD0 - 0xDF
	// RET NC
	instructions_[0xD0] = [this](){ return this->ReturnIf(Flags::Carry, false); };
	// POP DE
	instructions_[0xD1] = [this](){ return this->PopFromStack(Register16bit::DE); };
	// JP NC, nn
	instructions_[0xD2] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->JumpIf(address, Flags::Carry, false); };
	// EMPTY
	instructions_[0xD3] = [this](){ return this->WrongOpCode(); };
	// CALL NC, nn
	instructions_[0xD4] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->CallIf(address, Flags::Carry, false); };
	// PUSH DE
	instructions_[0xD5] = [this](){ return this->PushToStack(Register16bit::DE); };
	// SUB A, n
	instructions_[0xD6] = [this](){ return this->Sub(this->FetchByte()); };
	// RST 10h
	instructions_[0xD7] = [this](){ return this->Restart(0x10); };
	// RET C
	instructions_[0xD8] = [this](){ return this->ReturnIf(Flags::Carry, true); };
	// RETI
	instructions_[0xD9] = [this](){ return this->ReturnFromInterrupt(); };
	// JP C, nn
	instructions_[0xDA] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->JumpIf(address, Flags::Carry, true); };
	// EMPTY
	instructions_[0xDB] = [this](){ return this->WrongOpCode(); };
	// CALL C, nn
	instructions_[0xDC] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->CallIf(address, Flags::Carry, true); };
	// EMPTY
	instructions_[0xDD] = [this](){ return this->WrongOpCode(); };
	// SBC A, n
	instructions_[0xDE] = [this](){ return this->SubMinusCarry(this->FetchByte()); };
	// RST 18h
	instructions_[0xDF] = [this](){ return this->Restart(0x18); };
#pragma endregion

#pragma region Instructions 0xE0 - 0xEF
	// LDIO (n), A
	instructions_[0xE0] = [this](){ return this->LoadIOFromRegister(this->FetchByte(), Register8bit::A); };
	// POP HL
	instructions_[0xE1] = [this](){ return this->PopFromStack(Register16bit::HL); };
	// LDIO (C), A
	instructions_[0xE2] = [this](){ return this->LoadIOFromRegister(Register8bit::C, Register8bit::A); };
	// EMPTY
	instructions_[0xE3] = [this](){ return this->WrongOpCode(); };
	// EMPTY
	instructions_[0xE4] = [this](){ return this->WrongOpCode(); };
	// PUSH HL
	instructions_[0xE5] = [this](){ return this->PushToStack(Register16bit::HL); };
	// AND n
	instructions_[0xE6] = [this](){ return this->And(this->FetchByte()); };
	// RST 20h
	instructions_[0xE7] = [this](){ return this->Restart(0x20); };
	// ADD SP, n
	instructions_[0xE8] = [this](){ return this->Add(Register16bit::SP, this->FetchByte()); };
	// JP (HL)
	instructions_[0xE9] = [this](){ return this->Jump(Register16bit::HL); };
	// LD (nn), A
	instructions_[0xEA] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadAddressFromRegister(address, Register8bit::A); };
	// EMPTY
	instructions_[0xEB] = [this](){ return this->WrongOpCode(); };
	// EMPTY
	instructions_[0xEC] = [this](){ return this->WrongOpCode(); };
	// EMPTY
	instructions_[0xED] = [this](){ return this->WrongOpCode(); };
	// XOR n
	instructions_[0xEE] = [this](){ return this->Xor(this->FetchByte()); };
	// RST 28h
	instructions_[0xEF] = [this](){ return this->Restart(0x28); };
#pragma endregion

#pragma region Instructions 0xF0 - 0xFF
	// LDIO A, (n)
	instructions_[0xF0] = [this](){ return this->LoadRegisterFromIO(Register8bit::A, this->FetchByte()); };
	// POP AF
	instructions_[0xF1] = [this](){ return this->PopFromStack(Register16bit::AF); };
	// LDIO A, (C)
	instructions_[0xF2] = [this](){ return this->LoadRegisterFromIO(Register8bit::A, Register8bit::C); };
	// DI
	instructions_[0xF3] = [this](){ return this->DisableInterrupts(); };
	// EMPTY
	instructions_[0xF4] = [this](){ return this->WrongOpCode(); };
	// PUSH AF
	instructions_[0xF5] = [this](){ return this->PushToStack(Register16bit::AF); };
	// OR n
	instructions_[0xF6] = [this](){ return this->Or(this->FetchByte()); };
	// RST 30h
	instructions_[0xF7] = [this](){ return this->Restart(0x30); };
	// LD HL, SP+n
	instructions_[0xF8] = [this](){ return this->WrongOpCode(); };
	// LD SP, HL
	instructions_[0xF9] = [this](){ return this->LoadRegister(Register16bit::SP, Register16bit::HL); };
	// LD A, (nn)
	instructions_[0xFA] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegisterFromAddress(Register8bit::A, address); };
	// EI
	instructions_[0xFB] = [this](){ return this->EnableInterrupts(); };
	// EMPTY
	instructions_[0xFC] = [this](){ return this->WrongOpCode(); };
	// EMPTY
	instructions_[0xFD] = [this](){ return this->WrongOpCode(); };
	// CP n
	instructions_[0xFE] = [this](){ return this->Compare(this->FetchByte()); };
	// RST 38h
	instructions_[0xFF] = [this](){ return this->Restart(0x38); };
#pragma endregion

#pragma region Bit instructions 0x00 - 0x0F
	// RLC B
	bit_instructions_[0x00] = [this](){ return this->Rlc(Register8bit::B); };
	// RLC C
	bit_instructions_[0x01] = [this](){ return this->Rlc(Register8bit::C); };
	// RLC D
	bit_instructions_[0x02] = [this](){ return this->Rlc(Register8bit::D); };
	// RLC E
	bit_instructions_[0x03] = [this](){ return this->Rlc(Register8bit::E); };
	// RLC H
	bit_instructions_[0x04] = [this](){ return this->Rlc(Register8bit::H); };
	// RLC L
	bit_instructions_[0x05] = [this](){ return this->Rlc(Register8bit::L); };
	// RLC (HL)
	bit_instructions_[0x06] = [this](){ return this->Rlc(Register16bit::HL); };
	// RLC A
	bit_instructions_[0x07] = [this](){ return this->Rlc(Register8bit::A); };
	// RRC B
	bit_instructions_[0x08] = [this](){ return this->Rrc(Register8bit::B); };
	// RRC C
	bit_instructions_[0x09] = [this](){ return this->Rrc(Register8bit::C); };
	// RRC D
	bit_instructions_[0x0A] = [this](){ return this->Rrc(Register8bit::D); };
	// RRC E
	bit_instructions_[0x0B] = [this](){ return this->Rrc(Register8bit::E); };
	// RRC H
	bit_instructions_[0x0C] = [this](){ return this->Rrc(Register8bit::H); };
	// RRC L
	bit_instructions_[0x0D] = [this](){ return this->Rrc(Register8bit::L); };
	// RRC (HL)
	bit_instructions_[0x0E] = [this](){ return this->Rrc(Register16bit::HL); };
	// RRC A
	bit_instructions_[0x0F] = [this](){ return this->Rrc(Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x10 - 0x1F
	// RL B
	bit_instructions_[0x10] = [this](){ return this->Rl(Register8bit::B); };
	// RL C
	bit_instructions_[0x11] = [this](){ return this->Rl(Register8bit::C); };
	// RL D
	bit_instructions_[0x12] = [this](){ return this->Rl(Register8bit::D); };
	// RL E
	bit_instructions_[0x13] = [this](){ return this->Rl(Register8bit::E); };
	// RL H
	bit_instructions_[0x14] = [this](){ return this->Rl(Register8bit::H); };
	// RL L
	bit_instructions_[0x15] = [this](){ return this->Rl(Register8bit::L); };
	// RL (HL)
	bit_instructions_[0x16] = [this](){ return this->Rl(Register16bit::HL); };
	// RL A
	bit_instructions_[0x17] = [this](){ return this->Rl(Register8bit::A); };
	// RR B
	bit_instructions_[0x18] = [this](){ return this->Rr(Register8bit::B); };
	// RR C
	bit_instructions_[0x19] = [this](){ return this->Rr(Register8bit::C); };
	// RR D
	bit_instructions_[0x1A] = [this](){ return this->Rr(Register8bit::D); };
	// RR E
	bit_instructions_[0x1B] = [this](){ return this->Rr(Register8bit::E); };
	// RR H
	bit_instructions_[0x1C] = [this](){ return this->Rr(Register8bit::H); };
	// RR L
	bit_instructions_[0x1D] = [this](){ return this->Rr(Register8bit::L); };
	// RR (HL)
	bit_instructions_[0x1E] = [this](){ return this->Rr(Register16bit::HL); };
	// RR A
	bit_instructions_[0x1F] = [this](){ return this->Rr(Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x20 - 0x2F
	// SLA B
	bit_instructions_[0x20] = [this](){ return this->Sla(Register8bit::B); };
	// SLA C
	bit_instructions_[0x21] = [this](){ return this->Sla(Register8bit::C); };
	// SLA D
	bit_instructions_[0x22] = [this](){ return this->Sla(Register8bit::D); };
	// SLA E
	bit_instructions_[0x23] = [this](){ return this->Sla(Register8bit::E); };
	// SLA H
	bit_instructions_[0x24] = [this](){ return this->Sla(Register8bit::H); };
	// SLA L
	bit_instructions_[0x25] = [this](){ return this->Sla(Register8bit::L); };
	// SLA (HL)
	bit_instructions_[0x26] = [this](){ return this->Sla(Register16bit::HL); };
	// SLA A
	bit_instructions_[0x27] = [this](){ return this->Sla(Register8bit::A); };
	// SRA B
	bit_instructions_[0x28] = [this](){ return this->Sra(Register8bit::B); };
	// SRA C
	bit_instructions_[0x29] = [this](){ return this->Sra(Register8bit::C); };
	// SRA D
	bit_instructions_[0x2A] = [this](){ return this->Sra(Register8bit::D); };
	// SRA E
	bit_instructions_[0x2B] = [this](){ return this->Sra(Register8bit::E); };
	// SRA H
	bit_instructions_[0x2C] = [this](){ return this->Sra(Register8bit::H); };
	// SRA L
	bit_instructions_[0x2D] = [this](){ return this->Sra(Register8bit::L); };
	// SRA (HL)
	bit_instructions_[0x2E] = [this](){ return this->Sra(Register16bit::HL); };
	// SRA A
	bit_instructions_[0x2F] = [this](){ return this->Sra(Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x30 - 0x3F
	// SWAP B
	bit_instructions_[0x30] = [this](){ return this->Swap(Register8bit::B); };
	// SWAP C
	bit_instructions_[0x31] = [this](){ return this->Swap(Register8bit::C); };
	// SWAP D
	bit_instructions_[0x32] = [this](){ return this->Swap(Register8bit::D); };
	// SWAP E
	bit_instructions_[0x33] = [this](){ return this->Swap(Register8bit::E); };
	// SWAP H
	bit_instructions_[0x34] = [this](){ return this->Swap(Register8bit::H); };
	// SWAP L
	bit_instructions_[0x35] = [this](){ return this->Swap(Register8bit::L); };
	// SWAP (HL)
	bit_instructions_[0x36] = [this](){ return this->Swap(Register16bit::HL); };
	// SWAP A
	bit_instructions_[0x37] = [this](){ return this->Swap(Register8bit::A); };
	// SRL B
	bit_instructions_[0x38] = [this](){ return this->Srl(Register8bit::B); };
	// SRL C
	bit_instructions_[0x39] = [this](){ return this->Srl(Register8bit::C); };
	// SRL D
	bit_instructions_[0x3A] = [this](){ return this->Srl(Register8bit::D); };
	// SRL E
	bit_instructions_[0x3B] = [this](){ return this->Srl(Register8bit::E); };
	// SRL H
	bit_instructions_[0x3C] = [this](){ return this->Srl(Register8bit::H); };
	// SRL L
	bit_instructions_[0x3D] = [this](){ return this->Srl(Register8bit::L); };
	// SRL (HL)
	bit_instructions_[0x3E] = [this](){ return this->Srl(Register16bit::HL); };
	// SRL A
	bit_instructions_[0x3F] = [this](){ return this->Srl(Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x40 - 0x4F
	// BIT 0, B
	bit_instructions_[0x40] = [this](){ return this->TestBit(0, Register8bit::B); };
	// BIT 0, C
	bit_instructions_[0x41] = [this](){ return this->TestBit(0, Register8bit::C); };
	// BIT 0, D
	bit_instructions_[0x42] = [this](){ return this->TestBit(0, Register8bit::D); };
	// BIT 0, E
	bit_instructions_[0x43] = [this](){ return this->TestBit(0, Register8bit::E); };
	// BIT 0, H
	bit_instructions_[0x44] = [this](){ return this->TestBit(0, Register8bit::H); };
	// BIT 0, L
	bit_instructions_[0x45] = [this](){ return this->TestBit(0, Register8bit::L); };
	// BIT 0, (HL)
	bit_instructions_[0x46] = [this](){ return this->TestBit(0, Register16bit::HL); };
	// BIT 0, A
	bit_instructions_[0x47] = [this](){ return this->TestBit(0, Register8bit::A); };
	// BIT 1, B
	bit_instructions_[0x48] = [this](){ return this->TestBit(1, Register8bit::B); };
	// BIT 1, C
	bit_instructions_[0x49] = [this](){ return this->TestBit(1, Register8bit::C); };
	// BIT 1, D
	bit_instructions_[0x4A] = [this](){ return this->TestBit(1, Register8bit::D); };
	// BIT 1, E
	bit_instructions_[0x4B] = [this](){ return this->TestBit(1, Register8bit::E); };
	// BIT 1, H
	bit_instructions_[0x4C] = [this](){ return this->TestBit(1, Register8bit::H); };
	// BIT 1, L
	bit_instructions_[0x4D] = [this](){ return this->TestBit(1, Register8bit::L); };
	// BIT 1, (HL)
	bit_instructions_[0x4E] = [this](){ return this->TestBit(1, Register16bit::HL); };
	// BIT 1, A
	bit_instructions_[0x4F] = [this](){ return this->TestBit(1, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x50 - 0x5F
	// BIT 2, B
	bit_instructions_[0x50] = [this](){ return this->TestBit(2, Register8bit::B); };
	// BIT 2, C
	bit_instructions_[0x51] = [this](){ return this->TestBit(2, Register8bit::C); };
	// BIT 2, D
	bit_instructions_[0x52] = [this](){ return this->TestBit(2, Register8bit::D); };
	// BIT 2, E
	bit_instructions_[0x53] = [this](){ return this->TestBit(2, Register8bit::E); };
	// BIT 2, H
	bit_instructions_[0x54] = [this](){ return this->TestBit(2, Register8bit::H); };
	// BIT 2, L
	bit_instructions_[0x55] = [this](){ return this->TestBit(2, Register8bit::L); };
	// BIT 2, (HL)
	bit_instructions_[0x56] = [this](){ return this->TestBit(2, Register16bit::HL); };
	// BIT 2, A
	bit_instructions_[0x57] = [this](){ return this->TestBit(2, Register8bit::A); };
	// BIT 3, B
	bit_instructions_[0x58] = [this](){ return this->TestBit(3, Register8bit::B); };
	// BIT 3, C
	bit_instructions_[0x59] = [this](){ return this->TestBit(3, Register8bit::C); };
	// BIT 3, D
	bit_instructions_[0x5A] = [this](){ return this->TestBit(3, Register8bit::D); };
	// BIT 3, E
	bit_instructions_[0x5B] = [this](){ return this->TestBit(3, Register8bit::E); };
	// BIT 3, H
	bit_instructions_[0x5C] = [this](){ return this->TestBit(3, Register8bit::H); };
	// BIT 3, L
	bit_instructions_[0x5D] = [this](){ return this->TestBit(3, Register8bit::L); };
	// BIT 3, (HL)
	bit_instructions_[0x5E] = [this](){ return this->TestBit(3, Register16bit::HL); };
	// BIT 3, A
	bit_instructions_[0x5F] = [this](){ return this->TestBit(3, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x60 - 0x6F
	// BIT 4, B
	bit_instructions_[0x60] = [this](){ return this->TestBit(4, Register8bit::B); };
	// BIT 4, C
	bit_instructions_[0x61] = [this](){ return this->TestBit(4, Register8bit::C); };
	// BIT 4, D
	bit_instructions_[0x62] = [this](){ return this->TestBit(4, Register8bit::D); };
	// BIT 4, E
	bit_instructions_[0x63] = [this](){ return this->TestBit(4, Register8bit::E); };
	// BIT 4, H
	bit_instructions_[0x64] = [this](){ return this->TestBit(4, Register8bit::H); };
	// BIT 4, L
	bit_instructions_[0x65] = [this](){ return this->TestBit(4, Register8bit::L); };
	// BIT 4, (HL)
	bit_instructions_[0x66] = [this](){ return this->TestBit(4, Register16bit::HL); };
	// BIT 4, A
	bit_instructions_[0x67] = [this](){ return this->TestBit(4, Register8bit::A); };
	// BIT 5, B
	bit_instructions_[0x68] = [this](){ return this->TestBit(5, Register8bit::B); };
	// BIT 5, C
	bit_instructions_[0x69] = [this](){ return this->TestBit(5, Register8bit::C); };
	// BIT 5, D
	bit_instructions_[0x6A] = [this](){ return this->TestBit(5, Register8bit::D); };
	// BIT 5, E
	bit_instructions_[0x6B] = [this](){ return this->TestBit(5, Register8bit::E); };
	// BIT 5, H
	bit_instructions_[0x6C] = [this](){ return this->TestBit(5, Register8bit::H); };
	// BIT 5, L
	bit_instructions_[0x6D] = [this](){ return this->TestBit(5, Register8bit::L); };
	// BIT 5, (HL)
	bit_instructions_[0x6E] = [this](){ return this->TestBit(5, Register16bit::HL); };
	// BIT 5, A
	bit_instructions_[0x6F] = [this](){ return this->TestBit(5, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x70 - 0x7F
	// BIT 6, B
	bit_instructions_[0x70] = [this](){ return this->TestBit(6, Register8bit::B); };
	// BIT 6, C
	bit_instructions_[0x71] = [this](){ return this->TestBit(6, Register8bit::C); };
	// BIT 6, D
	bit_instructions_[0x72] = [this](){ return this->TestBit(6, Register8bit::D); };
	// BIT 6, E
	bit_instructions_[0x73] = [this](){ return this->TestBit(6, Register8bit::E); };
	// BIT 6, H
	bit_instructions_[0x74] = [this](){ return this->TestBit(6, Register8bit::H); };
	// BIT 6, L
	bit_instructions_[0x75] = [this](){ return this->TestBit(6, Register8bit::L); };
	// BIT 6, (HL)
	bit_instructions_[0x76] = [this](){ return this->TestBit(6, Register16bit::HL); };
	// BIT 6, A
	bit_instructions_[0x77] = [this](){ return this->TestBit(6, Register8bit::A); };
	// BIT 7, B
	bit_instructions_[0x78] = [this](){ return this->TestBit(7, Register8bit::B); };
	// BIT 7, C
	bit_instructions_[0x79] = [this](){ return this->TestBit(7, Register8bit::C); };
	// BIT 7, D
	bit_instructions_[0x7A] = [this](){ return this->TestBit(7, Register8bit::D); };
	// BIT 7, E
	bit_instructions_[0x7B] = [this](){ return this->TestBit(7, Register8bit::E); };
	// BIT 7, H
	bit_instructions_[0x7C] = [this](){ return this->TestBit(7, Register8bit::H); };
	// BIT 7, L
	bit_instructions_[0x7D] = [this](){ return this->TestBit(7, Register8bit::L); };
	// BIT 7, (HL)
	bit_instructions_[0x7E] = [this](){ return this->TestBit(7, Register16bit::HL); };
	// BIT 7, A
	bit_instructions_[0x7F] = [this](){ return this->TestBit(7, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x80 - 0x8F
	// RES 0, B
	bit_instructions_[0xC0] = [this](){ return this->ResetBit(0, Register8bit::B); };
	// RES 0, C
	bit_instructions_[0xC1] = [this](){ return this->ResetBit(0, Register8bit::C); };
	// RES 0, D
	bit_instructions_[0xC2] = [this](){ return this->ResetBit(0, Register8bit::D); };
	// RES 0, E
	bit_instructions_[0xC3] = [this](){ return this->ResetBit(0, Register8bit::E); };
	// RES 0, H
	bit_instructions_[0xC4] = [this](){ return this->ResetBit(0, Register8bit::H); };
	// RES 0, L
	bit_instructions_[0xC5] = [this](){ return this->ResetBit(0, Register8bit::L); };
	// RES 0, (HL)
	bit_instructions_[0xC6] = [this](){ return this->ResetBit(0, Register16bit::HL); };
	// RES 0, A
	bit_instructions_[0xC7] = [this](){ return this->ResetBit(0, Register8bit::A); };
	// RES 1, B
	bit_instructions_[0xC8] = [this](){ return this->ResetBit(1, Register8bit::B); };
	// RES 1, C
	bit_instructions_[0xC9] = [this](){ return this->ResetBit(1, Register8bit::C); };
	// RES 1, D
	bit_instructions_[0xCA] = [this](){ return this->ResetBit(1, Register8bit::D); };
	// RES 1, E
	bit_instructions_[0xCB] = [this](){ return this->ResetBit(1, Register8bit::E); };
	// RES 1, H
	bit_instructions_[0xCC] = [this](){ return this->ResetBit(1, Register8bit::H); };
	// RES 1, L
	bit_instructions_[0xCD] = [this](){ return this->ResetBit(1, Register8bit::L); };
	// RES 1, (HL)
	bit_instructions_[0xCE] = [this](){ return this->ResetBit(1, Register16bit::HL); };
	// RES 1, A
	bit_instructions_[0xCF] = [this](){ return this->ResetBit(1, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0x90 - 0x9F
	// RES 2, B
	bit_instructions_[0xD0] = [this](){ return this->ResetBit(2, Register8bit::B); };
	// RES 2, C
	bit_instructions_[0xD1] = [this](){ return this->ResetBit(2, Register8bit::C); };
	// RES 2, D
	bit_instructions_[0xD2] = [this](){ return this->ResetBit(2, Register8bit::D); };
	// RES 2, E
	bit_instructions_[0xD3] = [this](){ return this->ResetBit(2, Register8bit::E); };
	// RES 2, H
	bit_instructions_[0xD4] = [this](){ return this->ResetBit(2, Register8bit::H); };
	// RES 2, L
	bit_instructions_[0xD5] = [this](){ return this->ResetBit(2, Register8bit::L); };
	// RES 2, (HL)
	bit_instructions_[0xD6] = [this](){ return this->ResetBit(2, Register16bit::HL); };
	// RES 2, A
	bit_instructions_[0xD7] = [this](){ return this->ResetBit(2, Register8bit::A); };
	// RES 3, B
	bit_instructions_[0xD8] = [this](){ return this->ResetBit(3, Register8bit::B); };
	// RES 3, C
	bit_instructions_[0xD9] = [this](){ return this->ResetBit(3, Register8bit::C); };
	// RES 3, D
	bit_instructions_[0xDA] = [this](){ return this->ResetBit(3, Register8bit::D); };
	// RES 3, E
	bit_instructions_[0xDB] = [this](){ return this->ResetBit(3, Register8bit::E); };
	// RES 3, H
	bit_instructions_[0xDC] = [this](){ return this->ResetBit(3, Register8bit::H); };
	// RES 3, L
	bit_instructions_[0xDD] = [this](){ return this->ResetBit(3, Register8bit::L); };
	// RES 3, (HL)
	bit_instructions_[0xDE] = [this](){ return this->ResetBit(3, Register16bit::HL); };
	// RES 3, A
	bit_instructions_[0xDF] = [this](){ return this->ResetBit(3, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xA0 - 0xAF
	// RES 4, B
	bit_instructions_[0xE0] = [this](){ return this->ResetBit(4, Register8bit::B); };
	// RES 4, C
	bit_instructions_[0xE1] = [this](){ return this->ResetBit(4, Register8bit::C); };
	// RES 4, D
	bit_instructions_[0xE2] = [this](){ return this->ResetBit(4, Register8bit::D); };
	// RES 4, E
	bit_instructions_[0xE3] = [this](){ return this->ResetBit(4, Register8bit::E); };
	// RES 4, H
	bit_instructions_[0xE4] = [this](){ return this->ResetBit(4, Register8bit::H); };
	// RES 4, L
	bit_instructions_[0xE5] = [this](){ return this->ResetBit(4, Register8bit::L); };
	// RES 4, (HL)
	bit_instructions_[0xE6] = [this](){ return this->ResetBit(4, Register16bit::HL); };
	// RES 4, A
	bit_instructions_[0xE7] = [this](){ return this->ResetBit(4, Register8bit::A); };
	// RES 5, B
	bit_instructions_[0xE8] = [this](){ return this->ResetBit(5, Register8bit::B); };
	// RES 5, C
	bit_instructions_[0xE9] = [this](){ return this->ResetBit(5, Register8bit::C); };
	// RES 5, D
	bit_instructions_[0xEA] = [this](){ return this->ResetBit(5, Register8bit::D); };
	// RES 5, E
	bit_instructions_[0xEB] = [this](){ return this->ResetBit(5, Register8bit::E); };
	// RES 5, H
	bit_instructions_[0xEC] = [this](){ return this->ResetBit(5, Register8bit::H); };
	// RES 5, L
	bit_instructions_[0xED] = [this](){ return this->ResetBit(5, Register8bit::L); };
	// RES 5, (HL)
	bit_instructions_[0xEE] = [this](){ return this->ResetBit(5, Register16bit::HL); };
	// RES 5, A
	bit_instructions_[0xEF] = [this](){ return this->ResetBit(5, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xB0 - 0xBF
	// RES 6, B
	bit_instructions_[0xF0] = [this](){ return this->ResetBit(6, Register8bit::B); };
	// RES 6, C
	bit_instructions_[0xF1] = [this](){ return this->ResetBit(6, Register8bit::C); };
	// RES 6, D
	bit_instructions_[0xF2] = [this](){ return this->ResetBit(6, Register8bit::D); };
	// RES 6, E
	bit_instructions_[0xF3] = [this](){ return this->ResetBit(6, Register8bit::E); };
	// RES 6, H
	bit_instructions_[0xF4] = [this](){ return this->ResetBit(6, Register8bit::H); };
	// RES 6, L
	bit_instructions_[0xF5] = [this](){ return this->ResetBit(6, Register8bit::L); };
	// RES 6, (HL)
	bit_instructions_[0xF6] = [this](){ return this->ResetBit(6, Register16bit::HL); };
	// RES 6, A
	bit_instructions_[0xF7] = [this](){ return this->ResetBit(6, Register8bit::A); };
	// RES 7, B
	bit_instructions_[0xF8] = [this](){ return this->ResetBit(7, Register8bit::B); };
	// RES 7, C
	bit_instructions_[0xF9] = [this](){ return this->ResetBit(7, Register8bit::C); };
	// RES 7, D
	bit_instructions_[0xFA] = [this](){ return this->ResetBit(7, Register8bit::D); };
	// RES 7, E
	bit_instructions_[0xFB] = [this](){ return this->ResetBit(7, Register8bit::E); };
	// RES 7, H
	bit_instructions_[0xFC] = [this](){ return this->ResetBit(7, Register8bit::H); };
	// RES 7, L
	bit_instructions_[0xFD] = [this](){ return this->ResetBit(7, Register8bit::L); };
	// RES 7, (HL)
	bit_instructions_[0xFE] = [this](){ return this->ResetBit(7, Register16bit::HL); };
	// RES 7, A
	bit_instructions_[0xFF] = [this](){ return this->ResetBit(7, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xC0 - 0xCF
	// SET 0, B
	bit_instructions_[0xC0] = [this](){ return this->SetBit(0, Register8bit::B); };
	// SET 0, C
	bit_instructions_[0xC1] = [this](){ return this->SetBit(0, Register8bit::C); };
	// SET 0, D
	bit_instructions_[0xC2] = [this](){ return this->SetBit(0, Register8bit::D); };
	// SET 0, E
	bit_instructions_[0xC3] = [this](){ return this->SetBit(0, Register8bit::E); };
	// SET 0, H
	bit_instructions_[0xC4] = [this](){ return this->SetBit(0, Register8bit::H); };
	// SET 0, L
	bit_instructions_[0xC5] = [this](){ return this->SetBit(0, Register8bit::L); };
	// SET 0, (HL)
	bit_instructions_[0xC6] = [this](){ return this->SetBit(0, Register16bit::HL); };
	// SET 0, A
	bit_instructions_[0xC7] = [this](){ return this->SetBit(0, Register8bit::A); };
	// SET 1, B
	bit_instructions_[0xC8] = [this](){ return this->SetBit(1, Register8bit::B); };
	// SET 1, C
	bit_instructions_[0xC9] = [this](){ return this->SetBit(1, Register8bit::C); };
	// SET 1, D
	bit_instructions_[0xCA] = [this](){ return this->SetBit(1, Register8bit::D); };
	// SET 1, E
	bit_instructions_[0xCB] = [this](){ return this->SetBit(1, Register8bit::E); };
	// SET 1, H
	bit_instructions_[0xCC] = [this](){ return this->SetBit(1, Register8bit::H); };
	// SET 1, L
	bit_instructions_[0xCD] = [this](){ return this->SetBit(1, Register8bit::L); };
	// SET 1, (HL)
	bit_instructions_[0xCE] = [this](){ return this->SetBit(1, Register16bit::HL); };
	// SET 1, A
	bit_instructions_[0xCF] = [this](){ return this->SetBit(1, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xD0 - 0xDF
	// SET 2, B
	bit_instructions_[0xD0] = [this](){ return this->SetBit(2, Register8bit::B); };
	// SET 2, C
	bit_instructions_[0xD1] = [this](){ return this->SetBit(2, Register8bit::C); };
	// SET 2, D
	bit_instructions_[0xD2] = [this](){ return this->SetBit(2, Register8bit::D); };
	// SET 2, E
	bit_instructions_[0xD3] = [this](){ return this->SetBit(2, Register8bit::E); };
	// SET 2, H
	bit_instructions_[0xD4] = [this](){ return this->SetBit(2, Register8bit::H); };
	// SET 2, L
	bit_instructions_[0xD5] = [this](){ return this->SetBit(2, Register8bit::L); };
	// SET 2, (HL)
	bit_instructions_[0xD6] = [this](){ return this->SetBit(2, Register16bit::HL); };
	// SET 2, A
	bit_instructions_[0xD7] = [this](){ return this->SetBit(2, Register8bit::A); };
	// SET 3, B
	bit_instructions_[0xD8] = [this](){ return this->SetBit(3, Register8bit::B); };
	// SET 3, C
	bit_instructions_[0xD9] = [this](){ return this->SetBit(3, Register8bit::C); };
	// SET 3, D
	bit_instructions_[0xDA] = [this](){ return this->SetBit(3, Register8bit::D); };
	// SET 3, E
	bit_instructions_[0xDB] = [this](){ return this->SetBit(3, Register8bit::E); };
	// SET 3, H
	bit_instructions_[0xDC] = [this](){ return this->SetBit(3, Register8bit::H); };
	// SET 3, L
	bit_instructions_[0xDD] = [this](){ return this->SetBit(3, Register8bit::L); };
	// SET 3, (HL)
	bit_instructions_[0xDE] = [this](){ return this->SetBit(3, Register16bit::HL); };
	// SET 3, A
	bit_instructions_[0xDF] = [this](){ return this->SetBit(3, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xE0 - 0xEF
	// SET 4, B
	bit_instructions_[0xE0] = [this](){ return this->SetBit(4, Register8bit::B); };
	// SET 4, C
	bit_instructions_[0xE1] = [this](){ return this->SetBit(4, Register8bit::C); };
	// SET 4, D
	bit_instructions_[0xE2] = [this](){ return this->SetBit(4, Register8bit::D); };
	// SET 4, E
	bit_instructions_[0xE3] = [this](){ return this->SetBit(4, Register8bit::E); };
	// SET 4, H
	bit_instructions_[0xE4] = [this](){ return this->SetBit(4, Register8bit::H); };
	// SET 4, L
	bit_instructions_[0xE5] = [this](){ return this->SetBit(4, Register8bit::L); };
	// SET 4, (HL)
	bit_instructions_[0xE6] = [this](){ return this->SetBit(4, Register16bit::HL); };
	// SET 4, A
	bit_instructions_[0xE7] = [this](){ return this->SetBit(4, Register8bit::A); };
	// SET 5, B
	bit_instructions_[0xE8] = [this](){ return this->SetBit(5, Register8bit::B); };
	// SET 5, C
	bit_instructions_[0xE9] = [this](){ return this->SetBit(5, Register8bit::C); };
	// SET 5, D
	bit_instructions_[0xEA] = [this](){ return this->SetBit(5, Register8bit::D); };
	// SET 5, E
	bit_instructions_[0xEB] = [this](){ return this->SetBit(5, Register8bit::E); };
	// SET 5, H
	bit_instructions_[0xEC] = [this](){ return this->SetBit(5, Register8bit::H); };
	// SET 5, L
	bit_instructions_[0xED] = [this](){ return this->SetBit(5, Register8bit::L); };
	// SET 5, (HL)
	bit_instructions_[0xEE] = [this](){ return this->SetBit(5, Register16bit::HL); };
	// SET 5, A
	bit_instructions_[0xEF] = [this](){ return this->SetBit(5, Register8bit::A); };
#pragma endregion

#pragma region Bit instructions 0xF0 - 0xFF
	// SET 6, B
	bit_instructions_[0xF0] = [this](){ return this->SetBit(6, Register8bit::B); };
	// SET 6, C
	bit_instructions_[0xF1] = [this](){ return this->SetBit(6, Register8bit::C); };
	// SET 6, D
	bit_instructions_[0xF2] = [this](){ return this->SetBit(6, Register8bit::D); };
	// SET 6, E
	bit_instructions_[0xF3] = [this](){ return this->SetBit(6, Register8bit::E); };
	// SET 6, H
	bit_instructions_[0xF4] = [this](){ return this->SetBit(6, Register8bit::H); };
	// SET 6, L
	bit_instructions_[0xF5] = [this](){ return this->SetBit(6, Register8bit::L); };
	// SET 6, (HL)
	bit_instructions_[0xF6] = [this](){ return this->SetBit(6, Register16bit::HL); };
	// SET 6, A
	bit_instructions_[0xF7] = [this](){ return this->SetBit(6, Register8bit::A); };
	// SET 7, B
	bit_instructions_[0xF8] = [this](){ return this->SetBit(7, Register8bit::B); };
	// SET 7, C
	bit_instructions_[0xF9] = [this](){ return this->SetBit(7, Register8bit::C); };
	// SET 7, D
	bit_instructions_[0xFA] = [this](){ return this->SetBit(7, Register8bit::D); };
	// SET 7, E
	bit_instructions_[0xFB] = [this](){ return this->SetBit(7, Register8bit::E); };
	// SET 7, H
	bit_instructions_[0xFC] = [this](){ return this->SetBit(7, Register8bit::H); };
	// SET 7, L
	bit_instructions_[0xFD] = [this](){ return this->SetBit(7, Register8bit::L); };
	// SET 7, (HL)
	bit_instructions_[0xFE] = [this](){ return this->SetBit(7, Register16bit::HL); };
	// SET 7, A
	bit_instructions_[0xFF] = [this](){ return this->SetBit(7, Register8bit::A); };
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

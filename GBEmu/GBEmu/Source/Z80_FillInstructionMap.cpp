#include "Z80.h"

InstructionMap Z80::FillInstructionMap()
{
	InstructionMap instructions;

#pragma region Instructions 0x00 - 0x0F
	// NOP
	instructions[0x00] = [this](){ return this->NoOperation(); };
	// LD BC, nn
	instructions[0x01] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::BC, value); };
	// LD (BC), A
	instructions[0x02] = [this](){ return this->LoadAddressFromRegister(Register16bit::BC, Register8bit::A); };
	// INC BC
	instructions[0x03] = [this](){ return this->Increment(Register16bit::BC); };
	// INC B
	instructions[0x04] = [this](){ return this->Increment(Register8bit::B); };
	// DEC B
	instructions[0x05] = [this](){ return this->Decrement(Register8bit::B); };
	// LD B, n
	instructions[0x06] = [this](){ return this->LoadRegister(Register8bit::B, this->FetchByte()); };
	// RLCA
	instructions[0x07] = [this](){ return this->Rlca(); };
	// LD (nn), SP
	instructions[0x08] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadAddressFromRegister(value, Register16bit::SP); };
	// ADD HL, BC
	instructions[0x09] = [this](){ return this->Add(Register16bit::HL, Register16bit::BC); };
	// LD A, (BC)
	instructions[0x0A] = [this](){ return this->LoadRegisterFromAddress(Register8bit::A, Register16bit::BC); };
	// DEC BC
	instructions[0x0B] = [this](){ return this->Decrement(Register16bit::BC); };
	// INC C
	instructions[0x0C] = [this](){ return this->Increment(Register8bit::C); };
	// DEC C
	instructions[0x0D] = [this](){ return this->Decrement(Register8bit::C); };
	// LD C, n
	instructions[0x0E] = [this](){ return this->LoadRegister(Register8bit::C, this->FetchByte()); };
	// RRCA
	instructions[0x0F] = [this](){ return this->Rrca(); };
#pragma endregion

#pragma region Instructions 0x10 - 0x1F
	//TODO STOP
	instructions[0x10] = [this](){ return this->WrongOpCode(0x10); };
	// LD DE, nn
	instructions[0x11] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::DE, value); };
	// LD (DE), A
	instructions[0x12] = [this](){ return this->LoadAddressFromRegister(Register16bit::DE, Register8bit::A); };
	// INC DE
	instructions[0x13] = [this](){ return this->Increment(Register16bit::DE); };
	// INC D
	instructions[0x14] = [this](){ return this->Increment(Register8bit::D); };
	// DEC D
	instructions[0x15] = [this](){ return this->Decrement(Register8bit::D); };
	// LD D, n
	instructions[0x16] = [this](){ return this->LoadRegister(Register8bit::D, this->FetchByte()); };
	// RLA
	instructions[0x17] = [this](){ return this->Rla(); };
	// JR n
	instructions[0x18] = [this](){ return this->Jump(static_cast<int8_t>(this->FetchByte())); };
	// ADD HL, DE
	instructions[0x19] = [this](){ return this->Add(Register16bit::HL, Register16bit::DE); };
	// LD A, (DE)
	instructions[0x1A] = [this](){ return this->LoadRegisterFromAddress(Register8bit::A, Register16bit::DE); };
	// DEC DE
	instructions[0x1B] = [this](){ return this->Decrement(Register16bit::DE); };
	// INC E
	instructions[0x1C] = [this](){ return this->Increment(Register8bit::E); };
	// DEC E
	instructions[0x1D] = [this](){ return this->Decrement(Register8bit::E); };
	// LD E, n
	instructions[0x1E] = [this](){ return this->LoadRegister(Register8bit::E, this->FetchByte()); };
	// RRA
	instructions[0x1F] = [this](){ return this->Rra(); };
#pragma endregion

#pragma region Instructions 0x20 - 0x2F
	// JR NZ, n
	instructions[0x20] = [this](){ return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Zero, false); };
	// LD HL, nn
	instructions[0x21] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::HL, value); };
	// LD (HL+), A
	instructions[0x22] = [this](){ return this->LoadAddressAndIncrement(Register16bit::HL, Register8bit::A); };
	// INC HL
	instructions[0x23] = [this](){ return this->Increment(Register16bit::HL); };
	// INC H
	instructions[0x24] = [this](){ return this->Increment(Register8bit::H); };
	// DEC H
	instructions[0x25] = [this](){ return this->Decrement(Register8bit::H); };
	// LD H, n
	instructions[0x26] = [this](){ return this->LoadRegister(Register8bit::H, this->FetchByte()); };
	// DAA
	instructions[0x27] = [this](){ return this->DAA(); };
	// JR Z, n
	instructions[0x28] = [this](){ return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Zero, true); };
	// ADD HL, HL
	instructions[0x29] = [this](){ return this->Add(Register16bit::HL, Register16bit::HL); };
	// LD A, (HL+)
	instructions[0x2A] = [this](){ return this->LoadRegisterAndIncrement(Register8bit::A, Register16bit::HL); };
	// DEC HL
	instructions[0x2B] = [this](){ return this->Decrement(Register16bit::HL); };
	// INC L
	instructions[0x2C] = [this](){ return this->Increment(Register8bit::L); };
	// DEC L
	instructions[0x2D] = [this](){ return this->Decrement(Register8bit::L); };
	// LD L, n
	instructions[0x2E] = [this](){ return this->LoadRegister(Register8bit::L, this->FetchByte()); };
	// CPL
	instructions[0x2F] = [this](){ return this->InvertAccumulator(); };
#pragma endregion

#pragma region Instructions 0x30 - 0x3F
	// JR NC, n
	instructions[0x30] = [this](){ return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Carry, false); };
	// LD SP, nn
	instructions[0x31] = [this](){
		uint16_t value{ this->FetchByte() };
		value += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegister(Register16bit::SP, value); };
	// LD (HL-), A
	instructions[0x32] = [this](){ return this->LoadAddressAndDecrement(Register16bit::HL, Register8bit::A); };
	// INC SP
	instructions[0x33] = [this](){ return this->Increment(Register16bit::SP); };
	// INC (HL)
	instructions[0x34] = [this](){ return this->IncrementInAddress(Register16bit::HL); };
	// DEC (HL)
	instructions[0x35] = [this](){ return this->DecrementInAddress(Register16bit::HL); };
	// LD (HL), n
	instructions[0x36] = [this](){ return this->LoadAddress(Register16bit::HL, this->FetchByte()); };
	// SCF
	instructions[0x37] = [this](){ return this->SetCarryFlag(); };
	// JR C, n
	instructions[0x38] = [this](){ return this->JumpIf(static_cast<int8_t>(this->FetchByte()), Flags::Carry, true); };
	// ADD HL, SP
	instructions[0x39] = [this](){ return this->Add(Register16bit::HL, Register16bit::SP); };
	// LD A, (HL-)
	instructions[0x3A] = [this](){ return this->LoadRegisterAndDecrement(Register8bit::A, Register16bit::HL); };
	// DEC SP
	instructions[0x3B] = [this](){ return this->Decrement(Register16bit::SP); };
	// INC A
	instructions[0x3C] = [this](){ return this->Increment(Register8bit::A); };
	// DEC A
	instructions[0x3D] = [this](){ return this->Decrement(Register8bit::A); };
	// LD A, n
	instructions[0x3E] = [this](){ return this->LoadRegister(Register8bit::A, this->FetchByte()); };
	// CCF
	instructions[0x3F] = [this](){ return this->InvertCarryFlag(); };
#pragma endregion

#pragma region Instructions 0x40 - 0x4F
	// LD B, B
	instructions[0x40] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::B); };
	// LD B, C
	instructions[0x41] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::C); };
	// LD B, D
	instructions[0x42] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::D); };
	// LD B, E
	instructions[0x43] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::E); };
	// LD B, H
	instructions[0x44] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::H); };
	// LD B, L
	instructions[0x45] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::L); };
	// LD B, (HL)
	instructions[0x46] = [this](){ return this->LoadRegisterFromAddress(Register8bit::B, Register16bit::HL); };
	// LD B, A
	instructions[0x47] = [this](){ return this->LoadRegister(Register8bit::B, Register8bit::A); };
	// LD C, B
	instructions[0x48] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::B); };
	// LD C, C
	instructions[0x49] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::C); };
	// LD C, D
	instructions[0x4A] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::D); };
	// LD C, E
	instructions[0x4B] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::E); };
	// LD C, H
	instructions[0x4C] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::H); };
	// LD C, L
	instructions[0x4D] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::L); };
	// LD C, (HL)
	instructions[0x4E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::C, Register16bit::HL); };
	// LD C, A
	instructions[0x4F] = [this](){ return this->LoadRegister(Register8bit::C, Register8bit::A); };
#pragma endregion

#pragma region Instructions 0x50 - 0x5F
	// LD D, B
	instructions[0x50] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::B); };
	// LD D, C
	instructions[0x51] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::C); };
	// LD D, D
	instructions[0x52] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::D); };
	// LD D, E
	instructions[0x53] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::E); };
	// LD D, H
	instructions[0x54] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::H); };
	// LD D, L
	instructions[0x55] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::L); };
	// LD D, (HL)
	instructions[0x56] = [this](){ return this->LoadRegisterFromAddress(Register8bit::D, Register16bit::HL); };
	// LD D, A
	instructions[0x57] = [this](){ return this->LoadRegister(Register8bit::D, Register8bit::A); };
	// LD E, B
	instructions[0x58] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::B); };
	// LD E, C
	instructions[0x59] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::C); };
	// LD E, D
	instructions[0x5A] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::D); };
	// LD E, E
	instructions[0x5B] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::E); };
	// LD E, H
	instructions[0x5C] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::H); };
	// LD E, L
	instructions[0x5D] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::L); };
	// LD E, (HL)
	instructions[0x5E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::E, Register16bit::HL); };
	// LD E, A
	instructions[0x5F] = [this](){ return this->LoadRegister(Register8bit::E, Register8bit::A); };
#pragma endregion

#pragma region Instructions 0x60 - 0x6F
	// LD H, B
	instructions[0x60] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::B); };
	// LD H, C
	instructions[0x61] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::C); };
	// LD H, D
	instructions[0x62] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::D); };
	// LD H, E
	instructions[0x63] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::E); };
	// LD H, H
	instructions[0x64] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::H); };
	// LD H, L
	instructions[0x65] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::L); };
	// LD H, (HL)
	instructions[0x66] = [this](){ return this->LoadRegisterFromAddress(Register8bit::H, Register16bit::HL); };
	// LD H, A
	instructions[0x67] = [this](){ return this->LoadRegister(Register8bit::H, Register8bit::A); };
	// LD L, B
	instructions[0x68] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::B); };
	// LD L, C
	instructions[0x69] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::C); };
	// LD L, D
	instructions[0x6A] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::D); };
	// LD L, E
	instructions[0x6B] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::E); };
	// LD L, H
	instructions[0x6C] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::H); };
	// LD L, L
	instructions[0x6D] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::L); };
	// LD L, (HL)
	instructions[0x6E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::L, Register16bit::HL); };
	// LD L, A
	instructions[0x6F] = [this](){ return this->LoadRegister(Register8bit::L, Register8bit::A); };
#pragma endregion

#pragma region Instructions 0x70 - 0x7F
	// LD (HC), B
	instructions[0x70] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::B); };
	// LD (HC), C
	instructions[0x71] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::C); };
	// LD (HC), D
	instructions[0x72] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::D); };
	// LD (HC), E
	instructions[0x73] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::E); };
	// LD (HC), H
	instructions[0x74] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::H); };
	// LD (HC), L
	instructions[0x75] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::L); };
	// HALT
	instructions[0x76] = [this](){ return this->Halt(); };
	// LD (HC), A
	instructions[0x77] = [this](){ return this->LoadAddressFromRegister(Register16bit::HL, Register8bit::A); };
	// LD A, B
	instructions[0x78] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::B); };
	// LD A, C
	instructions[0x79] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::C); };
	// LD A, D
	instructions[0x7A] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::D); };
	// LD A, E
	instructions[0x7B] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::E); };
	// LD A, H
	instructions[0x7C] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::H); };
	// LD A, L
	instructions[0x7D] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::L); };
	// LD A, (HL)
	instructions[0x7E] = [this](){ return this->LoadRegisterFromAddress(Register8bit::A, Register16bit::HL); };
	// LD A, A
	instructions[0x7F] = [this](){ return this->LoadRegister(Register8bit::A, Register8bit::A); };
#pragma endregion

#pragma region Instructions 0x80 - 0x8F
	// ADD A, B
	instructions[0x80] = [this](){ return this->Add(Register8bit::B); };
	// ADD A, C
	instructions[0x81] = [this](){ return this->Add(Register8bit::C); };
	// ADD A, D
	instructions[0x82] = [this](){ return this->Add(Register8bit::D); };
	// ADD A, E
	instructions[0x83] = [this](){ return this->Add(Register8bit::E); };
	// ADD A, H
	instructions[0x84] = [this](){ return this->Add(Register8bit::H); };
	// ADD A, L
	instructions[0x85] = [this](){ return this->Add(Register8bit::L); };
	// ADD A, (HL)
	instructions[0x86] = [this](){ return this->Add(Register16bit::HL); };
	// ADD A, A
	instructions[0x87] = [this](){ return this->Add(Register8bit::A); };
	// ADC A, B
	instructions[0x88] = [this](){ return this->AddPlusCarry(Register8bit::B); };
	// ADC A, C
	instructions[0x89] = [this](){ return this->AddPlusCarry(Register8bit::C); };
	// ADC A, D
	instructions[0x8A] = [this](){ return this->AddPlusCarry(Register8bit::D); };
	// ADC A, E
	instructions[0x8B] = [this](){ return this->AddPlusCarry(Register8bit::E); };
	// ADC A, H
	instructions[0x8C] = [this](){ return this->AddPlusCarry(Register8bit::H); };
	// ADC A, L
	instructions[0x8D] = [this](){ return this->AddPlusCarry(Register8bit::L); };
	// ADC A, (HL)
	instructions[0x8E] = [this](){ return this->AddPlusCarry(Register16bit::HL); };
	// ADC A, A
	instructions[0x8F] = [this](){ return this->AddPlusCarry(Register8bit::A); };
#pragma endregion

#pragma region Instructions 0x90 - 0x9F
	// SUB A, B
	instructions[0x90] = [this](){ return this->Sub(Register8bit::B); };
	// SUB A, C
	instructions[0x91] = [this](){ return this->Sub(Register8bit::C); };
	// SUB A, D
	instructions[0x92] = [this](){ return this->Sub(Register8bit::D); };
	// SUB A, E
	instructions[0x93] = [this](){ return this->Sub(Register8bit::E); };
	// SUB A, H
	instructions[0x94] = [this](){ return this->Sub(Register8bit::H); };
	// SUB A, L
	instructions[0x95] = [this](){ return this->Sub(Register8bit::L); };
	// SUB A, (HL)
	instructions[0x96] = [this](){ return this->Sub(Register16bit::HL); };
	// SUB A, A
	instructions[0x97] = [this](){ return this->Sub(Register8bit::A); };
	// SBC A, B
	instructions[0x98] = [this](){ return this->SubMinusCarry(Register8bit::B); };
	// SBC A, C
	instructions[0x99] = [this](){ return this->SubMinusCarry(Register8bit::C); };
	// SBC A, D
	instructions[0x9A] = [this](){ return this->SubMinusCarry(Register8bit::D); };
	// SBC A, E
	instructions[0x9B] = [this](){ return this->SubMinusCarry(Register8bit::E); };
	// SBC A, H
	instructions[0x9C] = [this](){ return this->SubMinusCarry(Register8bit::H); };
	// SBC A, L
	instructions[0x9D] = [this](){ return this->SubMinusCarry(Register8bit::L); };
	// SBC A, (HL)
	instructions[0x9E] = [this](){ return this->SubMinusCarry(Register16bit::HL); };
	// SBC A, A
	instructions[0x9F] = [this](){ return this->SubMinusCarry(Register8bit::A); };
#pragma endregion

#pragma region Instructions 0xA0 - 0xAF
	// AND B
	instructions[0xA0] = [this](){ return this->And(Register8bit::B); };
	// AND C
	instructions[0xA1] = [this](){ return this->And(Register8bit::C); };
	// AND D
	instructions[0xA2] = [this](){ return this->And(Register8bit::D); };
	// AND E
	instructions[0xA3] = [this](){ return this->And(Register8bit::E); };
	// AND H
	instructions[0xA4] = [this](){ return this->And(Register8bit::H); };
	// AND L
	instructions[0xA5] = [this](){ return this->And(Register8bit::L); };
	// AND (HL)
	instructions[0xA6] = [this](){ return this->And(Register16bit::HL); };
	// AND A
	instructions[0xA7] = [this](){ return this->And(Register8bit::A); };
	// XOR B
	instructions[0xA8] = [this](){ return this->Xor(Register8bit::B); };
	// XOR C
	instructions[0xA9] = [this](){ return this->Xor(Register8bit::C); };
	// XOR D
	instructions[0xAA] = [this](){ return this->Xor(Register8bit::D); };
	// XOR E
	instructions[0xAB] = [this](){ return this->Xor(Register8bit::E); };
	// XOR H
	instructions[0xAC] = [this](){ return this->Xor(Register8bit::H); };
	// XOR L
	instructions[0xAD] = [this](){ return this->Xor(Register8bit::L); };
	// XOR (HL)
	instructions[0xAE] = [this](){ return this->Xor(Register16bit::HL); };
	// XOR A
	instructions[0xAF] = [this](){ return this->Xor(Register8bit::A); };
#pragma endregion

#pragma region Instructions 0xB0 - 0xBF
	// OR B
	instructions[0xB0] = [this](){ return this->Or(Register8bit::B); };
	// OR C
	instructions[0xB1] = [this](){ return this->Or(Register8bit::C); };
	// OR D
	instructions[0xB2] = [this](){ return this->Or(Register8bit::D); };
	// OR E
	instructions[0xB3] = [this](){ return this->Or(Register8bit::E); };
	// OR H
	instructions[0xB4] = [this](){ return this->Or(Register8bit::H); };
	// OR L
	instructions[0xB5] = [this](){ return this->Or(Register8bit::L); };
	// OR (HL)
	instructions[0xB6] = [this](){ return this->Or(Register16bit::HL); };
	// OR A
	instructions[0xB7] = [this](){ return this->Or(Register8bit::A); };
	// CP B
	instructions[0xB8] = [this](){ return this->Compare(Register8bit::B); };
	// CP C
	instructions[0xB9] = [this](){ return this->Compare(Register8bit::C); };
	// CP D
	instructions[0xBA] = [this](){ return this->Compare(Register8bit::D); };
	// CP E
	instructions[0xBB] = [this](){ return this->Compare(Register8bit::E); };
	// CP H
	instructions[0xBC] = [this](){ return this->Compare(Register8bit::H); };
	// CP L
	instructions[0xBD] = [this](){ return this->Compare(Register8bit::L); };
	// CP (HL)
	instructions[0xBE] = [this](){ return this->Compare(Register16bit::HL); };
	// CP A
	instructions[0xBF] = [this](){ return this->Compare(Register8bit::A); };
#pragma endregion

#pragma region Instructions 0xC0 - 0xCF
	// RET NZ
	instructions[0xC0] = [this](){ return this->ReturnIf(Flags::Zero, false); };
	// POP BC
	instructions[0xC1] = [this](){ return this->PopFromStack(Register16bit::BC); };
	// JP NZ, nn
	instructions[0xC2] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->JumpIf(address, Flags::Zero, false); };
	// JP nn
	instructions[0xC3] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->Jump(address); };
	// CALL NZ, nn
	instructions[0xC4] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->CallIf(address, Flags::Zero, false); };
	// PUSH BC
	instructions[0xC5] = [this](){ return this->PushToStack(Register16bit::BC); };
	// ADD A, n
	instructions[0xC6] = [this](){ return this->Add(this->FetchByte()); };
	// RST 00h
	instructions[0xC7] = [this](){ return this->Restart(0x00); };
	// RET Z
	instructions[0xC8] = [this](){ return this->ReturnIf(Flags::Zero, true); };
	// RET
	instructions[0xC9] = [this](){ return this->Return(); };
	// JP Z, nn
	instructions[0xCA] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->JumpIf(address, Flags::Zero, true); };
	// BITS
	instructions[0xCB] = [this](){ return this->bit_instructions_.at(this->FetchByte())(); };
	// CALL Z, nn
	instructions[0xCC] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->CallIf(address, Flags::Zero, true); };
	// CALL nn
	instructions[0xCD] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->Call(address); };
	// ADC A, n
	instructions[0xCE] = [this](){ return this->AddPlusCarry(this->FetchByte()); };
	// RST 08h
	instructions[0xCF] = [this](){ return this->Restart(0x08); };
#pragma endregion

#pragma region Instructions 0xD0 - 0xDF
	// RET NC
	instructions[0xD0] = [this](){ return this->ReturnIf(Flags::Carry, false); };
	// POP DE
	instructions[0xD1] = [this](){ return this->PopFromStack(Register16bit::DE); };
	// JP NC, nn
	instructions[0xD2] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->JumpIf(address, Flags::Carry, false); };
	// EMPTY
	instructions[0xD3] = [this](){ return this->WrongOpCode(0xD3); };
	// CALL NC, nn
	instructions[0xD4] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->CallIf(address, Flags::Carry, false); };
	// PUSH DE
	instructions[0xD5] = [this](){ return this->PushToStack(Register16bit::DE); };
	// SUB A, n
	instructions[0xD6] = [this](){ return this->Sub(this->FetchByte()); };
	// RST 10h
	instructions[0xD7] = [this](){ return this->Restart(0x10); };
	// RET C
	instructions[0xD8] = [this](){ return this->ReturnIf(Flags::Carry, true); };
	// RETI
	instructions[0xD9] = [this](){ return this->ReturnFromInterrupt(); };
	// JP C, nn
	instructions[0xDA] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->JumpIf(address, Flags::Carry, true); };
	// EMPTY
	instructions[0xDB] = [this](){ return this->WrongOpCode(0xDB); };
	// CALL C, nn
	instructions[0xDC] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->CallIf(address, Flags::Carry, true); };
	// EMPTY
	instructions[0xDD] = [this](){ return this->WrongOpCode(0xDD); };
	// SBC A, n
	instructions[0xDE] = [this](){ return this->SubMinusCarry(this->FetchByte()); };
	// RST 18h
	instructions[0xDF] = [this](){ return this->Restart(0x18); };
#pragma endregion

#pragma region Instructions 0xE0 - 0xEF
	// LDIO (n), A
	instructions[0xE0] = [this](){ return this->LoadIOFromRegister(this->FetchByte(), Register8bit::A); };
	// POP HL
	instructions[0xE1] = [this](){ return this->PopFromStack(Register16bit::HL); };
	// LDIO (C), A
	instructions[0xE2] = [this](){ return this->LoadIOFromRegister(Register8bit::C, Register8bit::A); };
	// EMPTY
	instructions[0xE3] = [this](){ return this->WrongOpCode(0xE3); };
	// EMPTY
	instructions[0xE4] = [this](){ return this->WrongOpCode(0xE4); };
	// PUSH HL
	instructions[0xE5] = [this](){ return this->PushToStack(Register16bit::HL); };
	// AND n
	instructions[0xE6] = [this](){ return this->And(this->FetchByte()); };
	// RST 20h
	instructions[0xE7] = [this](){ return this->Restart(0x20); };
	// ADD SP, n
	instructions[0xE8] = [this](){ return this->Add(Register16bit::SP, this->FetchByte()); };
	// JP (HL)
	instructions[0xE9] = [this](){ return this->Jump(Register16bit::HL); };
	// LD (nn), A
	instructions[0xEA] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadAddressFromRegister(address, Register8bit::A); };
	// EMPTY
	instructions[0xEB] = [this](){ return this->WrongOpCode(0xEB); };
	// EMPTY
	instructions[0xEC] = [this](){ return this->WrongOpCode(0xEC); };
	// EMPTY
	instructions[0xED] = [this](){ return this->WrongOpCode(0xED); };
	// XOR n
	instructions[0xEE] = [this](){ return this->Xor(this->FetchByte()); };
	// RST 28h
	instructions[0xEF] = [this](){ return this->Restart(0x28); };
#pragma endregion

#pragma region Instructions 0xF0 - 0xFF
	// LDIO A, (n)
	instructions[0xF0] = [this](){ return this->LoadRegisterFromIO(Register8bit::A, this->FetchByte()); };
	// POP AF
	instructions[0xF1] = [this](){ return this->PopFromStack(Register16bit::AF); };
	// LDIO A, (C)
	instructions[0xF2] = [this](){ return this->LoadRegisterFromIO(Register8bit::A, Register8bit::C); };
	// DI
	instructions[0xF3] = [this](){ return this->DisableInterrupts(); };
	// EMPTY
	instructions[0xF4] = [this](){ return this->WrongOpCode(0xF4); };
	// PUSH AF
	instructions[0xF5] = [this](){ return this->PushToStack(Register16bit::AF); };
	// OR n
	instructions[0xF6] = [this](){ return this->Or(this->FetchByte()); };
	// RST 30h
	instructions[0xF7] = [this](){ return this->Restart(0x30); };
	// LD HL, SP+n
	instructions[0xF8] = [this](){ return this->WrongOpCode(0xF8); };
	// LD SP, HL
	instructions[0xF9] = [this](){ return this->LoadRegister(Register16bit::SP, Register16bit::HL); };
	// LD A, (nn)
	instructions[0xFA] = [this](){
		uint16_t address{ this->FetchByte() };
		address += static_cast<uint16_t>(this->FetchByte()) << 8;
		return this->LoadRegisterFromAddress(Register8bit::A, address); };
	// EI
	instructions[0xFB] = [this](){ return this->EnableInterrupts(); };
	// EMPTY
	instructions[0xFC] = [this](){ return this->WrongOpCode(0xFC); };
	// EMPTY
	instructions[0xFD] = [this](){ return this->WrongOpCode(0xFD); };
	// CP n
	instructions[0xFE] = [this](){ return this->Compare(this->FetchByte()); };
	// RST 38h
	instructions[0xFF] = [this](){ return this->Restart(0x38); };
#pragma endregion

	return instructions;
}

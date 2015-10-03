#pragma once

#include <cstdint>
#include <map>
#include <functional>
#include <atomic>

#include "Registers.h"
#include "MMU.h"
#include "Clock.h"

class GPU;
using InstructionMap = std::map<uint8_t, std::function<Clock()>>;

class Z80 : public MMUObserver
{
public:
	enum class Interrupt
	{
		VBlank = 0x01,
		LcdStatus = 0x02,
		Timer = 0x04,
		SerialLink = 0x08,
		Joypad = 0x10
	};

	Z80(MMU &mmu, GPU &gpu);
	~Z80() = default;

	uint8_t FetchByte();
	void Execute(uint8_t opcode);
	void CheckAndHandleInterrupts();

	void OnMemoryWrite(MMU::Region region, uint16_t address, uint8_t value) override;

private:
	InstructionMap FillInstructionMap();
	InstructionMap FillBitInstructionMap();

	Clock WrongOpCode(uint8_t opcode);

#pragma region Instructions
#pragma region 8-bit load group
	// LD r, r'
	Clock LoadRegister(Register8bit dest, Register8bit source);
	// LD r, n
	Clock LoadRegister(Register8bit dest, uint8_t value);
	// LD r, (rr')
	Clock LoadRegisterFromAddress(Register8bit dest, Register16bit source_addr);
	// LD (rr), r'
	Clock LoadAddressFromRegister(Register16bit dest_addr, Register8bit source);
	// LD (rr), n
	Clock LoadAddress(Register16bit dest_addr, uint8_t value);
	// LD r, (nn)
	Clock LoadRegisterFromAddress(Register8bit dest, uint16_t addr);
	// LD (nn), r
	Clock LoadAddressFromRegister(uint16_t addr, Register8bit source);
#pragma endregion

#pragma region 16-bit load group
	// LD rr, nn
	Clock LoadRegister(Register16bit dest, uint16_t value);
	// LD (nn), rr
	Clock LoadAddressFromRegister(uint16_t addr, Register16bit source);
	// LD rr, rr'
	Clock LoadRegister(Register16bit dest, Register16bit source);
	// PUSH qq
	Clock PushToStack(Register16bit source);
	// POP qq
	Clock PopFromStack(Register16bit dest);
#pragma endregion

#pragma region 8-bit arithmetic group
	// ADD A, n
	Clock Add(uint8_t value);
	// ADD A, r
	Clock Add(Register8bit source);
	// ADD A, (rr)
	Clock Add(Register16bit source_addr);
	// ADC A, n
	Clock AddPlusCarry(uint8_t value);
	// ADC A, r
	Clock AddPlusCarry(Register8bit source);
	// ADC A, (rr)
	Clock AddPlusCarry(Register16bit source_addr);
	// SUB A, n
	Clock Sub(uint8_t value);
	// SUB A, r
	Clock Sub(Register8bit source);
	// SUB A, (rr)
	Clock Sub(Register16bit source_addr);
	// SBC A, n
	Clock SubMinusCarry(uint8_t value);
	// SBC A, r
	Clock SubMinusCarry(Register8bit source);
	// SBC A, (rr)
	Clock SubMinusCarry(Register16bit source_addr);
	// AND A, n
	Clock And(uint8_t value);
	// AND A, r
	Clock And(Register8bit source);
	// AND A, (rr)
	Clock And(Register16bit source_addr);
	// OR A, n
	Clock Or(uint8_t value);
	// OR A, r
	Clock Or(Register8bit source);
	// OR A, (rr)
	Clock Or(Register16bit source_addr);
	// XOR A, n
	Clock Xor(uint8_t value);
	// XOR A, r
	Clock Xor(Register8bit source);
	// XOR A, (rr)
	Clock Xor(Register16bit source_addr);
	// CP A, n
	Clock Compare(uint8_t value);
	// CP A, r
	Clock Compare(Register8bit source);
	// CP A, (rr)
	Clock Compare(Register16bit source_addr);
	// INC r
	Clock Increment(Register8bit reg);
	// INC (rr)
	Clock IncrementInAddress(Register16bit reg_addr);
	// DEC r
	Clock Decrement(Register8bit reg);
	// DEC (rr)
	Clock DecrementInAddress(Register16bit reg_addr);
#pragma endregion

#pragma region General purpose arithmetic and CPU control groups
	// DAA
	Clock DAA();
	// CPL
	Clock InvertAccumulator();
	// CCF
	Clock InvertCarryFlag();
	// SCF
	Clock SetCarryFlag();
	// NOP
	Clock NoOperation();
	// HALT
	Clock Halt();
	// DI
	Clock DisableInterrupts();
	// EI
	Clock EnableInterrupts();
#pragma endregion

#pragma region 16-bit arithmetic group
	// ADD rr, rr'
	Clock Add(Register16bit dest, Register16bit source);
	// ADC rr, rr'
	Clock AddPlusCarry(Register16bit dest, Register16bit source);
	// SBC rr, rr'
	Clock SubtractMinusCarry(Register16bit dest, Register16bit source);
	// INC rr
	Clock Increment(Register16bit reg);
	// DEC rr
	Clock Decrement(Register16bit reg);
#pragma endregion

#pragma region Rotate and shift group
	// RLCA
	Clock Rlca();
	// RLA
	Clock Rla();
	// RRCA
	Clock Rrca();
	// RRA
	Clock Rra();
	// RLC r
	Clock Rlc(Register8bit reg);
	// RLC (rr)
	Clock Rlc(Register16bit reg_addr);
	// RL r
	Clock Rl(Register8bit reg);
	// RL (rr)
	Clock Rl(Register16bit reg_addr);
	// RRC r
	Clock Rrc(Register8bit reg);
	// RRC (rr)
	Clock Rrc(Register16bit reg_addr);
	// RR r
	Clock Rr(Register8bit reg);
	// RR (rr)
	Clock Rr(Register16bit reg_addr);
	// SLA r
	Clock Sla(Register8bit reg);
	// SLA (rr)
	Clock Sla(Register16bit reg_addr);
	// SRA r
	Clock Sra(Register8bit reg);
	// SRA (rr)
	Clock Sra(Register16bit reg_addr);
	// SRL r
	Clock Srl(Register8bit reg);
	// SRL (rr)
	Clock Srl(Register16bit reg_addr);
#pragma endregion

#pragma region Bit set, reset and test group
	// BIT n, r
	Clock TestBit(uint8_t bit_index, Register8bit reg);
	// BIT n, (rr)
	Clock TestBit(uint8_t bit_index, Register16bit reg_addr);
	// SET n, r
	Clock SetBit(uint8_t bit_index, Register8bit reg);
	// SET n, (rr)
	Clock SetBit(uint8_t bit_index, Register16bit reg_addr);
	// RES n, r
	Clock ResetBit(uint8_t bit_index, Register8bit reg);
	// RES n, (rr)
	Clock ResetBit(uint8_t bit_index, Register16bit reg_addr);
#pragma endregion

#pragma region Jump group
	// JP nn
	Clock Jump(uint16_t address);
	// JP cc, nn
	Clock JumpIf(uint16_t address, Flags flag, bool flag_value);
	// JR n
	Clock Jump(int8_t displacement);
	// JR cc, n
	Clock JumpIf(int8_t displacement, Flags flag, bool flag_value);
	// JP (rr)
	Clock Jump(Register16bit reg_addr);
#pragma endregion

#pragma region Call and return group
	// CALL nn
	Clock Call(uint16_t address);
	// CALL cc, nn
	Clock CallIf(uint16_t address, Flags flag, bool flag_value);
	// RET
	Clock Return();
	// RET cc
	Clock ReturnIf(Flags flag, bool flag_value);
	// RETI
	Clock ReturnFromInterrupt();
	// RST p
	Clock Restart(uint16_t address);
#pragma endregion

#pragma region GB exclusive instructions group
	// LDI (rr), r'
	Clock LoadAddressAndIncrement(Register16bit dest_addr, Register8bit source);
	// LDD (rr), r'
	Clock LoadAddressAndDecrement(Register16bit dest_addr, Register8bit source);
	// LDI r, (rr)
	Clock LoadRegisterAndIncrement(Register8bit dest, Register16bit source_addr);
	// LDD r, (rr')
	Clock LoadRegisterAndDecrement(Register8bit dest, Register16bit source_addr);
	// SWAP r
	Clock Swap(Register8bit reg);
	// SWAP (rr)
	Clock Swap(Register16bit reg_address);
	// LDIO (n), r
	Clock LoadIOFromRegister(uint8_t displacement, Register8bit source);
	// LDIO (r), r'
	Clock LoadIOFromRegister(Register8bit reg_displacement, Register8bit source);
	// LDIO r, (n)
	Clock LoadRegisterFromIO(Register8bit dest, uint8_t displacement);
	// LDIO r, (r')
	Clock LoadRegisterFromIO(Register8bit dest, Register8bit reg_displacement);
	// LD rr, rr'+n
	Clock LoadRegisterFromAddress(Register16bit dest, Register16bit source_addr, int8_t displacement);
	// ADD rr, n
	Clock Add(Register16bit dest, int8_t value);
#pragma endregion
#pragma endregion

private:
	static const uint16_t interrupts_enable_register{ 0x007F };
	static const uint16_t interrupt_flags_register{ 0x000F };

	Registers registers_;
	Clock clock_;
	MMU &mmu_;
	GPU &gpu_;

	const InstructionMap instructions_;
	const InstructionMap bit_instructions_;

	enum class State
	{
		Running,
		Halted
	} state_{ State::Running };

	bool interrupt_master_enable_{ true };
	std::map<Interrupt, bool> interrupts_enabled_;
	std::map<Interrupt, bool> interrupts_signaled_;
	std::map<Interrupt, uint16_t> interrupt_handler_addresses_;

private:
	Z80& operator=(const Z80&) = delete;
	Z80& operator=(Z80&&) = delete;
};

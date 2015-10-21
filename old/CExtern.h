#ifndef INCLUDED_C_EXTERN_H
#define INCLUDED_C_EXTERN_H

#ifdef __cplusplus
extern "C" uint8_t R_FlashErase(uint32_t block);
extern "C" uint8_t R_FlashWrite(uint32_t flash_addr, uint32_t buffer_addr, uint16_t bytes);
extern "C" uint8_t R_FlashProgramLockBit(uint32_t block);
extern "C" uint8_t R_FlashReadLockBit(uint32_t block);
extern "C" uint8_t R_FlashSetLockBitProtection(uint32_t lock_bit);
extern "C" uint8_t R_FlashGetStatus(void);
extern "C" uint32_t R_FlashGetVersion(void);
extern "C" void R_FlashDataAreaAccess(uint16_t read_en_mask, uint16_t write_en_mask);
extern "C" uint8_t R_FlashDataAreaBlankCheck(uint32_t address, uint8_t size);

extern "C" void R_FlashCodeCopy(void);
#endif

extern "C" void interrupt_CMT0();
extern "C" void interrupt_CMT1();
extern "C" void interrupt_CMT2();
extern "C" void interrupt_S12AD0();

#endif

#ifndef __SOCKETAES_H__
# define __SOCKETAES_H__

#include <stdint.h>

typedef struct aesPayloadStruct {

   uint8_t  keyState[32];
   uint8_t  keyLen;

   uint8_t  ivState[16];
   uint32_t dataLen;

   uint8_t  ptDataBuff[2048*16*sizeof(uint8_t)];
   uint8_t  ctDataBuff[2048*16*sizeof(uint8_t)];

   uint32_t cpuAffinity;

} aesPayloadStruct;

typedef union bitpacketUnion {
  char      chr[4];
  uint32_t  data;
} bitPacket;

uint32_t getData(int sessionfd, char* dest, uint32_t maxBuffer);
uint32_t putData(int sessionfd, char* src, uint32_t buffLen);

void *aes_encrypt(void* args);
int aes_decrypt(const void* key, const void* ciphertext, void* plaintext, uint32_t size);

#endif /* __SOCKETAES_H__ */

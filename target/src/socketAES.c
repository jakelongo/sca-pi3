/*
 * AES excercising module
 */

// Uses WiringPi moduel for triggering GPIO

// Standard header files
#define _GNU_SOURCE
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sched.h>

// OpenSSL headers
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

// socket header files
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <wiringPi.h>

// Custom header
#include "socketAES.h"


#define ascii2Bin(x) ( ((x)>='0' && (x)<='9') ? (x)-'0' : 10+tolower(x)-'a' )
#define bin2Ascii(x) ( ((x)<=9) ? (x)+'0' : (x)+'A'-10 )

#define DATABUFFSIZE 4096*16*sizeof(uint8_t)
#define DEBUG

// Dreaded globals!
volatile int listenfd  = 0;
volatile int sessionfd = 0;


// Housekeeping if program interrupted
void intHandler(int var)
{
  printf("SIGINT : Closing down session\n");
  close(sessionfd);
  close(listenfd);
  exit(-1);
}

void *aes_encrypt(void* args)
{
  cpu_set_t        cpuset;
  int              cip_len;
  int              tmp_len;
  EVP_CIPHER_CTX   *ctx;
  aesPayloadStruct *aesPayload = (aesPayloadStruct*)args;

  CPU_ZERO(&cpuset);
  CPU_SET(aesPayload->cpuAffinity, &cpuset);
  sched_setaffinity(0, sizeof(cpuset), &cpuset);

  if(!(ctx = EVP_CIPHER_CTX_new())) {
    return NULL;
  }

  if (16 == aesPayload->keyLen) {
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, aesPayload->keyState, NULL)) {
      return NULL;
    }
  } else {
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, aesPayload->keyState, NULL)) {
      return NULL;
    }
  }

  if(1 != EVP_EncryptUpdate(ctx, aesPayload->ctDataBuff, &tmp_len, aesPayload->ptDataBuff, aesPayload->dataLen)) {
    return NULL;
  }

  cip_len = tmp_len;

  if(1 != EVP_EncryptFinal_ex(ctx, aesPayload->ptDataBuff + tmp_len, &tmp_len)) {
    return NULL;
  }

  cip_len += tmp_len;

  return NULL;
}

int aes_decrypt(const void *key, const void* ciphertext, void* plaintext, uint32_t size)
{
  return 0;
}

uint32_t getData(int sessionfd, char* dest, uint32_t maxBuffer)
{
  bitPacket   streamLength;
  uint32_t    byteCounter;
  char        terminationChar;

  byteCounter = 0;
  while (byteCounter < 4)
  {
    byteCounter += read(sessionfd, &(streamLength.chr[byteCounter]), 4-byteCounter);
  }

  #ifdef DEBUG
  printf("Payload length = %d\n", streamLength.data);
  #endif /* DEBUG */

  if (streamLength.data > maxBuffer)
  {
    printf("Payload length is greater than max buffer %d\n", streamLength.data);
  }

  byteCounter = 0;
  while (byteCounter < streamLength.data)
  {
    byteCounter += read(sessionfd, &(dest[byteCounter]), streamLength.data-byteCounter);
  }

  #ifdef DEBUG
  printf("Payload received\n");
  #endif /* DEBUG */

  byteCounter = 0;
  while (byteCounter < 1)
  {
    byteCounter += read(sessionfd, &terminationChar, 1-byteCounter);
  }

  if (terminationChar != '\n')
  {
    printf("Invalid termination character\n");
    return -1;
  }

  return streamLength.data;

}

uint32_t putData(int sessionfd, char* src, uint32_t buffLen)
{
  bitPacket   streamLength;
  uint32_t    byteCounter;
  char        terminationChar;

  streamLength.data = buffLen;

  byteCounter = 0;
  while (byteCounter < 4) {
    byteCounter += write(sessionfd, &(streamLength.chr[byteCounter]), 4-byteCounter);
  }

  byteCounter = 0;
  while (byteCounter < streamLength.data) {
    byteCounter += write(sessionfd, &(src[byteCounter]), streamLength.data-byteCounter);
  }

  #ifdef DEBUG
  printf("Payload Sent\n");
  #endif /* DEBUG */

  terminationChar = '\n';

  byteCounter = 0;
  while (byteCounter < 1) {
    byteCounter += write(sessionfd, &terminationChar, 1-byteCounter);
  }

  return streamLength.data;

}

void testEncrypt(void) {

  aesPayloadStruct  aesPayload;
  pthread_t         thread;
  int               i,j,k;

  aesPayload.keyLen      = 16;
  aesPayload.dataLen     = 2048*16*sizeof(uint8_t);

  for (k = 0; k < 1; k++) {
    for (j = 0; j < 4; j++) {
      aesPayload.cpuAffinity = j;
      for (i = 0; i < 256; ++i) {
        memset(aesPayload.ctDataBuff, 0x00, aesPayload.dataLen);
        pthread_create(&thread, NULL, &aes_encrypt, (void *)&aesPayload);
        pthread_join(thread, NULL);
      }
    }
  }

  return;
}


void commandHandler(int sessionfd)
{
  aesPayloadStruct  aesPayload;
  uint32_t          scratchVariable;
  char              cmdIdentifier;
  bitPacket         streamLength;
  pthread_t         thread;

  // Internal flags
  uint8_t closeServer = 0;

  aesPayload.cpuAffinity = 0;

  int i;

  while(!closeServer)
  {
    read(sessionfd, &cmdIdentifier, 1);
    #ifdef DEBUG
    printf("CMD=%c \n", cmdIdentifier);
    #endif /* DEBUG */


    switch(cmdIdentifier)
    {

      // Set key and check the key length
      case 'k':

        aesPayload.keyLen = getData(sessionfd, aesPayload.keyState, 32);

        if ((aesPayload.keyLen != 16) && (aesPayload.keyLen != 32)) {
          printf("Key length incorrect. Recieved %d bytes", aesPayload.keyLen);
        }

        printf("Key = ");
        for (i = 0 ; i < aesPayload.keyLen; ++i) {
          printf("%02X", aesPayload.keyState[i]);
        }
        printf("\n");

        break;

      // set IV
      case 'i':

        scratchVariable = getData(sessionfd, aesPayload.ivState, 16);

        if (scratchVariable != 16) {
          printf("IV length incorrect. Recieved %d bytes", scratchVariable);
        }

        printf("IV = ");
        for (i = 0 ; i < 16; ++i){
          printf("%02X", aesPayload.ivState[i]);
        }
        printf("\n");

        break;

      // Set data buffer
      case 'm':

        aesPayload.dataLen = getData(sessionfd, aesPayload.ptDataBuff, DATABUFFSIZE);
        printf("Recieved message payload of %d bytes\n", aesPayload.dataLen);

        printf("Mes = ");
        for (i = 0 ; i < 16; ++i){
          printf("%02X", aesPayload.ptDataBuff[i]);
        }

        if (aesPayload.dataLen > 16){
          printf("...\n");
        } else {
          printf("\n");
        }

        break;

      case 'e':

        i = 0;
        while (i < 4) {
          i += read(sessionfd, &(streamLength.chr[i]), 4-i);
        }

        printf("Encrypting...");

	      for (i = 0; i < streamLength.data; ++i) {
          memset(aesPayload.ctDataBuff, 0x00, aesPayload.dataLen);
          pthread_create(&thread, NULL, &aes_encrypt, (void *)&aesPayload);
          pthread_join(thread, NULL);
        }

        printf("Done\n");

        break;

      case 'c':

        putData(sessionfd, aesPayload.ctDataBuff, aesPayload.dataLen);

        break;

      case 'x':

        printf("Exit command received - closing down.\n");
        closeServer = 0x01;

        break;

      default:

        printf("Unkonwn command received %c\n Closing down server.\n", cmdIdentifier);
        closeServer = 0x01;

        break;

    }

  }

  close(sessionfd);

}

int main(int argc, char *argv[])
{

  /* Socket Setup */
  // default socket number
  int socketNumber = 5000;

  // Init the GPIO
  wiringPiSetup();
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);

  testEncrypt();
  return 1;

  if (argc > 1){
    socketNumber = atoi(argv[1]);
  }

  // Set up socket first
  struct sockaddr_in serv_addr;

  signal(SIGINT, intHandler);

  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&serv_addr,  '0', sizeof(serv_addr));

  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port        = htons(socketNumber);

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  listen(listenfd, 10);
  sessionfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

  /* End of Socket Setup */

  /* OpenSSL setup */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  /* End of OpenSSL setup */

  commandHandler(sessionfd);

  // Destructor for socket
  close(listenfd);
}



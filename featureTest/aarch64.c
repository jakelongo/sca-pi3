#include <stdio.h>  
#include <sys/auxv.h>  
#include <asm/hwcap.h>  

#define HWCAP_FP                (1 << 0)
#define HWCAP_ASIMD             (1 << 1)
#define HWCAP_AES               (1 << 3)
#define HWCAP_PMULL             (1 << 4)
#define HWCAP_SHA1              (1 << 5)
#define HWCAP_SHA2              (1 << 6)
#define HWCAP_CRC32             (1 << 7)
#define HWCAP_ATOMICS           (1 << 8)
  
int main()  
{  
    long hwcaps= getauxval(AT_HWCAP);  
  
    if(hwcaps & HWCAP_AES){  
        printf("AES instructions are available\n");  
    }  
    if(hwcaps & HWCAP_CRC32){  
        printf("CRC32 instructions are available\n");  
    }  
    if(hwcaps & HWCAP_PMULL){  
        printf("PMULL/PMULL2 instructions that operate on 64-bit data are available\n");  
    }  
    if(hwcaps & HWCAP_SHA1){  
        printf("SHA1 instructions are available\n");  
    }  
    if(hwcaps & HWCAP_SHA2){  
        printf("SHA2 instructions are available\n");  
    }  
    return 0;  
}  

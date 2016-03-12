#include <stdio.h>  
#include <sys/auxv.h>  
#include <asm/hwcap.h>  
  
int main()  
{  
    long hwcaps2 = getauxval(AT_HWCAP2);  
  
    if(hwcaps2 & HWCAP2_AES){  
        printf("AES instructions are available\n");  
    }  
    if(hwcaps2 & HWCAP2_CRC32){  
        printf("CRC32 instructions are available\n");  
    }  
    if(hwcaps2 & HWCAP2_PMULL){  
        printf("PMULL/PMULL2 instructions that operate on 64-bit data are available\n");  
    }  
    if(hwcaps2 & HWCAP2_SHA1){  
        printf("SHA1 instructions are available\n");  
    }  
    if(hwcaps2 & HWCAP2_SHA2){  
        printf("SHA2 instructions are available\n");  
    }  
    return 0;  
}  

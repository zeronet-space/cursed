#ifndef _SHA1_H_
#define _SHA1_H_

typedef struct SHA1Context {
    unsigned Message_Digest[5];
    unsigned Length_Low;
    unsigned Length_High;
    unsigned char Message_Block[64];
    int Message_Block_Index;
    int Computed;
    int Corrupted;
} SHA1Context;

void SHA1Reset(SHA1Context*);
int SHA1Result(SHA1Context*);
void SHA1Input(SHA1Context*, const unsigned char*, unsigned);
#endif
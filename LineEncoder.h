#include "CommonFunc.h"

#ifndef _ITCG_LINE_ENCODER_
    #define _ITCG_LINE_ENCODER_

    class Encoder {
    public:
        int codeword_len;
        int codeword_num;
        int message_blk_len;
        int **codebook;

        Encoder();
        ~Encoder();
        void Init(const char *config_file_path);
        void Encode(int *in, int *out, int in_len);
        void Decode4BPSK_AWGN(double *in, int *imd, int *out, int blk_num);
        void Clean();
        int CalcBlkNum(int bit_num);
    };
#endif

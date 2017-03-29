#include "CommonFunc.h"

#ifndef _ITCG_LINE_ENCODER_
    #define _ITCG_LINE_ENCODER_

    class Encoder {
    public:
        int codeword_len;
        int codeword_num;
        int **codebook;

        Encoder();
        ~Encoder();
        void Init(const char *config_file_path);
        void Encode(int *in, int *out, int in_len);
        void Clean();
        int CalcBlkNum(int bit_num);
    };
#endif

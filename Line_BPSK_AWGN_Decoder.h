#include "CommonFunc.h"
#include "LineEncoder.h"
#include "AWGN_Channel.h"

#ifndef _ITCG_LINE_BPSK_AWGN_DECODER_
    #define _ITCG_LINE_BPSK_AWGN_DECODER_

    class Decoder {
    public:
        int codeword_len;
        int codeword_num;
        int **codebook;
        int **all_codeword;


        Decoder();
        ~Decoder();
        void Init(char *config_file_path);
        void Decode(const Encoder &enc, const Channel &chn, double *in, int *imd, int *out, int blk_num);
        void Clean();
    };
#endif

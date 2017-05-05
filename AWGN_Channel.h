#include "CommonFunc.h"

#ifndef _ITCG_AWGN_CHANNEL_
    #define _ITCG_AWGN_CHANNEL_

    class Channel {
    public:
        double segma;

        Channel();
        ~Channel();
        void Init(const char *config_file_path);
        void SetNoisePower(double noise_power);
        void Transmit(int *in, double *out, int len);
        void Clean();
        double CntNoisePower();
    };

#endif

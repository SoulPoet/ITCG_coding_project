#include "CommonFunc.h"

#ifndef _ITCG_BPSK_Modem_
    #define _ITCG_BPSK_Modem_

    class Modem {
    public:
        Modem();
        ~Modem();
        static void Init(const char *config_file_path);
        static void Modulate(int *in, int *out, int len);
        static double CntSignalPower();
        static void Modem::Demodulate_Hard_Decision(double *in, int *out, int len);
        static void Clean();
    };

#endif

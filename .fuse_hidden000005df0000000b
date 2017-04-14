#include "CommonFunc.h"
#include "SourceCreate.h"
#include "LineEncoder.h"
#include "BPSK_Modulater.h"
#include "AWGN_Channel.h"
#include "Line_BPSK_AWGN_Decoder.h"

#ifndef _ITCG_Simulator_
	#define _ITCG_Simulator_

	class Simulator {
	public:
	    double min_snr;
	    double max_snr;


		int blk_num;
		int bit_num;
		int err_blk_cnt;
		int err_bit_cnt;
		double fer;
		double ber;

		int *source_signal;
		int *encoded_signal;
		int *send_signal;
		double *receive_signal;
		int *restore_codeword;
		int *restore_signal;
		FILE *out_file;
		Encoder enc1;
		Channel chn1;
		Decoder dec1;


		Simulator();
		~Simulator();
		void Init(const char *config_file_path);
		void Start();
		void Clean();
		void DisplaySourceSignal();
		void DisplayEncodedSignal();
		void DisplaySendSignal();
		void DisplayReceiveSignal();
		void DisplayRestoreCodeword();
		void DisplayRestoreSignal();
		void DisplayResult();
	};

#endif

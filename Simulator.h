#include "CommonFunc.h"
#include "SourceCreate.h"
#include "LineEncoder.h"
#include "BPSK_Modem.h"
#include "AWGN_Channel.h"

#ifndef _ITCG_Simulator_
	#define _ITCG_Simulator_

	class Simulator {
	public:
	    double m_min_snr;
	    double m_max_snr;
	    double m_snr_step;
	    double m_snr;

		int m_tot_blk_num;
		int m_tot_bit_num;
		int m_err_blk_cnt;
		int m_err_bit_cnt;
		double m_fer;
		double m_ber;
		int m_target_err_blk;

		int *source_signal;
		int *encoded_signal;
		int *send_signal;
		double *receive_signal;
		int *restore_codeword;
		int *restore_signal;
		FILE *out_file;
		Encoder objCodec;
		Channel objChannel;

		bool m_save_temp_info;
		char temp_file[50];

		Simulator();
		~Simulator();
		void Init(const char *config_file_path);
		void Start();
		void Clean();
		void SaveTempInfo();
		void DisplaySourceSignal();
		void DisplayEncodedSignal();
		void DisplaySendSignal();
		void DisplayReceiveSignal();
		void DisplayRestoreCodeword();
		void DisplayRestoreSignal();
		void DisplayResult();
	};

#endif

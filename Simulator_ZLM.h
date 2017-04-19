#include "CommonFunc_ZLM.h"
#include "SourceCreate_ZLM.h"
#include "LineEncoder.h"
#include "BPSK_Modulater.h"
#include "AWGN_Channel.h"

#ifndef _ITCG_Simulator_ZLM_
	#define _ITCG_Simulator_ZLM_

	class Simulator {
	public:
	    double min_snr;
	    double max_snr;
	    double snr_step;
	    double snr;


		int blk_num;
		int bit_num;
		int err_blk_cnt;
		int err_bit_cnt;
		double fer;
		double ber;

		BaseVector source_signal;
		BaseVector encoded_signal;
		SignalVector send_signal;
		SignalVector receive_signal;
		BaseVector restore_codeword;
		BaseVector restore_signal;
		FILE *out_file;
		Encoder enc1;
		Channel chn1;


		Simulator() {}
		~Simulator() {}

		void Init(const char *config_file_path) {
			char encoder_config_file_path[50];
		    char channel_config_file_path[50];
		    char str[50] = "";
		    char output_file_path[50];

		    FILE *in_file = OpenFile(config_file_path, "r");
		    while(~fscanf(in_file, "%[a-z_]=", str)) {
		        if(!strcmp(str, "bit_num"))
		            fscanf(in_file, "%d", &bit_num);
		        else if(!strcmp(str, "blk_num"))
		        	fscanf(in_file, "%d", &blk_num);
		        else if(!strcmp(str, "encoder_config_file"))
		            fscanf(in_file, "%s", encoder_config_file_path);
		        else if(!strcmp(str, "channel_config_file"))
		            fscanf(in_file, "%s", channel_config_file_path);
		        else if(!strcmp(str, "output_file"))
		            fscanf(in_file, "%s", output_file_path);
		        else if(!strcmp(str, "min_snr"))
		            fscanf(in_file, "%lf", &min_snr);
		        else if(!strcmp(str, "max_snr"))
		            fscanf(in_file, "%lf", &max_snr);
		        else if(!strcmp(str, "snr_step"))
		            fscanf(in_file, "%lf", &snr_step);
		        else if(!strcmp(str, "snr"))
		        	fscanf(in_file, "%lf", &snr);
		        str[0] = '\0';
		        fscanf(in_file, "%*c");
		    }

		    enc1.Init(encoder_config_file_path);
		    chn1.Init(channel_config_file_path);

		    blk_num = enc1.CalcBlkNum(bit_num);

		    encoded_signal.resize(blk_num * enc1.codeword_len);
		    send_signal.resize(blk_num * enc1.codeword_len];
		    receive_signal.resize(blk_num * enc1.codeword_len);
		    restore_codeword.resize(blk_num * enc1.codeword_len);
		    restore_signal.resize(bit_num);
		    
		    out_file = OpenFile(output_file_path, "w");

		}
		void Run() {
			for(snr = min_snr; snr <= max_snr; snr += snr_step) {
		        err_bit_cnt = err_blk_cnt = 0;
		        chn1.SetNoisePower(snr, Modulater::CntSignalPower());

		        SourceCreate::CreateStream(source_signal, bit_num);
		        enc1.Encode(source_signal, encoded_signal, blk_num);
		        Modulater::Modulate(encoded_signal, send_signal, blk_num * enc1.codeword_len);
		        chn1.transmit(send_signal, receive_signal, blk_num * enc1.codeword_len);
		        enc1.Decode4BPSK_AWGN(receive_signal, restore_codeword, restore_signal, blk_num);

		        err_bit_cnt = source_signal - restore_signal;
		        for(int i = 0; i < blk_num; ++i)
		            if(HammingDistance(encoded_signal + (i * enc1.codeword_len), restore_codeword + (i * enc1.codeword_len), enc1.codeword_len))
		                ++err_blk_cnt;
		        ber = (double)err_bit_cnt / bit_num;
		        fer = (double)err_blk_cnt / blk_num;
		        DisplayResult();
		    }
		}

		void Clean() {
			enc1.Clean();
			chn1.Clean();
			fclose(out_file);
		}

		void DisplaySourceSignal();
		void DisplayEncodedSignal();
		void DisplaySendSignal();
		void DisplayReceiveSignal();
		void DisplayRestoreCodeword();
		void DisplayRestoreSignal();
		void DisplayResult();
	};

#endif

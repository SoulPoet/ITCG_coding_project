#include "CommonFunc.h"

#ifndef _LAYERTYPE_
	#define _LAYERTYPE_
	enum LAYER_TYPE{ZERO_LAYER=0, INFO_LAYER=1};//1表示信息层,0表示归零层
#endif

#ifndef _EDGETYPE_
#define _EDGETYPE_
	enum EDGE_TYPE{EQ_EDGE=0, PLUS_EDGE=1};
#endif

#ifndef _ITCG_BMST_CODEC_
	#define _ITCG_BMST_CODEC_
	#define BASE_CODEC_CLASS LineEncoder

	struct ParallelEdge {
		int m_edge_no;
		struct Layer *m_eq_layer;
		struct Layer *m_plus_layer;
		
		double m_alpha[2];
		double m_beta[2];
		double *m_v2c;
		double *m_c2v;
		int *m_pai;

		struct ParallelEdge *m_left;
		struct ParallelEdge *m_right;
		struct ParallelEdge *m_up;
		struct ParallelEdge *m_down;
	};

	struct Layer {
	public:
		int m_layer_no;
		int *m_uu;
		int *m_cc;
		int *m_uu_hat;
		int *m_cc_hat;

		int m_success;
		double *m_S2C;
		double *m_C2S;

		double *m_Conv2Eq;
		double *m_Eq2Conv;

		ParallelEdge *m_EqEdge;
		ParallelEdge *m_PlusEdge;

		double *m_Channel2Plus;
		double *m_Plus2Channel;

		struct Layer *m_up;
		struct Layer *m_down;

		LAYER_TYPE m_layer_type;
	};

	class BMST_Codec {
	public:
		BMST_Codec();
		~BMST_Codec();

		
		//code parameters
		int LDPC_setup_no;
		int m_codedim;
		int m_codelen;
		double m_coderate;
		BASE_CODEC_CLASS m_codec;
		//char *m_LDPCfile;
		char m_ccfile[80];
		int m_len_layeruu;
		int m_len_layercc;

	//for Markov transmission
		int m_memory;
		int m_delay;
		int m_num_layer;
		int **m_pai;
		int m_tot_layer_num;

	//graph
		Layer *m_Layer;

	//
		int m_max_iter;
		double m_oldYentropy;
		double m_newYentropy;
		double m_entropy_delta;
		int m_success;

		void Init(const char *config_file_path);
		void Clean();

		/*
		void Malloc(int code_no, char *file_name);
		void Free();

		void InitLayerMsg(Layer *player);
		void AddLayer(int layer_no, LAYER_TYPE layer_type);
		void RemEdge(ParallelEdge* tempedge, EDGE_TYPE edge_type, bool bCombineMsg);//true是combine，false是不combine;
		void RemMidLayer(Layer *player, bool bCombineMsg);//true是combine，false是不combine;
		void RemTopLayer();
		void RemLayer();

		void InitDecoder(double *M2V);
		void Encoder(int *uu, int *cc);
		void UpdateLayer(Layer* player);
		int  DecoderTerminated(double *M2V, int *uu_hat);
		*/
	};

#endif
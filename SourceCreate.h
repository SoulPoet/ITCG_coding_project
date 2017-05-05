#include "CommonFunc.h"

#ifndef _ITCG_SOURCE_CREATE_
	#define _ITCG_SOURCE_CREATE_

	class SourceCreate {
	public:
		SourceCreate();
		~SourceCreate();
		static void Init(const char *config_file_path);
		static void CreateBitStream(int *bit_stream, int len);
		static void Clean();
	};

#endif

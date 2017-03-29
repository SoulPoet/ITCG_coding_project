#include "CommonFunc.h"

#ifndef _ITCG_SOURCE_CREATE_
	#define _ITCG_SOURCE_CREATE_

	class SourceCreate {
	public:
		SourceCreate();
		~SourceCreate();
		static void CreateBitStream(int *bit_stream, int len);
	};

#endif

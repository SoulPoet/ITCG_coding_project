#include "CommonFunc_ZLM.h"

#ifndef _ITCG_SOURCE_CREATE_ZLM_
	#define _ITCG_SOURCE_CREATE_ZLM_

	class SourceCreate {
	public:
		SourceCreate() {}
		~SourceCreate() {}
		static void CreateStream(BaseVector &t, int len) {
			long int seed = time(NULL);
			t.resize(len);
			for(int i = 0; i < len; ++i)
				t[i] = Uniform(0.0, 1.0, seed) * (t.base + 1);
		}
	};

#endif

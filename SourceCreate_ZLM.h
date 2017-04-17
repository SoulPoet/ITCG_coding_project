#include "CommonFunc.h"

#ifndef _ITCG_SOURCE_CREATE_ZLM_
	#define _ITCG_SOURCE_CREATE_ZLM_

	class SourceCreate {
	public:
		SourceCreate() {}
		~SourceCreate() {}
		static void CreateStream(CodeWord &t) {
			for(int i = 0; i < len; ++i)
				t[i] = Uniform(0.0, 1.0) * (t.base + 1);
		}
	};

#endif

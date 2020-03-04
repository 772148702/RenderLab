#ifndef _CPPUTIL_BASIC_MATH_UGM_EXT_NORM_BASE_H_
#define _CPPUTIL_BASIC_MATH_UGM_EXT_NORM_BASE_H_

namespace CppUtil {
	namespace Basic {
		namespace EXT {
			enum class NormType {
				InnerProduct, // Ҫ���������ڻ��ռ�
			};

			// ���������ԣ��ռ�
			// BaseT ��һ�����Կռ�
			template <int N, typename T, NormType normType, typename BaseT, typename ImplT>
			class Normed_Base;
		}
	}
}

#endif // !_CPPUTIL_BASIC_MATH_UGM_EXT_NORM_BASE_H_

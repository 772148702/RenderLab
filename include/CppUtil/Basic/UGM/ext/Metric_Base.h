#ifndef _CPPUTIL_BASIC_MATH_UGM_EXT_METRIC_BASE_H_
#define _CPPUTIL_BASIC_MATH_UGM_EXT_METRIC_BASE_H_

namespace CppUtil {
	namespace Basic {
		namespace EXT {
			enum class MetricType {
				Euclidean, // Ҫ����������������
			};

			template <int N, typename T, MetricType metricT, typename BaseT, typename ImplT>
			class Metric_Base;
		}
	}
}

#endif // !_CPPUTIL_BASIC_MATH_UGM_EXT_METRIC_BASE_H_

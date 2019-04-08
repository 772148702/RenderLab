#ifndef _BASIC_NODE_VISITOR_H_
#define _BASIC_NODE_VISITOR_H_

#include <CppUtil/Basic/HeapObj.h>
#include <CppUtil/Basic/TypeMap.h>
#include <CppUtil/Basic/GStorage.h>

#include <CppUtil/Basic/FunctionTraits.h>

#include <functional>

namespace CppUtil {
	namespace Basic {
		class ElementBase;

		// ��Ϊ Visitor �����Ͳ���Ҫ����������Ͳ��ٴ��� HeapObj �� ImplT ��
		class Visitor : public HeapObj<Visitor> {
		public:
			using HeapObj<Visitor>::HeapObj;

		public:
			static const Ptr<Visitor> New() { return Basic::New<Visitor>(); }

		protected:
			virtual ~Visitor() = default;

		private:
			template <typename ImplT, typename BaseT>
			friend class Element;

			// ��̬�����룩�ڵõ� typeid
			// ���ʵ��� ele ��ʵ������
			// ��֧�ּ̳�
			// private + friend ʹ�øú���ֻ���� Element<ImplT> �� Accept ��ʹ��
			template<typename T>
			void Visit(Basic::Ptr<T> ele) {
				auto target = visitOps.find(typeid(T));
				if (target != visitOps.end())
					target->second(ele);
			}

		public:
			template<typename LambadaExpr>
			void Reg(LambadaExpr lambdaVisitFunc) {
				using ptrE = FunctionTraitsLambda<LambadaExpr>::arg<0>::type;
				using EleType = ptrE::element_type;
				const Func func = [lambdaVisitFunc](Basic::Ptr<ElementBase> pEle) {
					lambdaVisitFunc(EleType::PtrCast(pEle));
				};
				visitOps[typeid(EleType)] = func;
			}

		protected:
			template<typename EleType, typename ImplT>
			void RegMemberFunc(void (ImplT::*visitFunc)(Basic::Ptr<EleType>)) {
				ImplT * obj = dynamic_cast<ImplT*>(this);
				visitOps[typeid(EleType)] = [obj, visitFunc](Basic::Ptr<ElementBase> pEle) {
					(obj->*visitFunc)(EleType::PtrCast(pEle));
				};
			}

		private:
			using Func = std::function< void(Basic::Ptr<ElementBase>) >;
			TypeMap< std::function< void(Basic::Ptr<ElementBase>) > > visitOps;
		};
	}
}

#endif // !_BASIC_NODE_VISITOR_H_

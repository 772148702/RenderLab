#pragma once

#include <Basic/HeapObj.h>
#include <Basic/TypeMap.h>
#include <Basic/GStorage.h>

#include <Basic/FunctionTraits.h>

#include <functional>

namespace Ubpa {
	class Element;

	class Visitor : public HeapObj {
	public:
		static const Ptr<Visitor> New() { return Ubpa::New<Visitor>(); }

	protected:
		// !!! �����ڹ��캯����ʹ�ã�����ʼ������ŵ� Init() ��
		const Ptr<Visitor> This() { return HeapObj::This<Visitor>(); }
		// !!! �����ڹ��캯����ʹ�ã�����ʼ������ŵ� Init() ��
		const PtrC<Visitor> This() const { return HeapObj::This<Visitor>(); }
		// !!! �����ڹ��캯����ʹ�ã�����ʼ������ŵ� Init() ��
		const WPtr<Visitor> WThis() noexcept { return This(); }
		// !!! �����ڹ��캯����ʹ�ã�����ʼ������ŵ� Init() ��
		const WPtrC<Visitor> WThis() const noexcept { return This(); }

	protected:
		virtual ~Visitor() = default;

	private:// Visit ����ֻ���� Element ����
		friend class Element;

		// ��̬
		void Visit(Ptr<Element> ele);

	public:
		template<typename LambadaExpr>
		void Reg(LambadaExpr lambdaVisitFunc) {
			using ptrE = typename FunctionTraitsLambda<LambadaExpr>::template arg<0>::type;
			using EleType = typename ptrE::element_type;
			const Func func = [lambdaVisitFunc](Ptr<Element> pEle) {
				lambdaVisitFunc(CastTo<EleType>(pEle));
			};
			visitOps[typeid(EleType)] = func;
		}

	protected:
		template<typename EleType, typename ImplT>
		void RegMemberFunc(void (ImplT::* visitFunc)(Ptr<EleType>)) {
			ImplT* obj = dynamic_cast<ImplT*>(this);
			visitOps[typeid(EleType)] = [obj, visitFunc](Ptr<Element> pEle) {
				(obj->*visitFunc)(CastTo<EleType>(pEle));
			};
		}

	private:
		using Func = std::function< void(Ptr<Element>) >;
		TypeMap< std::function< void(Ptr<Element>) > > visitOps;
	};
}

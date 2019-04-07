#ifndef _BASIC_HEAP_OBJ_HEAP_OBJ_H_
#define _BASIC_HEAP_OBJ_HEAP_OBJ_H_

#include <CppUtil/Basic/Ptr.h>

namespace CppUtil {
	namespace Basic {
		// ���� HeapObj ��Ҫģ��
		// ������Ҫһ�����еĻ��࣬�����ṩ��һ�� HeapObj_Base
		class HeapObj_Base {
		// �� new ��Ȩ�޽����� _New ����
		template<typename ImplT, typename ...Args>
		friend const CppUtil::Basic::Ptr<ImplT> _New(Args... args);

		protected:
			// ���ڹ��캯���в���ʹ�� This(), CThis(), WPtr(), WCPtr()
			// ����ר���ṩ��һ�� Init ����
			// �ú������� new �������� shared_ptr �����
			// ���Ըú����ڲ���ʹ�� This(), CThis(), WPtr(), WCPtr()
			virtual void Init() {}

		protected:
			// ���� protected ���캯������������
			// ʹ���û�����ֱ����ջ�ϴ��� HeapObj_Base ����
			// ���ǻ��ǿ�����ջ�ϴ��� ����
			// ��������ҲҪ protected ���캯������������

			HeapObj_Base() = default;
			virtual ~HeapObj_Base() = default;

		private:
			// private new �� delete
			// �����û����޷�ʹ�� new ��
			// ��������Ҳû�� new

			void * operator new(size_t size) {
				if (void *mem = malloc(size))
					return mem;
				else
					throw std::bad_alloc();
			}

			void operator delete(void * mem) noexcept {
				free(mem);
			}
		};

		// ���� ImplT �Ĺ��캯����Ȼ������ shared_ptr��Ȼ����� virtual �� Init ����
		template<typename ImplT, typename ...Args>
		const Ptr<ImplT> _New(Args... args) {
			const auto pImplT = CppUtil::Basic::Ptr<ImplT>(new ImplT(args...), &HeapObj_Base::operator delete);
			static_cast<CppUtil::Basic::Ptr<HeapObj_Base>>(pImplT)->Init();
			return pImplT;
		}
		
		/*
		HeapObj �Ѷ���
		[ʹ�÷���]
		class ImplT final : public HeapObj<ImplT> {
		HEAPOBJ
		private: ImplT(int n);
		public: static const Ptr New(int n) { return _New(n); }
		private: virtual void Init() override;
		protected: virtual ~ImplT();};
		*/
		template<typename ImplT>
		class HeapObj : public HeapObj_Base, public std::enable_shared_from_this<ImplT> {
		public:
			typedef CppUtil::Basic::Ptr<ImplT> Ptr;
			typedef CppUtil::Basic::CPtr<ImplT> CPtr;
			typedef CppUtil::Basic::WPtr<ImplT> WPtr;
			typedef CppUtil::Basic::WCPtr<ImplT> WCPtr;

		public:
			// !!! �����ڹ��캯����ʹ��
			const Ptr This() { return shared_from_this(); }
			// !!! �����ڹ��캯����ʹ��
			const CPtr CThis() const { return shared_from_this(); }
			// !!! �����ڹ��캯����ʹ��
			const WPtr WThis() noexcept { return weak_from_this(); }
			// !!! �����ڹ��캯����ʹ��
			const WCPtr WCThis() const noexcept { return weak_from_this(); }

		protected:
			// ���� protected ���캯������������
			// ʹ���û�����ֱ����ջ�ϴ��� HeapObj ����
			// ���ǻ��ǿ�����ջ�ϴ��� ����
			// ��������ҲҪ protected ���캯������������

			HeapObj() = default;
			virtual ~HeapObj() = default;

		private:
			using std::enable_shared_from_this<ImplT>::shared_from_this;
			using std::enable_shared_from_this<ImplT>::weak_from_this;
		};
	}
}

#endif // !_BASIC_HEAP_OBJ_HEAP_OBJ_H_

#ifndef _BASIC_HEAP_OBJ_HEAP_OBJ_H_
#define _BASIC_HEAP_OBJ_HEAP_OBJ_H_

#include <CppUtil/Basic/Ptr.h>

namespace CppUtil {
	namespace Basic {
		// ���� HeapObj ��Ҫģ��
		// ������Ҫһ�����еĻ��࣬�����ṩ��һ�� HeapObjBase
		class HeapObjBase {
		// �� new �� Delete ��Ȩ�޽����� New ����
		template<typename ImplT, typename ...Args>
		friend const Ptr<ImplT> New(Args && ... args);

		protected:
			// ���ڹ��캯���в���ʹ�� This(), WPtr()
			// ����ר���ṩ��һ�� Init ����
			// �ú������� new �������� shared_ptr �����
			// ���Ըú����ڲ���ʹ�� This(), WPtr()
			virtual void Init() {}

		protected:
			// ���� protected ���캯������������
			// ʹ���û�����ֱ����ջ�ϴ��� HeapObjBase ����
			// ���ǻ��ǿ�����ջ�ϴ��� ����
			// ��������ҲҪ protected ���캯������������

			HeapObjBase() = default;
			virtual ~HeapObjBase() = default;

		private:
			// ���� New ��ɾ��
			static void Delete(HeapObjBase * obj) {
				delete obj;
			}

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
		const Ptr<ImplT> New(Args && ... args) {
			const auto pImplT = Ptr<ImplT>(new ImplT(args...), HeapObjBase::Delete);
			static_cast<Ptr<HeapObjBase>>(pImplT)->Init();
			return pImplT;
		}
		
		/*
		�Ѷ���
		ImplT ������Ҫʵ�ֵ���
		BaseT Ҫ���� HeapObjBase ������
		*/
		template<typename ImplT, typename BaseT = HeapObjBase>
		class HeapObj : public BaseT, public std::enable_shared_from_this<ImplT> {
		public:
			using BaseT::BaseT;

		public:
			// !!! �����ڹ��캯����ʹ�ã�����ʼ������ŵ� Init() ��
			const Ptr<ImplT> This() { return shared_from_this(); }
			// !!! �����ڹ��캯����ʹ�ã�����ʼ������ŵ� Init() ��
			const CPtr<ImplT> This() const { return shared_from_this(); }
			// !!! �����ڹ��캯����ʹ�ã�����ʼ������ŵ� Init() ��
			const WPtr<ImplT> WThis() noexcept { return weak_from_this(); }
			// !!! �����ڹ��캯����ʹ�ã�����ʼ������ŵ� Init() ��
			const WCPtr<ImplT> WThis() const noexcept { return weak_from_this(); }

		public:
			template<typename T>
			static const Ptr<ImplT> PtrCast(const Ptr<T> & ptrT) {
				return std::dynamic_pointer_cast<ImplT>(ptrT);
			}

		protected:
			// ���� protected ���캯������������
			// ʹ���û�����ֱ����ջ�ϴ��� HeapObj ����
			// ���ǻ��ǿ�����ջ�ϴ��� ����
			// ��������ҲҪ protected ���캯������������

			virtual ~HeapObj() = default;

		private:
			using std::enable_shared_from_this<ImplT>::shared_from_this;
			using std::enable_shared_from_this<ImplT>::weak_from_this;
		};
	}
}

#endif // !_BASIC_HEAP_OBJ_HEAP_OBJ_H_

#ifndef _BASIC_HEAP_OBJ_HEAP_OBJ_H_
#define _BASIC_HEAP_OBJ_HEAP_OBJ_H_

#include <CppUtil/Basic/Ptr.h>

namespace CppUtil {
	namespace Basic {
		// ���� HeapObj ��Ҫģ��
		// ������Ҫһ�����еĻ��࣬�����ṩ��һ�� HeapObj_Base
		class HeapObj_Base {
		// �� new �� Delete ��Ȩ�޽����� New ����
		template<typename ImplT, typename ...Args>
		friend const Ptr<ImplT> New(Args && ... args);

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
			// ���� New ��ɾ��
			static void Delete(HeapObj_Base * obj) {
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
			const auto pImplT = Ptr<ImplT>(new ImplT(args...), HeapObj_Base::Delete);
			static_cast<Ptr<HeapObj_Base>>(pImplT)->Init();
			return pImplT;
		}
		
		/*
		HeapObj �Ѷ���
		[��]
		class ImplT final : public HeapObj<ImplT> {
		public: ImplT(int n);
		private: virtual void Init() override;
		protected: virtual ~ImplT();};
		[����]
		Ptr<ImplT> impl = New<Impl>(0);//���ݹ��캯���Ĳ�������������
		*/
		template<typename ImplT>
		class HeapObj : public HeapObj_Base, public std::enable_shared_from_this<ImplT> {
		public:
			// !!! �����ڹ��캯����ʹ��
			const Ptr<ImplT> This() { return shared_from_this(); }
			// !!! �����ڹ��캯����ʹ��
			const CPtr<ImplT> CThis() const { return shared_from_this(); }
			// !!! �����ڹ��캯����ʹ��
			const WPtr<ImplT> WThis() noexcept { return weak_from_this(); }
			// !!! �����ڹ��캯����ʹ��
			const WCPtr<ImplT> WCThis() const noexcept { return weak_from_this(); }

		public:
			template<typename T>
			static const Ptr<ImplT> Cast(const Ptr<T> & ptrT) {
				return std::dynamic_pointer_cast<ImplT>(ptrT);
			}

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

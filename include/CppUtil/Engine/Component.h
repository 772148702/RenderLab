#ifndef _COMPONENT_H_
#define _COMPONENT_H_

// ����϶�Ҫ���� SObj����Ϊ�����и�ģ�庯��ʹ���� SObj �Ķ���
#include <CppUtil/Engine/SObj.h>

#include <CppUtil/Basic/Element.h>
#include <functional>

#include <type_traits>

#define COMPONENT_SETUP(CLASS) \
ELE_SETUP_SELF_DEL(CLASS) \
virtual void InitAfterGenSharePtr() { AttachSObj(GetSObj()); } \
void CLASS::AttachSObj(Basic::Ptr<SObj> sobj) { \
	Component::AttachSObj<CLASS>(sobj, This()); \
}

namespace CppUtil {
	namespace Engine {
		class SObj;

		class Component : public Basic::Element {
			ELE_SETUP(Component)
		public:
			Component(Basic::Ptr<SObj> sobj = nullptr);

			Basic::Ptr<SObj> GetSObj() { return wSObj.lock(); }
			bool IsEnabled() const { return isEnabled; }
			void SetEnable(bool isEnabled) { this->isEnabled = isEnabled; }
			void Detach();

			// ��������Ѿ��� COMPONENT_SETUP(CLASS) ��������
			virtual void AttachSObj(Basic::Ptr<SObj> sobj) = 0;

		protected:
			template<typename ChildT>
			void AttachSObj(Basic::Ptr<SObj> sobj, Basic::Ptr<ChildT> component);

		private:
			Basic::WPtr<SObj> wSObj;
			std::function<void()> detachOp;
			bool isEnabled;
		};

		template<typename ChildT>
		void Component::AttachSObj(Basic::Ptr<SObj> sobj, Basic::Ptr<ChildT> component) {
			Detach();

			if (sobj == nullptr)
				return;

			this->wSObj = sobj;
			sobj->AttachComponent(component);
			detachOp = [this]() {
				if (!this->wSObj.expired()) {
					this->wSObj.lock()->DetachComponent<ChildT>();
					this->wSObj.reset();
				}
			};
		}
	}
}

#endif // !_COMPONENT_H_

#ifndef _ENGINE_SCENE_SOBJ_H_
#define _ENGINE_SCENE_SOBJ_H_ 

#include <CppUtil/Basic/LStorage.h>
#include <CppUtil/Basic/Node.h>
#include <CppUtil/Basic/TypeMap.h>
#include <CppUtil/Basic/Visitor.h>
#include <CppUtil/Basic/UGM/Transform.h>

#include <string>
#include <vector>

namespace CppUtil {
	namespace Engine {
		class ComponentBase;

		// ���ʽ��̣���Ҫ�����������ˣ��� component ���������
		class SObj final : public Basic::Node<SObj> {
		public:
			SObj(Basic::Ptr<SObj> parent = nullptr, const std::string & name = "SObj")
				: Node(parent), name(name) { }

			bool Save(const std::string & path);

		public:
			static const Basic::Ptr<SObj> New(Basic::Ptr<SObj> parent = nullptr, const std::string & name = "SObj") {
				return Basic::New<SObj>(parent, name);
			}

			static const Basic::Ptr<SObj> Load(const std::string & path);

		protected:
			virtual ~SObj() = default;

		public:
			template<typename T>
			const Basic::Ptr<T> GetComponent(T * useless_parameter = nullptr) const;

			bool HaveComponentSameTypeWith(Basic::Ptr<ComponentBase> ptr) const;

			const std::vector<Basic::Ptr<ComponentBase>> GetAllComponents() const;

			void AttachComponent(Basic::Ptr<ComponentBase> component);

			template<typename T>
			void DetachComponent(T * useless_parameter = nullptr);

			template<typename T>
			void DetachComponent(Basic::Ptr<T> component);

			template<typename T>
			const Basic::Ptr<T> GetComponentInChildren();

			template<typename T>
			const std::vector<Basic::Ptr<T>> GetComponentsInChildren();

		public:
			// ��ʱ����
			const Basic::Transform GetLocalToWorldMatrix();
			const Basic::Transform GetWorldToLocalMatrix() {
				return GetLocalToWorldMatrix().Inverse();
			}
			const Point3 GetWorldPos() {
				return GetLocalToWorldMatrix()(Point3(0));
			}

		public:
			std::string name;

		private:
			template<typename T, bool T_is_base_of_Component>
			struct Wrapper;

		private:
			Basic::TypeMap<Basic::Ptr<ComponentBase>> components;
		};

#include <CppUtil/Engine/SObj.inl>
	}
}
#endif//!_ENGINE_SCENE_SOBJ_H_

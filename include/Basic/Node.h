#ifndef _BASIC_NODE_NODE_H_
#define _BASIC_NODE_NODE_H_

#include <Basic/Element.h>

#include <set>

namespace CppUtil {
	namespace Basic {
		template<typename ImplT>
		class Node : public Element {
		protected:
			Node(Ptr<ImplT> parent = nullptr)
				: parent(parent) { }

			virtual ~Node() = default;

		public:
			// ��� child ��ԭ���ӹ�ϵ��Ȼ���������¸��ӹ�ϵ 
			void AddChild(Ptr<ImplT> child) {
				if (!child->parent.expired())
					child->parent.lock()->DelChild(child);

				child->parent = This<ImplT>();
				children.insert(child);
			}

			void DelChild(Ptr<ImplT> child) {
				if (child->parent.lock() == This()) {
					children.erase(child);
					child->parent.reset();
				}
			}

			const Ptr<ImplT> GetParent() const { return parent.lock(); }
			const std::set<Ptr<ImplT>> & GetChildren() const { return children; }

			bool IsDescendantOf(PtrC<ImplT> node) const {
				if (This<ImplT>() == node)
					return true;

				if (parent.expired())
					return false;

				return parent.lock()->IsDescendantOf(node);
			}

			bool IsAncestorOf(PtrC<ImplT> node) const {
				return node->IsDescendantOf(This<ImplT>());
			}

		public:
			void TraverseAccept(Ptr<Visitor> visitor) {
				Accept(visitor);
				for (auto child : GetChildren())
					child->TraverseAccept(visitor);
			}

			void AscendAccept(Ptr<Visitor> visitor) {
				Accept(visitor);
				const auto parent = GetParent();
				if (parent)
					parent->AscendAccept(visitor);
			}

		protected:
			virtual void Init_AfterGenPtr() override {
				const auto parent = GetParent();
				if (parent)
					parent->AddChild(This<ImplT>());
			}

		private:
			WPtr<ImplT> parent;
			std::set<Ptr<ImplT>> children;
		};
	}
}

#endif // !_BASIC_NODE_NODE_H_

#include <CppUtil/Engine/SObj.h>

#include <CppUtil/Engine/Component.h>

#include <iostream>

using namespace CppUtil::Engine;

SObj::~SObj() {
	// ע�����ѭ������

	// ��Ϊ Detach() ��ɾ�� component������Ӧ����ȡ����һ�� component
	auto next = components.begin();
	while (next != components.end()) {
		auto cur = next;
		next++;
		cur->second->Detach();
	}
	
}

void SObj::AttachComponent(CppUtil::Basic::Ptr<Component> component) {
	auto target = components.find(typeid(component));
	if (target != components.end())
		target->second->Detach();

	components[typeid(component)] = component;
}

std::vector<CppUtil::Basic::Ptr<Component>> SObj::GetAllComponents() const {
	std::vector<CppUtil::Basic::Ptr<Component>> rst;

	for (auto & component : components)
		rst.push_back(component.second);

	return rst;
}

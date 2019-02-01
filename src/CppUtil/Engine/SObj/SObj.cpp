#include <CppUtil/Engine/SObj.h>

#include <CppUtil/Engine/Component.h>

using namespace CppUtil::Engine;

SObj::~SObj() {
	// ע�����ѭ������

	// ��Ϊ Detach() ��ɾ�� component������Ӧ����ȡ����һ�� component
	auto next = components.begin();
	while (next != components.end()) {
		auto cur = next;
		next++;
		(*cur)->Detach();
	}
	
}
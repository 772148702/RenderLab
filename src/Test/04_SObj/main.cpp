#include <CppUtil/Engine/CmptCamera.h>
#include <CppUtil/Engine/SObj.h>

#include <CppUtil/Basic/Math.h>

#include <iostream>
#include <string>

using namespace CppUtil::Engine;
using namespace CppUtil::Basic;
using namespace std;

int main() {
	auto sobj1 = SObj::New(nullptr, "sobj_test");

	cout << "new camera2, not attach" << endl;
	auto camera2 = CmptCamera::New(nullptr);
	cout << "camera2 have " << (camera2->GetSObj() ? "" : "not ") << "sobj." << endl;

	{
		auto sobj2 = SObj::New(nullptr, "sobj_test");

		cout << "enter scope" << endl << endl;

		cout << "sobj1 have " << sobj1->GetAllComponents().size() << " components." << endl;

		cout << "camera0 attach to sobj1" << endl;
		auto camera0 = CmptCamera::New(sobj1);

		cout << "camera1 attach to sobj1" << endl;
		// ��Ϊ sobj1 ֻ����һ�� camera component�������Ȼ�� camera0 ���
		auto camera1 = CmptCamera::New(sobj1);

		cout << "camera0 have " << (camera0->GetSObj() ? "" : "not ") << "sobj." << endl;
		cout << "camera1 have " << (camera1->GetSObj() ? "" : "not ") << "sobj." << endl;

		cout << "sobj1 have " << sobj1->GetAllComponents().size() << " components." << endl;

		cout << "datach camera1" << endl;
		camera1->GetSObj()->DetachComponent(camera1);
		cout << "camera1 have " << (camera1->GetSObj() ? "" : "not ") << "sobj." << endl;
		cout << "sobj1 have " << (sobj1->GetComponent<CmptCamera>() ? "" : "not ") << "a [ Camera ] component." << endl;


		cout << "camera2 attach to sobj2" << endl;
		sobj2->AttachComponent(camera2);
		cout << "camera2 have " << (camera2->GetSObj() ? "" : "not ") << "sobj." << endl;
		cout << "sobj2 have " << sobj2->GetAllComponents().size() << " components." << endl;

		cout << endl << "exit scope" << endl;
	}

	// ��Ϊ camera2 û�� sobj2 ������Ȩָ�룬�������뿪��Χʱ sobj2 ���������Ӷ������ camera2 �İ�
	cout << "camera2 have " << (camera2->GetSObj() ? "" : "not ") << "sobj." << endl;

	return 0;
}

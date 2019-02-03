#include <CppUtil/Engine/Camera.h>
#include <CppUtil/Engine/SObj.h>

#include <CppUtil/Basic/Math.h>

#include <iostream>
#include <string>

using namespace CppUtil::Engine;
using namespace CppUtil::Basic;
using namespace glm;
using namespace std;

int main() {
	SObj::Ptr sobj1 = ToPtr(new SObj("sobj_test"));

	cout << "new camera2, not attach" << endl;
	Camera::Ptr camera2 = ToPtr(new Camera(nullptr));
	cout << "camera2 have " << (camera2->GetSObj() ? "" : "not ") << "sobj." << endl;

	{
		SObj::Ptr sobj2 = ToPtr(new SObj("sobj_test"));

		cout << "enter scope" << endl << endl;

		cout << "sobj1 have " << sobj1->GetAllComponents().size() << " components." << endl;

		cout << "camera0 attach to sobj1" << endl;
		Camera::Ptr camera0 = ToPtr(new Camera(sobj1));

		cout << "camera1 attach to sobj1" << endl;
		// ��Ϊ sobj1 ֻ����һ�� camera component�������Ȼ�� camera0 ���
		Camera::Ptr camera1 = ToPtr(new Camera(sobj1));

		cout << "camera0 have " << (camera0->GetSObj() ? "" : "not ") << "sobj." << endl;
		cout << "camera1 have " << (camera1->GetSObj() ? "" : "not ") << "sobj." << endl;

		cout << "sobj1 have " << sobj1->GetAllComponents().size() << " components." << endl;

		cout << "datach camera1" << endl;
		camera1->Detach();
		cout << "camera1 have " << (camera1->GetSObj() ? "" : "not ") << "sobj." << endl;
		cout << "sobj1 have " << (sobj1->GetComponent<Camera>() ? "" : "not ") << "a [ Camera ] component." << endl;


		cout << "camera2 attach to sobj2" << endl;
		camera2->AttachSObj(sobj2);
		cout << "camera2 have " << (camera2->GetSObj() ? "" : "not ") << "sobj." << endl;
		cout << "sobj2 have " << sobj2->GetAllComponents().size() << " components." << endl;

		cout << endl << "exit scope" << endl;
	}

	// ��Ϊ camera2 û�� sobj2 ������Ȩָ�룬�������뿪��Χʱ sobj2 ���������Ӷ������ camera2 �İ�
	cout << "camera2 have " << (camera2->GetSObj() ? "" : "not ") << "sobj." << endl;

	return 0;
}

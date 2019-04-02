#include "Picker.h"

#include <UI/Attribute.h>

#include <CppUtil/Engine/Viewer.h>
#include <CppUtil/Engine/Scene.h>
#include <CppUtil/Engine/SObj.h>
#include <CppUtil/Engine/RayIntersector.h>
#include <CppUtil/Engine/Ray.h>
#include <CppUtil/Engine/Roamer.h>

#include <CppUtil/Qt/RawAPI_OGLW.h>

#include <CppUtil/OpenGL/Camera.h>

#include <CppUtil/Basic/LambdaOp.h>
#include <CppUtil/Basic/EventManager.h>

#include <QtWidgets/QApplication>

using namespace CppUtil::Engine;
using namespace CppUtil::Basic;

Picker::Picker(Viewer * viewer)
	: viewer(viewer), rayIntersector(ToPtr(new RayIntersector)) { }

void Picker::Init() {
	auto MRB_PressOp = ToPtr(new LambdaOp([this]() {
		Viewer * viewer = this->GetViewer();
		auto pOGWL = viewer->GetOGLW();
		auto camera = viewer->GetRoamer()->GetCamera();

		int x = pOGWL->x;
		int y = pOGWL->y;

		// y ��Ҫ������
		Point3 posOnScreen(x / float(pOGWL->w), 1 - y / float(pOGWL->h), 1);
		Point3 posInNorm = Vec3(posOnScreen) * 2.0f - 1.0f;
		Point3 posInWorld = (camera->GetProjectionMatrix() * camera->GetViewMatrix()).Inverse()(posInNorm);

		Vec3 dir = posInWorld - camera->GetPos();
		Ray ray(camera->GetPos(), dir);
		rayIntersector->Init(&ray);
		viewer->GetScene()->GetRoot()->Accept(rayIntersector);
		auto closestRst = rayIntersector->GetRst();
		if (closestRst.closestSObj)
			Ui::Attribute::GetInstance()->SetSObj(closestRst.closestSObj);
	}));
	EventMngr::GetInstance().Reg(Qt::LeftButton, (void*)viewer->GetOGLW(), EventMngr::MOUSE_PRESS, MRB_PressOp);
}

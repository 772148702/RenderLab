#ifndef _RTX_RT_MATERIAL_DIELETRIC_H_
#define _RTX_RT_MATERIAL_DIELETRIC_H_

#include <CppUtil/RTX/Material.h>
#include <glm/vec3.hpp>

namespace RTX {
	class Dielectric : public Material {
		MATERIAL_SETUP(Dielectric)
	public:
		Dielectric(float refractIndex, glm::vec3 attenuationConst = glm::vec3(0));
		
		// ����ֵΪ true ˵�����߼�������
		// ����ֵΪ false ˵�����߲��ٴ���
		virtual bool Scatter(const HitRecord & rec) const;
		float GetRafractIndex() const { return refractIndex; }
	private:
		float refractIndex;
		glm::vec3 attenuationConst;
	};
}

#endif // !_RTX_RT_MATERIAL_DIELETRIC_H_

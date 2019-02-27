#ifndef _CPPUTIL_ENGINE_SCENE_SOBJ_LOADER_H_
#define _CPPUTIL_ENGINE_SCENE_SOBJ_LOADER_H_

#include <3rdParty/tinyxml2.h>
#include <CppUtil/Basic/Ptr.h>

#include <string>
#include <map>
#include <functional>
#include <type_traits>

namespace CppUtil {
	namespace Engine {
		class SObj;

		class Camera;

		class Geometry;
		class Sphere;
		class Plane;
		class TriMesh;

		class Light;
		class AreaLight;
		class PointLight;

		class Material;
		class BSDF_CookTorrance;
		class BSDF_Diffuse;
		class BSDF_Emission;
		class BSDF_Glass;
		class BSDF_MetalWorkflow;
		class BSDF_Mirror;

		class Transform;

		class SObjLoader {
		public:
			static Basic::Ptr<SObj> Load(const std::string & path);
		private:
			// �� ele ��Ϊ ValImplType ����
			template<typename ValImplType>
			static Basic::Ptr<ValImplType> Load(tinyxml2::XMLElement * ele, ValImplType * useless = nullptr);

			// �� ele ��Ϊ ValImplType ���룬�ڲ�Ҫ���а󶨣����߽��� obj �����񴫵���һ��
			template<typename ValImplType, typename ObjType>
			static void LoadAndBind(tinyxml2::XMLElement * ele, Basic::Ptr<ObjType> obj, ValImplType * useless = nullptr);

		private:
			typedef std::map<std::string, std::function<void(tinyxml2::XMLElement *)> > FuncMap;
			// ���� funcMap ���� element ����
			static void LoadChildrenEles(tinyxml2::XMLElement * ele, const FuncMap & funcMap);

			// �� name == key �� ele �� text �� T ��ֵ�� val
			template<typename T>
			static void Reg(FuncMap & funcMap, const char * const key, T & val) {
				funcMap[key] = [&](tinyxml2::XMLElement * ele) {
					val = To<T>(ele->GetText());
				};
			}

			// �� name == key �� ele �� text �� T ����������func
			template<typename T>
			static void Reg(FuncMap & funcMap, const char * const key, const std::function<void(const T &)> & func) {
				funcMap[key] = [=](tinyxml2::XMLElement * ele) {
					func(To<T>(ele->GetText()));
				};
			}

			// �� name == key �� ele �� text �� T ���������� obj��setVal
			template<typename ValType, typename ObjType, typename RetType>
			static void Reg(FuncMap & funcMap, const char * const key, Basic::Ptr<ObjType> obj, RetType(ObjType::*setVal)(ValType)) {
				funcMap[key] = [=](tinyxml2::XMLElement * ele) {
					((*obj).*setVal)(To<std::remove_cv<std::remove_reference<ValType>::type>::type>(ele->GetText()));
				};
			}
			
			// �� name == key �� ele ���������� func
			static void Reg(FuncMap & funcMap, const char * const key, const std::function<void(tinyxml2::XMLElement *)> & func) {
				funcMap[key] = [=](tinyxml2::XMLElement * ele) {
					func(ele);
				};
			}

			// �� name == key �� ele ��Ϊ ValImpl������ Load ����
			template<typename ValImplType>
			static void Reg(FuncMap & funcMap, const char * const key, ValImplType * useless = nullptr) {
				Reg(funcMap, key, [=](tinyxml2::XMLElement * ele) {
					Load<ValImplType>(ele);
				});
			}

			// �� name == key �� ele ��Ϊ ValImpl������ LoadAndBind ���벢��
			template<typename ValImplType, typename ObjType>
			static void Reg(FuncMap & funcMap, const char * const key, Basic::Ptr<ObjType> obj, ValImplType * useless = nullptr) {
				Reg(funcMap, key, [=](tinyxml2::XMLElement * ele) {
					LoadAndBind<ValImplType>(ele, obj);
				});
			}

			// �� ele ��Ϊ ValImpl ���룬������һ�����ú���
			template<typename ValImplType, typename ValType, typename ObjType>
			static void Reg(FuncMap & funcMap, const char * const key, Basic::Ptr<ObjType> obj, void(ObjType::*setVal)(Basic::Ptr<ValType>), ValImplType * useless = nullptr) {
				Reg(funcMap, key, [=](tinyxml2::XMLElement * ele) {
					((*obj).*setVal)(Load<ValImplType>(ele));
				});
			}

			// ����Ҫ�� ObjBaseType �� ObjType �Ļ���
			template<typename ObjType, typename ValType>
			struct Pack {
				template<typename ObjBaseType>
				Pack(Basic::Ptr<ObjType> obj, void(ObjBaseType::*setVal)(Basic::Ptr<ValType>))
					:obj(obj), setVal(setVal) { }

				FuncMap funcMap;
				Basic::Ptr<ObjType> obj;
				void(ObjType::*setVal)(Basic::Ptr<ValType>);
			};

			// �������� Pack�������Ͳ���Ҫ�ֶ�������ģ��������
			template<typename ObjType, typename ValType, typename ObjBaseType>
			static Pack<ObjType, ValType> GenPack(Basic::Ptr<ObjType> obj, void(ObjBaseType::*setVal)(Basic::Ptr<ValType>)) {
				return Pack<ObjType, ValType>(obj, setVal);
			}

			// �����ú���
			template<typename ValImplType, typename ValType, typename ObjType>
			static void Reg(Pack<ObjType, ValType> & pack, const char * const key, ValImplType * useless = nullptr) {
				Reg<ValImplType, ValType, ObjType>(pack.funcMap, key, pack.obj, pack.setVal);
			}

		private:
			template<typename T>
			static T To(const std::string & key, T * useless_arg = nullptr);
		};
	}
}

#endif//!_CPPUTIL_ENGINE_SCENE_SOBJ_SAVER_H_

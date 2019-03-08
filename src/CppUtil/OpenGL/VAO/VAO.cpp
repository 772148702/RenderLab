#include <CppUtil/OpenGL/VAO.h>

#include<CppUtil/OpenGL/Shader.h>

#if USE_QT_OPENGL_API
#include <CppUtil/Qt/RawAPI_Define.h>
#else
#include <glad/glad.h>
#endif // USE_QT_OPENGL_API

using namespace CppUtil::OpenGL;
using namespace std;

VAO::VAO() :isValid(false) { }

VAO::VAO(float const * data, uint dataSize, const std::vector<uint> & attrLen){
	if (data == NULL || dataSize == 0 || attrLen.size() == 0) {
		isValid = false;
		ID = 0;
		return;
	}
	
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

	uint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	uint patchLen = 0;
	for (auto & len : attrLen)
		patchLen += len;
	for (uint i = 0, accu = 0; i < attrLen.size(); i++) {
		glVertexAttribPointer(i, attrLen[i], GL_FLOAT, GL_FALSE, patchLen * sizeof(float), (void*)(accu * sizeof(float)));
		glEnableVertexAttribArray(i);
		accu += attrLen[i];
	}
	attrNum = static_cast<uint>(attrLen.size());
	//����û��������������� pointNum
	this->pointNum = dataSize / (sizeof(float) * patchLen);
	isValid = true;
	hasIndex = false;
}

VAO::VAO(float const * data, uint dataSize, const std::vector<uint> & attrLen, uint const * index, uint indexSize)
	: VAO(data, dataSize, attrLen){
	if (IsValid()) {
		pointNum = indexSize / sizeof(uint);
		hasIndex = true;
		isValid = GenBindEBO(index, indexSize);
	}
}

VAO::VAO(const std::vector<VBO_DataPatch> & vec_VBO_DataPatch, const std::vector<uint> & divisors) {
	if (vec_VBO_DataPatch.size() == 0
		|| (divisors.size() > 0 && vec_VBO_DataPatch.size() != divisors.size())) {
		isValid = false;
		return;
	}

	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

	for (uint i = 0; i < vec_VBO_DataPatch.size(); i++) {
		auto & dataPatch = vec_VBO_DataPatch[i];
		uint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, dataPatch.dataSize, dataPatch.data, GL_STATIC_DRAW);
		glVertexAttribPointer(i, dataPatch.attrLen, GL_FLOAT, GL_FALSE, dataPatch.attrLen * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(i);
		if(divisors.size()!=0 && divisors[i] > 0)
			glVertexAttribDivisor(i, divisors[i]);
	}
	//����û��������������� pointNum
	this->pointNum = vec_VBO_DataPatch[0].dataSize / (sizeof(float) * vec_VBO_DataPatch[0].attrLen);
	hasIndex = false;
	isValid = true;
}

VAO::VAO(const std::vector<VBO_DataPatch> & vec_VBO_DataPatch, uint const * index, uint indexSize, const std::vector<uint> & divisors)
	: VAO(vec_VBO_DataPatch, divisors){
	if (IsValid()) {
		pointNum = indexSize / sizeof(uint);
		hasIndex = true;
		isValid = GenBindEBO(index, indexSize);
	}
}

bool VAO::GenBindEBO(uint const * index, uint indexSize) {
	uint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, index, GL_STATIC_DRAW);
	return true;
}

uint VAO::GetID() const {
	return ID;
}

uint VAO::GetAttrNum() const {
	return ID;
}

bool VAO::IsValid() const {
	return isValid;
}

bool VAO::Use() const {
	if (!IsValid())
		return false;

	glBindVertexArray(ID);
	return true;
}

bool VAO::Draw(const Shader & shader) const {
	if (!Use() || !shader.Use())
		return false;
	if (hasIndex)
		glDrawElements(GL_TRIANGLES, pointNum, GL_UNSIGNED_INT, NULL);
	else
		glDrawArrays(GL_TRIANGLES, 0, pointNum);

	return true;
}
#include <CppUtil/Basic/HeapObj.h>

#include <iostream>
#include <vector>
#include <string>

using namespace CppUtil::Basic;
using namespace std;

// ������ģ�嶨�壬�� ImplT ���� HeapObj ��
// �����ֱ���� Ptr, CPtr, WPtr, WCPtr
template<typename ImplT>
class Node: public HeapObj<ImplT> {
protected:// ���캯�� protected������д��Ӧ�� New ����
	Node(Ptr parent = nullptr)
		: parent(parent) { }

protected:// ����һ��Ĭ�ϵ�����������
	virtual ~Node() = default;

public:
	const Ptr GetParent() { return parent.lock(); }
	const CPtr GetParent() const { return parent.lock(); }
	vector<Ptr> & GetChildren() { return children; }
	const vector<Ptr> & GetChildren() const { return children; }

protected:
	virtual void Init() override {
		if (!parent.expired())
			parent.lock()->children.push_back(This());
	}

private:
	WPtr parent;
	vector<Ptr> children;
};

class SObj final : public Node<SObj> {
public:// ���� _New ��Ҫ�õ����캯��������Ҫ public
	SObj(const string & name, SObj::Ptr parent = nullptr)
		: name(name), Node<SObj>(parent) { }

public:
	// Ϊÿһ�����캯��дһ����Ӧ�� New ����
	static const Ptr New(const string & name, SObj::Ptr parent = nullptr) {
		return _New<SObj>(name, parent);
	}

public:
	void Print(int depth = 0) const {
		for (int i = 0; i < depth; i++)
			cout << " ";

		cout << name << endl;
		for (const auto child : GetChildren())
			child->Print(depth + 1);
	}

private:
	virtual ~SObj() = default;

private:
	string name;
};

int main() {
	const auto a = SObj::New("A");
	const auto ab = SObj::New("B", a);
	const auto ac = SObj::New("C", a);
	const auto abd = SObj::New("D", ab);
	const auto ace = SObj::New("E", ac);

	a->Print();

	return 0;
}

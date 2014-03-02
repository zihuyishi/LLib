#ifndef _LLIB_LVECTOR_
#define _LLIB_LVECTOR_
#include <Windows.h>
namespace LLib{
	typedef unsigned long		size_type;

	template<typename Type>
	class LVector_base {
	public:
		typedef LVector_base<Type> myType;
	public:
		LVector_base()
		{}
		LVector_base(size_type inSize)
		{
			m_p = static_cast<Type*>(malloc(inSize * sizeof(Type)));
			memset(m_p, 0, inSize * sizeof(Type));
		}
		~LVector_base()
		{
			if (m_p != nullptr) {
				free(m_p);
				m_p = nullptr;
			}
		}
	private:
		LVector_base(const myType& lhs)
		{
		}
		myType& operator=(const myType& lhs)
		{
		}
	public:
		int AddRef() { m_count++; return m_count; }
		int Release() 
		{
			size_type tmp = m_count--;
			if (tmp == 0) {
				delete this;
			}
			return tmp;
		}
	public:
		void push_back(const Type& t)
		{
			m_length++;
			if (m_length >= m_capacity) {
				if (!Increase()) return;
			}
			*(m_p + m_length - 1) = t;
		}
		const Type& ItemAt(size_type index) const
		{
			return m_p[index];
		}
		const Type& operator[](size_type index) const
		{
			return ItemAt(index);
		}
		Type& ItemAt(size_type index)
		{
			return m_p[index];
		}
		Type& operator[](size_type index)
		{
			return ItemAt(index);
		}
		size_type Get_Length() const {
			return m_length;
		}
		size_type Get_Capacity() const {
			return m_capacity;
		}
	private:
		int Increase()
		{
			size_type new_cap = 1 + static_cast<size_type>(m_capacity * 1.5);
			m_p = static_cast<Type*>(realloc(m_p, new_cap * sizeof(Type)));
			if (m_p == NULL) {
				m_p = nullptr;
				return 0;
			}
			memset(m_p + m_capacity, 0, (new_cap - m_capacity) * sizeof(Type));
			m_capacity = new_cap;
			return m_capacity;
		}
	private:
		size_type		m_count = 1;
		Type*			m_p = nullptr;
		size_type		m_capacity;
		size_type		m_length = 0;
	};

	template<typename Type>
	class LVector {
	public:
		typedef LVector<Type>		myType;
	public:
		LVector() :
			m_p(new LVector_base<Type>)
		{

		}
		LVector(size_type inSize)
		{
			m_p = new LVector_base<Type>(inSize);
		}
		~LVector()
		{
			if (m_p != nullptr) {
				m_p->Release();
			}
		}
		LVector(const myType& lhs) :
			m_p(new LVector_base<Type>)
		{
			*this = lhs;
		}
		myType& operator=(const myType& lhs)
		{
			m_p->Release();
			m_p = lhs.m_p;
			if (m_p != nullptr) {
				m_p->AddRef();
			}
			return *this;
		}
		
	public:
		void push_back(const Type& t)
		{
			m_p->push_back(t);
		}
		const Type& ItemAt(size_type index) const
		{
			return m_p->ItemAt(index);
		}
		const Type& operator[](size_type index) const
		{
			return m_p->ItemAt(index);
		}
		Type& ItemAt(size_type index)
		{
			return m_p->ItemAt(index);
		}
		Type& operator[](size_type index)
		{
			return m_p->ItemAt(index);
		}
		size_type Get_Length() const 
		{
			return m_p->Get_Length();
		}
		size_type Get_Capacity() const 
		{
			return m_p->Get_Capacity();
		}
		myType CopyTo()
		{
			myType tmp(*this);
			m_p->Release();
			m_p = new LVector_base<Type>;
			return tmp;
		}
	private:
		LVector_base<Type>*		m_p;
	};
}

#endif
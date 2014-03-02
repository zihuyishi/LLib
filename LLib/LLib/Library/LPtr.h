#ifndef _LLIB_LPTR_
#define _LLIB_LPTR_
namespace LLib {
	
	template<typename Type>
	class LPtr_base {
	public:
		template<typename X>
		friend class LPtr;
	protected:
		LPtr_base() :_ptr(nullptr), _count(1)
		{}
		LPtr_base(Type ptr) :
			 _count(1)
		{
			_ptr = new Type(ptr);
		}
		~LPtr_base() {
			if (_ptr != nullptr)
				delete _ptr;
		}
		operator bool() const { return _ptr == nullptr; }
		Type& operator*() {
			return *_ptr;
		}
		size_t AddRef() { ++_count; return _count; }
		size_t Release() {
			size_t tmp = --_count;
			if (tmp == 0) {
				delete this;
			}
			return tmp;
		}
		Type *_ptr;
		size_t _count;
	};

	template<typename Type>
	class LPtr {
	private:
		typedef LPtr_base<Type> point;
	public:
		LPtr() : p(new point())
		{}
		LPtr(Type ptr) :p(new point)
		{
			p = new point(ptr);
		}
		LPtr& operator=(const LPtr &ptr) {
			if (p && p != ptr.p) {
				p->Release();
			}
			p = ptr.p;
			if (p) {
				p->AddRef();
			}
			return *this;
		}
		
		LPtr(LPtr &ptr) :p(new point()) {
			*this = ptr;
		}
		~LPtr() {
			p->Release();
		}
	public:
		Type& operator*() const {
			return p->operator*();
		}
		operator bool() const {
			return p->operator bool();
		}
		const Type* operator->() const {
			return this->Get();
		}

		const Type* Get() const {
			return p->_ptr;
		}
		Type* Get() {
			return p->_ptr;
		}
	private:
		bool operator=(const Type &ptr)
		{}
	private:
		LPtr_base<Type> *p;
	};
	template<typename Type>
	LPtr<Type> make_LPtr(const Type &src) {
		LPtr<Type> tmp(src);
		return tmp;
	}
}

#endif
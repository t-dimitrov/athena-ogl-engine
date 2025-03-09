#pragma once
#include <memory>

namespace Athena
{
	class RefCounted
	{
	public:
		void IncreaseRef() const { _refCount++; }
		void DecreaseRef() const { _refCount--; }

		uint32_t GetRefCount() const { return _refCount; }

	private:
		mutable uint32_t _refCount = 0;
	};

	template <typename T>
	class Ref
	{
	public:
		Ref()
			: _instance(nullptr)
		{
		}

		Ref(T* instance)
			: _instance(instance)
		{
			static_assert(std::is_base_of<RefCounted, T>::value, "Class is not reference counted");
			IncreaseRef();
		}

		Ref(std::nullptr_t)
			: _instance(nullptr)
		{
		}

		~Ref()
		{
			DecreaseRef();
		}

		Ref(const Ref<T>& other)
			: _instance(other._instance)
		{
			IncreaseRef();
		}

		Ref& operator=(const Ref<T>& other)
		{
			other.IncreaseRef();
			DecreaseRef();

			_instance = other._instance;
			return *this;
		}

		template<typename T2>
		Ref(const Ref<T2>& other)
		{
			_instance = (T*)other._instance;
			IncreaseRef();
		}

		template <typename T2>
		Ref& operator=(const Ref<T2>& other)
		{
			other.IncreaseRef();
			DecreaseRef();
			_instance = (T*)other._instance;
			return *this;
		}

		template<typename T2>
		Ref(Ref<T2>&& other)
		{
			_instance = (T*)other._instance;
			other._instance = nullptr;
		}

		template<class T2>
		Ref& operator=(Ref<T2>&& other)
		{
			DecreaseRef();
			_instance = (T*)other._instance;
			other._instance = nullptr;
			return *this;
		}

		Ref& operator=(std::nullptr_t)
		{
			DecreaseRef();
			_instance = nullptr;
			return *this;
		}

		template <typename... Args>
		static Ref<T> Create(Args&&... args)
		{
			return Ref<T>(new T(std::forward<Args>(args)...));
		}

		T* operator->() { return _instance; }
		const T* operator->() const { return _instance; }

		T& operator*() { return *_instance; }
		const T& operator*() const { return *_instance; }

		T* Raw() { return _instance; }
		const T* Raw() const { return _instance; }

		operator bool() { return _instance != nullptr; }
		operator bool() const { return _instance != nullptr; }

		void Reset(T* instance = nullptr)
		{
			DecreaseRef();
			_instance = instance;
		}

		bool operator==(const Ref<T>& other) const
		{
			return _instance == other._instance;
		}

		bool operator!=(const Ref<T>& other) const
		{
			return !(*this == other);
		}

		// Compares the internal objects of both Refs
		bool Equals(const Ref<T>& other)
		{
			if (!_instance || !other._instance)
			{
				return false;
			}

			return *_instance == *other._instance;
		}

		template <class T2>
		Ref<T2> As() const
		{
			return Ref<T2>(*this);
		}

	private:
		void IncreaseRef() const
		{
			if (_instance)
			{
				_instance->IncreaseRef();
			}
		}

		void DecreaseRef() const
		{
			if (_instance)
			{
				_instance->DecreaseRef();
				if (_instance->GetRefCount() == 0)
				{
					delete _instance;
					_instance = nullptr;
				}
			}
		}

	private:
		mutable T* _instance;

		template <class T2>
		friend class Ref;
	};
}

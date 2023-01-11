#pragma once

#include "PrimitiveTypes.h"

namespace esc {

	class ReferenceCounted
	{
	public:
		inline uint32 IncrementReferenceCount() const
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}

		inline uint32 DecrementReferenceCount() const
		{
			m_ReferenceCount--;
			return m_ReferenceCount;
		}

		uint32 GetReferenceCount() const { return m_ReferenceCount; }
	private:
		mutable std::atomic<uint32> m_ReferenceCount = 0;
	};

	template<typename T>
	class Ref
	{
	public:
		Ref()
			: m_Reference(nullptr)
		{
		}

		Ref(std::nullptr_t)
			: m_Reference(nullptr)
		{
		}

		Ref(T* reference)
			: m_Reference(reference)
		{
			IncrementReferenceCount();
		}

		Ref(const Ref<T>& other)
			: m_Reference(other.m_Reference)
		{
			IncrementReferenceCount();
		}

		Ref(const Ref<T>&& other)
		{
			m_Reference = other.m_Reference;
			other->m_Reference = nullptr;
		}

		template<typename TOther>
		Ref(const Ref<TOther>& other)
		{
			m_Reference = static_cast<T*>(other.m_Reference);
			IncrementReferenceCount();
		}

		template<typename TOther>
		Ref(Ref<TOther>&& other)
		{
			m_Reference = static_cast<T*>(other.m_Reference);
			other.m_Reference = nullptr;
		}

		~Ref()
		{
			DecrementReferenceCount();
		}

		Ref& operator=(std::nullptr_t)
		{
			DecrementReferenceCount();
			m_Reference = nullptr;
			return *this;
		}

		Ref& operator=(const Ref<T>& other)
		{
			other.IncrementReferenceCount();
			DecrementReferenceCount();
			m_Reference = other.m_Reference;
			return *this;
		}

		Ref& operator=(const Ref<T>&& other)
		{
			DecrementReferenceCount();
			m_Reference = other.m_Reference;
			other.m_Reference = nullptr;
			return *this;
		}

		template<typename TOther>
		Ref& operator=(const Ref<TOther>& other)
		{
			other.IncrementReferenceCount();
			DecrementReferenceCount();
			m_Reference = other.m_Reference;
			return *this;
		}

		template<typename TOther>
		Ref& operator=(const Ref<TOther>&& other)
		{
			DecrementReferenceCount();
			m_Reference = other.m_Reference;
			other.m_Reference = nullptr;
			return *this;
		}

		operator bool() { return m_Reference != nullptr; }
		operator bool() const { return m_Reference != nullptr; }

		T* operator->() { return m_Reference; }
		const T* operator->() const { return m_Reference; }

		T& operator*() { return *m_Reference; }
		const T& operator*() const { return *m_Reference; }

		T* Get() { return m_Reference; }
		const T* Get() const { return m_Reference; }

		template<typename TOther>
		Ref<TOther> As() const
		{
			return Ref<TOther>(*this);
		}

		template<typename... TArgs>
		static Ref<T> Create(TArgs&&... args)
		{
			return Ref<T>(new T(std::forward<TArgs>(args)...));
		}
	private:
		void IncrementReferenceCount() const
		{
			if (m_Reference)
				m_Reference->IncrementReferenceCount();
		}

		void DecrementReferenceCount() const
		{
			if (!m_Reference)
				return;

			m_Reference->DecrementReferenceCount();

			if (m_Reference->GetReferenceCount() == 0)
			{
				delete m_Reference;
				m_Reference = nullptr;
			}
		}
	private:
		mutable T* m_Reference;

		template<class TOther>
		friend class Ref;
	};

}
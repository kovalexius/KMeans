#include "helpers.h"

#include <iostream>
#include <thread>

namespace multiThread
{
	AtomicFloat::AtomicFloat(const float _val): m_value(_val)
	{
	}
	
	AtomicFloat::~AtomicFloat()
	{
		/*
		std::cout << "tid: " << std::this_thread::get_id() << 
					" " << __FUNCTION__ << std::endl;
					*/
	}
	
	bool AtomicFloat::operator == (const float _val) const
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		return m_value == _val;
	}
	
	bool AtomicFloat::operator != (const float _val) const
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		return m_value != _val;
	}
	
	AtomicFloat& AtomicFloat::operator += (const float _val)
	{
		auto tid = std::this_thread::get_id();
		//std::cout << "  lock tid: " << tid << std::endl;
		
		std::unique_lock<std::mutex> lk(m_mutex);
		m_value += _val;
		
		//std::cout << "unlock tid: " << tid << std::endl;
		
		return *this;
	}
	
	AtomicFloat& AtomicFloat::operator = (const float _val)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		m_value = _val;
		return *this;
	}
	
	AtomicFloat::operator float() const
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		return m_value;
	}
}

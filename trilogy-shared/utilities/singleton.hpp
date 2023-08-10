#pragma once

template <class singleton_t>
class c_singleton
{
protected:
	c_singleton() {}
	~c_singleton() {}

	c_singleton(const c_singleton&) = delete;
	c_singleton& operator=(const c_singleton&) = delete;

	c_singleton(c_singleton&&) = delete;
	c_singleton& operator=(c_singleton&&) = delete;

public:
	static singleton_t* instance()
	{
		if (!instance_ptr)
			instance_ptr = new singleton_t;

		return instance_ptr;
	}

private:
	inline static singleton_t* instance_ptr;
};

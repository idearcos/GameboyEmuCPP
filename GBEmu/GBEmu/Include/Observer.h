#pragma once

#include <set>

template <typename Observer>
class Subject
{
public:
	void AddObserver(Observer *observer)
	{
		observers_.insert(observer);
	}

	void RemoveObserver(Observer *observer)
	{
		observers_.erase(observer);
	}

protected:
	template <typename Function, typename... Args>
	void Notify(Function func, Args&&... args)
	{
		for (auto& observer : observers_)
		{
			(observer->*func)(std::forward<Args>(args)...);
		}
	}

private:
	std::set<Observer*> observers_;
};

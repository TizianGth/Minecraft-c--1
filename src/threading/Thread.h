#pragma once
#include <future>

class Thread
{
public:
	Thread(int id, std::future<bool> &finished) {
		m_Id = id;
		m_Finished = finished.share();
	};
	~Thread() {
	};


	int m_Id = 0;
	std::shared_future<bool> m_Finished;
	private:
	 
};


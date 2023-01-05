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


	unsigned int m_Id = -1;
	std::shared_future<bool> m_Finished;
	private:
	 
};


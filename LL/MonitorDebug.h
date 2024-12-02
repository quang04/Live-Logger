#pragma once
#include "CallbackEventHandler.h"
#include "AppDef.h"

#include <thread>
#include <memory>
#include <queue>
#include <mutex>


class MonitorDebug
{
public:
	MonitorDebug();
	virtual ~MonitorDebug();
public:
	void Start();
	inline bool AddCallback(const std::function<void(std::string, int)>& f, int id)
	{
		return m_callbackController.AddHandler(f, id);
	}
	inline bool RemoveCallback(int id)
	{
		return m_callbackController.RemoveHandler(id);
	}
private:
	bool Initialize();
	void UnInitialize();
private:
	std::queue<std::unique_ptr<DVBuffer>> m_queueMonitorData;
	std::mutex m_mutexMonitorData;

	/// <summary>
	/// content and tab id
	/// </summary>
	CallbackEventHandler<std::string, int> m_callbackController;

	HANDLE m_hBufferEntry;
	HANDLE m_hBufferReady;
	HANDLE m_hBufferData;
	bool m_bIsStopMonitor;

	DVBuffer* m_pBuffer;
	std::thread m_threadMonitorProducer;
	std::thread m_threadMonitorComsumer;

	void DebugMonitorThreadProducer();
	void DebugMonitorThreadConsumer();
};
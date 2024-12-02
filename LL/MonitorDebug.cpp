#include "pch.h"
#include "MonitorDebug.h"


MonitorDebug::MonitorDebug()
	: m_hBufferEntry(NULL)
	, m_hBufferReady(NULL)
	, m_hBufferData(NULL)
	, m_bIsStopMonitor(false)
	, m_pBuffer(nullptr)
{
}

MonitorDebug::~MonitorDebug()
{
	m_bIsStopMonitor = true;

	SetEvent(m_hBufferReady);

	UnInitialize();
}

void MonitorDebug::Start()
{
	Initialize();
}

bool MonitorDebug::Initialize()
{
	m_hBufferEntry = OpenEvent(EVENT_ALL_ACCESS,
		FALSE,
		DV_BUFFER_ENTRY);

	// try to create if not exist
	if (m_hBufferEntry == NULL)
	{
		m_hBufferEntry = CreateEvent(NULL,
			FALSE,
			TRUE,//signaled
			DV_BUFFER_ENTRY);


		if (m_hBufferEntry == NULL)
		{
			// error handler at here
			return false;
		}
	}

	m_hBufferReady = OpenEvent(SYNCHRONIZE, FALSE, DV_BUFFER_READY);

	if (m_hBufferReady == NULL)
	{
		m_hBufferReady = CreateEvent(NULL,
			FALSE,
			FALSE, // nonsignaled
			DV_BUFFER_READY);


		if (m_hBufferReady == NULL)
		{
			// error handler at here
			return false;
		}
	}

	m_hBufferData = OpenFileMapping(FILE_MAP_READ, FALSE, DV_BUFFER_DATA);

	if (m_hBufferData == NULL)
	{
		m_hBufferData = CreateFileMapping(INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(DVBuffer),
			DV_BUFFER_DATA);

		if (m_hBufferData == NULL)
		{
			// error handler at here
			return false;
		}
	}

	m_pBuffer = reinterpret_cast<DVBuffer*>(MapViewOfFile(m_hBufferData,
		SECTION_MAP_READ,
		0,
		0,
		0));

	if (m_pBuffer == nullptr)
	{
		// error handler at here
		return false;
	}


	// starting producer and consumer thread
	std::thread monitorProducer(&MonitorDebug::DebugMonitorThreadProducer, this);
	m_threadMonitorProducer = std::move(monitorProducer);

	std::thread monitorConsumer(&MonitorDebug::DebugMonitorThreadConsumer, this);
	m_threadMonitorComsumer = std::move(monitorConsumer);

	return true;
}

void MonitorDebug::UnInitialize()
{
	if (m_threadMonitorProducer.joinable())
		m_threadMonitorProducer.join();


	if (m_threadMonitorComsumer.joinable())
		m_threadMonitorComsumer.join();

	if (m_hBufferData)
	{
		UnmapViewOfFile(m_pBuffer);
		CloseHandle(m_hBufferData);
	}

	if (m_hBufferReady)
	{
		CloseHandle(m_hBufferReady);
	}

	if (m_hBufferEntry)
	{
		CloseHandle(m_hBufferEntry);
	}
}

void MonitorDebug::DebugMonitorThreadProducer()
{
	while (!m_bIsStopMonitor)
	{
		auto ret = WaitForSingleObject(m_hBufferReady, DV_WAIT_DATA_TIMEOUT);

		// only execute when signaled
		if (ret != WAIT_OBJECT_0) continue;
		
		{
			std::lock_guard lock(m_mutexMonitorData);
			m_queueMonitorData.emplace(std::make_unique<DVBuffer>(*m_pBuffer));
		}
		
		// signaled for next sending
		SetEvent(m_hBufferEntry);

		// sleep ms to avoid overheat
		Sleep(1);
	}

	return;
}

void MonitorDebug::DebugMonitorThreadConsumer()
{

	while (true)
	{
		if (m_bIsStopMonitor) return;


		if (m_queueMonitorData.empty())
		{
			Sleep(10);
			continue;
		}

		
		std::unique_lock lock(m_mutexMonitorData);
		auto data = std::move(m_queueMonitorData.front());
		m_queueMonitorData.pop();
		lock.unlock();

		m_callbackController.InvokeAll(std::string(data.get()->data), data.get()->tab);
		

		Sleep(5);
	}

}

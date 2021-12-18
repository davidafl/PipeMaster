#include "CLogger.h"

const std::string CLogger::m_fileName = "log.txt";

CLogger* CLogger::m_singleton = NULL;
std::ofstream CLogger::m_Logfile;

CLogger::CLogger()
{
}

CLogger::~CLogger()
{
    if (m_singleton != NULL)
        m_Logfile.close();
}

CLogger* CLogger::GetLogger() {
    if (m_singleton == NULL) {
        m_singleton = new CLogger();
        m_Logfile.open(m_fileName.c_str(), std::ios::out | std::ios::app);
    }
    return m_singleton;
}


void CLogger::Log(const std::string& msg)
{
    m_Logfile << msg << "\n";
}

CLogger& CLogger::operator<<(const std::string& msg)
{
    m_Logfile << msg << "\n";
    return *this;
}
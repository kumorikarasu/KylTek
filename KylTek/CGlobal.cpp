#include "CGlobal.h"
CGlobal* CGlobal::m_pInstance = 0;
CGlobal* CGlobal::GetInstance(){
	if (m_pInstance == 0) {
        m_pInstance = new CGlobal;
    }
    return m_pInstance;
}
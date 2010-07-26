#ifndef DEBUG_H_
#define DEBUG_H_

#include "CGUIBase.h"
#include "CGUITextBox.h"
#include "CConsole.h"

class CDebug{
private:
	static CDebug* m_pInstance;
	CGUITextBox* m_pDisplay;
	int m_flags;
	CConsole* m_pConsole;

	CDebug();
	~CDebug();
public:
	
	static CDebug* GetInstance();

	enum eFlags{
		Empty,
		Enabled = 1 << 0, 
		Globals = 1 << 1, 
		HitBoxes = 1 << 2, 
		QuadCells = 1 << 3, 
		NetGraph = 1 << 4,
		ColLines = 1 << 5
	};

	CConsole* GetConsole(){return m_pConsole;}
	void ToConsole(std::wstring text){ m_pConsole->AddLine(text); }

	void setFlags(int flags)	{ m_flags |= flags; }
	void unsetFlags(int flags)	{ m_flags &= ~flags; }
	bool CheckFlag(int flags)	{ if(m_flags & flags) return true; return false; }

	void Draw(CGraphicsHandler *g);
};
#define Debug CDebug::GetInstance()
#endif
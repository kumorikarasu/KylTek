#pragma once
#ifndef CGUILABEL_H_
#define CGUILABEL_H_

class CGUILabel : public CGUIBase{  
private:
	std::wstring m_Text;
public:
	CGUILabel(GUI_PARAMS* params, GUI_COLORS* colors, GUI_SPRITES* sprites, LPTSTR text) : CGUIBase(params, colors, sprites){
		m_Type=GUITYPE_LABEL;
		SetText(text);
	}
	~CGUILabel(){};

	virtual bool WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){ return false; }
	virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ){ return false; }
	virtual bool HandleMouse( UINT uMsg, WPARAM wParam, LPARAM lParam ){ return false; }

	virtual bool RequestFocus(){return false;}
	virtual bool CanHaveFocus(){return false;}
	virtual void OnFocusIn(){};
	virtual void OnFocusOut(){};
	virtual void OnMouseEnter(){};
	virtual void OnMouseLeave(){};
	virtual void OnHotkey(){};

	void SetText(std::wstring text){ m_Text=text; };
	std::wstring GetText(){ return m_Text; }

	virtual void Step(){};
	virtual void Draw(CGraphicsHandler *g){
		if(GetVisible()){
			UpdateBB();
			if(m_pSprites){
				if(m_bButtonDown && m_bMouseOver && m_pSprites->Button->MouseDown)
					g->DrawSpriteTL(m_pSprites->Button->MouseDown, 0, m_pos);
				else if((m_bMouseOver || m_bButtonDown) && m_pSprites->Button->MouseOver)
					g->DrawSpriteTL(m_pSprites->Button->MouseOver, 0, m_pos);
				else if(m_pSprites->Button->Default)
					g->DrawSpriteTL(m_pSprites->Button->Default, 0, m_pos);
			}else if(m_pColors)
				g->DrawRectTL(m_pos, m_width, m_height, m_pColors->BG, m_border, m_pColors->Border);

			g->DrawTextTL(m_Text, Vector2(m_pos.x+m_border+2, m_pos.y+m_border+3), m_pColors->Text);
		
			if(!GetEnabled())
				g->DrawRectTL(m_pos, m_width, m_height, ColorARGB(128, 128, 128, 128));
			else if(m_bMouseOver && m_pSprites && m_pSprites->Cursor)
				g->DrawSpriteTL(m_pSprites->Cursor, 0, m_pos);
		}
	}
};

#endif
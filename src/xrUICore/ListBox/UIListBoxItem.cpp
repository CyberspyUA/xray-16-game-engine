#include "pch.hpp"
#include "UIListBoxItem.h"
#include "ScrollView/UIScrollView.h"
#include "Common/object_broker.h"
#include "Static/UIStatic.h"

CUIListBoxItem::CUIListBoxItem(float height)
    : CUIFrameLineWnd(CUIListBoxItem::GetDebugType()), m_text(nullptr), tag(u32(-1))
{
    SetHeight(height);
    m_text = AddTextField("", 10.0f);
    UI().Focus().RegisterFocusable(this);
}

CUIListBoxItem::~CUIListBoxItem()
{
    UI().Focus().UnregisterFocusable(this);
}

void CUIListBoxItem::SetTAG(u32 value) { tag = value; }
u32 CUIListBoxItem::GetTAG() { return tag; }
void CUIListBoxItem::Draw()
{
    if (m_bSelected)
        DrawElements();

    CUIWindow::Draw();
}

void CUIListBoxItem::OnFocusReceive()
{
    inherited::OnFocusReceive();
    GetMessageTarget()->SendMessage(this, LIST_ITEM_FOCUS_RECEIVED);
}

void CUIListBoxItem::InitDefault() { InitTexture("ui_listline"); }
void CUIListBoxItem::SetFont(CGameFont* F) { m_text->SetFont(F); }
CGameFont* CUIListBoxItem::GetFont() { return (m_text) ? m_text->GetFont() : NULL; }
bool CUIListBoxItem::OnMouseDown(int mouse_btn)
{
    if (mouse_btn == MOUSE_1)
    {
        smart_cast<CUIScrollView*>(GetParent()->GetParent())->SetSelected(this);
        GetMessageTarget()->SendMessage(this, LIST_ITEM_SELECT, &tag);
        GetMessageTarget()->SendMessage(this, LIST_ITEM_CLICKED, &tag);
        return true;
    }
    if (mouse_btn == MOUSE_2)
    {
        GetMessageTarget()->SendMessage(this, WINDOW_RBUTTON_DOWN, &tag);
        return true;
    }
    return false;
}

void CUIListBoxItem::SetTextColor(u32 color) { m_text->SetTextColor(color); }
u32 CUIListBoxItem::GetTextColor() { return (m_text) ? m_text->GetTextColor() : 0xffffffff; }
float CUIListBoxItem::FieldsLength() const
{
    if (m_ChildWndList.empty())
        return 0.0f;

    float len = 0.0f;
    /*
        WINDOW_LIST::const_iterator it		= m_ChildWndList.begin();
        WINDOW_LIST::const_iterator it_e	= m_ChildWndList.end();

        for(;it!=it_e;++it)
        {
            CUIWindow* w	= *it;
            len				+= w->GetWndPos().x + w->GetWidth();
        }
    */
    CUIWindow* w = m_ChildWndList.back();
    len += w->GetWndPos().x + w->GetWidth();
    return len;
}

CUIStatic* CUIListBoxItem::AddIconField(float width)
{
    CUIStatic* st = xr_new<CUIStatic>("Icon field");
    st->SetAutoDelete(true);
    st->SetWndPos(Fvector2().set(FieldsLength(), 0.0f));
    st->SetWndSize(Fvector2().set(width, GetHeight()));
    AttachChild(st);
    return st;
}

CUIStatic* CUIListBoxItem::AddTextField(LPCSTR txt, float width)
{
    auto* st = xr_new<CUIStatic>("Text field");
    st->SetAutoDelete(true);
    st->SetWndPos(Fvector2().set(FieldsLength(), 0.0f));
    st->SetWndSize(Fvector2().set(width, GetHeight()));

    AttachChild(st);

    st->SetFont(GetFont());
    st->SetTextColor(GetTextColor());
    st->SetText(txt);
    st->SetVTextAlignment(valCenter);
    return st;
}

void CUIListBoxItem::SetData(void* data) { pData = data; }
void* CUIListBoxItem::GetData() { return pData; }
void CUIListBoxItem::SetText(LPCSTR txt) { m_text->SetText(txt); }
LPCSTR CUIListBoxItem::GetText() { return m_text->GetText(); }

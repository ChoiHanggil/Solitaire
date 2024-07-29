#include "Game.h"

using namespace solitaire;

void solitaire::Game::Init(HWND hwnd)
{
	mHwnd = hwnd;
	mBackgroundImage = std::make_unique<Gdiplus::Image>(L"images/bg_blank.png");
	CreateCradDack();
}

void solitaire::Game::Release()
{
	mDeck.clear();
	mBackgroundImage.reset();
}

void solitaire::Game::Draw(Gdiplus::Graphics& graphics)
{
	graphics.DrawImage(mBackgroundImage.get(),
		0, 0, mBackgroundImage->GetWidth(), mBackgroundImage->GetHeight());
	for (auto& card : mDeck)
	{
		card.Draw(graphics);
	}

	Gdiplus::PointF pos(895.0f, 20.0f);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));
	Gdiplus::Font font(L"¸¼Àº °íµñ", 20);
	graphics.DrawString(L"Å¬¸¯¼ö", -1, &font, pos, &brush);

	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	graphics.DrawString(
		std::to_wstring(mFlipCount).c_str(),
		-1,
		&font,
		BOARD_COUNT,
		&format,
		&brush);

}

void solitaire::Game::OnClick(int x, int y)
{
	for (auto& card : mDeck)
	{
		if (card.CheckClicked(x, y))
		{

		}
	}
}

void solitaire::Game::CreateCradDack()
{
	//°õ Â¦¼ö, ´ª´ô Â¦¼ö, ¿ë Â¦¼ö
	//ÃÑ 40Àå
	//°ñ°í·ç ¼¯¾î¼­


}

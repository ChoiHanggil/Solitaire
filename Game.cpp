#include "Game.h"
#include <random>
#include <vector>
using namespace solitaire;

void solitaire::Game::Init(HWND hwnd)
{
	mHwnd = hwnd;
	mBackgroundImage = std::make_unique<Gdiplus::Image>(L"images/bg_blank.png");
	CreateCradDack();
	mpSelectedCard = nullptr;
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
	Card* pCard{};

	for (auto& card : mDeck)
	{
		if (card.CheckClicked(x, y))
		{
			pCard = &card;
			break;
		}
	}
	if (pCard)
	{
		mFlipCount++;
		RECT rct = { (LONG)BOARD_COUNT.GetLeft(), (LONG)BOARD_COUNT.GetTop(),
					 (LONG)BOARD_COUNT.GetRight(), (LONG)BOARD_COUNT.GetBottom() };

		InvalidateRect(mHwnd, &rct, false);

		if (mpSelectedCard == nullptr)
		{
			mpSelectedCard = pCard;
		}
		else
		{
			if (pCard->GetType() == mpSelectedCard->GetType() && pCard->GetIndex() != mpSelectedCard->GetIndex())
			{
				mpSelectedCard->Invalidate();

				mDeck.remove_if([&](Card& card) {return card.GetIndex() == pCard->GetIndex(); });
				mDeck.remove_if([&](Card& card) {return card.GetIndex() == mpSelectedCard->GetIndex(); });

				mpSelectedCard = nullptr;
			}
			else
			{
				UpdateWindow(mHwnd);
				Sleep(500);

				pCard->Flip(false);
				mpSelectedCard->Flip(false);
				mpSelectedCard = nullptr;
			}
		}
	}

}

void solitaire::Game::CreateCradDack()
{
	//°õ Â¦¼ö, ´ª´ô Â¦¼ö, ¿ë Â¦¼ö
	//ÃÑ 40Àå
	//°ñ°í·ç ¼¯¾î¼­

	std::vector<Card::Type> types;

	while (types.size() < (size_t)BOARD_COLUWN * BOARD_ROW)
	{
		int mod = types.size() % 6;
		switch (mod)
		{
		case 0:
			types.push_back(Card::Type::Bear);
			types.push_back(Card::Type::Bear);
			break;
		case 2:
			types.push_back(Card::Type::Wolf);
			types.push_back(Card::Type::Wolf);
			break;
		case 4:
			types.push_back(Card::Type::Dragon);
			types.push_back(Card::Type::Dragon);
			break;
		}

	}
	std::random_device rd;
	std::mt19937 gen(rd());

	std::shuffle(types.begin(), types.end(), gen);

	int index{};
	int posX{ 15 }, posY{10};

	for (int x = 0; x < BOARD_COLUWN; ++x)
	{
		posY = 10;
		for (int y = 0; y < BOARD_ROW; ++y)
		{
			mDeck.push_back(Card(mHwnd, index, types[index++], posX, posY));
			posY += 150;
		}
		posX += 110;
	}
}


//c++ ½ºÅ¸ÀÏ
// std::random_device rd;
// std::mt19937 gen(rd());	//Function Object - Functor ÇÔ¼ö¿À¹ö·Îµù
// gen() % x(¹üÀ§) <-»ç¿ë
//
// std::uniform_int_distribution<> dist(0,9);  //°í¸£°Ô ¸¸µé±â / Á¤±ÔºÐÆ÷
// dist(gen) 
//
// std::shuffle(deck.begin(), deck.end(), gen);
//
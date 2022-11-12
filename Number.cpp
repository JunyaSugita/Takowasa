#include "Number.h"
#include "TextureManager.h"

void Number::Initialize(uint32_t texture)
{
	numTexture_ = texture;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			sprite_[j][i] = Sprite::Create(numTexture_, { 0,0 });
			sprite_[j][i]->SetTextureRect({ (float)48 * i,0 }, { 48,64 });
			sprite_[j][i]->SetSize({ 48,64 });
		}
	}
}

void Number::Update() {}

void Number::Draw(Vector2 pos, Vector4 color, uint32_t num, uint32_t size)
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			sprite_[j][i]->SetSize({ (float)size * 3,(float)size * 4 });
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			sprite_[j][i]->SetPosition(pos);
			sprite_[j][i]->SetColor(color);
		}
	}
	//1Œ…
	if (num < 10) {
		sprite_[0][num]->Draw();
	}
	//2Œ…
	else if (num < 100) {
		pos.x -= (float)size * 3 / 2;
		for (int i = 0; i < 10; i++) {
			sprite_[0][i]->SetPosition(pos);
		}
		sprite_[0][num / 10]->Draw();

		pos.x += (float)size * 3;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[1][num % 10]->Draw();
	}
	//3Œ…
	else if (num < 1000) {
		pos.x -= (float)size * 3;
		for (int i = 0; i < 10; i++) {
			sprite_[0][i]->SetPosition(pos);
		}
		sprite_[0][num / 100]->Draw();
		pos.x += (float)size * 3;
		for (int i = 0; i < 10; i++) {
			sprite_[1][i]->SetPosition(pos);
		}
		sprite_[1][num / 10 % 10]->Draw();
		pos.x += (float)size * 3;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[2][num % 10]->Draw();
	}
	//4Œ…
	else if (num < 10000) {
		sprite_[0][num / 1000]->Draw();
		pos.x += (float)size * 3;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[1][num / 100 % 10]->Draw();
		pos.x += (float)size * 3;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[2][num / 10 % 10]->Draw();
		pos.x += (float)size * 3;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[3][num % 10]->Draw();
	}
	//5Œ…
	else if (num < 100000) {
		sprite_[0][num / 10000]->Draw();
		pos.x += (float)size * 3;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[1][num / 1000 % 10]->Draw();
		pos.x += (float)size * 3;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[2][num / 100 % 10]->Draw();
		pos.x += (float)size * 3;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[3][num / 10 % 10]->Draw();
		pos.x += (float)size * 3;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[4][num % 10]->Draw();
	}
}

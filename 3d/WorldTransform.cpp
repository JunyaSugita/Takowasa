#include "WorldTransform.h"
#include "MathUtility.h"

void WorldTransform::UpdateMatrix()
{
	//子
	 //変換行列
	 //スケール
	Matrix4 matScale;
	SetScaleMatrix(matScale, scale_);

	//回転
	//回転行列
	Matrix4 matRot, matRotX, matRotY, matRotZ;
	SetRotationMatrix(matRotX, rotation_.x, 'x');
	SetRotationMatrix(matRotY, rotation_.y, 'y');
	SetRotationMatrix(matRotZ, rotation_.z, 'z');
	matRot = matRotZ * matRotX * matRotY;

	//平行移動
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	SetTranslationMatrix(matTrans, translation_);
	matWorld_ = normal;

	if (parent_ == nullptr)//world自体が親だったら（親がいない）
	{
		Matrix4xMatrix4(matWorld_,
			matScale * matRot * matTrans);
	}
	else//world自体が子だったら（親がいる）
	{
		//matworldに入れる
		Matrix4xMatrix4(matWorld_,
			matScale * matRot * matTrans * parent_->matWorld_);
	}

	TransferMatrix();
}

// 사각형 텍스처

#pragma once
#ifndef __VIBUFFER_RECTTEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_RectTexture final : public CVIBuffer
{
protected:
	explicit CVIBuffer_RectTexture(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CVIBuffer_RectTexture() = default;

public:
	// 컴포넌트 프로토타입 초기화
	virtual HRESULT ReadyComponentPrototype() override;
	// 컴포넌트 클론 초기화
	virtual HRESULT ReadyComponent(void* pArg = nullptr) override;
	// 렌더
	virtual HRESULT Render_VIBuffer() override;

public:
	// 프로토타입 생성
	static CVIBuffer_RectTexture* Create(LPDIRECT3DDEVICE9 pDevice);
	// 프로토타입 복제
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END

#define __VIBUFFER_RECTTEXTURE_H__
#endif


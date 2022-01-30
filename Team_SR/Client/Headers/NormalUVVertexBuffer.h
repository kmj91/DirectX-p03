// 버텍스 버퍼

#pragma once

#ifndef NormalUVVertexBuffer_h__
#define NormalUVVertexBuffer_h__
#include "Component.h"
#include "Vertexs.h"
#include "DXWrapper.h"

USING(Engine)

class CNormalUVVertexBuffer : public CComponent
{
public:
	explicit CNormalUVVertexBuffer(LPDIRECT3DDEVICE9 pDevice);
public:
	using Super = CComponent;
	// 컴포넌트 프로토타입 초기화
	virtual HRESULT ReadyComponentPrototype() override;
	// 컴포넌트 복제 초기화
	virtual HRESULT ReadyComponent(void* pArg = nullptr) override;
	// 렌더
	virtual HRESULT Render();
public:
	static CNormalUVVertexBuffer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone(void* pArg = nullptr)override;
	virtual void Free() override;
	// 버텍스 버퍼
	std::shared_ptr<IDirect3DVertexBuffer9> _VertexBuf{ nullptr };
private:
	uint32_t VertexCount = 6ul;
	uint32_t TriangleCount = 2ul;
	uint16_t VertexByteSize =   0ul ;
	std::shared_ptr<IDirect3DVertexDeclaration9> VertexDecl{ nullptr };
};

#endif // NormalUVVertexBuffer_h__
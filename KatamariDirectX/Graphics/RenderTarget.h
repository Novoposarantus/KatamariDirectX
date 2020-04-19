#include "RenderableGameObject.h"

class RenderTarget
{
public:
	bool Init(ID3D11Device*, float, float);

	void SetRenderTarget(ID3D11DeviceContext*);
	void ClearRenderTarget(ID3D11DeviceContext* , float, float, float, float);

	// �������� �������� RT � ���� shader resource view
	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11ShaderResourceView** GetShaderResourceViewAddress();

	XMMATRIX GetProjectionMatrix();

private:
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_orthoMatrix;
	D3D11_VIEWPORT m_viewport;


	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_RTTexture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DSTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
};
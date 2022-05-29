//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::BuildDefaultLightsAndMaterials()
{
	m_nLights = 4;
	m_pLights = new LIGHT[m_nLights];
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);

	/*m_pLights[0].m_bEnable = true;
	m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights[0].m_fRange = 1000.0f;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[0].m_xmf3Position = XMFLOAT3(30.0f, 30.0f, 30.0f);
	m_pLights[0].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);*/

	m_pLights[1].m_bEnable = true;
	m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights[1].m_fRange = 500.0f;
	m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[1].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[1].m_fFalloff = 8.0f;
	m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	m_pLights[2].m_bEnable = true;
	m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[2].m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	m_pLights[2].m_xmf3Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);

	m_pLights[3].m_bEnable = true;
	m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights[3].m_fRange = 600.0f;
	m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.7f, 0.3f, 0.3f, 1.0f);
	m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[3].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[3].m_xmf3Position = XMFLOAT3(0.0f, 30.0f, 0.0f);
	m_pLights[3].m_xmf3Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pLights[3].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[3].m_fFalloff = 8.0f;
	m_pLights[3].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
	m_pLights[3].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	BuildDefaultLightsAndMaterials();

	m_nGameObjects = 10;
	m_ppGameObjects = new CGameObject*[m_nGameObjects];

	CGameObject *pPoliceCarModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/car_1203_blue.bin");
	//CGameObject* pPoliceCarModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/PirateCoin_(Brass).bin");
	pPoliceCarModel->SetOOBB(BoundingOrientedBox(pPoliceCarModel->GetCenter(), pPoliceCarModel->GetExtent(), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)));
	CGameObject* pOldCarModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/car_1203_yellow.bin");
	pOldCarModel->SetOOBB(BoundingOrientedBox(pOldCarModel->GetCenter(), pOldCarModel->GetExtent(), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)));

	CCarObject* pCarObject = NULL;

	for (int i = 0; i < m_nGameObjects; ++i) {
		pCarObject = new CCarObject();
		if (i % 2 == 0) {
			pCarObject->SetChild(pPoliceCarModel, true);
			m_ppGameObjects[i] = pCarObject;
			m_ppGameObjects[i]->SetOOBB(pPoliceCarModel->GetOOBB());
		}
		else {
			pCarObject->SetChild(pOldCarModel, true);
			m_ppGameObjects[i] = pCarObject;
			m_ppGameObjects[i]->SetOOBB(pOldCarModel->GetOOBB());
		}
		m_ppGameObjects[i]->GetOOBB().Extents.x *= m_ppGameObjects[i]->GetScaleVal(); m_ppGameObjects[i]->GetOOBB().Extents.y *= m_ppGameObjects[i]->GetScaleVal(); m_ppGameObjects[i]->GetOOBB().Extents.z *= m_ppGameObjects[i]->GetScaleVal();
		m_ppGameObjects[i]->UpdateOOBB(m_ppGameObjects[i]->GetPosition());
	}

	m_nMapObjects = 20;
	m_ppGameMap = new CGameObject * [m_nMapObjects];

	CGameObject* pFloorModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Bridge_Large.bin");
	CMapObject* pFloorObject = NULL;

	for (int i = 0; i < 6; ++i) {
		pFloorObject = new CMapObject();
		pFloorObject->SetChild(pFloorModel, true);
		pFloorObject->SetPosition(0.0f, 0.0f, 300.0f + i*350.0f);
		pFloorObject->SetScale(50.0f, 50.0f, 50.0f);
		m_ppGameMap[i] = pFloorObject;
	}
	CGameObject* pTreeModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Tree.bin");
	CMapObject* pTreeObject = NULL;

	for (int i = 4; i < m_nMapObjects; ++i) {	
		pTreeObject = new CMapObject();
		pTreeObject->SetChild(pTreeModel, true);
		pTreeObject->SetPosition(-120.0f * (float)pow(-1, i), 0.0f, 100.0f + 100.0f*(i - 4));
		pTreeObject->SetScale(10.0f, 10.0f, 10.0f);
		m_ppGameMap[i] = pTreeObject;
	}

	m_nCoinObjects = 20;
	m_nItemObjects = 25;
	m_ppItemObjects = new CGameObject * [m_nItemObjects];

	CGameObject* pCoinModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/PirateCoin_(Brass).bin");
	CRotatingObject* pCoinObject = NULL;

	for (int i = 0; i < m_nCoinObjects; ++i) {
		pCoinObject = new CRotatingObject();
		pCoinObject->SetChild(pCoinModel, true);
		m_ppItemObjects[i] = pCoinObject;
		//m_ppItemObjects[i]->SetShow(true);
	}

	CGameObject* pShieldModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Shield.bin");
	CShieldObject* pShieldObject = NULL;
	int nShieldIndex = m_nCoinObjects;

	pShieldObject = new CShieldObject();
	pShieldObject->SetChild(pShieldModel, true);
	m_ppItemObjects[nShieldIndex] = pShieldObject;

	CGameObject* pLifeModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/car_1203_green.bin");
	CLifeObject* pLifeObject = NULL;

	for (int i = m_nCoinObjects + 1; i < m_nItemObjects; ++i) {
		pLifeObject = new CLifeObject();
		pLifeObject->SetChild(pLifeModel, true);
		m_ppItemObjects[i] = pLifeObject;
		m_ppItemObjects[i]->SetPosition(63.0f - ((i - m_nCoinObjects - 1) * 9.0f), 93.0f, -70.0f);
		m_ppItemObjects[i]->SetShow(true);
	}

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Release();
		delete[] m_ppGameObjects;
	}
	if (m_ppGameMap)
	{
		for (int i = 0; i < m_nMapObjects; ++i)
			if (m_ppGameMap[i])
				m_ppGameMap[i]->Release();
		delete[] m_ppGameMap;
	}
	if (m_ppItemObjects) {
		for (int i = 0; i < m_nItemObjects; ++i)
			if (m_ppItemObjects[i])
				m_ppItemObjects[i]->Release();
		delete[] m_ppItemObjects;
	}

	ReleaseShaderVariables();

	if (m_pLights) 
		delete[] m_pLights;
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[3];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nGameObjects; i++)
		m_ppGameObjects[i]->ReleaseUploadBuffers();
	for (int i = 0; i < m_nMapObjects; i++)
		m_ppGameMap[i]->ReleaseUploadBuffers();
}

extern random_device rd;
extern mt19937 engine;
extern uniform_int_distribution<> uidi;
std::uniform_real_distribution<> uid(0.2f, 1.0f);
void CScene::SpeedDown()
{
	float fVelocity;
	for (int i = 0; i < m_nGameObjects; ++i) {
		fVelocity = m_ppGameObjects[i]->GetVelocity() - 0.3f;
		if (fVelocity < 0.4f)
			fVelocity = (float)uid(engine);
		m_ppGameObjects[i]->SetVelocity(fVelocity);
	}
	for (int i = 0; i < m_nMapObjects; ++i) {
		fVelocity = m_ppGameMap[i]->GetVelocity() - 0.3f;
		if (fVelocity < 1.0f)
			fVelocity = 1.0f;
		m_ppGameMap[i]->SetVelocity(fVelocity);
	}
	for (int i = 0; i < m_nItemObjects; ++i) {
		fVelocity = m_ppItemObjects[i]->GetVelocity() - 0.3f;
		if (fVelocity < 0.4f)
			fVelocity = 0.4f;
		m_ppItemObjects[i]->SetVelocity(fVelocity);
	}
}

void CScene::CollideAnimate()
{
	XMFLOAT3 xmf3Pos;
	for (int i = 0; i < m_nGameObjects; ++i) {
		xmf3Pos = m_ppGameObjects[i]->GetPosition();
		xmf3Pos.z += 1.0f;
		m_ppGameObjects[i]->SetPosition(xmf3Pos);
		m_ppGameObjects[i]->UpdateOOBB(m_ppGameObjects[i]->GetPosition());
	}
	for (int i = 0; i < m_nMapObjects; ++i) {
		xmf3Pos = m_ppGameMap[i]->GetPosition();
		xmf3Pos.z += 1.0f;
		m_ppGameMap[i]->SetPosition(xmf3Pos);
	}
	for (int i = 0; i < m_nCoinObjects; ++i) {
		xmf3Pos = m_ppItemObjects[i]->GetPosition();
		xmf3Pos.z += 1.0f;
		m_ppItemObjects[i]->SetPosition(xmf3Pos);
		m_ppItemObjects[i]->UpdateOOBB(m_ppItemObjects[i]->GetPosition());
	}
	if (m_nAnimate == 2 && !m_pPlayer->GetPlayOnce()) {
		XMFLOAT3 xmf3pos = m_pPlayer->GetPosition();
		xmf3pos.y = 0.0f;
		m_pPlayer->SetPosition(xmf3pos);
		m_pPlayer->SetJumpDir(1);
		m_pPlayer->SetPlayOnce(true);
	}
	m_pPlayer->PlayerCollideAnimate(m_nAnimate);

}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'S': 
			if (m_pPlayer->GetCoinCnt() >= 5 && !m_ppItemObjects[m_nCoinObjects]->GetShow())
				CreateShield();
			break;
		case VK_RETURN:
			m_bGameStart = true;
			break;
		case VK_SPACE:
			if (!m_pPlayer->GetJumping())
				m_pPlayer->SetJumping(true);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

void CScene::CheckCollisionPlayerCar()
{
	// 차량 오브젝트와 충돌 + 쉴드
	for (int i = 0; i < m_nGameObjects; ++i) {
		if (m_ppGameObjects[i] && !m_pPlayer->GetInvincible()) {
			if (m_pPlayer->GetOOBB().Intersects(m_ppGameObjects[i]->GetOOBB()) && m_ppGameObjects[i]->GetCollide()) {	
				if (m_ppItemObjects[m_nCoinObjects]->GetShow()) {
					m_ppItemObjects[m_nCoinObjects]->SetShow(false);
					m_ppGameObjects[i]->SetPosition(XMFLOAT3(0.0f, 0.0f, -180.0f));
				}
				else {
					m_pPlayer->SetInvincible(true);
					m_ppGameObjects[i]->SetCollide(false);
					if (!m_pPlayer->GetCollide())
						m_nAnimate = uidi(engine);
					m_pPlayer->SetCollide(true);
					SpeedDown();
					for (int i = m_nCoinObjects + 3; i >= m_nCoinObjects + 1; --i) {
						if (m_ppItemObjects[i]->GetShow()) {
							m_ppItemObjects[i]->SetShow(false);
							if (i == m_nCoinObjects + 1)
								std::cout << "Game Over" << std::endl;
							break;
						}
					}
					m_ppGameObjects[i]->SetCollide(true);
				}
			}
		}
	}

	// 코인 오브젝트과 충돌
	for (int i = 0; i < m_nCoinObjects; ++i) {
		if (m_ppItemObjects[i] && m_ppItemObjects[i]->GetShow()) {
			if (m_pPlayer->GetOOBB().Intersects(m_ppItemObjects[i]->GetOOBB())) {
				m_ppItemObjects[i]->SetShow(false);
				m_ppItemObjects[i]->ResetSpawnTime();
				m_ppItemObjects[i]->SetCollide(true);
				m_pPlayer->AddCoinCnt();
			}
		}
	}
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;

	if (m_bGameStart && !m_pPlayer->GetCollide()) {
		for (int i = 0; i < m_nGameObjects; ++i)
			m_ppGameObjects[i]->Animate(fTimeElapsed, NULL);
		for (int i = 0; i < m_nMapObjects; ++i)
			m_ppGameMap[i]->Animate(fTimeElapsed, NULL);
		for (int i = 0; i < m_nItemObjects; ++i)					//nCoinObjects까지만 해도됨
			m_ppItemObjects[i]->Animate(fTimeElapsed, NULL);

		XMFLOAT3 xmf3pos = m_pPlayer->GetPosition();
		xmf3pos.y += 13.0f;
		m_ppItemObjects[m_nCoinObjects]->SetPosition(xmf3pos.x, xmf3pos.y, m_ppItemObjects[m_nCoinObjects]->GetPosition().z);
	}

	for (int i = m_nCoinObjects + 1; i < m_nCoinObjects + 4; ++i) {		// nCoinObjects + 1부터 목숨개수까지
		m_ppItemObjects[i]->SetPosition(m_pPlayer->GetPosition().x + 63.0f - ((i - m_nCoinObjects - 1) * 9.0f),
			m_pPlayer->GetPosition().y + 93.0f, -70.0f);
	}

	if (m_pPlayer->GetCollide()) {
		m_pPlayer->PlayerJump();
		CollideAnimate();
	}

	if (m_pLights)
	{
		m_pLights[1].m_xmf3Position = m_pPlayer->GetPosition();
		m_pLights[1].m_xmf3Direction = m_pPlayer->GetLookVector();
	}

	CheckCollisionPlayerCar();
}

void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);			// Light update

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights

	for (int i = 0; i < m_nGameObjects; ++i)
	{
		if (m_ppGameObjects[i])
		{
			if (m_bGameStart && !m_pPlayer->GetCollide())
				m_ppGameObjects[i]->Animate(m_fElapsedTime, NULL);
			m_ppGameObjects[i]->UpdateTransform(NULL);
			m_ppGameObjects[i]->Render(pd3dCommandList, pCamera);
		}
	}

	for (int i = 0; i < m_nMapObjects; ++i)
	{
		if (m_ppGameMap[i])
		{
			if (m_bGameStart && !m_pPlayer->GetCollide())
				m_ppGameMap[i]->Animate(m_fElapsedTime, NULL);
			m_ppGameMap[i]->UpdateTransform(NULL);
			m_ppGameMap[i]->Render(pd3dCommandList, pCamera);
		}
	}

	for (int i = 0; i < m_nItemObjects; ++i) {
		if (m_ppItemObjects[i] && m_ppItemObjects[i]->GetShow()) {
			if (m_bGameStart && !m_pPlayer->GetCollide())
				m_ppItemObjects[i]->Animate(m_fElapsedTime, NULL);
			m_ppItemObjects[i]->UpdateTransform(NULL);
			m_ppItemObjects[i]->Render(pd3dCommandList, pCamera);
		}
	}
}

void CScene::CreateShield()
{
	XMFLOAT3 xmf3pos = m_pPlayer->GetPosition();
	xmf3pos.y = 13.0f;
	xmf3pos.z += 14.5f;
	m_pPlayer->SetCointCnt(m_pPlayer->GetCoinCnt() - 5);
	m_ppItemObjects[m_nCoinObjects]->SetShow(true);
	m_ppItemObjects[m_nCoinObjects]->SetPosition(xmf3pos);
	m_ppItemObjects[m_nCoinObjects]->UpdateOOBB(xmf3pos);
}
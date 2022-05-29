//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Player.h"
#include "Shader.h"

int CPlayer::m_nSpinCnt = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CPlayer::CPlayer()
{
	m_pCamera = NULL;

	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pPlayerUpdatedContext = NULL;
	m_pCameraUpdatedContext = NULL;
}

CPlayer::~CPlayer()
{
	ReleaseShaderVariables();

	if (m_pCamera) 
		delete m_pCamera;
}

void CPlayer::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (m_pCamera) 
		m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CPlayer::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CPlayer::ReleaseShaderVariables()
{
	if (m_pCamera) 
		m_pCamera->ReleaseShaderVariables();
}

void CPlayer::Move(DIR dir)
{
#define MOVE_SPEED 3.0f

	if (dir == DIR::LEFT) {
		XMFLOAT3 xmf3pos = GetPosition();
		xmf3pos.x -= MOVE_SPEED;
		if (xmf3pos.x <= LEFTROAD) {
			xmf3pos.x = LEFTROAD;
			m_bStop = true;
			m_dPrevDir = DIR::LEFT;
		}
		else if (m_dPrevDir == DIR::RIGHT) {
			if (xmf3pos.x <= MIDDLEROAD) {
				xmf3pos.x = MIDDLEROAD;
				m_bStop = true;
				m_dPrevDir = DIR::MIDDLE;
			}
		}
		SetPosition(xmf3pos);
	}
	else {
		XMFLOAT3 xmf3pos = GetPosition();
		xmf3pos.x += MOVE_SPEED;
		if (xmf3pos.x >= RIGHTROAD) {
			xmf3pos.x = RIGHTROAD;
			m_bStop = true;
			m_dPrevDir = DIR::RIGHT;
		}
		else if (m_dPrevDir == DIR::LEFT) {
			if (xmf3pos.x >= MIDDLEROAD) {
				xmf3pos.x = MIDDLEROAD;
				m_bStop = true;
				m_dPrevDir = DIR::MIDDLE;
			}
		}
		SetPosition(xmf3pos);
	}
}

void CPlayer::Move(ULONG dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);

		Move(xmf3Shift, bUpdateVelocity);
	}
}

void CPlayer::Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
		m_pCamera->Move(xmf3Shift);
	}
}

void CPlayer::PlayerJump()
{
#define JUMP_SPEED 2.0f
	XMFLOAT3 xmf3pos = GetPosition();

	if (!m_bCollide) {
		m_bJumping = true;

		xmf3pos.y += JUMP_SPEED * m_nJumpDir;
		if (xmf3pos.y >= 40.0f) {
			m_nJumpDir *= -1;
		}
		if (xmf3pos.y < 0.0f) {
			m_nJumpDir *= -1;
			m_bJumping = false;
			xmf3pos.y = 0.0f;
		}
	}
	SetPosition(xmf3pos);
}

void CPlayer::ResetRULvec()
{
	m_xmf3Right.x = 1.0f; m_xmf3Right.y = 0.0f; m_xmf3Right.z = 0.0f;
	m_xmf3Up.x = 0.0f; m_xmf3Up.y = 1.0f; m_xmf3Up.z = 0.0f;
	m_xmf3Look.x = 0.0f; m_xmf3Look.y = 0.0f; m_xmf3Look.z = 1.0f;
}

void CPlayer::PlayerCollideAnimate(int nAnimate)
{
	XMFLOAT3 xmf3pos = GetPosition();

	switch(nAnimate)
	{
	case 0:
		xmf3pos.y += JUMP_SPEED * m_nJumpDir;
		if (xmf3pos.y >= 45.0f) {
			m_nJumpDir *= -1;
		}
		if (xmf3pos.y <= 0.0f) {
			m_bCollide = false;
			m_bJumping = false;
			m_nJumpDir = 1;
			xmf3pos.y = 0.0f;
			ResetRULvec();
			ResetSpawnTime();
		}
		SetPosition(xmf3pos);
		break;
	case 1:
		Rotate(0.0f, 20.0f, 0.0f);
		++m_nSpinCnt;
		if (m_nSpinCnt == 30) {
			m_nSpinCnt = 0;
			m_bCollide = false;
			m_bJumping = false;
			m_nJumpDir = 1;
			ResetRULvec();
			ResetSpawnTime();
		}
		break;
	case 2: 
		xmf3pos.y += JUMP_SPEED * m_nJumpDir;
		if (xmf3pos.y >= 45.0f) {
			m_nJumpDir *= -1;
		}
		Rotate(-10.0f, 0.0f, 0.0f);
		++m_nSpinCnt;
		if (m_nSpinCnt == 40) {
			m_nSpinCnt = 0;
			m_bCollide = false;
			if (xmf3pos.y >= 0.0f) {
				xmf3pos.y = 0.0f;
				ResetRULvec();
			}
			SetPlayOnce(false);
			m_nJumpDir = 1;
			ResetSpawnTime();
			m_bJumping = false;
		}
		SetPosition(xmf3pos);
		break;
	default:
		break;
	}
		
	UpdateOOBB(GetPosition());
}

void CPlayer::Rotate(float x, float y, float z)
{
	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +36.0f) {
				m_fPitch -= 360.0f;
			}
			if (m_fPitch < 0.0f) {
				m_fPitch += 360.0f;
			}
		}
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		m_pCamera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);

		}
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}
	else if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_pCamera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		}
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
		if (z != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(z));
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);
}

void CPlayer::Update(float fTimeElapsed)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) 
		m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);

	if (m_pPlayerUpdatedContext) 
		OnPlayerUpdateCallback(fTimeElapsed);

	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) 
		m_pCamera->Update(m_xmf3Position, fTimeElapsed);
	if (m_pCameraUpdatedContext) 
		OnCameraUpdateCallback(fTimeElapsed);
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) 
		m_pCamera->SetLookAt(m_xmf3Position);
	m_pCamera->RegenerateViewMatrix();

	fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength) 
		fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));
}

CCamera *CPlayer::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	CCamera *pNewCamera = NULL;
	switch (nNewCameraMode)
	{
		case FIRST_PERSON_CAMERA:
			pNewCamera = new CFirstPersonCamera(m_pCamera);
			break;
		case THIRD_PERSON_CAMERA:
			pNewCamera = new CThirdPersonCamera(m_pCamera);
			break;
		case SPACESHIP_CAMERA:
			pNewCamera = new CSpaceShipCamera(m_pCamera);
			break;
	}
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf3Right.x, 0.0f, m_xmf3Right.z));
		m_xmf3Up = Vector3::Normalize(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf3Look.x, 0.0f, m_xmf3Look.z));

		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = Vector3::Angle(XMFLOAT3(0.0f, 0.0f, 1.0f), m_xmf3Look);
		if (m_xmf3Look.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		m_xmf3Right = m_pCamera->GetRightVector();
		m_xmf3Up = m_pCamera->GetUpVector();
		m_xmf3Look = m_pCamera->GetLookVector();
	}

	if (pNewCamera)
	{
		pNewCamera->SetMode(nNewCameraMode);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	return(pNewCamera);
}

void CPlayer::OnPrepareRender()
{
	m_xmf4x4Transform._11 = m_xmf3Right.x; m_xmf4x4Transform._12 = m_xmf3Right.y; m_xmf4x4Transform._13 = m_xmf3Right.z;
	m_xmf4x4Transform._21 = m_xmf3Up.x; m_xmf4x4Transform._22 = m_xmf3Up.y; m_xmf4x4Transform._23 = m_xmf3Up.z;
	m_xmf4x4Transform._31 = m_xmf3Look.x; m_xmf4x4Transform._32 = m_xmf3Look.y; m_xmf4x4Transform._33 = m_xmf3Look.z;
	m_xmf4x4Transform._41 = m_xmf3Position.x; m_xmf4x4Transform._42 = m_xmf3Position.y; m_xmf4x4Transform._43 = m_xmf3Position.z;

	UpdateTransform(NULL);
}

void CPlayer::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	DWORD nCameraMode = (pCamera) ? pCamera->GetMode() : 0x00;
	if (nCameraMode == THIRD_PERSON_CAMERA) {
		if (m_bInvincible && m_llSpawnTime != 0) {
			long long time = ::GetTickCount64();
			if ((time - m_llSpawnTime) % 300 >= 0 && (time - m_llSpawnTime) % 300 <= 150) {
				CGameObject::Render(pd3dCommandList, pCamera);
			}
			if ((time - m_llSpawnTime) >= m_nInvincibleTime) {
				m_bInvincible = false;
				m_llSpawnTime = 0;
			}
		}
		else {
			CGameObject::Render(pd3dCommandList, pCamera);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CCarPlayer::CCarPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
#define SCALE_VALUE 8.5f
//#define SCALE_VALUE 100.0f
	m_pCamera = ChangeCamera(/*SPACESHIP_CAMERA*/THIRD_PERSON_CAMERA, 0.0f);

	CGameObject* pGameObject = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/car_1203_green.bin");
	
	pGameObject->SetOOBB(BoundingOrientedBox(pGameObject->GetCenter(), pGameObject->GetExtent(), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)));

	pGameObject->SetScale(SCALE_VALUE, SCALE_VALUE, SCALE_VALUE);
	pGameObject->GetOOBB().Extents.x *= SCALE_VALUE; pGameObject->GetOOBB().Extents.y *= SCALE_VALUE; pGameObject->GetOOBB().Extents.z *= SCALE_VALUE;
	SetChild(pGameObject, true);
	this->SetOOBB(pGameObject->GetOOBB());
	this->SetCollide(false);

	OnInitialize();

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

CCarPlayer::~CCarPlayer()
{
}

void CCarPlayer::OnInitialize()
{
	/*m_pMainRotorFrame = FindFrame("Rotor");
	m_pTailRotorFrame = FindFrame("Back_Rotor");*/
	m_pMainRotorFrame = nullptr;
	m_pTailRotorFrame = nullptr;
}

void CCarPlayer::Animate(float fTimeElapsed, XMFLOAT4X4 *pxmf4x4Parent)
{
	if (m_bJumping)
		PlayerJump();

	UpdateOOBB(GetPosition());
	CPlayer::Animate(fTimeElapsed, pxmf4x4Parent);
}

void CCarPlayer::OnPrepareRender()
{
	CPlayer::OnPrepareRender();
}

CCamera *CCarPlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	XMFLOAT3 xmf3pos(100.0f, 10.0f, 1.0f);
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return(m_pCamera);
	switch (nNewCameraMode)
	{
		case FIRST_PERSON_CAMERA:
			SetFriction(2.0f);
			SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			SetMaxVelocityXZ(2.5f);
			SetMaxVelocityY(40.0f);
			m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(1.0f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 10.0f, 0.0f));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			m_pCamera->SetLookAt(xmf3pos);
			break;
		case SPACESHIP_CAMERA:
			SetFriction(100.5f);
			SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			SetMaxVelocityXZ(40.0f);
			SetMaxVelocityY(40.0f);
			m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.0f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		case THIRD_PERSON_CAMERA:
			SetFriction(20.5f);
			SetFriction(10.5f);
			SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			SetMaxVelocityXZ(25.5f);
			SetMaxVelocityY(40.0f);
			m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(1.0f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 105.0f, -150.0f));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		default:
			break;
	}

	m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));

	Update(fTimeElapsed);

	return(m_pCamera);
}


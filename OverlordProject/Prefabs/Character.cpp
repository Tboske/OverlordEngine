#include "stdafx.h"
#include "Character.h"

Character::Character(const CharacterDesc& characterDesc) :
	m_CharacterDesc{ characterDesc },
	m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime),
	m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime)
{
	
}

void Character::Initialize(const SceneContext& /*sceneContext*/)
{
	//Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	//Camera
	const auto pCamera = AddChild(new FixedCamera());
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true); //Uncomment to make this camera the active camera

	pCamera->GetTransform()->Translate(0.f, m_CharacterDesc.controller.height * .5f, 0.f);
}

void Character::Update(const SceneContext& /*sceneContext*/)
{
	if (m_pCameraComponent->IsActive())
	{
		//constexpr float epsilon{ 0.01f }; //Constant that can be used to compare if a float is near zero
		auto dTime = GetScene()->GetSceneContext().pGameTime->GetElapsed();
		auto pInput = GetScene()->GetSceneContext().pInput;
		//***************
		//HANDLE INPUT

		//## Input Gathering (move)
		XMFLOAT2 move = XMFLOAT2(); //Uncomment
		bool isMoving = false;
		//move.y should contain a 1 (Forward) or -1 (Backward) based on the active input (check corresponding actionId in m_CharacterDesc)
		if (pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward))
		{
			move.y += 1;
			isMoving = true;
		}
		if (pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward))
		{
			move.y -= 1;
			isMoving = true;
		}
		//Optional: if move.y is near zero (abs(move.y) < epsilon), you could use the ThumbStickPosition.y for movement
		//if (abs(move.y) < epsilon)
			//move.y = InputManager::GetThumbstickPosition().y;

		//move.x should contain a 1 (Right) or -1 (Left) based on the active input (check corresponding actionId in m_CharacterDesc)
		if (pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight))
		{
			move.x += 1;
			isMoving = true;
		}
		if (pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft))
		{
			move.x -= 1;
			isMoving = true;
		}
		//Optional: if move.x is near zero (abs(move.x) < epsilon), you could use the Left ThumbStickPosition.x for movement
		//if (abs(move.x) < epsilon)
			//move.x = InputManager::GetThumbstickPosition().x;

		//## Input Gathering (look)
		XMFLOAT2 look{ 0.f, 0.f }; //Uncomment
		//Only if the Left Mouse Button is Down >
		if (InputManager::IsMouseButton(InputState::down, 1))
		{
			// Store the MouseMovement in the local 'look' variable (cast is required)
			look.y = float(InputManager::GetMouseMovement().x);
			look.x = float(InputManager::GetMouseMovement().y);
		}
		//Optional: in case look.x AND look.y are near zero, you could use the Right ThumbStickPosition for look
		//if (abs(look.x) < epsilon && abs(look.y) < epsilon)
			//look = InputManager::GetThumbstickPosition(false);
		

		//************************
		//GATHERING TRANSFORM INFO

		//Retrieve the TransformComponent
		auto pTrans = GetComponent<TransformComponent>();
		//Retrieve the forward & right vector (as XMVECTOR) from the TransformComponent
		XMVECTOR forward = XMLoadFloat3(&pTrans->GetForward());
		XMVECTOR right = XMLoadFloat3(&pTrans->GetRight());

		//***************
		//CAMERA ROTATION

		//Adjust the TotalYaw (m_TotalYaw) & TotalPitch (m_TotalPitch) based on the local 'look' variable
		//Make sure this calculated on a framerate independent way and uses CharacterDesc::rotationSpeed.
		m_TotalYaw += look.x * dTime * m_CharacterDesc.rotationSpeed;
		m_TotalPitch += look.y * dTime * m_CharacterDesc.rotationSpeed;

		//Rotate this character based on the TotalPitch (X) and TotalYaw (Y)
		pTrans->Rotate(m_TotalYaw, m_TotalPitch, 0);

		//********
		//MOVEMENT

		//## Horizontal Velocity (Forward/Backward/Right/Left)
		//Calculate the current move acceleration for this frame (m_MoveAcceleration * ElapsedTime)
		auto currentMoveAcc = m_MoveAcceleration * dTime;
		//If the character is moving (= input is pressed)
		if (isMoving)
		{
			//Calculate & Store the current direction (m_CurrentDirection) >> based on the forward/right vectors and the pressed input
			m_CurrentDirection = { 0,0,0 };
			auto currPos = XMLoadFloat3(&m_CurrentDirection);
			currPos += forward * move.y * m_MoveSpeed * dTime;
			currPos += right * move.x * m_MoveSpeed * dTime;

			XMStoreFloat3(&m_CurrentDirection, currPos);

			//Increase the current MoveSpeed with the current Acceleration (m_MoveSpeed)
			m_MoveSpeed += currentMoveAcc;
			//Make sure the current MoveSpeed stays below the maximum MoveSpeed (CharacterDesc::maxMoveSpeed)
			if (m_MoveSpeed > m_CharacterDesc.maxMoveSpeed)
				m_MoveSpeed = m_CharacterDesc.maxMoveSpeed;
		}
		//Else (character is not moving, or stopped moving)
		else
		{
			//Decrease the current MoveSpeed with the current Acceleration (m_MoveSpeed)
			m_MoveSpeed -= currentMoveAcc;
			//Make sure the current MoveSpeed doesn't get smaller than zero
			if (m_MoveSpeed < 0)
				m_MoveSpeed = 0;
		}

		//Now we can calculate the Horizontal Velocity which should be stored in m_TotalVelocity.xz
		//Calculate the horizontal velocity (m_CurrentDirection * MoveSpeed)
		auto horizontalVelocity = XMLoadFloat3(&m_CurrentDirection) * m_MoveSpeed;
		XMFLOAT3 l{};
		XMStoreFloat3(&l, horizontalVelocity);
		//Set the x/z component of m_TotalVelocity (horizontal_velocity x/z)
		m_TotalVelocity.x = l.x;
		m_TotalVelocity.z = l.z;
		//It's important that you don't overwrite the y component of m_TotalVelocity (contains the vertical velocity)

		//## Vertical Movement (Jump/Fall)
		//If the Controller Component is NOT grounded (= freefall)
		if (m_pControllerComponent->GetCollisionFlags() != PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			//Decrease the y component of m_TotalVelocity with a fraction (ElapsedTime) of the Fall Acceleration (m_FallAcceleration)
			m_TotalVelocity.y -= m_FallAcceleration * dTime;
			//Make sure that the minimum speed stays above -CharacterDesc::maxFallSpeed (negative!)
			if (m_TotalVelocity.y < -m_CharacterDesc.maxFallSpeed)
				m_TotalVelocity.y = -m_CharacterDesc.maxFallSpeed;
		}
		//Else If the jump action is triggered
		else if (pInput->IsActionTriggered(m_CharacterDesc.actionId_Jump))
			//Set m_TotalVelocity.y equal to CharacterDesc::JumpSpeed
			m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
		//Else (=Character is grounded, no input pressed)
		else
			//m_TotalVelocity.y is zero
			m_TotalVelocity.y = 0;

		//************
		//DISPLACEMENT
		//The displacement required to move the Character Controller (ControllerComponent::Move) can be calculated using our TotalVelocity (m/s)
		auto vel = XMLoadFloat3(&m_TotalVelocity);
		vel *= dTime;
		XMFLOAT3 displacement{};
		XMStoreFloat3(&displacement, vel);
		//Calculate the displacement (m) for the current frame and move the ControllerComponent
		m_pControllerComponent->Move(displacement);

		//The above is a simple implementation of Movement Dynamics, adjust the code to further improve the movement logic and behaviour.
		//Also, it can be usefull to use a seperate RayCast to check if the character is grounded (more responsive)
	}
}

void Character::DrawImGui()
{
	if (ImGui::CollapsingHeader("Character"))
	{
		ImGui::Text(std::format("Move Speed: {:0.1f} m/s", m_MoveSpeed).c_str());
		ImGui::Text(std::format("Fall Speed: {:0.1f} m/s", m_TotalVelocity.y).c_str());

		ImGui::Text(std::format("Move Acceleration: {:0.1f} m/s2", m_MoveAcceleration).c_str());
		ImGui::Text(std::format("Fall Acceleration: {:0.1f} m/s2", m_FallAcceleration).c_str());

		const float jumpMaxTime = m_CharacterDesc.JumpSpeed / m_FallAcceleration;
		const float jumpMaxHeight = (m_CharacterDesc.JumpSpeed * jumpMaxTime) - (0.5f * (m_FallAcceleration * powf(jumpMaxTime, 2)));
		ImGui::Text(std::format("Jump Height: {:0.1f} m", jumpMaxHeight).c_str());

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Move Speed (m/s)", &m_CharacterDesc.maxMoveSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Move Acceleration Time (s)", &m_CharacterDesc.moveAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_MoveAcceleration = m_CharacterDesc.maxMoveSpeed / m_CharacterDesc.moveAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Fall Speed (m/s)", &m_CharacterDesc.maxFallSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Fall Acceleration Time (s)", &m_CharacterDesc.fallAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_FallAcceleration = m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		ImGui::DragFloat("Jump Speed", &m_CharacterDesc.JumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
		ImGui::DragFloat("Rotation Speed (deg/s)", &m_CharacterDesc.rotationSpeed, 0.1f, 0.f, 0.f, "%.1f");

		bool isActive = m_pCameraComponent->IsActive();
		if(ImGui::Checkbox("Character Camera", &isActive))
		{
			m_pCameraComponent->SetActive(isActive);
		}
	}
}
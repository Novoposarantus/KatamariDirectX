
#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	timer.Start();
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
	{
		return false;
	}
	if (!gfx.Initialize(this->render_window.GetHWND(), width, height))
	{
		return false;
	}
	return true;
}

bool Engine::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

void Engine::Update()
{
	float dt = timer.GetMilisecondsElapsed();
	timer.Restart();
	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
	}

	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}

	//while (!mouse.EventBufferIsEmpty())
	//{
	//	MouseEvent me = mouse.ReadEvent();
	//	if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
	//	{
	//		this->gfx.camera.Rotation((float)me.GetPosX() * dt, (float)me.GetPosY() * dt);
	//	}
	//}

	if (keyboard.KeyIsPressed(VK_LEFT))
	{
		this->gfx.camera.Rotation(-3 * dt, 0);
	}
	if (keyboard.KeyIsPressed(VK_RIGHT))
	{
		this->gfx.camera.Rotation(3 * dt, 0);
	}

	if (keyboard.KeyIsPressed(VK_NUMPAD7))
	{
		this->gfx.light.Rotate(Vector3(1,0,0), dt);
	}
	if (keyboard.KeyIsPressed(VK_NUMPAD9))
	{
		this->gfx.light.Rotate(Vector3(-1, 0, 0), dt);
	}
	
	if (keyboard.KeyIsPressed(VK_NUMPAD4))
	{
		this->gfx.light.Rotate(Vector3(0, 1, 0), dt);
	}
	if (keyboard.KeyIsPressed(VK_NUMPAD6))
	{
		this->gfx.light.Rotate(Vector3(0, -1, 0), dt);
	}

	if (keyboard.KeyIsPressed(VK_NUMPAD1))
	{
		this->gfx.light.Rotate(Vector3(0, 0, 1), dt);
	}
	if (keyboard.KeyIsPressed(VK_NUMPAD3))
	{
		this->gfx.light.Rotate(Vector3(0, 0, -1), dt);
	}

	auto mainObjectPos = Vector3(0, 0, 0);
	auto mainObjectRot = Vector3(0, 0, 0);

	if (keyboard.KeyIsPressed('W'))
	{
		mainObjectPos += this->gfx.camera.GetForwardVector(true) * this->gfx.camera.GetSpeed() * dt;
		mainObjectRot += this->gfx.camera.GetLeftVector(true) * this->gfx.camera.GetSpeed();
	}
	if (keyboard.KeyIsPressed('S'))
	{
		mainObjectPos += -this->gfx.camera.GetForwardVector(true) * this->gfx.camera.GetSpeed() * dt;
		mainObjectRot += -this->gfx.camera.GetLeftVector(true) * this->gfx.camera.GetSpeed();
	}
	if (keyboard.KeyIsPressed('A'))
	{
		mainObjectPos += this->gfx.camera.GetLeftVector(true) * this->gfx.camera.GetSpeed() * dt;
		mainObjectRot += -this->gfx.camera.GetForwardVector(true) * this->gfx.camera.GetSpeed();
	}
	if (keyboard.KeyIsPressed('D'))
	{
		mainObjectPos += -this->gfx.camera.GetLeftVector(true) * this->gfx.camera.GetSpeed() * dt;
		mainObjectRot += this->gfx.camera.GetForwardVector(true) * this->gfx.camera.GetSpeed();
	}

	if (mainObjectPos.x != 0 || mainObjectPos.y != 0 || mainObjectPos.z != 0)
	{
		for (int i = 0; i < this->gfx.gameObjects.size(); i++)
		{
			if (	!this->gfx.gameObjects[i].IsAttachedToMain()
				&&	this->gfx.gameObjects[i].CheckFutureColision(this->gfx.mainObject, mainObjectPos)
				&&	!this->gfx.gameObjects[i].CanAttach(this->gfx.mainObjectSize))
			{
				return;
			}
		}
		this->gfx.mainObject.AdjustPosition(mainObjectPos);
		this->gfx.mainObject.Rotate(mainObjectRot, dt);
		this->gfx.camera.UpdateViewMatrix();
	}

}

void Engine::RenderFrame()
{
	gfx.RenderFrame();
}


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

	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();
		if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
		{
			auto posX = (float)me.GetPosX();
			this->gfx.camera.Rotation(posX * dt, (float)me.GetPosY() * dt);
			//this->gfx.mainObject.AdjustRotation(0, -posX * this->gfx.camera.rotationSpeed * dt, 0);
		}
	}

	if (keyboard.KeyIsPressed(VK_UP))
	{
		this->gfx.camera.Rotation(0, 10);
	}
	if (keyboard.KeyIsPressed(VK_DOWN))
	{
		this->gfx.camera.Rotation(0, -10);
	}
	
	auto mainObjectRot = Vector3(0, 0, 0);
	auto mainObjectPos = Vector3(0, 0, 0);

	if (keyboard.KeyIsPressed('W'))
	{
		mainObjectPos += -this->gfx.camera.GetForwardVector() * dt;
		mainObjectRot += this->gfx.camera.GetLeftVector() * dt;
		//mainObjectRot += Vector3(this->gfx.mainObject.rotationSpeed * dt, 0, 0);
	}
	if (keyboard.KeyIsPressed('S'))
	{
		mainObjectPos += this->gfx.camera.GetForwardVector() * dt;
		mainObjectRot += -this->gfx.camera.GetLeftVector() * dt;
		//mainObjectRot += Vector3(-this->gfx.mainObject.rotationSpeed * dt, 0, 0);
	}
	if (keyboard.KeyIsPressed('A'))
	{
		mainObjectPos += this->gfx.camera.GetLeftVector() * dt;
		mainObjectRot += this->gfx.camera.GetForwardVector() * dt;
		//mainObjectRot += Vector3(0, 0, this->gfx.mainObject.rotationSpeed * dt);
	}
	if (keyboard.KeyIsPressed('D'))
	{
		mainObjectPos += -this->gfx.camera.GetLeftVector() * dt;
		mainObjectRot += -this->gfx.camera.GetForwardVector() * dt;
		//mainObjectRot += Vector3(0, 0, -this->gfx.mainObject.rotationSpeed * dt);
	}

	if (mainObjectPos.x != 0 || mainObjectPos.y != 0 || mainObjectPos.z != 0)
	{
		this->gfx.mainObject.AdjustPosition(mainObjectPos);
		this->gfx.camera.UpdateViewMatrix();
	}

	if (mainObjectRot.x != 0 || mainObjectRot.y != 0 || mainObjectRot.z != 0)
	{
		this->gfx.mainObject.AdjustRotation(mainObjectRot);
		for (int i = 0; i < this->gfx.gameObjects.size(); i++)
		{
			if (this->gfx.gameObjects[i].IsAttachedToMain())
			{
				this->gfx.gameObjects[i].AdjustRotation(mainObjectRot);
			}
		}
	}

}

void Engine::RenderFrame()
{
	gfx.RenderFrame();
}

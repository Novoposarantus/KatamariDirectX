
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
			this->gfx.camera.Rotation((float)me.GetPosX(), (float)me.GetPosY());
		}
	}

	const float cameraSpeed = 0.01f;

	if (keyboard.KeyIsPressed('W'))
	{
		this->gfx.mainObject.AdjustPosition(-this->gfx.camera.GetForwardVector() * cameraSpeed * dt);
		//this->gfx.camera.UpdateViewMatrix();
	}
	if (keyboard.KeyIsPressed('S'))
	{
		this->gfx.mainObject.AdjustPosition(-this->gfx.camera.GetBackwardVector() * cameraSpeed * dt);
		//this->gfx.camera.UpdateViewMatrix();
	}
	if (keyboard.KeyIsPressed('A'))
	{
		this->gfx.mainObject.AdjustPosition(-this->gfx.camera.GetLeftVector() * cameraSpeed * dt);
		//this->gfx.camera.UpdateViewMatrix();
	}
	if (keyboard.KeyIsPressed('D'))
	{
		this->gfx.mainObject.AdjustPosition(-this->gfx.camera.GetRightVector() * cameraSpeed * dt);
		//this->gfx.camera.UpdateViewMatrix();
	}
	//if (keyboard.KeyIsPressed(VK_SPACE))
	//{
	//	this->gfx.camera.AdjustPosition(0.0f, cameraSpeed * dt, 0.0f);
	//}
	//if (keyboard.KeyIsPressed('Z'))
	//{
	//	this->gfx.camera.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);
	//}

}

void Engine::RenderFrame()
{
	gfx.RenderFrame();
}

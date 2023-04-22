#pragma once
#include "H00NWindow.h"
#include "Timer.h"
#include "Input.h"

class H00NEngine : public H00NWindow, public enable_shared_from_this<H00NEngine>
{
public:
	H00NEngine();
	virtual ~H00NEngine();

protected:
	virtual void Startup() override;
	virtual void MainUpdate() override;
	virtual void MainRender() override;
	virtual void Cleanup() override;

protected:
	virtual void Init();
	virtual void Update(float dt);
	virtual void Render(HDC hdc);
	virtual void Release();

public:
	shared_ptr<Timer> GetTimer() { return m_timer; }
	shared_ptr<Input> GetInput() { return m_input; }

private:
	shared_ptr<Timer> m_timer;
	shared_ptr<Input> m_input;

private:
	HDC		m_hBackDC;
	HBITMAP m_hBitmap;
};


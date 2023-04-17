#pragma once
#include "H00NWindow.h"

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

private:
	HDC		m_hBackDC;
	HBITMAP m_hBitmap;
};


#pragma once

class BTSWindow
{
public:
	BTSWindow();
	virtual ~BTSWindow();

public:
	int		Run(HINSTANCE hInst, LPWSTR lpCmdLine, int nCmdShow, int width = SCREEN_WIDTH, int height = SCREEN_HEIGHT);
	static	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	void	registerWndClass();
	void	createWindow();
	void	showWindow(int nCmdShow);
	int		messageLoop();

protected:
	virtual void	Startup() = 0;
	virtual void	MainUpdate() = 0;
	virtual void	MainRender() = 0;
	virtual void	Cleanup() = 0;

public:
	HWND		GetWndHandle() { return m_hWnd; }
	HINSTANCE	GetInstanceHandle() { return m_hInstance; }
	UINT		GetWidth() { return m_nWidth;  }
	UINT		GetHeight() { return m_nHeight;  }

protected:
	HWND		m_hWnd;
	HINSTANCE	m_hInstance;
	UINT		m_nWidth;
	UINT		m_nHeight;
};


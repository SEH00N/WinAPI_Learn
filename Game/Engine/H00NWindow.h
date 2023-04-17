#pragma once

class H00NWindow
{
public:
	H00NWindow();
	~H00NWindow();

public:
	static	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	int		Run(HINSTANCE hInst, LPWSTR lpCmdLine, int nCmdShow);

private:
	void	registerWndClass();
	void	createWindow();
	void	showWindow(int nCmdShow);
	int		messageLoop();

protected:
	virtual void Startup() = 0;
	virtual void MainUpdate() = 0;
	virtual void MainRender() = 0;
	virtual void Cleanup() = 0; 

public:
	HWND		GetWindowHandle() { return m_hWnd; }
	HINSTANCE	GetInstanceHandle() { return m_hInstance; }

protected:
	HWND		m_hWnd;
	HINSTANCE	m_hInstance;
	UINT		m_nWidth;
	UINT		m_nHeight;
};
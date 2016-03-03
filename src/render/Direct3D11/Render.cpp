#include "Render.h"
#include<iostream>
namespace render {
	static vector<Window*> WindowSet;
	static int WindowsSize = 0;
	static IWICImagingFactory* IWICFactory(NULL);
	static ID3D11Device* Device(NULL);
	static ID3D11DeviceContext* DeviceContext(NULL);
	static ID3D11RasterizerState* RasteState(NULL);
	static D3D11_RASTERIZER_DESC RasteDesc;

	static SIZE System;
    int GetWindowsNum() {
		return WindowsSize;
	}
	static void ChangeSystem(int Width, int Height) {
		DEVMODE DevMode;
		DevMode.dmPelsWidth = Width;
		DevMode.dmPelsHeight = Height;
		ChangeDisplaySettings(&DevMode, 0);
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		int WindowNum = -1;
		Window::Event Event;
		int Size = WindowSet.size();
		for (int i = 0; i <= Size - 1; i++)
			if (WindowSet[i]->GetHwnd() == hWnd) {
				WindowNum = i;
				break;
			}
		switch (message)
		{
		case WM_KILLFOCUS: {
			if (WindowNum != -1 && WindowSet[WindowNum]->GetFullScreen() == true)
				ShowWindow(hWnd, SW_MINIMIZE);
			ChangeSystem(System.cx, System.cy);
			break;
		}
		case WM_SETFOCUS: {
			if (WindowNum != -1) {
				WindowSet[WindowNum]->NeedFullScreen = true;
			}
			break;
		}
		case WM_KEYUP: {
			unsigned modifiers = 0;
			if (GetAsyncKeyState(VK_LCONTROL) < 0) {
				modifiers += Modifier::LCTRL;
			}
			if (GetAsyncKeyState(VK_RCONTROL) < 0) {
				modifiers += Modifier::RCTRL;
			}
			if (GetAsyncKeyState(VK_LMENU) < 0) {
				modifiers += Modifier::LALT;
			}
			if (GetAsyncKeyState(VK_RMENU) < 0) {
				modifiers += Modifier::RALT;
			}
			if (GetAsyncKeyState(VK_LSHIFT) < 0) {
				modifiers += Modifier::LSHIFT;
			}
			if (GetAsyncKeyState(VK_RSHIFT) < 0) {
				modifiers += Modifier::RSHIFT;
			}
			if (GetAsyncKeyState(VK_SCROLL) == 1) {
				modifiers += Modifier::SCROLL_LOCK;
			}
			if (GetAsyncKeyState(VK_CAPITAL) == 1) {
				modifiers += Modifier::CAPS_LOCK;
			}
			if (GetAsyncKeyState(VK_NUMLOCK) == 1) {
				modifiers += Modifier::CAPS_LOCK;
			}
			KeyboardEventArgs* EventArg = new KeyboardEventArgs();
			EventArg->code = (Keycode)wParam;
			EventArg->pressed = false;
			EventArg->released = true;
			EventArg->modifiers = modifiers;
			Event.Args = (EventArgs*)EventArg;
			Event.Type = EventType::Keyboard;
			WindowSet[WindowNum]->MsgTeam.push(Event);
			break;
		}
		case WM_KEYDOWN: {
			unsigned modifiers = 0;
			if (GetAsyncKeyState(VK_LCONTROL) < 0) {
				modifiers += Modifier::LCTRL;
			}
			if (GetAsyncKeyState(VK_RCONTROL) < 0) {
				modifiers += Modifier::RCTRL;
			}
			if (GetAsyncKeyState(VK_LMENU) < 0) {
				modifiers += Modifier::LALT;
			}
			if (GetAsyncKeyState(VK_RMENU) < 0) {
				modifiers += Modifier::RALT;
			}
			if (GetAsyncKeyState(VK_LSHIFT) < 0) {
				modifiers += Modifier::LSHIFT;
			}
			if (GetAsyncKeyState(VK_RSHIFT) < 0) {
				modifiers += Modifier::RSHIFT;
			}
			if (GetAsyncKeyState(VK_SCROLL) == 1) {
				modifiers += Modifier::SCROLL_LOCK;
			}
			if (GetAsyncKeyState(VK_CAPITAL) == 1) {
				modifiers += Modifier::CAPS_LOCK;
			}
			if (GetAsyncKeyState(VK_NUMLOCK) == 1) {
				modifiers += Modifier::CAPS_LOCK;
			}
			KeyboardEventArgs* EventArg = new KeyboardEventArgs();
			EventArg->code = (Keycode)wParam;
			EventArg->pressed = true;
			EventArg->released = false;
			EventArg->modifiers = modifiers;
			Event.Args = (EventArgs*)EventArg;
			Event.Type = EventType::Keyboard;
			WindowSet[WindowNum]->MsgTeam.push(Event);
			break;
		}
		case WM_MOUSEWHEEL: {
			MouseWheelEventArgs* EventArg = new MouseWheelEventArgs();
			EventArg->x = GET_X_LPARAM(lParam);
			EventArg->y = GET_Y_LPARAM(lParam);
			EventArg->offest_y = GET_WHEEL_DELTA_WPARAM(wParam) / 120;
			Event.Type = EventType::MouseWheel;
			Event.Args = (EventArgs*)EventArg;
			WindowSet[WindowNum]->MsgTeam.push(Event);
			break;
		}
		case WM_MBUTTONUP: {
			MouseClickEventArgs* EventArg = new MouseClickEventArgs();
			EventArg->x = LOWORD(lParam);
			EventArg->y = HIWORD(lParam);
			EventArg->button = MouseButton::Middle;
			EventArg->pressed = false;
			EventArg->released = true;
			Event.Type = EventType::MouseClick;
			Event.Args = (EventArgs*)EventArg;
			WindowSet[WindowNum]->MsgTeam.push(Event);
			break;
		}
		case WM_MBUTTONDOWN: {
			MouseClickEventArgs* EventArg = new MouseClickEventArgs();
			EventArg->x = LOWORD(lParam);
			EventArg->y = HIWORD(lParam);
			EventArg->button = MouseButton::Middle;
			EventArg->pressed = true;
			EventArg->released = false;
			Event.Type = EventType::MouseClick;
			Event.Args = (EventArgs*)EventArg;
			WindowSet[WindowNum]->MsgTeam.push(Event);
			break;
		}
		case WM_RBUTTONUP: {
			MouseClickEventArgs* EventArg = new MouseClickEventArgs();
			EventArg->x = LOWORD(lParam);
			EventArg->y = HIWORD(lParam);
			EventArg->button = MouseButton::Right;
			EventArg->pressed = false;
			EventArg->released = true;
			Event.Type = EventType::MouseClick;
			Event.Args = (EventArgs*)EventArg;
			WindowSet[WindowNum]->MsgTeam.push(Event);
			break;
		}
		case WM_RBUTTONDOWN: {
			MouseClickEventArgs* EventArg = new MouseClickEventArgs();
			EventArg->x = LOWORD(lParam);
			EventArg->y = HIWORD(lParam);
			EventArg->button = MouseButton::Right;
			EventArg->pressed = true;
			EventArg->released = false;
			Event.Type = EventType::MouseClick;
			Event.Args = (EventArgs*)EventArg;
			WindowSet[WindowNum]->MsgTeam.push(Event);
			break;
		}
		case WM_LBUTTONUP: {
			MouseClickEventArgs* EventArg = new MouseClickEventArgs();
			EventArg->x = LOWORD(lParam);
			EventArg->y = HIWORD(lParam);
			EventArg->button = MouseButton::Left;
			EventArg->pressed = false;
			EventArg->released = true;
			Event.Type = EventType::MouseClick;
			Event.Args = (EventArgs*)EventArg;
			WindowSet[WindowNum]->MsgTeam.push(Event);
			break;
		}
		case WM_LBUTTONDOWN: {
			MouseClickEventArgs* EventArg = new MouseClickEventArgs();
			EventArg->x = LOWORD(lParam);
			EventArg->y = HIWORD(lParam);
			EventArg->button = MouseButton::Left;
			EventArg->pressed = true;
			EventArg->released = false;
			Event.Type = EventType::MouseClick;
			Event.Args = (EventArgs*)EventArg;
			WindowSet[WindowNum]->MsgTeam.push(Event);
			break;
		}
		case WM_MOUSEMOVE: {
			MouseMotionEventArgs* EventArg = new MouseMotionEventArgs();
			EventArg->x = LOWORD(lParam);
			EventArg->y = HIWORD(lParam);
			Event.Args = (EventArgs*)EventArg;
			Event.Type = EventType::MouseMotion;
			WindowSet[WindowNum]->MsgTeam.push(Event);
			break;
		}
		case WM_SIZE: {
			if (WindowNum != -1) {
				ResizeEventArgs* EventArg = new ResizeEventArgs();
				EventArg->width = LOWORD(lParam);
				EventArg->height = HIWORD(lParam);
				Event.Type = EventType::Resize;
				Event.Args = (EventArgs*)EventArg;
				WindowSet[WindowNum]->SetWidth(EventArg->width);
				WindowSet[WindowNum]->SetHeight(EventArg->height);
				WindowSet[WindowNum]->MsgTeam.push(Event);
			}
			//窗口大小的固定和重置
			break;
		}
		case WM_DESTROY: {
			Event.Args = NULL;
			Event.Type = EventType::Close;
			WindowSet[WindowNum]->MsgTeam.push(Event);
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
	//
	
	
	//

	void Initialize() {
		System.cx = GetSystemMetrics(SM_CXSCREEN);
		System.cy = GetSystemMetrics(SM_CYSCREEN);
		HRESULT hr;
		//Create IWICFactory
		CoInitialize(NULL);
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC,
			IID_IWICImagingFactory,
			reinterpret_cast<void **>(&IWICFactory));
		SendMsg(hr, L"Create IWICFactory Failed");


		D3D_FEATURE_LEVEL Features[3] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3 };
		D3D_FEATURE_LEVEL MyFeature;
		hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_BGRA_SUPPORT, Features, 3,
			D3D11_SDK_VERSION, &Device, &MyFeature, &DeviceContext);
		SendMsg(hr, L"Create Device Failed");


		RasteDesc.FillMode = D3D11_FILL_SOLID;
		RasteDesc.CullMode = D3D11_CULL_NONE;
		RasteDesc.FrontCounterClockwise = FALSE;
		RasteDesc.DepthBias = 0;
		RasteDesc.SlopeScaledDepthBias = 0.0f;
		RasteDesc.DepthBiasClamp = 0.0f;
		RasteDesc.DepthClipEnable = TRUE;
		RasteDesc.ScissorEnable = FALSE;
		RasteDesc.MultisampleEnable = FALSE;
		RasteDesc.AntialiasedLineEnable = FALSE;

		Device->CreateRasterizerState(&RasteDesc, &RasteState);
		DeviceContext->RSSetState(RasteState);
	}
	void Terminate() {
		SafeRelease(IWICFactory);
		SafeRelease(Device);
		SafeRelease(DeviceContext);
		SafeRelease(RasteState);
	}
	Image::Image(const CharType* filepath) {
		IWICBitmapFrameDecode *pSource = NULL;
		IWICStream *pStream = NULL;
		IWICFormatConverter *pConverter = NULL;
		HRESULT hr = IWICFactory->CreateDecoderFromFilename(
			(LPCWSTR)filepath,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&this->Resource
			);
		if (SUCCEEDED(hr))
		{
			hr = this->Resource->GetFrame(0, &pSource);
		}
		SendMsg(hr, L"Get Frame Failed");
		if (SUCCEEDED(hr))
		{
			// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
			hr = IWICFactory->CreateFormatConverter(&pConverter);
		}
		SendMsg(hr, L"Get Format Failed");
		if (SUCCEEDED(hr))
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
				);
		}
		//Get Size
		pSource->GetSize(&this->Width, &this->Height);
		//Save File Name
		FileName = (wchar_t*)filepath;
		SafeRelease(pSource);
		SafeRelease(pStream);
		SafeRelease(pConverter);
	}
	Image::~Image() {
		SafeRelease(this->Resource);
	}

	auto Image::GetWidth() const -> int {
		return (int)this->Width;
	}
	auto Image::GetHeight() const -> int {
		return (int)this->Height;
	}
	auto Image::IsValid() const -> bool {
		if (this->Resource)
			return true;
		else return false;
	}

	void DoWindowEvents() {
		int Size = WindowSet.size();
		for (int i = 0; i <= Size - 1; i++)
			if (WindowSet[i]) {
				MSG Msg = { 0 };
				Msg.hwnd = WindowSet[i]->GetHwnd();
				if (PeekMessage(&Msg, NULL, NULL, NULL, PM_REMOVE)) {
					TranslateMessage(&Msg);
					DispatchMessage(&Msg);
				}
				while (!WindowSet[i]->MsgTeam.empty()) {
					Window::Event Event;
					Event = WindowSet[i]->MsgTeam.front();
					WindowSet[i]->MsgTeam.pop();
					if (Event.Type == EventType::Close) {
						WindowSet[i]->Close();
					}
					if (Event.Type == EventType::Resize) {
						ResizeEventArgs* EventArg=(ResizeEventArgs*)Event.Args;
						WindowSet[i]->Resize(EventArg->width, EventArg->height);
					}
					int FunctionSize = WindowSet[i]->EventFunction[(int)Event.Type].size();
					for (int j = 0; j <= FunctionSize - 1; j++) {
						WindowSet[i]->EventFunction[(int)Event.Type][j](WindowSet[i], Event.Args);
					}
					delete Event.Args;
				}
			}
	}

	Window::Window(const int width,
		const int height,
		const CharType *title,
		const Image &icon,
		const WindowFlags flags) {
		this->Hinstance = ::GetModuleHandle(NULL);
		//Register Window
		WNDCLASS WindowClass;



		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = (WNDPROC)WndProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = this->Hinstance;
		WindowClass.hIcon = (HICON)LoadImage(0, icon.FileName, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
		WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		WindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = (wchar_t*)title;


		RegisterClass(&WindowClass);
		//Create Window

		this->Hwnd = CreateWindowW((wchar_t*)title, (wchar_t*)title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, this->Hinstance, nullptr);

		if (!(flags & WindowFlags::RESIZABLE) || flags == WindowFlags::DEFAULT_FLAGS) {
			long Style;
			Style = GetWindowLong(this->Hwnd, GWL_STYLE);
			Style = (Style &~WS_SIZEBOX);
			Style = (Style &~WS_MAXIMIZEBOX);
			SetWindowLong(this->Hwnd, GWL_STYLE, Style);
		}

		ShowWindow(this->Hwnd, SW_SHOWNORMAL);
		UpdateWindow(this->Hwnd);

		this->Width = width;
		this->Height = height;
		if ((flags & WindowFlags::FULLSCREEN) && flags != WindowFlags::DEFAULT_FLAGS)
			this->FullScrren = true;
		else this->FullScrren = false;
		WindowsSize++;
		WindowSet.push_back(this);
	}
	Window::~Window() {
		
	}


	void Window::AddHandler(const EventType &type,
		const CallbackType &callback) {
		this->EventFunction[(int)type].push_back(callback);
	}
	void Window::RemoveHandlers(const EventType &type) {
		this->EventFunction[(int)type].clear();
	}
	void Window::Close() {
		this->Hwnd = NULL;
		for (int i = 0; i <= 5; i++)
			this->EventFunction[i].clear();
		this->Hwnd = NULL;
		WindowsSize--;
	}
	void Window::Resize(const int Width, const int Height) {
	}
	void Window::ChangeFullScreen() {
		if (this->FullScrren == false)
			this->FullScrren = true;
		else this->FullScrren = false;
	}
	void Window::SetWidth(int Width) {
		this->Width = Width;
	}
	void Window::SetHeight(int Height) {
		this->Height = Height;
	}
	auto Window::IsValid()const -> bool {
		if (!this->Hwnd)
			return false;
		else return true;
	}
	auto Window::GetFullScreen()const->bool {
		return this->FullScrren;
	}
	auto Window::GetHwnd()const->HWND {
		return this->Hwnd;
	}
	auto Window::GetWidth()const->int {
		return this->Width;
	}
	auto Window::GetHeight()const->int {
		return this->Height;
	}

	Texture::Texture(const Image &image) {
		HRESULT hr;
		hr = D3DX11CreateShaderResourceViewFromFile(Device, image.FileName, 0, 0, &this->Tex, 0);
		SendMsg(hr, L"Create Texture Failed");
	}
	Texture::~Texture() {
		SafeRelease(this->Tex);
	}

	auto Texture::IsValid() const ->bool {
		if (this->Tex == NULL)
			return false;
		else return true;
	}
	auto Texture::GetTexture() const -> TextureResoure {
		return this->Tex;
	}

	VertexBuffer::VertexBuffer() {

	}
	VertexBuffer::~VertexBuffer() {
		SafeRelease(this->Vertex);
		SafeRelease(this->Layout);
	}


	auto VertexBuffer::IsValid() const -> bool {
		if (this->Vertex == NULL)
			return false;
		else return true;
	}
	auto VertexBuffer::GetVertex() const -> Buffer {
		return this->Vertex;
	}
	auto VertexBuffer::GetLayout() const -> InputLayout {
		return this->Layout;
	}
	

	IndexBuffer::IndexBuffer() {

	}
	IndexBuffer::~IndexBuffer() {
		SafeRelease(this->Index);
	}

	auto IndexBuffer::IsValid() const -> bool {
		if (this->Index == NULL)
			return false;
		else return true;
	}
	auto IndexBuffer::GetIndex() const -> Buffer {
		return this->Index;
	}



	Shader::Shader(const CharType *filepath, const ShaderType &type) {
		Type = type;
		ShaderBlob ErrorMsg(NULL);
		HRESULT hr;
		UINT flag(0);
#if defined(DEBUG) || defined(_DEBUG)  
		flag |= D3D10_SHADER_DEBUG;
		flag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif  
		switch (type)
		{
		case ShaderType::VertexShader: {
			hr = D3DX11CompileFromFile((wchar_t*)filepath, 0, 0, "main", "vs_5_0",
				flag, 0, 0, &this->Blob, &ErrorMsg, 0);
			SendMsg(hr, L"Compile VertexShader Failed");
			hr = Device->CreateVertexShader(this->Blob->GetBufferPointer(),
				this->Blob->GetBufferSize(), NULL, &this->Vs);
			SendMsg(hr, L"Create VertexShader Failed");
			break;
		}
		case ShaderType::PixelShader: {
			hr = D3DX11CompileFromFile((wchar_t*)filepath, 0, 0, "main", "ps_5_0",
				flag, 0, 0, &this->Blob, &ErrorMsg, 0);
			SendMsg(hr, L"Compile PixelShader Failed");
			hr = Device->CreatePixelShader(this->Blob->GetBufferPointer(),
				this->Blob->GetBufferSize(), NULL, &this->Ps);
			SendMsg(hr, L"Create PixelShader Failed");
			break;
		}
		default:
			break;
		}
	}
	Shader::~Shader() {
		SafeRelease(this->Blob);
		SafeRelease(this->Vs);
		SafeRelease(this->Ps);
	}


	auto Shader::IsValid() const -> bool {
		if (this->Blob == NULL)
			return false;
		else return true;
	}
	auto Shader::GetBlob() const->ShaderBlob {
		return this->Blob;
	}
	auto Shader::GetVertexShader() const -> VertexShader {
		return this->Vs;
	}
	auto Shader::GetPixelShader() const -> PixelShader {
		return this->Ps;
	}
	auto Shader::GetShaderType() const -> ShaderType {
		return this->Type;
	}





	ShaderProgram::ShaderProgram(Shader *vertex, Shader *pixel) {
		this->Vertex = vertex;
		this->Pixel = pixel;
		this->Valid = true;
	}
	ShaderProgram::~ShaderProgram() {
		Valid = false;
	}


	auto ShaderProgram::IsValid() const -> bool {
		return Valid;
	}
	auto ShaderProgram::GetShaderBlob() const -> ShaderBlob {
		return this->Vertex->GetBlob();
	}
	auto ShaderProgram::GetVertexShader() const->VertexShader {
		return this->Vertex->GetVertexShader();
	}
	auto ShaderProgram::GetPixelShader() const -> PixelShader {
		return this->Pixel->GetPixelShader();
	}


	void Renderer::CreateShaderDataBuffer(Buffer* DataBuffer, UINT BufferSize, void* Data) {
		HRESULT hr;
		D3D11_BUFFER_DESC constantBufferDesc = { 0 };
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.ByteWidth = BufferSize * 8;
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hr = Device->CreateBuffer(&constantBufferDesc,
			(D3D11_SUBRESOURCE_DATA*)Data, DataBuffer);
		SendMsg(hr, L"Create Shader DataBuffer Failed");
	}



	Renderer::Renderer(Window *window, ShaderProgram *program) {
		this->_Window = window;
		this->Shader = program;
		HRESULT hr;
		IDXGIDevice *DXGIDevice(NULL);
		IDXGIAdapter *DXGIAdapter(NULL);
		IDXGIFactory *DXGIFactory(NULL);
		ID3D11Texture2D *BackBuffer(NULL);
		ID3D11Texture2D *DepthStencilBuffer(NULL);
		UINT MSAA4xQuality(0);


		Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &MSAA4xQuality);


		DXGI_SWAP_CHAIN_DESC Swap = { 0 };
		Swap.BufferDesc.Width = this->_Window->GetWidth();
		Swap.BufferDesc.Height = this->_Window->GetHeight();
		Swap.BufferDesc.RefreshRate.Denominator = 1;
		Swap.BufferDesc.RefreshRate.Numerator = 60;
		Swap.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		Swap.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		Swap.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		Swap.BufferCount = 1;
		Swap.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		Swap.Flags = 0;
		Swap.OutputWindow = this->_Window->GetHwnd();
		Swap.SampleDesc.Count = 4;
		Swap.SampleDesc.Quality = MSAA4xQuality - 1;
		Swap.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		if (_Window->GetFullScreen() == true)
			Swap.Windowed = false;
		else Swap.Windowed = true;

		Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&DXGIDevice));
		DXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&DXGIAdapter));
		DXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&DXGIFactory));


		hr = DXGIFactory->CreateSwapChain(Device, &Swap, &SwapChain);
		SendMsg(hr, L"Create SwapChain Failed");


		this->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
		hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView);

		D3D11_TEXTURE2D_DESC Desc = { 0 };
		Desc.Width = this->_Window->GetWidth();
		Desc.Height = this->_Window->GetHeight();
		Desc.MipLevels = 1;
		Desc.ArraySize = 1;
		Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		Desc.MiscFlags = 0;
		Desc.SampleDesc.Count = 4;
		Desc.SampleDesc.Quality = MSAA4xQuality - 1;
		Desc.Usage = D3D11_USAGE_DEFAULT;
		Desc.CPUAccessFlags = 0;

		hr = Device->CreateTexture2D(&Desc, NULL, &DepthStencilBuffer);
		SendMsg(hr, L"Create DepthStencilBuffer Failed");

		hr = Device->CreateDepthStencilView(DepthStencilBuffer, NULL, &DepthStencilView);
		SendMsg(hr, L"Create DepthStencilView Failed");

	
		DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

		D3D11_VIEWPORT ViewPort = { 0 };
		ViewPort.Width = (FLOAT)(this->_Window->GetWidth());
		ViewPort.Height = (FLOAT)(this->_Window->GetHeight());
		ViewPort.MinDepth = 0.f;
		ViewPort.MaxDepth = 1.f;
		ViewPort.TopLeftX = 0.f;
		ViewPort.TopLeftY = 0.f;
		DeviceContext->RSSetViewports(1, &ViewPort);

		DeviceContext->VSSetShader(this->Shader->GetVertexShader(), 0, 0);
		DeviceContext->PSSetShader(this->Shader->GetPixelShader(), 0, 0);

		this->CreateShaderDataBuffer(&this->ProjectionMatrix, sizeof(mat4), nullptr);
		this->CreateShaderDataBuffer(&this->ModelMatrix, sizeof(mat4), nullptr);
		this->CreateShaderDataBuffer(&this->ViewMatrix, sizeof(mat4), nullptr);
		this->CreateShaderDataBuffer(&this->IsUsedTex, sizeof(bool) * 2, nullptr);
		
		SafeRelease(DXGIDevice);
		SafeRelease(DXGIAdapter);
		SafeRelease(DXGIFactory);
		SafeRelease(BackBuffer);
		SafeRelease(DepthStencilBuffer);
		
	}
	Renderer::~Renderer() {
		SafeRelease(this->SwapChain);
		SafeRelease(this->DepthStencilView);
		SafeRelease(this->RenderTargetView);
	}


	void Renderer::SetProjectionMatrix(const glm::mat4 &matrix) {
		mat4 Matrix;
		Matrix = glm::transpose(matrix);
		DeviceContext->UpdateSubresource(this->ProjectionMatrix, 0, NULL, &Matrix, 0, 0);
		DeviceContext->VSSetConstantBuffers(0, 1, &this->ProjectionMatrix);
	}
	void Renderer::SetModelMatrix(const glm::mat4 &matrix) {
		mat4 Matrix;
		Matrix = glm::transpose(matrix);
		DeviceContext->UpdateSubresource(this->ModelMatrix, 0, NULL, &Matrix, 0, 0);
		DeviceContext->VSSetConstantBuffers(1, 1, &this->ModelMatrix);
	}
	void Renderer::SetViewMatrix(const glm::mat4 &matrix) {
		mat4 Matrix;
		Matrix = glm::transpose(matrix);
		DeviceContext->UpdateSubresource(this->ViewMatrix, 0, NULL, &Matrix, 0, 0);
		DeviceContext->VSSetConstantBuffers(2, 1, &this->ViewMatrix);
	}
	void Renderer::SetProjectionMatrix(const DirectX::XMMATRIX &matrix) {
		DeviceContext->UpdateSubresource(this->ProjectionMatrix, 0, NULL, &matrix, 0, 0);
		DeviceContext->VSSetConstantBuffers(0, 1, &this->ProjectionMatrix);
	}
	void Renderer::SetModelMatrix(const DirectX::XMMATRIX &matrix) {
		DeviceContext->UpdateSubresource(this->ModelMatrix, 0, NULL, &matrix, 0, 0);
		DeviceContext->VSSetConstantBuffers(1, 1, &this->ModelMatrix);
	}
	void Renderer::SetViewMatrix(const DirectX::XMMATRIX &matrix) {
		DeviceContext->UpdateSubresource(this->ViewMatrix, 0, NULL, &matrix, 0, 0);
		DeviceContext->VSSetConstantBuffers(2, 1, &this->ViewMatrix);
	}


	void Renderer::BindCurrentTexture(const Texture &texture) {
		TextureResoure Resource = texture.GetTexture();
		bool IsUsedTexture = true;
		DeviceContext->UpdateSubresource(this->IsUsedTex, 0, NULL, &IsUsedTexture, 0, 0);
		DeviceContext->PSSetConstantBuffers(0, 1, &this->IsUsedTex);
		DeviceContext->PSSetShaderResources(0, 1, &Resource);
	}
	void Renderer::UnbindTexture() {
		bool IsUsedTexture = false;
		DeviceContext->UpdateSubresource(this->IsUsedTex, 0, NULL, &IsUsedTexture, 0, 0);
		DeviceContext->PSSetConstantBuffers(0, 1, &this->IsUsedTex);

	}

	void Renderer::ResetShaderProgram(ShaderProgram* program) {
		this->Shader = program;
		DeviceContext->VSSetShader(this->Shader->GetVertexShader(), 0, 0);
		DeviceContext->PSSetShader(this->Shader->GetPixelShader(), 0, 0);
	}

	void Renderer::SetVertexBuffer(VertexBuffer *target, const int size, Vertex data[],
		const PrimitiveType type) {
		HRESULT hr;
		D3D11_INPUT_ELEMENT_DESC InputDesc[4];
		
		InputDesc[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 };
		InputDesc[1] = { "COLOR",   0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA ,0 };
		InputDesc[2] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,28,D3D11_INPUT_PER_VERTEX_DATA,0 };
		InputDesc[3] = { "NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,36,D3D11_INPUT_PER_VERTEX_DATA,0 };
		
		hr = Device->CreateInputLayout(InputDesc, 4, this->Shader->GetShaderBlob()->GetBufferPointer(),
			this->Shader->GetShaderBlob()->GetBufferSize(), &target->Layout);
		SendMsg(hr, L"Create InputLayout Failed");

		D3D11_BUFFER_DESC BufferDesc = { 0 };
		
		BufferDesc.ByteWidth = size*sizeof(Vertex);
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		
		D3D11_SUBRESOURCE_DATA BufferData = { 0 };

		BufferData.pSysMem = data;
		target->Type = type;
		target->VertexNum = size;
		hr = Device->CreateBuffer(&BufferDesc, &BufferData, &target->Vertex);
		SendMsg(hr, L"Create VertexBuffer Failed");

	}
	void Renderer::SetIndexBuffer(IndexBuffer *target,
		VertexBuffer* vertex, 
		const int size, 
		unsigned data[],
		const PrimitiveType type) {
		HRESULT hr;
		D3D11_BUFFER_DESC BufferDesc = { 0 };
		
		BufferDesc.ByteWidth = size * 4;
	    BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		
		D3D11_SUBRESOURCE_DATA BufferData = { 0 };
		
		BufferData.pSysMem = data;
		
		hr = Device->CreateBuffer(&BufferDesc, &BufferData, &target->Index);
		SendMsg(hr, L"Create IndexBuffer Failed");
		target->Type = type;
		target->IndexNum = size;
		target->Vertex = vertex;
	}
    
	void Renderer::Clear(const float red,const float green,const float blue) {
		float float4[4] = { red,green,blue,1};
		DeviceContext->ClearRenderTargetView(this->RenderTargetView, float4);
		DeviceContext->ClearDepthStencilView(this->DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
	void Renderer::Begin() {

	}
	void Renderer::End() {
	}
	void Renderer::Present() {
		
		if (this->_Window->NeedFullScreen == true) {
			DXGI_MODE_DESC Desc;
			HRESULT hr;
			Desc.Height = this->_Window->GetHeight();
			Desc.Width = this->_Window->GetWidth();
			Desc.RefreshRate.Denominator = 1;
			Desc.RefreshRate.Numerator = 60;
			Desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			Desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			hr = this->SwapChain->ResizeTarget(&Desc);
			SendMsg(hr, L"Resize Failed");
			if (this->_Window->GetFullScreen() == true) {
				ChangeSystem(this->_Window->GetWidth(), this->_Window->GetHeight());
				hr = this->SwapChain->SetFullscreenState(TRUE, NULL);
			}
			else hr = this->SwapChain->SetFullscreenState(FALSE, NULL);
			this->_Window->NeedFullScreen = false;
		}
		this->SwapChain->Present(0, 0);

	}
	
	template <>
	void render::Renderer::DrawBuffer(const render::VertexBuffer &buffer) {
		DeviceContext->IASetInputLayout(buffer.Layout);
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, &buffer.Vertex, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)buffer.Type);
		DeviceContext->Draw(buffer.VertexNum, 0);
	}
	template <>
	void render::Renderer::DrawBuffer(const render::IndexBuffer &buffer) {
		DeviceContext->IASetInputLayout(buffer.Vertex->Layout);
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, &buffer.Vertex->Vertex, &stride, &offset);
		DeviceContext->IASetIndexBuffer(buffer.Index, DXGI_FORMAT_R32_UINT, 0);
		DeviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)buffer.Type);
		DeviceContext->DrawIndexed(buffer.IndexNum, 0, 0);
	}
}
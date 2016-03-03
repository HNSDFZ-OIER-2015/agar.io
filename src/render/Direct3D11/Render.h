#pragma once
#ifndef RENDER_Render_h_
#define RENDER_Render_h_

#include<queue>
#include<d3d11.h>
#include<D3DX11.h>
#include<glm.hpp>
#include<functional>
#include<Windows.h>
#include<wincodec.h>
#include<windowsx.h>
#include<DirectXMath.h>
#include<gtc\type_ptr.hpp>
#include<gtc/matrix_transform.hpp>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib, "Windowscodecs.lib" )
using namespace std;
using namespace glm;
using namespace DirectX;
#define BACKEND_DIRECT3D11 
namespace render {
	//233
	class Window;
	class IndexBuffer;
	//
	typedef IWICBitmapDecoder* ImageResoure;
	typedef ID3D11ShaderResourceView* TextureResoure;
	typedef ID3D11Buffer* Buffer;
	typedef ID3DBlob* ShaderBlob;
	typedef ID3D11VertexShader* VertexShader;
	typedef ID3D11PixelShader* PixelShader;
	typedef ID3D11InputLayout* InputLayout;
	typedef char16_t CharType;
	typedef CharType* UTF16String;
	//SendMsg
	int GetWindowsNum();
	static void SendMsg(HRESULT hr, LPCWSTR Message) {
		if (FAILED(hr))
			MessageBox(NULL, Message, L"Error", 0);
	}
	//Release
	template<typename Interface>
	void SafeRelease(Interface* Obj) {
		if (Obj)
			Obj->Release();
		Obj = NULL;
	}
	//IWIC
	
	
	
	void Initialize(); /*implement this*/
	void Terminate();  /*implement this*/

					   ///////////////
					   // Resources //
					   ///////////////


	class Image {
	private:
		ImageResoure Resource;
		UINT Width;
		UINT Height;
	public:
		LPCWSTR FileName;
		Image() = delete;
		Image(const CharType *filepath); /*implement this*/
		~Image();                        /*implement this*/

		Image(const Image &) = delete;
		auto operator=(const Image &)->Image & = delete;

		Image(Image &&) = delete;
		auto operator=(Image &&)->Image & = delete;

		void Save(const CharType *filepath);   /*implement this*/
		auto GetWidth() const ->int;          /*implement this*/
		auto GetHeight() const ->int;        /*implement this*/
		auto IsValid() const -> bool;          /*implement this*/
	};                                         // class Image

											   //////////////
											   // Platform //
											   //////////////

	enum class EventType {
		Close,
		Resize,
		MouseMotion,
		MouseWheel,
		MouseClick,
		Keyboard,
	};  // enum class EventType

		/*implement this*/
	enum class MouseButton {
		Left,
		Middle,
		Right,
	};  // enum class MouseButton

		/*implement this*/
	enum class Keycode {
		Unknown = 0,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		Num0 = 48,
		Num1 = 49,
		Num2 = 50,
		Num3 = 51,
		Num4 = 52,
		Num5 = 53,
		Num6 = 54,
		Num7 = 55,
		Num8 = 56,
		Num9 = 57,
		Numpad0 = 96,
		Numpad1 = 97,
		Numpad2 = 98,
		Numpad3 = 99,
		Numpad4 = 100,
		Numpad5 = 101,
		Numpad6 = 102,
		Numpad7 = 103,
		Numpad8 = 104,
		Numpad9 = 105,
		Quote = 0xDE,
		Backslash = 0xDC,
		Backspace = 8,
		CapsLock = 20,
		Comma = 0xBC,
		Delete = 46,
		Down = 40,
		End = 35,
		Equal = 0x92,
		Escape = 27,
		F1 = 112,
		F2 = 113,
		F3 = 114,
		F4 = 115,
		F5 = 116,
		F6 = 117,
		F7 = 118,
		F8 = 119,
		F9 = 120,
		F10 = 121,
		F11 = 122,
		F12 = 123,
		F13 = 124,
		F14 = 125,
		F15 = 126,
		F16 = 127,
		F17 = 128,
		F18 = 129,
		F19 = 130,
		F20 = 131,
		F21 = 132,
		F22 = 133,
		F23 = 134,
		F24 = 135,
		Backquote = 0xC0,
		Home = 36,
		Insert = 45,
		LeftAlt = 0xA4,
		LeftControl = 0xA2,
		Left = 37,
		Leftbracket = 0xDB,
		LeftShift = 0xA0,
		Minus = 0xBD,
		NumLock = 144,
		PageDown = 34,
		PageUp = 33,
		Period = 0xBE,
		PrintScreen = 44,
		RightAlt = 0xA5,
		RightControl = 0xA3,
		RightShift = 0xA1,
		Return = 13,
		Right = 39,
		Rightbracket = 0xDD,
		ScrollLock = 145,
		Semicolon = 0xBA,
		Slash = 0xBF,
		Space = 32,
		Tab = 9,
		Up = 38,
		Ampersand = Unknown,
		Asterisk = Unknown,
		At = Unknown,
		Caret = Unknown,
		Colon = Unknown,
		Dollar = Unknown,
		Exclaim = Unknown,
		Greater = Unknown,
		Hash = Unknown,
		LeftParen = Unknown,
		Less = Unknown,
		Percent = Unknown,
		Plus = Unknown,
		Question = Unknown,
		DoubleQuote = Unknown,
		RightParen = Unknown,
		Underscore = Unknown,
	};  // enum class Keycode

	enum Modifier : unsigned {
		LCTRL = 1 << 0,
		RCTRL = 1 << 1,
		LSHIFT = 1 << 2,
		RSHIFT = 1 << 3,
		LALT = 1 << 4,
		RALT = 1 << 5,
		NUM_LOCK = 1 << 6,
		SCROLL_LOCK = 1 << 7,
		CAPS_LOCK = 1 << 8,
	};  // enum Modifier

	struct EventArgs {};  // struct EventArgs

	struct CloseEventArgs : public EventArgs {};  // struct CloseEventArgs

	struct ResizeEventArgs : public EventArgs {
		int width;
		int height;
	};  // struct ResizeEventArgs

	struct MouseMotionEventArgs : public EventArgs {
		int x;
		int y;
	};  // struct MouseMotionEventArgs

	struct MouseWheelEventArgs : public EventArgs {
		int x;
		int y;
		int offest_x;
		int offest_y;
	};  // struct MouseWheelEventArgs

	struct MouseClickEventArgs : public EventArgs {
		int x;
		int y;
		MouseButton button;
		bool pressed;
		bool released;
	};  // struct MouseClickEventArgs

	struct KeyboardEventArgs : public EventArgs {
		Keycode code;
		unsigned modifiers;
		bool pressed;
		bool released;
	};  // struct KeyboardEventArgs

	typedef std::function<void(void *, EventArgs *)> CallbackType;

	void DoWindowEvents(); /*implement this*/


	typedef enum {
		DEFAULT_FLAGS = 0,
		FULLSCREEN,
		RESIZABLE
	}WindowFlags;

	class Window {
	private:
		HWND Hwnd;	
		HINSTANCE Hinstance;
		bool FullScrren;
		int  Width;
		int  Height;	
	public:
		struct Event {
			EventType Type;
			EventArgs* Args;
		};
		bool NeedFullScreen;
		
		queue<Event> MsgTeam;
		vector<CallbackType> EventFunction[6];
		Window() = delete;
		Window(const int width,
			const int height,
			const CharType *title,
			const Image &icon,
			const WindowFlags flags=DEFAULT_FLAGS); /*implement this*/
		~Window();                             /*implement this*/

		Window(const Window &) = delete;
		auto operator=(const Window &)->Window & = delete;

		Window(Window &&) = delete;
		auto operator=(Window &&)->Window & = delete;

		
		void AddHandler(const EventType &type,
			const CallbackType &callback); /*implement this*/
		void RemoveHandlers(const EventType &type);    /*implement this*/

		void Close();                                   /*implement this*/
		void Resize(const int width, const int height); /*implement this*/
		void ChangeFullScreen();
		void SetWidth(int Width);
		void SetHeight(int Height);


		auto IsValid() const -> bool; /*implement this*/
		auto GetFullScreen() const ->bool;
		auto GetHwnd() const -> HWND;
		auto GetWidth() const -> int;
		auto GetHeight()const -> int;

	};                                // class Window

									  ///////////////
									  // Rendering //
									  ///////////////

	class Texture {
	private:
		TextureResoure Tex;
	public:	
		Texture() = delete;
		Texture(const Image &image); /*implement this*/
		~Texture();                  /*implement this*/

		Texture(const Texture &) = delete;
		auto operator=(const Texture &)->Texture & = delete;

		Texture(Texture &&) = delete;
		auto operator=(Texture &&)->Texture & = delete;

		auto IsValid() const -> bool; /*implement this*/
		auto GetTexture() const -> TextureResoure;
	};                                // class Texture

	struct Vertex {
		float x, y, z;
		float r, g, b, a;
		float u, v;
		float nx, ny, nz;
	};  // struct Vertex

	enum class PrimitiveType {
		Unknown = 0,
		Point = 1,
		Line = 2,
		LineStrip = 3,
		Triangle = 4,
		TriangleStrip = 5,
	};  // enum class PrimitiveType


	class VertexBuffer {
	public:
		UINT VertexNum;
		Buffer Vertex;
		InputLayout Layout;
		PrimitiveType Type;

		VertexBuffer();  /*implement this*/
		~VertexBuffer(); /*implement this*/

		VertexBuffer(const VertexBuffer &) = delete;
		auto operator=(const VertexBuffer &)->VertexBuffer & = delete;

		VertexBuffer(VertexBuffer &&) = delete;
		auto operator=(VertexBuffer &&)->VertexBuffer & = delete;

		auto IsValid() const -> bool; /*implement this*/

		auto GetVertex() const->Buffer;
		auto GetLayout() const->InputLayout;
		
	};                             // class VertexBuffer

	class IndexBuffer {
	public:
		UINT IndexNum;
		VertexBuffer* Vertex;
		Buffer Index;
		PrimitiveType Type;
		IndexBuffer();  /*implement this*/
		~IndexBuffer(); /*implement this*/

		IndexBuffer(const IndexBuffer &) = delete;
		auto operator=(const IndexBuffer &)->IndexBuffer & = delete;

		IndexBuffer(IndexBuffer &&) = delete;
		auto operator=(IndexBuffer &&)->IndexBuffer & = delete;
		auto IsValid() const -> bool; /*implement this*/

		auto GetIndex() const->Buffer;
	};                               // class IndexBuffer

	enum class ShaderType {
		VertexShader,
		PixelShader,
	};  // enum class ShaderType

	class Shader {
	private:
		ShaderType Type;
		ShaderBlob Blob;
		VertexShader Vs;
		PixelShader Ps;
	public:
		Shader() = delete;
		Shader(const CharType *filepath, const ShaderType &type); /*implement this*/
		~Shader();                                                /*implement this*/

		Shader(const Shader &) = delete;
		auto operator=(const Shader &)->Shader & = delete;

		Shader(Shader &&) = delete;
		auto operator=(Shader &&)->Shader & = delete;

		auto IsValid() const -> bool; /*implement this*/
		auto GetBlob() const-> ShaderBlob;
		auto GetVertexShader() const-> VertexShader;
		auto GetPixelShader() const-> PixelShader;
		auto GetShaderType() const-> ShaderType;
	};                                // class Shader

	class ShaderProgram {
		bool Valid;
		Shader* Vertex;
		Shader* Pixel;
	public:
		ShaderProgram() = delete;
		ShaderProgram(Shader *vertex, Shader *pixel); /*implement this*/
		~ShaderProgram();                             /*implement this*/

		ShaderProgram(const ShaderProgram &) = delete;
		auto operator=(const ShaderProgram &)->ShaderProgram & = delete;

		ShaderProgram(ShaderProgram &&) = delete;
		auto operator=(ShaderProgram &&)->ShaderProgram & = delete;

		auto IsValid() const -> bool; /*implement this*/
		
		auto GetShaderBlob() const-> ShaderBlob;
		auto GetVertexShader() const -> VertexShader;
		auto GetPixelShader() const-> PixelShader;
	};                                // class ShaderProgram

	class Renderer {
	private:
	    Window* _Window;
		ShaderProgram* Shader;
		Buffer ProjectionMatrix;
		Buffer ModelMatrix;
		Buffer ViewMatrix;
		Buffer IsUsedTex;
		IDXGISwapChain		*SwapChain;
		ID3D11DepthStencilView	*DepthStencilView;
		ID3D11RenderTargetView	*RenderTargetView;
		void CreateShaderDataBuffer(Buffer* DataBuffer, UINT BuffSize, void* Data);
	public:
		Renderer() = delete;
		Renderer(Window *window, ShaderProgram *program); /*implement this*/
		~Renderer();                                      /*implement this*/

		Renderer(const Renderer &) = delete;
		auto operator=(const Renderer &)->Renderer & = delete;

		Renderer(Renderer &&) = delete;
		auto operator=(Renderer &&)->Renderer & = delete;

		void SetProjectionMatrix(const glm::mat4 &matrix); /*implement this*/
		void SetModelMatrix(const glm::mat4 &matrix);      /*implement this*/
		void SetViewMatrix(const glm::mat4 &matrix);       /*implement this*/
		void SetProjectionMatrix(const DirectX::XMMATRIX &matrix);
		void SetModelMatrix(const DirectX::XMMATRIX &matrix);
		void SetViewMatrix(const DirectX::XMMATRIX &matrix);

		void BindCurrentTexture(const Texture &texture); /*implement this*/
		void UnbindTexture();                  /*implement this*/

		void ResetShaderProgram(ShaderProgram *program); /*implement this*/

		void SetVertexBuffer(VertexBuffer *target,
			const int size,
			Vertex data[],
			const PrimitiveType type = PrimitiveType::Triangle); /*implement this*/
		void SetIndexBuffer(IndexBuffer *target,
			VertexBuffer* vertex,
			const int size,
			unsigned data[],
			const PrimitiveType type = PrimitiveType::Triangle); /*implement this*/

		template <typename TContainer>
		void SetVertexBuffer(VertexBuffer *target, const TContainer &data) {
			SetVertexBuffer(target, data.size(), data.data());
		}
		
		  template <typename TContainer>
		  void SetIndexBuffer(IndexBuffer *target,
			  VertexBuffer *vertex,
			  const TContainer &data) {
			  SetIndexBuffer(target, vertex, data.size(), data.data());
		  }


		void Clear(const float red = 0.0f,
			const float green = 0.0f,
			const float blue = 0.0f); /*implement this*/
		void Begin();                         /*implement this*/
		void End();                           /*implement this*/
		void Present();                       /*implement this*/

		template <typename TBuffer>
		void DrawBuffer(const TBuffer &buffer); /*implement this*/

												// In implementation
												// template <>
												// void Renderer::DrawBuffer(const VertexBuffer &buffer) {}
												// template <>
												// void Renderer::DrawBuffer(const IndexBuffer &buffer) {}
	};  // class Renderer

}  // namespace render

   // Hash for EventType
namespace std {
	template <>
	struct hash<render::EventType> {
		auto operator()(const render::EventType &value) const -> size_t {
			return static_cast<size_t>(value);
		}
	};  // struct hash<render::EventType>
}  // namespace std
#endif
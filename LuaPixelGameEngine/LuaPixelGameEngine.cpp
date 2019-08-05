#define SOL_ALL_SAFETIES_ON 1
#define OLC_PGE_APPLICATION
#include "LuaPixelGameEngine.h"

class LuaPixelGameEngine final : public olc::PixelGameEngine
{
public:
	LuaPixelGameEngine() = default;
	explicit LuaPixelGameEngine(const std::string& aName) { sAppName = aName; }

	bool OnUserCreate() override
	{
		LoadLua(SList);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (LuaGameState)
		{
			auto& lua = *LuaGameState;
			lua["OnUserUpdate"](fElapsedTime);
		}

		if (GetAsyncKeyState(VK_F5) & 1)
		{
			LoadLua(SList);
		}

		if (GetAsyncKeyState(VK_F4) & 1)
		{
			UnLoadLua();
			Clear(olc::BLACK);
		}
		return true;
	}
};

bool LoadLua(const std::vector<const char*>& scriptList)
{
	if (!LuaGameState)
	{
		LuaGameState = std::make_unique<sol::state>();
		auto& lua = *LuaGameState;
		lua.open_libraries();
		auto engine = static_cast<LuaPixelGameEngine*>(EnginePtr);

		auto screen = lua.new_usertype<ScreenNamespace>("Screen", sol::no_constructor);
		{
			//to set read write 
			//screen.set("x", sol::property([engine]() { return engine->ScreenWidth; }, [engine](int width) { engine->ScreenWidth = width;}));
			screen.set("x", sol::readonly_property([engine]() { return engine->ScreenWidth(); }));
			screen.set("y", sol::readonly_property([engine]() { return engine->ScreenHeight(); }));
			screen.set("hasFocus", sol::readonly_property([engine]() { return engine->IsFocused(); }));
			screen.set("Clear", [engine](const olc::Pixel p) { return engine->Clear(p); });
		}

		auto hwButton = lua.new_usertype<olc::HWButton>("HWButton", sol::no_constructor);
		{
			hwButton.set("pressed", sol::readonly_property(&olc::HWButton::bPressed));
			hwButton.set("released", sol::readonly_property(&olc::HWButton::bReleased));
			hwButton.set("held", sol::readonly_property(&olc::HWButton::bHeld));
		}

		auto io = lua.new_usertype<IoNamespace>("IO", sol::no_constructor);
		{
			io.set("Key", [engine](const olc::Key k) { return engine->GetKey(k); });
			io.set("Mouse", [engine](const uint32_t b) { return engine->GetMouse(b); });
			io.set("mouseX", sol::readonly_property([engine]() { return engine->GetMouseX(); }));
			io.set("mouseY", sol::readonly_property([engine]() { return engine->GetMouseY(); }));
			io.set("mouseWheel", sol::readonly_property([engine]() { return engine->GetMouseWheel(); }));
		}

		auto draw = lua["Draw"].get_or_create<sol::table>();
		{
			draw.set_function("Pixel", &LuaPixelGameEngine::Draw, engine);
			draw.set_function("Circle", &LuaPixelGameEngine::DrawCircle, engine);
			draw.set_function("Line", &LuaPixelGameEngine::DrawLine, engine);
			draw.set_function("PartialSprite", &LuaPixelGameEngine::DrawPartialSprite, engine);
			draw.set_function("Sprite", &LuaPixelGameEngine::DrawSprite, engine);
			draw.set_function("String", &LuaPixelGameEngine::DrawString, engine);
			draw.set_function("Triangle", &LuaPixelGameEngine::DrawTriangle, engine);
			draw.set_function("FillRect", &LuaPixelGameEngine::FillRect, engine);
			draw.set("SetPixelMode", [engine](const int32_t m) { return engine->SetPixelMode(static_cast<olc::Pixel::Mode>(m)); });
		}

		auto pixel = lua.new_usertype<olc::Pixel>("Pixel", sol::constructors<olc::Pixel(), olc::Pixel(uint32_t), olc::Pixel(uint8_t, uint8_t, uint8_t, uint8_t)>());
		{
			pixel["a"] = &olc::Pixel::a;
			pixel["r"] = &olc::Pixel::r;
			pixel["g"] = &olc::Pixel::g;
			pixel["b"] = &olc::Pixel::b;
		}

		auto sprite = lua.new_usertype<olc::Sprite>("Sprite", sol::constructors<olc::Sprite(), olc::Sprite(std::string), olc::Sprite(std::string, olc::ResourcePack*)>());
		{
			sprite["x"] = &olc::Sprite::width;
			sprite["y"] = &olc::Sprite::height;
		}

		lua.new_enum<olc::Key>("Key", {
			{"NONE", olc::NONE},

			{"A", olc::A},
			{"B", olc::B},
			{"C", olc::C},
			{"D", olc::D},
			{"E", olc::E},
			{"F", olc::F},
			{"G", olc::G},
			{"H", olc::H},
			{"I", olc::I},
			{"J", olc::J},
			{"K", olc::K},
			{"L", olc::L},
			{"M", olc::M},
			{"N", olc::N},
			{"O", olc::O},
			{"P", olc::P},
			{"Q", olc::Q},
			{"R", olc::R},
			{"S", olc::S},
			{"T", olc::T},
			{"U", olc::U},
			{"V", olc::V},
			{"W", olc::W},
			{"X", olc::X},
			{"Y", olc::Y},
			{"Z", olc::Z},

			{"K0", olc::K0},
			{"K1", olc::K1},
			{"K2", olc::K2},
			{"K3", olc::K3},
			{"K4", olc::K4},
			{"K5", olc::K5},
			{"K6", olc::K6},
			{"K7", olc::K7},
			{"K8", olc::K8},
			{"K9", olc::K9},

			{"F1", olc::F1},
			{"F2", olc::F2},
			{"F3", olc::F3},
			{"F4", olc::F4},
			{"F5", olc::F5},
			{"F6", olc::F6},
			{"F7", olc::F7},
			{"F8", olc::F8},
			{"F9", olc::F9},
			{"F10", olc::F10},
			{"F11", olc::F11},
			{"F12", olc::F12},

			{"UP", olc::UP},
			{"DOWN", olc::DOWN},
			{"LEFT", olc::LEFT},
			{"RIGHT", olc::RIGHT},

			{"SPACE", olc::SPACE},
			{"TAB", olc::TAB},
			{"SHIFT", olc::SHIFT},
			{"CTRL", olc::CTRL},
			{"INS", olc::INS},
			{"DEL", olc::DEL},
			{"HOME", olc::HOME},
			{"END", olc::END},
			{"PGUP", olc::PGUP},
			{"PGDN", olc::PGDN},

			{"BACK", olc::BACK},
			{"ESCAPE", olc::ESCAPE},
			{"RETURN", olc::RETURN},
			{"ENTER", olc::ENTER},
			{"PAUSE", olc::PAUSE},
			{"SCROLL", olc::SCROLL},

			{"NP0", olc::NP0},
			{"NP1", olc::NP1},
			{"NP2", olc::NP2},
			{"NP3", olc::NP3},
			{"NP4", olc::NP4},
			{"NP5", olc::NP5},
			{"NP6", olc::NP6},
			{"NP7", olc::NP7},
			{"NP8", olc::NP8},
			{"NP9", olc::NP9},

			{"NP_MUL", olc::NP_MUL},
			{"NP_DIV", olc::NP_DIV},
			{"NP_ADD", olc::NP_ADD},
			{"NP_SUB", olc::NP_SUB},
			{"NP_DECIMAL", olc::NP_DECIMAL},
		});

		for (auto script : scriptList)
		{
			try 
			{
				lua.safe_script_file(script);
			}
			catch (const sol::error& e) 
			{
				std::cerr << "an error has occurred: " << e.what() << "\n";
				LuaGameState.reset();
			}
		}
		lua["OnUserCreate"]();
		return true;
	}
	return false;
}

void UnLoadLua()
{
	if (LuaGameState)
		LuaGameState.reset();
}

int main()
{
	const auto engine = new LuaPixelGameEngine("Lua Game Engine");
	EnginePtr = static_cast<LPVOID>(engine);
	int32_t sx, sy, px, py;
	std::cout << "Welcome to LuaGameEngine!\n\n";
	std::cout << "This project is based on the olcPixelEngine which can be found here,\n\n";
	std::cout << "https://github.com/OneLoneCoder/olcPixelGameEngine\n\n";
	std::cout << "F5 will load an lua script, F4 will unload an lua script.\n\n";
	std::cout << "To start lets get some info,\n\n";
	std::cout << "Please enter 4 numbers for screen X and Y and pixel X and Y.\n\n";
	std::cout << "Example 400 300 2 2\n";
	std::cin >> sx >> sy >> px >> py;
	if (sx && sy && px && py && engine->Construct(sx, sy, px, py))
		engine->Start();
	else
	{
		std::cout << "error: exiting...\n";
		Sleep(3000);
	}
	return 0;
}
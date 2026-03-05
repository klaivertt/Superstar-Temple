#pragma once

#include "Common.hpp"
#include "Tools/Miscellaneous/Delegate.hpp"

#define GUI_UPDATE_INTERVAL 1.f/60.f
#define GUI_UPDATE_INTERVAL_HIDE 2.5f

class Actor;

namespace Debug
{
	class GuiManager
	{
	private:
		sf::RenderWindow* render = nullptr;
		float lastUpdate = 0.f;

		float frameRateTarget = GUI_UPDATE_INTERVAL;
	public:
		GuiManager(sf::RenderWindow* _render);
		~GuiManager(void);

		void ProcessEvent(const sf::Event& _event);
		void Update(float _dt);
		void Render(void);

		// Fonctions pour ajouter des contrôles
		// Slider
		void AddSliderFloat(std::string _win, std::string _cat, std::string _label, float* _value, float _min, float _max, std::function<void(float, std::string)> _call = nullptr, sf::Color _color = sf::Color::White);
		void AddSliderInt(std::string _win, std::string _cat, std::string _label, int* _value, int _min, int _max, std::function<void(int, std::string)> _call = nullptr);

		// CheckBock
		void AddCheckbox(std::string _win, std::string _cat, std::string _label, bool* _value, std::function<void(bool, std::string)> _call = nullptr);

		// ComboBox
		void AddComboBox(std::string _win, std::string _cat, std::string _label, void** _objectArray, const char** _namesArray, int _size, std::function<void(void*, std::string)> _call = nullptr);

		void AddButton(std::string _win, std::string _cat, std::string _label, std::function<void(std::string)> _call = nullptr);
		void AddHierarchyButton(std::string _win, std::string _cat, std::string _label, Actor* _actor, std::function<void(Actor* _actor, std::string)> _call = nullptr);

		void AddInputText(std::string _win, std::string _cat, std::string _label, char* _buffer, size_t _bufferSize, std::function<void(const char*)> _call = nullptr);

		// Gestions des windows
		void RegisterWindow(std::string _name, bool _open = true, ImGuiWindowFlags flags = 0);
		void RemoveWindow(std::string _name);

		// Supprimer une catégorie
		void RemoveCategory(std::string _winName, std::string _catName);

		// Tout vider
		void Clear();

		void BeginWindow(std::string windowName, bool* open = nullptr, ImGuiWindowFlags flags = 0);
		void EndWindow();
		bool CollapsingHeader(std::string label, ImGuiTreeNodeFlags flags = 0);
		void Text(std::string text);
		bool InputText(std::string label, char* buffer, size_t bufferSize);
		bool Button(std::string label);
		void SameLine();
		void Separator();
		bool Selectable(std::string label, bool selected);
		void PushID(int id);
		void PopID();
		void Indent(float indentWidth = 0.f);
		void Unindent(float indentWidth = 0.f);

		class GuiControl
		{
		public:
			virtual ~GuiControl() = default;
			virtual void Render(std::string _catName) = 0;
			std::string label;

			ImVec4 color = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImGuiCol_ colorType = ImGuiCol_COUNT;

			ImVec4 backColor = ImVec4(1.f, 1.f, 1.f, 0.5f);
			ImGuiCol_ backColorType = ImGuiCol_COUNT;
		};

		class GuiTextInput : public GuiControl
		{
		public:
			char* buffer;
			size_t bufferSize;
			std::function<void(const char*)> callback;
			void Render(std::string _catName) override
			{
				if (ImGui::InputText(label.c_str(), buffer, bufferSize))
				{
					if (callback)
					{
						callback(buffer);
					}
				}
			}
		};

		class GuiSliderFloat : public GuiControl
		{
		public:
			float* value;
			float min;
			float max;
			std::function<void(float, std::string)> callback;

			void Render(std::string _catName) override
			{
				float oldValue = *value;
				if (ImGui::SliderFloat(label.c_str(), value, min, max))
				{
					if (callback)
					{
						callback(*value, _catName);
					}
				}
			}
		};

		class GuiSliderInt : public GuiControl
		{
		public:
			int* value;
			int max;
			int min;
			std::function<void(bool, std::string)> callback;

			void Render(std::string _catName) override
			{
				bool oldValue = *value;
				if (ImGui::SliderInt(label.c_str(), value, min, max))
				{
					if (callback)
					{
						callback(*value, _catName);
					}
				}
			}
		};

		class GuiCheckbox : public GuiControl
		{
		public:
			bool* value;
			std::function<void(bool, std::string)> callback;

			void Render(std::string _catName) override
			{
				bool oldValue = *value;
				if (ImGui::Checkbox(label.c_str(), value))
				{
					if (callback)
					{
						callback(*value, _catName);
					}
				}
			}
		};

		class GuiComboBox : public GuiControl
		{
		public:
			bool* value = nullptr;
			int* current = nullptr;
			int* count = nullptr;
			void** items = nullptr;
			const char** itemsNames = nullptr;
			void* selected_item = nullptr;

			std::function<void(void*, std::string)> callback;

			void Render(std::string _catName) override
			{
				if (ImGui::Combo(label.c_str(), current, itemsNames, *count))
				{
					if (callback)
					{
						callback(items[*current], _catName);
					}
				}
			}
		};

		class GuiButton : public GuiControl
		{
		public:
			std::function<void(std::string)> callback;

			void Render(std::string _catName) override
			{
				if (ImGui::Button(label.c_str()))
				{
					if (callback)
					{
						callback(_catName);
					}
				}
			}
		};

		class GuiHierarchyButton : public GuiControl
		{
		public:
			std::function<void(Actor*, std::string)> callback;
			Actor* actor = nullptr;
			void Render(std::string _catName) override
			{
				if (ImGui::Button(label.c_str()))
				{
					if (callback)
					{
						callback(actor, _catName);
					}
				}
			}
		};

		struct Category
		{
			std::string name;
			std::vector<std::unique_ptr<GuiControl>> controls;
			bool isOpen = true; // Pour l'état du collapsing header
		};

		struct Window
		{
			std::string name;
			std::map<std::string, Category> categories;
			bool open = true;
			ImGuiWindowFlags flags = 0;
		};

		// Map des fenetres ImGui
		std::map<std::string, Window> windows;

		Category& GetOrCreateCategory(std::string _window, std::string _name);
		Window& GetOrCreateWindow(std::string _name);
	};
}
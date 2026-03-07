#include "ImGuiManager.hpp"
#include "Logger.hpp"

Debug::GuiManager::GuiManager(sf::RenderWindow* _render)
{
	render = _render;
	lastUpdate = 0.f;

	ImGui::SFML::Init(*render);
}

Debug::GuiManager::~GuiManager(void)
{
	ImGui::SFML::Shutdown();
}

void Debug::GuiManager::ProcessEvent(const sf::Event& _event)
{
	ImGui::SFML::ProcessEvent(*render, _event);
}

void Debug::GuiManager::Update(float _dt)
{
	lastUpdate += _dt;
}

void Debug::GuiManager::Render(void)
{
	if (lastUpdate >= frameRateTarget)
	{
		lastUpdate -= frameRateTarget;

		lastUpdate = (lastUpdate < 0.f) ? 0.01f : lastUpdate;
		//Logger::Log("Updating ImGui with dt: " + std::to_string(lastUpdate) + " seconds");
		// IMPORTANT: Appeler Update AVANT toute utilisation d'ImGui
		ImGui::SFML::Update(*render, sf::seconds(lastUpdate));

		// --- DEBUT DE LA FRAME IMGUI ---
		// Maintenant on peut appeler toutes les fonctions ImGui

		if (windows.size() > 0)
		{
			// Rendre les fenetres enregistr�es (nouveau)
			for (auto& [winName, win] : windows)
			{
				if (win.open && win.categories.size() > 0)
				{
					ImGui::Begin(win.name.c_str(), &win.open, win.flags);

					for (auto& [categoryName, category] : win.categories)
					{
						// Cr�er un collapsing header pour chaque cat�gorie
						if (ImGui::CollapsingHeader(categoryName.c_str(),
							category.isOpen ? ImGuiTreeNodeFlags_DefaultOpen : 0))
						{
							// Permet de decaler les contr�les � l'int�rieur du collapsing header
							ImGui::Indent(15.f);

							for (auto& control : category.controls)
							{
								int pushedColors = 0;

								if (control->colorType != ImGuiCol_COUNT)
								{
									ImGui::PushStyleColor(control->colorType, control->color);
									pushedColors++;
								}

								if (control->backColorType != ImGuiCol_COUNT)
								{
									ImGui::PushStyleColor(control->backColorType, control->backColor);
									pushedColors++;
								}

								control->Render(categoryName);

								if (pushedColors > 0)
								{
									ImGui::PopStyleColor(pushedColors);
								}
							}

							ImGui::Unindent(15.f);
							ImGui::Spacing();
						}
					}

					ImGui::End();
				}
			}
		}

		// --- FIN DE LA FRAME IMGUI ---

		// Render � la fin
	}
	ImGui::SFML::Render(*render);
}

void Debug::GuiManager::AddSliderFloat(std::string _win, std::string _cat, std::string _label, float* _value, float _min, float _max, std::function<void(float, std::string)> _call, sf::Color _color)
{
	Category& cat = GetOrCreateCategory(_win, _cat);

	auto slider = std::make_unique<GuiSliderFloat>();
	slider->label = _label;
	slider->value = _value;
	slider->min = _min;
	slider->max = _max;
	slider->callback = _call;

	slider->colorType = ImGuiCol_SliderGrab;
	slider->color = ImVec4(_color.r / 255.f, _color.g / 255.f, _color.b / 255.f, _color.a / 255.f);

	slider->backColorType = ImGuiCol_FrameBg;
	slider->backColor = ImVec4(_color.r / 255.f, _color.g / 255.f, _color.b / 255.f, _color.a / 255.f / 2.f);

	cat.controls.push_back(std::move(slider));
}

void Debug::GuiManager::AddSliderInt(std::string _win, std::string _cat, std::string _label, int* _value, int _min, int _max, std::function<void(int, std::string)> _call)
{
	Category& cat = GetOrCreateCategory(_win, _cat);

	auto slider = std::make_unique<GuiSliderInt>();
	slider->label = _label;
	slider->value = _value;
	slider->min = _min;
	slider->max = _max;
	slider->callback = _call;

	cat.controls.push_back(std::move(slider));
}

void Debug::GuiManager::AddCheckbox(std::string _win, std::string _cat, std::string _label, bool* _value, std::function<void(bool, std::string)> _call)
{
	Category& cat = GetOrCreateCategory(_win, _cat);

	auto checkbox = std::make_unique<GuiCheckbox>();
	checkbox->label = _label;
	checkbox->value = _value;
	checkbox->callback = _call;

	cat.controls.push_back(std::move(checkbox));
}

void Debug::GuiManager::AddComboBox(std::string _win, std::string _cat, std::string _label, void** _objectArray, const char** _namesArray, int _size, std::function<void(void*, std::string)> _call, int _currentIndex)
{
	Category& cat = GetOrCreateCategory(_win, _cat);

	auto combo = std::make_unique<GuiComboBox>();
	combo->label = _label;
	combo->currentIndex = _currentIndex;
	combo->callback = _call;

	for (int i = 0; i < _size; i++)
	{
		combo->items.push_back(_objectArray[i]);
		combo->itemNames.push_back(_namesArray[i] != nullptr ? _namesArray[i] : "");
	}
	combo->SyncRawNames();

	cat.controls.push_back(std::move(combo));
}

void Debug::GuiManager::AddButton(std::string _win, std::string _cat, std::string _label, std::function<void(std::string)> _call)
{
	Category& cat = GetOrCreateCategory(_win, _cat);

	auto button = std::make_unique<GuiButton>();
	button->label = _label;
	button->callback = _call;

	cat.controls.push_back(std::move(button));
}

void Debug::GuiManager::AddHierarchyButton(std::string _win, std::string _cat, std::string _label, Actor* _actor, std::function<void(Actor* _actor, std::string)> _call)
{
	Category& cat = GetOrCreateCategory(_win, _cat);

	auto button = std::make_unique<GuiHierarchyButton>();
	button->label = _label;
	button->callback = _call;
	button->actor = _actor;

	cat.controls.push_back(std::move(button));
}

void Debug::GuiManager::AddInputText(std::string _win, std::string _cat, std::string _label, char* _buffer, size_t _bufferSize, std::function<void(const char*)> _call)
{
	Category& cat = GetOrCreateCategory(_win, _cat);

	auto inputText = std::make_unique<GuiTextInput>();
	inputText->label = _label;
	inputText->buffer = _buffer;
	inputText->bufferSize = _bufferSize;
	inputText->callback = _call;

	cat.controls.push_back(std::move(inputText));
}

void Debug::GuiManager::RegisterWindow(std::string _name, bool _open, ImGuiWindowFlags flags)
{
	auto& win = GetOrCreateWindow(_name);
	win.open = _open;
	win.flags = flags;
}

void Debug::GuiManager::RemoveWindow(std::string _name)
{
	auto it = windows.find(_name);
	if (it == windows.end())
	{
		return;
	}

	it->second.categories.clear();
	windows.erase(it);
	Logger::Info("ImGui window removed: " + _name);
}

void Debug::GuiManager::ClearWindow(std::string _name)
{
	auto it = windows.find(_name);
	if (it == windows.end())
	{
		return;
	}

	it->second.categories.clear();
}

void Debug::GuiManager::RemoveCategory(std::string _winName, std::string _catName)
{
	auto winIt = windows.find(_winName);
	if (winIt == windows.end())
	{
		return;
	}

	winIt->second.categories.erase(_catName);
}

void Debug::GuiManager::Clear()
{
	windows.clear();
}

void Debug::GuiManager::BeginWindow(std::string windowName, bool* open, ImGuiWindowFlags flags)
{

}

void Debug::GuiManager::EndWindow()
{
}

bool Debug::GuiManager::CollapsingHeader(std::string label, ImGuiTreeNodeFlags flags)
{
	return ImGui::CollapsingHeader(label.c_str(), flags);
}

void Debug::GuiManager::Text(std::string text)
{
	ImGui::Text("%s", text.c_str());
}

bool Debug::GuiManager::InputText(std::string label, char* buffer, size_t bufferSize)
{
	return ImGui::InputText(label.c_str(), buffer, bufferSize);
}

bool Debug::GuiManager::Button(std::string label)
{
	return ImGui::Button(label.c_str());
}

void Debug::GuiManager::SameLine()
{
	ImGui::SameLine();
}

void Debug::GuiManager::Separator()
{
	ImGui::Separator();
}

bool Debug::GuiManager::Selectable(std::string label, bool selected)
{
	return ImGui::Selectable(label.c_str(), selected);
}

void Debug::GuiManager::PushID(int id)
{
	ImGui::PushID(id);
}

void Debug::GuiManager::PopID()
{
	ImGui::PopID();
}

void Debug::GuiManager::Indent(float indentWidth)
{
	if (indentWidth > 0.f)
	{
		ImGui::Indent(indentWidth);
	}
	else
	{
		ImGui::Indent();
	}
}

void Debug::GuiManager::Unindent(float indentWidth)
{
	if (indentWidth > 0.f)
	{
		ImGui::Unindent(indentWidth);
	}
	else
	{
		ImGui::Unindent();
	}
}

Debug::GuiManager::Category& Debug::GuiManager::GetOrCreateCategory(std::string _window, std::string _name)
{
	Window& win = GetOrCreateWindow(_window);
	Category& cat = win.categories[_name];
	cat.name = _name;
	return cat;
}

Debug::GuiManager::Window& Debug::GuiManager::GetOrCreateWindow(std::string _name)
{
	if (windows.find(_name) == windows.end())
	{
		windows[_name].name = _name;
	}

	return windows[_name];
}

#include "AutoUi.hpp"

#include "../../GameData.hpp"
#include "../SceneManager.hpp"

AutoUi::AutoUi(GameData* _data)
{
	data = _data;

	data->manager->currentScene->AddUi(this);
}

AutoUi::AutoUi(GameData* _data, std::string _name)
{
	data = _data;
	name = _name;

	data->manager->currentScene->AddUi(this);
}

AutoUi::~AutoUi(void)
{
	if (this)
	{
		data->manager->currentScene->RemoveUi(this);
	}
}

void AutoUi::Update(float _dt)
{
}

void AutoUi::Draw(sf::RenderTarget* _render)
{
}

sf::FloatRect AutoUi::GetBounds() const
{
	return sf::FloatRect(0.f, 0.f, 0.f, 0.f);
}
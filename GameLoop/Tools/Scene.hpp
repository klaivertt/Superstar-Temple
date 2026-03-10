#ifndef SCENE_H
#define SCENE_H

#include "../Common.hpp"

class SceneManager;
class Actor;
class AutoUi;
class GameData;

class Scene
{
public:
	// Array of all the actors in the scene
	Array<Actor*>* actors;
	Array<AutoUi*>* uiElements;
	// Number of actors created in the scene, used to give them unique names
	std::map<std::string, int> actorTypeCounters;
	int actorsCount = 0;

	GameData* data = nullptr;
	sf::View* camera = nullptr;
public:
	bool ordered = false;

	Scene(GameData* _data);
	~Scene(void);
	virtual void Load(void);
	virtual void Update(float _dt);
	void PollEvents(sf::Event& _event);
	virtual void Draw(sf::RenderTarget* _render);
	virtual void Destroy(void);

	void AddActor(Actor* _actor);
	void RemoveActor(Actor* _actor);

	void AddUi(AutoUi* _ui);
	void RemoveUi(AutoUi* _ui);

	template <typename T>
	void SpawnActor(void)
	{
		new T(data);
	}

	template <typename T>
	Array<T*> GetAllActorsOfClass(void)
	{
		Array<T*> result;

		for (int i = 0; i < actors->Size(); i++)
		{
			T* casted = dynamic_cast<T*>(actors->Get(i));
			if (casted)
			{
				result.Add(casted);
			}
		}

		return result;
	}
protected:
	void DrawWorld(sf::RenderTarget* _render);
	void DrawUi(sf::RenderTarget* _render);
	void EnsureActorsOrder();

	virtual void KeyPressed(sf::Keyboard::Key _key);
	virtual void KeyReleased(sf::Keyboard::Key _key);
	virtual void MousePressed(sf::Event::MouseButtonEvent _button);
	virtual void MouseReleased(sf::Event::MouseButtonEvent _button);
	virtual void MouseMoved(sf::Event::MouseMoveEvent _moved);
	virtual void TextEntered(sf::Event::TextEvent _text);

	void ChangeObjectSeeker(Actor*);
private:
	int GetNextActorID(const std::string& _className);
	void UpdateCollisions(float _dt);
};
#endif // !SCENE_H

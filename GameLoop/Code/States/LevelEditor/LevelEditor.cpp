#include "LevelEditor.hpp"

#include "Actors/LevelActor.hpp"
#include "Actors/LevelBoxCollider.hpp"
#include "Actors/Interactable/LevelLever.hpp"
#include "Actors/Interactable/Door.hpp"

LevelEditor::LevelEditor(GameData* _data) : Scene(_data)
{
    currentInspectedActor = nullptr;
    editorCamera = nullptr;
    grid = nullptr;
    selectionManager = nullptr;
    inspec = nullptr;
    isDragging = false;
}

LevelEditor::~LevelEditor()
{
    Destroy();
}

bool LevelEditor::IsActorStillValid(Actor* _actor)
{
    if (!_actor || !actors) 
    {
        return false;
    }

    for (int i = 0; i < actors->Size(); i++)
    {
        if (actors->Get(i) == _actor)
        {
            return true;
        }
    }

    return false;
}

void LevelEditor::CloseInspector()
{
    if (inspec)
    {
        delete inspec;
        inspec = nullptr;
    }
    currentInspectedActor = nullptr;
}

void LevelEditor::OpenInspector(Actor* _actor)
{
    if (!_actor || !IsActorStillValid(_actor))
    {
        CloseInspector();
        return;
    }

    if (currentInspectedActor != _actor)
    {
        CloseInspector();

        inspec = new Inspector(data, _actor);
        if (inspec)
        {
            currentInspectedActor = _actor;
        }
    }
}

void LevelEditor::StartDragging(const sf::Vector2f& _mousePos)
{
    if (!selectionManager) 
    {
        return;
    }
    if (selectionManager->GetSelectionCount() == 0) 
    {
        return;
    }

    isDragging = true;
    dragStartPos = _mousePos;

    actorsInitialPositions.clear();
    std::vector<Actor*>& selectedActors = selectionManager->GetSelectedActors();

    for (int i = 0; i < selectedActors.size(); i++)
    {
        Actor* actor = selectedActors[i];
        if (actor && IsActorStillValid(actor))
        {
            actorsInitialPositions.push_back(sf::Vector2f(actor->position.x, actor->position.y));
        }
    }
}


void LevelEditor::UpdateDragging(const sf::Vector2f& _mousePos)
{
    if (!isDragging)
    {
        return;
    }
    if (!selectionManager) 
    {
        return;
    }

    sf::Vector2f delta = _mousePos - dragStartPos;

    if (grid && grid->IsSnapEnabled())
    {
        float gridSize = grid->GetGridSize();
        delta.x = std::round(delta.x / gridSize) * gridSize;
        delta.y = std::round(delta.y / gridSize) * gridSize;
    }

    std::vector<Actor*>& selectedActors = selectionManager->GetSelectedActors();

    for (int i = 0; i < selectedActors.size() && i < actorsInitialPositions.size(); i++)
    {
        Actor* actor = selectedActors[i];
        if (actor && IsActorStillValid(actor))
        {
            Vec2 newPos;
            newPos.x = actorsInitialPositions[i].x + delta.x;
            newPos.y = actorsInitialPositions[i].y + delta.y;

            actor->position = newPos;

            if (B2_IS_NON_NULL(actor->body))
            {
                b2Body_SetTransform(actor->body,
                    b2Vec2(actor->position.x / METERS_TO_PIXELS,
                        -actor->position.y / METERS_TO_PIXELS),
                    b2Rot());
            }
        }
    }
}

void LevelEditor::EndDragging()
{
    isDragging = false;
    actorsInitialPositions.clear();
}

void LevelEditor::HandleMouseSelection(sf::Event& _event)
{
    if (_event.type == sf::Event::MouseButtonPressed && _event.mouseButton.button == sf::Mouse::Left)
    {
        isMousePressed = true;

        Actor* clickedActor = GetActorAtPosition(lastMouseWorldPos);

        if (clickedActor && IsActorStillValid(clickedActor))
        {
            bool additive = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);

            if (additive)
            {
                if (selectionManager)
                {
                    if (selectionManager->IsSelected(clickedActor))
                    {
                        selectionManager->RemoveFromSelection(clickedActor);
                    }
                    else
                    {
                        selectionManager->AddToSelection(clickedActor);
                    }
                }
            }
            else
            {
                if (selectionManager)
                {
                    if (!selectionManager->IsSelected(clickedActor))
                    {
                        selectionManager->Select(clickedActor, false);
                    }
                }
            }

            OpenInspector(clickedActor);
            UpdateHierarchy();

            if (selectionManager && selectionManager->GetSelectionCount() > 0)
            {
                StartDragging(lastMouseWorldPos);
            }
        }
        else
        {
            if (selectionManager)
            {
                selectionManager->StartSelectionRect(lastMouseWorldPos);
            }
        }
    }

    if (_event.type == sf::Event::MouseMoved && isMousePressed)
    {
        if (isDragging)
        {
            UpdateDragging(lastMouseWorldPos);
        }
        else if (selectionManager && selectionManager->IsDrawingRect())
        {
            selectionManager->UpdateSelectionRect(lastMouseWorldPos);
        }
    }

    if (_event.type == sf::Event::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Left)
    {
        isMousePressed = false;

        if (isDragging)
        {
            EndDragging();
        }
        else if (selectionManager && selectionManager->IsDrawingRect())
        {
            bool additive = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);

            std::vector<Actor*> allActors;
            if (actors)
            {
                for (int i = 0; i < actors->Size(); i++)
                {
                    Actor* actor = actors->Get(i);
                    if (actor && IsActorStillValid(actor))
                    {
                        allActors.push_back(actor);
                    }
                }
            }

            selectionManager->EndSelectionRect(allActors, additive);

            if (selectionManager->GetSelectionCount() > 0)
            {
                Actor* firstSelected = selectionManager->GetSelectedActors()[0];
                if (firstSelected && IsActorStillValid(firstSelected))
                {
                    OpenInspector(firstSelected);
                }
            }

            UpdateHierarchy();
        }
    }
}

void LevelEditor::UpdateHierarchy(void)
{
    if (!data) 
    {
        return;
    }
    if (!data->guiManager) 
    {
        return;
    }
    if (!actors) 
    {
        return;
    }

    data->guiManager->RemoveWindow("Hierarchy");

    for (int i = 0; i < actors->Size(); i++)
    {
        Actor* actor = actors->Get(i);
        if (!actor)
        {
            i = i;
        }
        else
        {
            bool selected = selectionManager ? selectionManager->IsSelected(actor) : false;
            std::string displayName = actor->name;
            if (selected)
            {
                displayName = "> " + displayName;
            }

            data->guiManager->AddHierarchyButton("Hierarchy", "Actors", displayName, actor,
                [this](Actor* _actor, std::string _n)
                {
                    if (!_actor) return;
                    if (!IsActorStillValid(_actor)) return;

                    bool additive = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
                    if (selectionManager)
                    {
                        selectionManager->Select(_actor, additive);
                    }

                    OpenInspector(_actor);
                    UpdateHierarchy();
                });
        }
    }
}

void LevelEditor::Load(void)
{
    editorCamera = new EditorCamera(data);
    grid = new EditorGrid();
    selectionManager = new SelectionManager();

    if (!editorCamera || !grid || !selectionManager)
    {
        if (data && data->log)
        {
            data->log->Error("Failed to initialize editor components");
        }
        return;
    }

    if (!data) 
    {
        return;
    }
    if (!data->guiManager)
    {
        return;
    }

    data->guiManager->AddCheckbox("Controls", "View", "Show Grid", &showGrid,
        [this](bool _checked, std::string _n)
        {
            if (grid) grid->SetVisible(_checked);
        });

    data->guiManager->AddCheckbox("Controls", "View", "Snap to Grid", &snapToGrid,
        [this](bool _checked, std::string _n)
        {
            if (grid) grid->SetSnapEnabled(_checked);
        });

    data->guiManager->AddButton("Controls", "Selection", "Deselect All",
        [this](std::string _n)
        {
            if (selectionManager)
            {
                selectionManager->DeselectAll();
            }
            UpdateHierarchy();
        });

    data->guiManager->AddSliderFloat("Controls", "View", "Grid Size", &grid->GetGridSize(), 4.f, 128.f,
        [this](float _value, std::string _n)
        {
            if (grid) grid->SetGridSize(_value);
		});
    data->guiManager->AddButton("Controls", "View", "Grid Size: 16",
        [this](std::string _n)
        {
            if (grid) grid->SetGridSize(16.0f);
        });

    data->guiManager->AddButton("Controls", "View", "Grid Size: 32",
        [this](std::string _n)
        {
            if (grid) grid->SetGridSize(32.0f);
        });

    data->guiManager->AddButton("Controls", "View", "Grid Size: 64",
        [this](std::string _n)
        {
            if (grid) grid->SetGridSize(64.0f);
        });

   data->guiManager->AddButton("Controls", "Editor", "Add Box Collider", [this](std::string _n)
		{
			Vec2 pos = Random(Vec2(0.f, 0.f), Vec2(SCREEN_W, SCREEN_H));

			LevelBoxCollider* actor = new LevelBoxCollider(data, pos);
			actor->name = "BoxCollider_" + std::to_string(actorsCount);

			data->guiManager->AddHierarchyButton("Hierarchy", "Actors", actor->name, actor, [this](Actor* _actor, std::string _n)
				{
					if (inspec)
					{
						delete inspec;
					}
					inspec = new Inspector(data, _actor);
				});
		});

	data->guiManager->AddButton("Controls", "Editor", "Add Box Lever", [this](std::string _n)
		{
			Vec2 pos = Random(Vec2(0.f, 0.f), Vec2(SCREEN_W, SCREEN_H));

			LevelLever* actor = new LevelLever(data, pos);
			actor->name = "Lever_" + std::to_string(actorsCount);

			data->guiManager->AddHierarchyButton("Hierarchy", "Actors", actor->name, actor, [this](Actor* _actor, std::string _n)
				{
					if (inspec)
					{
						delete inspec;
					}
					inspec = new Inspector(data, _actor);
				});
		});

	data->guiManager->AddButton("Controls", "Editor", "Add Door", [this](std::string _n)
		{
			Vec2 pos = Random(Vec2(0.f, 0.f), Vec2(SCREEN_W, SCREEN_H));
			LevelDoor* actor = new LevelDoor(data, pos);
			actor->name = "Door_" + std::to_string(actorsCount);
			data->guiManager->AddHierarchyButton("Hierarchy", "Actors", actor->name, actor, [this](Actor* _actor, std::string _n)
				{
					if (inspec)
					{
						delete inspec;
					}
					inspec = new Inspector(data, _actor);
				});
		});

    if (editorCamera)
    {
        camera = editorCamera->GetView();
    }

    Scene::Load();
}

void LevelEditor::Update(float _dt)
{
    if (editorCamera)
    {
        editorCamera->Update(_dt);
    }

    if (data && data->render && editorCamera)
    {
        sf::Vector2i mousePixel = sf::Mouse::getPosition(*data->render);
        lastMouseWorldPos = editorCamera->ScreenToWorld(mousePixel);
    }

    if (currentInspectedActor && !IsActorStillValid(currentInspectedActor))
    {
        CloseInspector();
        UpdateHierarchy();
    }

    Scene::Update(_dt);
}

void LevelEditor::Draw(sf::RenderTarget* _render)
{
    if (!_render)
    {
        return;
    }

    if (editorCamera && editorCamera->GetView())
    {
        _render->setView(*editorCamera->GetView());
    }

    if (grid && editorCamera && editorCamera->GetView())
    {
        grid->Draw(_render, *editorCamera->GetView());
    }

    if (selectionManager)
    {
        selectionManager->DrawSelection(_render);
        selectionManager->DrawSelectionRect(_render);
    }


    Scene::Draw(_render);
    _render->setView(_render->getDefaultView());
}

void LevelEditor::Destroy(void)
{
    CloseInspector();

    if (editorCamera)
    {
        delete editorCamera;
        editorCamera = nullptr;
    }

    if (grid)
    {
        delete grid;
        grid = nullptr;
    }

    if (selectionManager)
    {
        delete selectionManager;
        selectionManager = nullptr;
    }

    Scene::Destroy();
}

void LevelEditor::HandleInput(sf::Event& _event)
{
    if (_event.type == sf::Event::MouseWheelScrolled)
    {
        if (_event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel && editorCamera)
        {
            float delta = _event.mouseWheelScroll.delta;

            if (delta > 0)
            {
                editorCamera->Zoom(-0.1f);
            }
            else
            {
                editorCamera->Zoom(0.1f);
            }
        }
    }

    if (_event.type == sf::Event::KeyPressed)
    {
        if (_event.key.code == sf::Keyboard::G && grid)
        {
            grid->SetVisible(!grid->IsVisible());
        }

        if (_event.key.code == sf::Keyboard::Escape)
        {
            if (selectionManager)
            {
                selectionManager->DeselectAll();
            }

            CloseInspector();
            UpdateHierarchy();
        }
    }

    HandleMouseSelection(_event);
}

Actor* LevelEditor::GetActorAtPosition(const sf::Vector2f& _worldPos)
{
    if (!actors)
    {
        return nullptr;
    }

    for (int i = actors->Size() - 1; i >= 0; i--)
    {
        Actor* actor = actors->Get(i);
        if (actor)
        {
            sf::FloatRect bounds = actor->GetBounds();
            if (bounds.contains(_worldPos))
            {
                return actor;
            }
        }
    }

    return nullptr;
}
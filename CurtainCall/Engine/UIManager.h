#pragma once
#include "InputManager.h"
#include "UIObject.h"
#include "UIFunctorFactory.h"

class UIManager
    : public InputProcesser
{
public:
    UIManager();

public:
    template<typename UIType, typename MouseOverType, typename MouseClickType>
        requires std::is_base_of<UIObject, UIType>::value &&
    (std::is_base_of<UIMouseHoverFunctor, MouseOverType>::value || std::is_same<MouseOverType, void>::value) &&
        (std::is_base_of<UIMouseClickFunctor, MouseClickType>::value || std::is_same<MouseClickType, void>::value)
    std::shared_ptr<UIObject> CreateUI(const std::string& name);

    void Update(float deltaTime);

private:
    void ClearFocusedUI();
    void UpdateHoveredUI(float deltaTime);
    void UpdateClickedUI(float deltaTime);

private:
    std::vector<std::shared_ptr<UIObject>> m_UIObjects;
    std::vector<std::weak_ptr<UIObject>> m_HoveredUIObjects;
    std::vector<std::weak_ptr<UIObject>> m_ClickedUIObjects;

    // InputProcesser을(를) 통해 상속됨
    void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;
};

template<typename UIType, typename MouseHoverType, typename MouseClickType>
    requires std::is_base_of<UIObject, UIType>::value &&
(std::is_base_of<UIMouseHoverFunctor, MouseHoverType>::value || std::is_same<MouseHoverType, void>::value) &&
(std::is_base_of<UIMouseClickFunctor, MouseClickType>::value || std::is_same<MouseClickType, void>::value)
inline std::shared_ptr<UIObject> UIManager::CreateUI(const std::string& name)
{
    auto uiObject = std::make_shared<UIType>(name);

    if constexpr (!std::is_same<MouseHoverType, void>::value) {
        auto mouseHoverFunctor = UIFunctorFactory::GetFunctor<MouseHoverType>();
        uiObject->SetMouseHoverFunctor(mouseHoverFunctor);
    }
    else {
        uiObject->SetMouseHoverFunctor(nullptr);
    }

    if constexpr (!std::is_same<MouseClickType, void>::value) {
        auto mouseClickFunctor = UIFunctorFactory::GetFunctor<MouseClickType>();
        uiObject->SetMouseClickFunctor(mouseClickFunctor);
    }
    else {
        uiObject->SetMouseClickFunctor(nullptr);
    }

    m_UIObjects.push_back(uiObject);
    return uiObject;
}

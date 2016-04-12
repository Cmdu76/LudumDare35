#include "CameraComponent.hpp"
#include "World.hpp"

NCameraComponent::NCameraComponent() : NComponent()
{
    NWorld::getCameraManager().addCamera(this);
    mView = NWorld::getWindow().getView();
}

NCameraComponent::~NCameraComponent()
{
    NWorld::getCameraManager().removeCamera(this);
}

sf::View& NCameraComponent::getView()
{
    return mView;
}

void NCameraComponent::onMoved()
{
    mView.setCenter(NVector::NToSFML2F(getFinalPosition()));
}

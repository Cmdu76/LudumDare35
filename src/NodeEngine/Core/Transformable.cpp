#include "Transformable.hpp"

NTransformable::NTransformable()
{
}

NVector NTransformable::getPosition() const
{
    NVector v = NVector::SFML2FToN(mTransformable.getPosition());
    v.z = mZ;
    return v;
}

void NTransformable::setPosition(NVector const& position)
{
    setPosition(NVector::NToSFML2F(position),position.z);
}

void NTransformable::setPosition(sf::Vector2f const& position, float z)
{
    mTransformable.setPosition(position);
    mZ = z;
    onMoved();
}

void NTransformable::setPosition(float x, float y, float z)
{
    setPosition(NVector(x,y,z));
}

void NTransformable::move(NVector const& movement)
{
    setPosition(movement + getPosition());
}

void NTransformable::move(float x, float y, float z)
{
    move(NVector(x,y,z));
}

NVector NTransformable::getScale() const
{
    return NVector::SFML2FToN(mTransformable.getScale());
}

void NTransformable::setScale(NVector const& scale)
{
    setScale(NVector::NToSFML2F(scale));
}

void NTransformable::setScale(sf::Vector2f const& scale)
{
    mTransformable.setScale(scale);
    onScaled();
}

void NTransformable::setScale(float x, float y)
{
    setScale(NVector(x,y));
}

float NTransformable::getRotation() const
{
    return mTransformable.getRotation();
}

void NTransformable::setRotation(float rotation)
{
    mTransformable.setRotation(rotation);
    onRotated();
}

sf::Transform NTransformable::getTransform() const
{
    return mTransformable.getTransform();
}

void NTransformable::onMoved()
{
}

void NTransformable::onScaled()
{
}

void NTransformable::onRotated()
{
}

#ifndef SOFA_COMPONENTS_COMMON_FACTORY_H
#define SOFA_COMPONENTS_COMMON_FACTORY_H

#include <map>
#include <iostream>
#include <typeinfo>

#include "Sofa/Components/Common/config.h"

namespace Sofa
{

namespace Components
{

namespace Common
{

/// Decode the type's name to a more readable form if possible
std::string gettypename(const std::type_info& t);

template <class Object, class Argument>
class BaseCreator
{
public:
    virtual ~BaseCreator() { }
    virtual Object *createInstance(Argument arg) = 0;
    virtual const std::type_info& type() = 0;
};

template <typename TKey, class TObject, typename TArgument>
class Factory
{
public:
    typedef TKey Key;
    typedef TObject Object;
    typedef TArgument Argument;
    typedef BaseCreator<Object, Argument> Creator;

protected:
    std::multimap<Key, Creator*> registry;
    static Factory<Key, Object, Argument> factory;

public:
//  typedef InterfaceCreator<Object>* Creator;

    bool registerCreator(Key key, Creator* creator, bool multi=false)
    {
        if(!multi && this->registry.find(key) != this->registry.end())
            return false; // key used
        // commented by Sylvere F.
        // TODO : search where are the errors for gettypename
        // error : operator << not found for right value type std::string
        // std::cout << gettypename(typeid(Object)) << (multi?" template class ":" class ")
        //          << gettypename(creator->type()) << " registered as " << key << std::endl;
        this->registry.insert(std::pair<Key, Creator*>(key, creator));
        return true;
    }

    Object* createObject(Key key, Argument arg);

    static Factory<Key, Object, Argument>* getInstance();

    static Object* CreateObject(Key key, Argument arg)
    {
        return getInstance()->createObject(key, arg);
    }

};

/// Generic object creator. Can be specialized for custom objects creation
template<class Object, class Argument>
void create(Object*& obj, Argument arg)
{
    obj = new Object(arg);
}

template <class Factory, class RealObject>
class Creator : public Factory::Creator, public Factory::Key
{
public:
    typedef typename Factory::Object Object;
    typedef typename Factory::Argument Argument;
    typedef typename Factory::Key Key;
    Creator(Key key, bool multi=false)
        : Key(key)
    {
        Factory::getInstance()->registerCreator(key, this, multi);
    }
    Object *createInstance(Argument arg)
    {
        RealObject* instance = NULL;
        create(instance, arg);
        return instance;
    }
    const std::type_info& type()
    {
        return typeid(RealObject);
    }
};

template <class Factory, class RealObject>
class CreatorFn : public Factory::Creator, public Factory::Key
{
public:
    typedef typename Factory::Object Object;
    typedef typename Factory::Argument Argument;
    typedef typename Factory::Key Key;
    typedef void Fn(RealObject*& obj, Argument arg);
    Fn* constructor;

    CreatorFn(Key key, Fn* constructor, bool multi=false)
        : Key(key), constructor(constructor)
    {
        Factory::getInstance()->registerCreator(key, this, multi);
    }

    Object *createInstance(Argument arg)
    {
        RealObject* instance = NULL;
        (*constructor)(instance, arg);
        return instance;
    }
};

} // namespace Common

} // namespace Components

} // namespace Sofa

#endif

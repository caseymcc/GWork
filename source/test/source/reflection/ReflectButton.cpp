/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Util/ControlReflect.h>
#include <ponder/classget.hpp>
#include <ponder/class.hpp>
#include <ponder/pondertype.hpp>
#include <ponder/uses/runtime.hpp>

using namespace Gwk;

class ReflectButton : public TestUnit
{
public:

    GWK_CONTROL(ReflectButton, TestUnit);
    
    ~ReflectButton();

    void onButtonA(Event::Info info)
    {
        OutputToLog("Button Pressed (using 'OnPress' event)");
    }

    void OnToggle(Controls::Base* control)
    {
        OutputToLog("Button Toggled (using 'OnToggle' event)");
    }

    void OnToggleOn(Controls::Base* control)
    {
        OutputToLog("Button Toggled ON (using 'OnToggleOn' event)");
    }

    void OnToggleOff(Controls::Base* control)
    {
        OutputToLog("Button Toggled Off (using 'OnToggleOff' event)");
    }

    ponder::UserObject m_buttonA;
};

static void declare()
{
    ponder::Class::declare<ReflectButton>()
        .base<Controls::Base>();
}

PONDER_AUTO_TYPE(ReflectButton, &declare);

GWK_CONTROL_CONSTRUCTOR(ReflectButton)
{
    using namespace ponder;
    
    const ponder::Class& metaclass = ponder::classByType<Controls::Button>();
    runtime::ObjectFactory fact(metaclass);

    // Normal button
    m_buttonA = fact.create(static_cast<Controls::Base*>(this));
    m_buttonA.set("text", "Hello world!");
    m_buttonA.get("onPress").to<Event::Listener*>()->Add(this, &ReflectButton::onButtonA);
    
    // Unicode test
    auto buttonB = fact.create(static_cast<Controls::Base*>(this));
    buttonB.set("text", Utility::Narrow(
                L"\u0417\u0430\u043C\u0435\u0436\u043D\u0430\u044F \u043C\u043E\u0432\u0430"));
    
    // Gwk::Align::PlaceBelow(buttonB, buttonA, 10);
    runtime::callStatic(classByType<Align>().function("placeBelow"), buttonB, m_buttonA, 20);
    
    // Tooltip Button
    auto buttonC = fact.create(static_cast<Controls::Base*>(this));
    buttonC.set("text", "With tooltip");
    runtime::call(metaclass.function("setTooltip"), buttonC, "This is a tooltip!");
    
    // Gwk::Align::PlaceBelow(buttonB, buttonA, 10);
    runtime::callStatic(classByType<Align>().function("placeBelow"), buttonC, buttonB, 30);
}

ReflectButton::~ReflectButton()
{
    ponder::runtime::destroy(m_buttonA);
}

DECLARE_TEST(ReflectButton);
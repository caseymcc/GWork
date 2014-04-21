
#pragma once

#include "Gwen/Gwen.h"
#include "ControlFactory.h"

namespace Gwen
{
    namespace ControlFactory
    {
        class Base;
        typedef std::list<ControlFactory::Base*> List;

        List&                 GetList();
        ControlFactory::Base* Find(const Gwen::String& name);
        Controls::Base*       Clone(Controls::Base* pEnt, ControlFactory::Base* pFactory);
        

        class Property
        {
        public:

            typedef std::list<Property*> List;

            virtual Gwen::String Name() = 0;
            virtual Gwen::String Description() = 0;

            virtual Gwen::String GetValue(Gwen::Controls::Base* ctrl) = 0;
            virtual void         SetValue(Gwen::Controls::Base* ctrl, const Gwen::String& str) = 0;

            virtual int OptionNum()
            {
                return 0;
            }

            virtual Gwen::String OptionGet(int i)
            {
                return "";
            }

            virtual int NumCount()
            {
                return 0;
            }

            virtual Gwen::String NumName(int i)
            {
                return "unknown";
            }

            virtual float NumGet(Gwen::Controls::Base* ctrl, int i)
            {
                return 0.0f;
            }

            virtual void NumSet(Gwen::Controls::Base* ctrl, int i, float f)
            {
            }

            inline void NumSet(Gwen::Controls::Base* ctrl, const Gwen::String& str, float f)
            {
                for (int i = 0; i < NumCount(); i++)
                {
                    if (NumName(i) == str)
                        NumSet(ctrl, i, f);
                }
            }

        };


        class PropertyBool : public Property
        {
        public:

            static const Gwen::String True;
            static const Gwen::String False;

            virtual int OptionNum()
            {
                return 2;
            }

            virtual Gwen::String OptionGet(int i)
            {
                if (i == 0)
                    return False;

                return True;
            }

        };


        /// Base class for ControlFactorys.
        //
        class Base
        {
        public:

            Base();

            virtual Gwen::String Name() = 0;
            virtual Gwen::String BaseName() = 0;

            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent) = 0;

            Base* GetBaseFactory();
            void  AddProperty(Property* pProp);

            Property* GetProperty(const Gwen::String& name);
            void      SetControlValue(Gwen::Controls::Base* ctrl, const Gwen::String& name,
                                      const Gwen::String& str);

            const Property::List& Properties()
            {
                return m_Properties;
            }

            /// Called when the control is drag and dropped onto the parent, even
            /// when just moving in the designer
            virtual void AddChild(Gwen::Controls::Base* ctrl,
                                  Gwen::Controls::Base* child,
                                  const Gwen::Point& pos);

            /// Called when creating the control - param might be empty
            virtual void AddChild(Gwen::Controls::Base* ctrl,
                                  Gwen::Controls::Base* child,
                                  int iPage = 0);

            /// Called when a child is clicked on in an editor
            virtual bool ChildTouched(Gwen::Controls::Base* ctrl,
                                      Gwen::Controls::Base* pChildControl)
            {
                return false;
            }

            //
            /// If a control is parented to a control with multiple sections, or
            /// pages these functions are used to store which page they're on - on the
            /// child control
            //
            virtual void SetParentPage(Gwen::Controls::Base* ctrl, int i);
            virtual int  GetParentPage(Gwen::Controls::Base* ctrl);

        protected:

            Property::List m_Properties;
        };


    } // ControlFactory

} // Gwen


/// Declare a ControlFactory constructor.
#define GWEN_CONTROL_FACTORY_CONSTRUCTOR(CLASS_NAME, PARENT_CLASS) \
    typedef PARENT_CLASS ParentClass; \
    typedef CLASS_NAME ThisClass; \
    CLASS_NAME() : ParentClass()

/// Instance a ControlFactory.
/// @param FACTORY_CLASS - The name of the factory.
#define GWEN_CONTROL_FACTORY(FACTORY_CLASS) \
    void GWENCONTROLFACTORY##FACTORY_CLASS() \
    { \
        new FACTORY_CLASS(); \
    }

/// Declare a ControlFactory factory so that it can be called.
#define DECLARE_GWEN_CONTROL_FACTORY(FACTORY_CLASS) \
    extern void GWENCONTROLFACTORY##FACTORY_CLASS(); \
    GWENCONTROLFACTORY##FACTORY_CLASS();

#define GWEN_CONTROL_FACTORY_PROPERTY(PROP_NAME, DESCRIPTION) \
public: \
    Gwen::String Name()         { return #PROP_NAME; } \
    Gwen::String Description()  { return DESCRIPTION; }



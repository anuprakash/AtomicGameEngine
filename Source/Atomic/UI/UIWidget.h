
#pragma once

#include <ThirdParty/TurboBadger/tb_widgets.h>
#include <ThirdParty/TurboBadger/tb_widgets_common.h>

#include "../Core/Object.h"

#include "UIPreferredSize.h"
#include "UIDragObject.h"

namespace Atomic
{

/// Defines widget visibility, used with UIWidget::SetVisibility.
enum UI_WIDGET_VISIBILITY
{
    /// Visible (default)
    UI_WIDGET_VISIBILITY_VISIBLE = tb:: WIDGET_VISIBILITY_VISIBLE,
    /// Invisible, but layouted. Interaction disabled.
    UI_WIDGET_VISIBILITY_INVISIBLE = tb::WIDGET_VISIBILITY_INVISIBLE,
    /// Invisible and no layout. Interaction disabled.
    UI_WIDGET_VISIBILITY_GONE = tb::WIDGET_VISIBILITY_GONE
};

/// TBWidget gravity (may be combined).
/// Gravity gives hints about positioning and sizing preferences.
enum UI_GRAVITY {

    UI_GRAVITY_NONE = tb::WIDGET_GRAVITY_NONE,
    UI_GRAVITY_LEFT = tb::WIDGET_GRAVITY_LEFT,
    UI_GRAVITY_RIGHT = tb::WIDGET_GRAVITY_RIGHT,
    UI_GRAVITY_TOP = tb::WIDGET_GRAVITY_TOP,
    UI_GRAVITY_BOTTOM = tb::WIDGET_GRAVITY_BOTTOM,

    UI_GRAVITY_LEFT_RIGHT	= tb::WIDGET_GRAVITY_LEFT_RIGHT,
    UI_GRAVITY_TOP_BOTTOM	= tb::WIDGET_GRAVITY_TOP_BOTTOM,
    UI_GRAVITY_ALL			= tb::WIDGET_GRAVITY_ALL,
    UI_GRAVITY_DEFAULT		= tb::WIDGET_GRAVITY_DEFAULT
};

enum UI_EVENT_TYPE {
    /** Click event is what should be used to trig actions in almost all cases.

        It is invoked on a widget after POINTER_UP if the pointer is still inside
        its hit area. It can also be invoked by keyboard on some clickable widgets
        (see TBWidget::SetClickByKey).

        If panning of scrollable widgets start while the pointer is down, CLICK
        won't be invoked when releasing the pointer (since that should stop panning). */
    UI_EVENT_TYPE_CLICK = tb::EVENT_TYPE_CLICK,

    /** Long click event is sent when the pointer has been down for some time
        without moving much.

        It is invoked on a widget that has enabled it (TBWidget::SetWantLongClick
        If this event isn't handled, the widget will invoke a CONTEXT_MENU event.
        If any of those are handled, the CLICK event that would normally be
        invoked after the pending POINTER_UP will be suppressed. */
    UI_EVENT_TYPE_LONG_CLICK = tb::EVENT_TYPE_LONG_CLICK,
    UI_EVENT_TYPE_POINTER_DOWN = tb::EVENT_TYPE_POINTER_DOWN,
    UI_EVENT_TYPE_POINTER_UP = tb::EVENT_TYPE_POINTER_UP,
    UI_EVENT_TYPE_POINTER_MOVE = tb::EVENT_TYPE_POINTER_MOVE,
    UI_EVENT_TYPE_RIGHT_POINTER_DOWN = tb::EVENT_TYPE_RIGHT_POINTER_DOWN,
    UI_EVENT_TYPE_RIGHT_POINTER_UP = tb::EVENT_TYPE_RIGHT_POINTER_UP,
    UI_EVENT_TYPE_WHEEL = tb::EVENT_TYPE_WHEEL,

    /** Invoked after changing text in a TBTextField, changing selected item
        in a TBSelectList etc. Invoking this event trigs synchronization with
        connected TBWidgetValue and other widgets connected to it. */
    UI_EVENT_TYPE_CHANGED = tb::EVENT_TYPE_CHANGED,
    UI_EVENT_TYPE_KEY_DOWN = tb::EVENT_TYPE_KEY_DOWN,
    UI_EVENT_TYPE_KEY_UP = tb::EVENT_TYPE_KEY_UP,

    /** Invoked by the platform when a standard keyboard shortcut is pressed.
        It's called before InvokeKeyDown (EVENT_TYPE_KEY_DOWN) and if the event
        is handled (returns true), the KeyDown is canceled.
        The ref_id will be set to one of the following:
            "cut", "copy", "paste", "selectall", "undo", "redo", "new", "open", "save". */
    UI_EVENT_TYPE_SHORTCUT = tb::EVENT_TYPE_SHORTCUT,

    /** Invoked when a context menu should be opened at the event x and y coordinates.
        It may be invoked automatically for a widget on long click, if nothing handles
        the long click event. */
    UI_EVENT_TYPE_CONTEXT_MENU = tb::EVENT_TYPE_CONTEXT_MENU,

    /** Invoked by the platform when one or multiple files has been dropped on
        the widget. The event is guaranteed to be a TBWidgetEventFileDrop. */
    UI_EVENT_TYPE_FILE_DROP = tb::EVENT_TYPE_FILE_DROP,

    /** Invoked by the platform when a tab container's tab changed */
    UI_EVENT_TYPE_TAB_CHANGED = tb::EVENT_TYPE_TAB_CHANGED,

    /** Custom event. Not used internally. ref_id may be used for additional type info. */
    UI_EVENT_TYPE_CUSTOM = tb::EVENT_TYPE_CUSTOM
};

/** Defines widget z level relative to another widget, used with TBWidget::AddChildRelative. */
enum UI_WIDGET_Z_REL {
    UI_WIDGET_Z_REL_BEFORE = tb::WIDGET_Z_REL_BEFORE,		///< Before the reference widget (visually behind reference).
    UI_WIDGET_Z_REL_AFTER = tb::WIDGET_Z_REL_AFTER			///< After the reference widget (visually above reference).
};

class UIView;
class UILayoutParams;
class UIFontDescription;

/// Wraps a TurboBadger widget in our Object model
class UIWidget : public Object, public tb::TBWidgetDelegate
{
    friend class UI;

    OBJECT(UIWidget)

    public:

        UIWidget(Context* context, bool createWidget = true);
    virtual ~UIWidget();

    bool Load(const String& filename);

    const String& GetId();

    UIWidget* GetParent();
    UIWidget* GetContentRoot();
    IntRect GetRect();

    UIPreferredSize* GetPreferredSize();
    String GetText();

    void SetRect(IntRect r);
    void SetSize(int width, int height);
    void SetPosition(int x, int y);
    void SetText(const String& text);
    void SetSkinBg(const String& id);
    void SetLayoutParams(UILayoutParams* params);
    void SetFontDescription(UIFontDescription* fd);

    void RemoveChild(UIWidget* child, bool cleanup = true);

    void DeleteAllChildren();

    // String ID
    virtual void SetId(const String& id);

    void Center();
    void SetGravity(UI_GRAVITY gravity);

    void SetValue(double value);
    double GetValue();

    void SetFocus();

    void SetState(/*WIDGET_STATE*/ unsigned state, bool on);
    bool GetState(/*WIDGET_STATE*/ unsigned state);

    void SetVisibility(UI_WIDGET_VISIBILITY visibility);
    UI_WIDGET_VISIBILITY GetVisibility();

    void SetStateRaw(/*WIDGET_STATE*/ unsigned state);
    /*WIDGET_STATE*/ unsigned GetStateRaw();

    void Invalidate();
    void Die();

    void SetDragObject(UIDragObject* object) { dragObject_ = object; }
    UIDragObject* GetDragObject() { return dragObject_; }

    UIWidget* GetFirstChild();
    UIWidget* GetNext();

    bool IsAncestorOf(UIWidget* widget);

    void SetIsFocusable(bool value);

    // get this or child widget with id
    UIWidget* GetWidget(const String& id);

    UIView* GetView();

    virtual void AddChild(UIWidget* child);

    /// Add the child to this widget. See AddChild for adding a child to the top or bottom.
    /// This takes a relative Z and insert the child before or after the given reference widget.*/
    void AddChildRelative(UIWidget* child, UI_WIDGET_Z_REL z, UIWidget* reference);


    tb::TBWidget* GetInternalWidget() { return widget_; }

protected:

    void ConvertEvent(UIWidget* handler, UIWidget* target, const tb::TBWidgetEvent &ev, VariantMap& data);

    void SetWidget(tb::TBWidget* widget);

    virtual bool OnEvent(const tb::TBWidgetEvent &ev);
    virtual void OnDelete();

    String id_;
    tb::TBWidget* widget_;

    SharedPtr<UIPreferredSize> preferredSize_;

    SharedPtr<UIDragObject> dragObject_;

};

}

#ifndef UIVERSE_COMPONENT_H
#define UIVERSE_COMPONENT_H

#include "scene/gui/control.h"
#include "core/dictionary.h"
#include "core/string_name.h"

class UiverseComponent : public Control {
    GDCLASS(UiverseComponent, Control);

protected:
    static void _bind_methods();

private:
    String component_id;
    Dictionary style_overrides;
    bool interactable;
    String html_content;
    String css_content;

    void _update_component();
    void _apply_styles();
    void _setup_interactivity();

public:
    UiverseComponent();
    ~UiverseComponent();

    // Component management
    void set_component_id(const String& p_id);
    String get_component_id() const;

    void set_html_content(const String& p_content);
    String get_html_content() const;

    void set_css_content(const String& p_content);
    String get_css_content() const;

    // Style management
    void set_style_overrides(const Dictionary& p_styles);
    Dictionary get_style_overrides() const;

    void add_style_override(const String& p_property, const Variant& p_value);
    void remove_style_override(const String& p_property);

    // Interactivity
    void set_interactable(bool p_interactable);
    bool is_interactable() const;

    // Component preview
    void update_preview();
    Error load_from_uiverse(const String& p_component_id);

    // Event handling
    virtual void _gui_input(const Ref<InputEvent>& p_event) override;
    virtual void _notification(int p_what) override;

    // Size management
    virtual Size2 get_minimum_size() const override;

    // Signals
    static void _register_signals();
};

#endif // UIVERSE_COMPONENT_H 
#include "uiverse_component.h"
#include "core/os/os.h"
#include "scene/main/viewport.h"
#include "scene/gui/label.h"
#include "scene/resources/style_box.h"

void UiverseComponent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_component_id", "id"), &UiverseComponent::set_component_id);
    ClassDB::bind_method(D_METHOD("get_component_id"), &UiverseComponent::get_component_id);

    ClassDB::bind_method(D_METHOD("set_html_content", "content"), &UiverseComponent::set_html_content);
    ClassDB::bind_method(D_METHOD("get_html_content"), &UiverseComponent::get_html_content);

    ClassDB::bind_method(D_METHOD("set_css_content", "content"), &UiverseComponent::set_css_content);
    ClassDB::bind_method(D_METHOD("get_css_content"), &UiverseComponent::get_css_content);

    ClassDB::bind_method(D_METHOD("set_style_overrides", "styles"), &UiverseComponent::set_style_overrides);
    ClassDB::bind_method(D_METHOD("get_style_overrides"), &UiverseComponent::get_style_overrides);

    ClassDB::bind_method(D_METHOD("add_style_override", "property", "value"), &UiverseComponent::add_style_override);
    ClassDB::bind_method(D_METHOD("remove_style_override", "property"), &UiverseComponent::remove_style_override);

    ClassDB::bind_method(D_METHOD("set_interactable", "interactable"), &UiverseComponent::set_interactable);
    ClassDB::bind_method(D_METHOD("is_interactable"), &UiverseComponent::is_interactable);

    ClassDB::bind_method(D_METHOD("update_preview"), &UiverseComponent::update_preview);
    ClassDB::bind_method(D_METHOD("load_from_uiverse", "component_id"), &UiverseComponent::load_from_uiverse);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "component_id"), "set_component_id", "get_component_id");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "html_content"), "set_html_content", "get_html_content");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "css_content"), "set_css_content", "get_css_content");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "style_overrides"), "set_style_overrides", "get_style_overrides");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "interactable"), "set_interactable", "is_interactable");

    _register_signals();
}

void UiverseComponent::_register_signals() {
    ADD_SIGNAL(MethodInfo("component_loaded"));
    ADD_SIGNAL(MethodInfo("component_error"));
    ADD_SIGNAL(MethodInfo("interaction_event", PropertyInfo(Variant::STRING, "event_type")));
}

UiverseComponent::UiverseComponent() : interactable(true) {
    set_clip_contents(true);
}

UiverseComponent::~UiverseComponent() {
}

void UiverseComponent::set_component_id(const String& p_id) {
    if (component_id != p_id) {
        component_id = p_id;
        _update_component();
    }
}

String UiverseComponent::get_component_id() const {
    return component_id;
}

void UiverseComponent::set_html_content(const String& p_content) {
    if (html_content != p_content) {
        html_content = p_content;
        _update_component();
    }
}

String UiverseComponent::get_html_content() const {
    return html_content;
}

void UiverseComponent::set_css_content(const String& p_content) {
    if (css_content != p_content) {
        css_content = p_content;
        _update_component();
    }
}

String UiverseComponent::get_css_content() const {
    return css_content;
}

void UiverseComponent::set_style_overrides(const Dictionary& p_styles) {
    style_overrides = p_styles;
    _apply_styles();
}

Dictionary UiverseComponent::get_style_overrides() const {
    return style_overrides;
}

void UiverseComponent::add_style_override(const String& p_property, const Variant& p_value) {
    style_overrides[p_property] = p_value;
    _apply_styles();
}

void UiverseComponent::remove_style_override(const String& p_property) {
    if (style_overrides.has(p_property)) {
        style_overrides.erase(p_property);
        _apply_styles();
    }
}

void UiverseComponent::set_interactable(bool p_interactable) {
    if (interactable != p_interactable) {
        interactable = p_interactable;
        _setup_interactivity();
    }
}

bool UiverseComponent::is_interactable() const {
    return interactable;
}

void UiverseComponent::_update_component() {
    // Clear existing content
    for (int i = 0; i < get_child_count(); i++) {
        Node* child = get_child(i);
        remove_child(child);
        memdelete(child);
    }

    if (component_id.empty() && html_content.empty()) {
        // Create placeholder content
        Label* placeholder = memnew(Label);
        placeholder->set_text("No component loaded");
        placeholder->set_align(Label::ALIGN_CENTER);
        placeholder->set_valign(Label::VALIGN_CENTER);
        add_child(placeholder);
        return;
    }

    // TODO: Implement HTML/CSS rendering using a WebView or custom renderer
    // For now, we'll just display the component ID or content
    Label* content = memnew(Label);
    if (!component_id.empty()) {
        content->set_text("Uiverse Component: " + component_id);
    } else {
        content->set_text(html_content);
    }
    content->set_align(Label::ALIGN_CENTER);
    content->set_valign(Label::VALIGN_CENTER);
    add_child(content);

    _apply_styles();
    _setup_interactivity();
    update();
}

void UiverseComponent::_apply_styles() {
    // Apply custom styles from style_overrides
    // TODO: Implement proper CSS styling
    Ref<StyleBoxFlat> style = memnew(StyleBoxFlat);
    
    if (style_overrides.has("background_color")) {
        style->set_bg_color(style_overrides["background_color"]);
    } else {
        style->set_bg_color(Color(0.2, 0.2, 0.2));
    }

    if (style_overrides.has("border_color")) {
        style->set_border_color(style_overrides["border_color"]);
    }

    if (style_overrides.has("border_width")) {
        int width = style_overrides["border_width"];
        style->set_border_width_all(width);
    }

    if (style_overrides.has("corner_radius")) {
        int radius = style_overrides["corner_radius"];
        style->set_corner_radius_all(radius);
    }

    add_style_override("panel", style);
}

void UiverseComponent::_setup_interactivity() {
    set_process_input(interactable);
    // TODO: Implement proper event handling for HTML elements
}

void UiverseComponent::_gui_input(const Ref<InputEvent>& p_event) {
    if (!interactable) return;

    // TODO: Implement proper event handling
    // For now, we'll just emit a signal for mouse clicks
    if (p_event->is_class("InputEventMouseButton")) {
        emit_signal("interaction_event", "click");
    }
}

void UiverseComponent::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_READY: {
            _update_component();
        } break;

        case NOTIFICATION_RESIZED: {
            _update_component();
        } break;
    }
}

Size2 UiverseComponent::get_minimum_size() const {
    return Size2(100, 100); // Default minimum size
}

Error UiverseComponent::load_from_uiverse(const String& p_component_id) {
    // TODO: Implement actual loading from uiverse.io API
    // For now, we'll just set the component ID
    set_component_id(p_component_id);
    emit_signal("component_loaded");
    return OK;
}

void UiverseComponent::update_preview() {
    _update_component();
} 
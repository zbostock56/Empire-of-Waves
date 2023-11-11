#include <invasion.h>

void init_invasion_bar() {
  invasion_buffer[0] = '\0';

  vec2 top = { 0.0, 1.0 };
  init_menu(
    top, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    invasion_buffer, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0, // height
    PIVOT_TOP, // pivot
    T_CENTER, // text_anchor
    get_ui_component_by_ID(INVASION_BAR) // dest
  );
}

void open_invasion_bar() {
  UI_COMPONENT *invasion_bar = get_ui_component_by_ID(INVASION_BAR);
  invasion_bar->enabled = 1;
  snprintf(invasion_buffer, INVASION_BUFF_SIZE,
           "Your home island is being invaded!");
}

void close_invasion_bar() {
  UI_COMPONENT *invasion_bar = get_ui_component_by_ID(INVASION_BAR);
  invasion_bar->enabled = 0;
  invasion_bar->text[0] = '\0';
}


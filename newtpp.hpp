#pragma once
#include <algorithm>
#include <array>
#include <concepts>
#include <newt.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

namespace newt {

/*
 *    CONCEPTS AND UTILITYS
 */

class component;
template <class T>
concept generic_component = requires {
                              requires std::derived_from<T, component> or std::same_as<T, component>;
                            };

template <class T>
concept component_collection = requires(T collection) {
                                 requires std::derived_from<typename std::remove_cvref_t<decltype(collection.get_components())>::value_type, component>;
                               };

template <class T>
concept component_or_collection = requires {
                                    requires generic_component<T> or component_collection<T>;
                                  };

template <typename type>
class conditional_ownership_ptr {
  public:
  using value_type = type;
  using value_type_ptr = type*;
  using deleter_ptr = void (*)(value_type_ptr);

  private:
  value_type_ptr data { nullptr };
  deleter_ptr deleter { &operator delete };

  public:
  static void no_delete(value_type_ptr /*unused*/) { }
  conditional_ownership_ptr(value_type_ptr pointer, deleter_ptr deleter_fun)
      : data { pointer }
      , deleter(deleter_fun)
  {
  }

  conditional_ownership_ptr(conditional_ownership_ptr& other) noexcept
      : data(other.data)
      , deleter(other.deleter)
  {
    other.deleter = no_delete;
  }

  conditional_ownership_ptr(conditional_ownership_ptr&& other) noexcept
      : data(other.data)
      , deleter(other.deleter)
  {
    other.deleter = no_delete;
  }

  /*
   * Allowing a copy wold mean that copying from a pointer that
   * is currently an owner would result in two owning pointers
   */
  conditional_ownership_ptr& operator=(const conditional_ownership_ptr&) = delete;
  conditional_ownership_ptr& operator=(conditional_ownership_ptr&& other) noexcept
  {
    deleter(data);

    data = other.data;
    deleter = other.deleter;
    other.deleter = conditional_ownership_ptr::no_delete;

    return *this;
  }

  bool operator<=>(const conditional_ownership_ptr&) const = default;

  [[nodiscard]] value_type_ptr get() const
  {
    return data;
  }

  [[nodiscard]] value_type_ptr own()
  {
    deleter = no_delete;
    return data;
  }

  ~conditional_ownership_ptr() noexcept
  {
    deleter(data);
  }
};

/*
 *      SIZING AND POSITION
 */

template <std::integral number>
struct size_base {
  number width { 0 };
  number height { 0 };

  size_base() = default;

  // NOLINTNEXTLINE
  size_base(const number WIDHT, const number HEIGHT)
      : width(WIDHT)
      , height(HEIGHT)
  {
  }

  template <std::integral other_number>
  explicit size_base(const size_base<other_number> OTHER)
      : width(static_cast<number>(OTHER.width))
      , height(static_cast<number>(OTHER.height))
  {
  }

  size_base operator+(const number SCALAR) const
  {
    return size_base { width + SCALAR, height + SCALAR };
  }

  size_base operator-(const number SCALAR) const
  {
    return size_base { width - SCALAR, height - SCALAR };
  }

  size_base operator*(const number SCALAR) const
  {
    return size_base { width * SCALAR, height * SCALAR };
  }

  size_base operator/(const number SCALAR) const
  {
    return size_base { width / SCALAR, height / SCALAR };
  }

  size_base operator+(const size_base OTHER) const
  {
    return size_base { width + OTHER.width, height + OTHER.height };
  }

  size_base operator-(const size_base OTHER) const
  {
    return size_base { width - OTHER.width, height - OTHER.height };
  }

  size_base operator*(const size_base OTHER) const
  {
    return size_base { width * OTHER.width, height * OTHER.height };
  }

  size_base operator/(const size_base OTHER) const
  {
    return size_base { width / OTHER.width, height / OTHER.height };
  }
};

using size = size_base<int>;
using usize = size_base<unsigned int>;

struct position : public size_base<int> {
  int& left { width };
  int& top { height };

  position() = default;

  position(const int LEFT, const int TOP)
      : size_base(LEFT, TOP)
  {
  }
};

/*
 *     COLOR MANAGMENT
 */

namespace theme {
  struct colors {
    std::string_view root_fg;
    std::string_view root_bg;
    std::string_view border_fg;
    std::string_view border_bg;
    std::string_view window_fg;
    std::string_view window_bg;
    std::string_view shadow_fg;
    std::string_view shadow_bg;
    std::string_view title_fg;
    std::string_view title_bg;
    std::string_view button_fg;
    std::string_view button_bg;
    std::string_view act_button_fg;
    std::string_view act_button_bg;
    std::string_view checkbox_fg;
    std::string_view checkbox_bg;
    std::string_view act_checkbox_fg;
    std::string_view act_checkbox_bg;
    std::string_view entry_fg;
    std::string_view entry_bg;
    std::string_view label_fg;
    std::string_view label_bg;
    std::string_view listbox_fg;
    std::string_view listbox_bg;
    std::string_view act_listbox_fg;
    std::string_view act_listbox_bg;
    std::string_view textbox_fg;
    std::string_view textbox_bg;
    std::string_view act_textbox_fg;
    std::string_view act_textbox_bg;
    std::string_view help_line_fg;
    std::string_view help_line_bg;
    std::string_view root_text_fg;
    std::string_view root_text_bg;
    std::string_view empty_scale;
    std::string_view full_scale;
    std::string_view disabled_entry_fg;
    std::string_view disabled_entry_bg;
    std::string_view compact_button_fg;
    std::string_view compact_button_bg;
    std::string_view act_sel_listbox_fg;
    std::string_view act_sel_listbox_bg;
    std::string_view sel_listbox_fg;
    std::string_view sel_listbox_bg;

    constexpr explicit operator newtColors() const
    {
      return {
        // newt takes char* to construct themes, so this is necessary
        // NOLINTBEGIN(cppcoreguidelines-pro-type-const-cast)
        .rootFg = const_cast<char*>(root_fg.data()),
        .rootBg = const_cast<char*>(root_bg.data()),
        .borderFg = const_cast<char*>(border_fg.data()),
        .borderBg = const_cast<char*>(border_bg.data()),
        .windowFg = const_cast<char*>(window_fg.data()),
        .windowBg = const_cast<char*>(window_bg.data()),
        .shadowFg = const_cast<char*>(shadow_fg.data()),
        .shadowBg = const_cast<char*>(shadow_bg.data()),
        .titleFg = const_cast<char*>(title_fg.data()),
        .titleBg = const_cast<char*>(title_bg.data()),
        .buttonFg = const_cast<char*>(button_fg.data()),
        .buttonBg = const_cast<char*>(button_bg.data()),
        .actButtonFg = const_cast<char*>(act_button_fg.data()),
        .actButtonBg = const_cast<char*>(act_button_bg.data()),
        .checkboxFg = const_cast<char*>(checkbox_fg.data()),
        .checkboxBg = const_cast<char*>(checkbox_bg.data()),
        .actCheckboxFg = const_cast<char*>(act_checkbox_fg.data()),
        .actCheckboxBg = const_cast<char*>(act_checkbox_bg.data()),
        .entryFg = const_cast<char*>(entry_fg.data()),
        .entryBg = const_cast<char*>(entry_bg.data()),
        .labelFg = const_cast<char*>(label_fg.data()),
        .labelBg = const_cast<char*>(label_bg.data()),
        .listboxFg = const_cast<char*>(listbox_fg.data()),
        .listboxBg = const_cast<char*>(listbox_bg.data()),
        .actListboxFg = const_cast<char*>(act_listbox_fg.data()),
        .actListboxBg = const_cast<char*>(act_listbox_bg.data()),
        .textboxFg = const_cast<char*>(textbox_fg.data()),
        .textboxBg = const_cast<char*>(textbox_bg.data()),
        .actTextboxFg = const_cast<char*>(act_textbox_fg.data()),
        .actTextboxBg = const_cast<char*>(act_textbox_bg.data()),
        .helpLineFg = const_cast<char*>(help_line_fg.data()),
        .helpLineBg = const_cast<char*>(help_line_bg.data()),
        .rootTextFg = const_cast<char*>(root_text_fg.data()),
        .rootTextBg = const_cast<char*>(root_text_bg.data()),
        .emptyScale = const_cast<char*>(empty_scale.data()),
        .fullScale = const_cast<char*>(full_scale.data()),
        .disabledEntryFg = const_cast<char*>(disabled_entry_fg.data()),
        .disabledEntryBg = const_cast<char*>(disabled_entry_bg.data()),
        .compactButtonFg = const_cast<char*>(compact_button_fg.data()),
        .compactButtonBg = const_cast<char*>(compact_button_bg.data()),
        .actSelListboxFg = const_cast<char*>(act_sel_listbox_fg.data()),
        .actSelListboxBg = const_cast<char*>(act_sel_listbox_bg.data()),
        .selListboxFg = const_cast<char*>(sel_listbox_fg.data()),
        .selListboxBg = const_cast<char*>(sel_listbox_bg.data())
        // NOLINTEND(cppcoreguidelines-pro-type-const-cast)
      };
    }
  };

  static void set(const colors& THEME)
  {
    newtSetColors(static_cast<newtColors>(THEME));
  }

  constexpr static colors ONE_DARK {
    "#abb2bf", "#282c34", /* root fg, bg */
    "#282c34", "#5c6370", /* border fg, bg */
    "#101215", "#5c6370", /* window fg, bg */
    "#abb2bf", "#101215", /* shadow fg, bg */
    "#61afef", "#5c6370", /* title fg, bg */
    "#282c34", "#e06c75", /* button fg, bg */
    "#e06c75", "#282c34", /* active button fg, bg */
    "#282c34", "#56b6c2", /* checkbox fg, bg */
    "#282c34", "#c678dd", /* active checkbox fg, bg */
    "#abb2bf", "#5c6370", /* entry box fg, bg */
    "#abb2bf", "#5c6370", /* label fg, bg */
    "#282c34", "#56b6c2", /* listbox fg, bg */
    "#282c34", "#c678dd", /* active listbox fg, bg */
    "#abb2bf", "#282c34", /* textbox fg, bg */
    "#282c34", "#e06c75", /* active textbox fg, bg */
    "#abb2bf", "#282c34", /* help line */
    "#abb2bf", "#282c34", /* root text */
    "#e06c75", /* scale full*/
    "#98c379", /* scale empty*/
    "#abb2bf", "#282c34", /* disabled entry fg, bg */
    "#282c34", "#e06c75", /* compact button fg, bg */
    "#282c34", "#3c909b", /* active & sel listbox */
    "#282c34", "#56b6c2" /* selected listbox */
  };

  constexpr static colors ONE_LIGHT {
    "#abb2bf", "#828997", /* root fg, bg */
    "#282c34", "#abb2bf", /* border fg, bg */
    "#101215", "#abb2bf", /* window fg, bg */
    "#abb2bf", "#282c34", /* shadow fg, bg */
    "#3b84c0", "#abb2bf", /* title fg, bg */
    "#282c34", "#e06c75", /* button fg, bg */
    "#e06c75", "#282c34", /* active button fg, bg */
    "#282c34", "#56b6c2", /* checkbox fg, bg */
    "#282c34", "#c678dd", /* active checkbox fg, bg */
    "#282c34", "#abb2bf", /* entry box fg, bg */
    "#282c34", "#abb2bf", /* label fg, bg */
    "#282c34", "#56b6c2", /* listbox fg, bg */
    "#282c34", "#c678dd", /* active listbox fg, bg */
    "#282c34", "#828997", /* textbox fg, bg */
    "#282c34", "#e06c75", /* active textbox fg, bg */
    "#282c34", "#828997", /* help line */
    "#abb2bf", "#282c34", /* root text */
    "#be5046", /* scale empty*/
    "#7a9f60", /* scale full*/
    "#abb2bf", "#282c34", /* disabled entry fg, bg */
    "#282c34", "#e06c75", /* compact button fg, bg */
    "#282c34", "#3c909b", /* active & sel listbox */
    "#282c34", "#56b6c2" /* selected listbox */
  };
};

/*
 *    ROOT WINDOW AND OTHER FREE FUNCTIONS
 */

class root_window {
  public:
  static void init(const theme::colors& THEME = theme::ONE_DARK) noexcept
  {
    newtInit();
    newtCls();
    theme::set(THEME);
  }

  static void finish() noexcept
  {
    newtFinished();
  }

  root_window() = delete;
  root_window(root_window&) = delete;
  root_window(root_window&&) = delete;
  root_window& operator=(root_window&) = delete;
  root_window& operator=(root_window&&) = delete;
  ~root_window() = delete;

  static void draw_text(const position POS, const std::string_view TEXT)
  {
    newtDrawRootText(POS.left, POS.top, TEXT.data());
  }

  static void push_help_line(const std::string_view TEXT)
  {
    newtPushHelpLine(TEXT.data());
  }

  static void push_default_help_line()
  {
    // newt automatically pushes the default help line if the given pointer is nullptr
    newtPushHelpLine(nullptr);
  }

  static void clear_help_line()
  {
    // newt automatically clears the default help line if the given pointer is pointing to an empty string
    push_help_line("");
  }

  static void pop_help_line()
  {
    newtPopHelpLine();
  }

  /* TODO: Add suspand api  <29-03-23, Giuseppe> */
};

inline void refresh()
{
  newtRefresh();
}

inline void bell()
{
  newtBell();
}

inline size get_screen_size()
{
  size screen_size;
  newtGetScreenSize(&screen_size.width, &screen_size.height);

  return screen_size;
}

inline usize get_screen_usize()
{
  return usize { get_screen_size() };
}

inline void clear_key_buffer()
{
  newtClearKeyBuffer();
}

inline void wait_for_key()
{
  newtWaitForKey();
}

inline void reflow_text(std::string& text, const int WIDTH)
{
  const int MAX_FLEX { static_cast<int>(WIDTH / 5) };
  newtReflowText(text.data(), WIDTH, MAX_FLEX, MAX_FLEX, nullptr, nullptr);
}

[[nodiscard]] inline std::string compute_filler(const std::string_view OLD, const std::string_view NEW)
{
  size_t filling_size { OLD.size() - NEW.size() };

  /*
   * if filling_size is > OLD.size() it means that NEW.size() is bigger
   * and doing the subtraction we caused the filling_size to overflow so
   * instead we use 0
   */

  filling_size = (filling_size < OLD.size()) ? filling_size : 0;

  std::string filler;
  std::fill_n(filler.begin(), filling_size, ' ');

  return filler;
}

void inline resize_screen(const int REDRAW)
{
  newtResizeScreen(REDRAW);
}

void inline delay(const unsigned int USECS)
{
  newtDelay(USECS);
}

void inline cursor_on()
{
  newtCursorOn();
}

void inline cursor_off()
{
  newtCursorOff();
}

/*
 *    GENERIC COMPNENT
 */

class component {

  public:
  using ptr_type = conditional_ownership_ptr<std::remove_pointer<newtComponent>::type>;

  protected:
  public:
  ptr_type data;
  explicit component(newtComponent COMP, auto deleter) noexcept
      : data(COMP, deleter)
  {
  }

  explicit component(newtComponent COMP) noexcept
      : data(COMP, newtComponentDestroy)
  {
  }

  component(component& OTHER) = default;
  component(component&& OTHER) = default;
  component& operator=(const component&) noexcept = delete;
  component& operator=(component&&) noexcept = default;
  ~component() = default;

  explicit operator newtComponent() const
  {
    return data.get();
  }

  newtComponent get_owneship()
  {
    return data.own();
  }

  template <generic_component component_t>
  bool operator==(const component_t& other) const
  {
    return data == other.data;
  }

  /* TODO: Add general component manipulation <29-03-23, Giuseppe> */
};

/*
 *       GRIDS
 */

struct padding {
  int left { 0 };
  int top { 0 };
  int right { 0 };
  int bottom { 0 };
};

enum anchor {
  NOWHERE = 0,
  LEFT = NEWT_ANCHOR_LEFT,
  RIGHT = NEWT_ANCHOR_RIGHT,
  TOP = NEWT_ANCHOR_TOP,
  BOTTOM = NEWT_ANCHOR_BOTTOM,
};

enum grow {
  NO = 0,
  X = NEWT_GRID_FLAG_GROWX,
  Y = NEWT_GRID_FLAG_GROWY,
};

class grid {
  newtGrid data;
  int cols;
  int rows;

  int auto_cols { 0 };
  int auto_rows { 0 };

  void increment_auto()
  {
    ++auto_cols;
    auto_rows += auto_cols / cols;
    auto_cols = auto_cols % cols;
  }

  public:
  explicit grid(const int COLS, const int ROWS) noexcept
      : data(newtCreateGrid(COLS, ROWS))
      , cols(COLS)
      , rows(ROWS)
  {
  }

  template <component_or_collection... component_types>
  explicit grid(const int COLS, const int ROWS, const component_types&... COMPONENTS) noexcept
      : data(newtCreateGrid(COLS, ROWS))
      , cols(COLS)
      , rows(ROWS)
  {
    set_fields(COMPONENTS...);
  }

  grid(const grid&) = default;
  grid(grid&&) = default;
  grid& operator=(const grid&) = default;
  grid& operator=(grid&&) = default;

  ~grid()
  {
    newtGridFree(data, 1);
  }

  template <generic_component component_t>
  void set_field(const int COL, const int ROW, const component_t& COMPONENT, const padding PADDING = {}, const int ANCHOR = anchor::NOWHERE, const int GROW = grow::NO)
  {
    newtGridSetField(data, COL, ROW, NEWT_GRID_COMPONENT, static_cast<newtComponent>(COMPONENT), PADDING.left, PADDING.top, PADDING.right, PADDING.bottom, ANCHOR, GROW);
  }

  template <generic_component component_t>
  void auto_set_field(const component_t& COMPONENT)
  {
    newtGridSetField(data, auto_cols, auto_rows, NEWT_GRID_COMPONENT, static_cast<newtComponent>(COMPONENT), 1, 0, 1, (auto_rows != (rows - 1)) ? 1 : 0, anchor::NOWHERE, 0);
    increment_auto();
  }

  template <component_collection component_t>
  void auto_set_field(const component_t& COLLECTION)
  {
    const auto& COMPONENTS { COLLECTION.get_components() };

    for (const auto& COMPONENT : COMPONENTS) {
      newtGridSetField(data, auto_cols, auto_rows, NEWT_GRID_COMPONENT, static_cast<newtComponent>(COMPONENT), 0, 0, 0, (auto_rows != (rows - 1)) ? 1 : 0, 0, 0);
      increment_auto();
    }
  }

  template <component_or_collection... component_and_collection_t>
  void set_fields(const component_and_collection_t&... COMPONENTS)
  {
    (auto_set_field(COMPONENTS), ...);
  }

  std::pair<int, int> get_cols_rows()
  {
    return { cols, rows };
  }

  explicit operator newtGrid() const
  {
    return data;
  }
};

/*
 *         WINDOW
 */

class window {
  public:
  explicit window(const usize SIZE, const std::string_view TITLE = std::string_view {}) noexcept
  {
    newtCenteredWindow(SIZE.width, SIZE.height, TITLE.data());
  }

  window(const position POS, const usize SIZE, const std::string_view TITLE = std::string_view {}) noexcept
  {
    newtOpenWindow(POS.left, POS.top, SIZE.width, SIZE.height, TITLE.data());
  }

  [[nodiscard]] explicit window(const grid& GRID, const std::string_view TITLE = {}) noexcept
  {
    /* I hate that i had to use const cast but newt is not consistent with
     its API, why does newtGridWrappedWindow takes a char* and other methods
     to construct windows instead are taking const char*
     NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast) */
    newtGridWrappedWindow(static_cast<newtGrid>(GRID), const_cast<char*>(TITLE.data()));
  }

  ~window()
  {
    newtPopWindow();
  }

  window(window&) noexcept = delete;
  window(window&&) noexcept = delete;
  window& operator=(window&) noexcept = delete;
  window& operator=(window&&) noexcept = delete;
};

/*
 *         ALL COMPONENTS TYPES!
 */

class scroll_bar : public component {

  public:
  /* TODO: Change colors things <02-04-23, Giuseppe> */
  scroll_bar(const position POS, const int HEIGHT, const int NORMAL_COLOR_SET, const int THUMB_COLOR_SET) noexcept
      : component(newtVerticalScrollbar(POS.left, POS.top, HEIGHT, NORMAL_COLOR_SET, THUMB_COLOR_SET))
  {
  }

  void set(const int WHERE, const int TOTAL)
  {
    newtScrollbarSet(data.get(), WHERE, TOTAL);
  }

  void set_color(const int NORMAL, const int THUMB)
  {
    newtScrollbarSetColors(data.get(), NORMAL, THUMB);
  }
};

enum exit_reason : std::underlying_type_t<decltype(newtExitStruct::NEWT_EXIT_COMPONENT)> {
  HOTKEY = newtExitStruct::NEWT_EXIT_HOTKEY,
  COMPONENT = newtExitStruct::NEWT_EXIT_COMPONENT,
  TIMER = newtExitStruct::NEWT_EXIT_TIMER,
  FDREADY = newtExitStruct::NEWT_EXIT_FDREADY,
  ERROR = newtExitStruct::NEWT_EXIT_ERROR
};

struct exit_info {
  exit_reason reason;
  std::variant<int, component> data;

  explicit exit_info(const newtExitStruct& OTHER)
      : reason(static_cast<exit_reason>(
            static_cast<std::underlying_type_t<decltype(newtExitStruct::NEWT_EXIT_COMPONENT)>>(
              OTHER.reason)))
  {
    switch (reason) {
    case exit_reason::HOTKEY:
      data = OTHER.u.key;
      break;
    case exit_reason::COMPONENT:
      data = component { OTHER.u.co, component::ptr_type::no_delete };
      break;
    case exit_reason::TIMER:
      data = OTHER.u.watch;
      break;
    case exit_reason::FDREADY:
    case exit_reason::ERROR:
    default:
      break;
    }
  }
};

class form : public component {
  public:
  explicit form(void* help_tag = nullptr, const int FLAGS = 0) noexcept
      : component(newtForm(nullptr, help_tag, FLAGS), newtFormDestroy)
  {
  }

  /* TODO:  what the fuck is helptag? <02-04-23, Giuseppe> */
  explicit form(scroll_bar& bar, void* help_tag = nullptr, const int FLAGS = 0) noexcept
      : component(newtForm(bar.get_owneship(), help_tag, FLAGS), newtFormDestroy)
  {
    /* TODO: Implement parameters support <29-03-23, Giuseppe> */
  }

  template <component_or_collection... components_and_collections_t>
  explicit form(components_and_collections_t&... components) noexcept
      : component(newtForm(nullptr, nullptr, 0), newtFormDestroy)
  {
    add_components(components...);
  }

  template <generic_component component_t>
  void add_component(component_t& comp)
  {
    newtFormAddComponent(data.get(), comp.get_owneship());
  }

  template <component_collection collection_t>
  void add_component(collection_t& comps)
  {
    for (auto& comp : comps.get_components()) {
      newtFormAddComponent(data.get(), comp.get_owneship());
    }
  }

  template <component_or_collection... collections_and_components_t>
  void add_components(collections_and_components_t&... components)
  {
    (add_component(components), ...);
  }

  [[nodiscard]] auto run()
  {

    newtExitStruct result {};
    newtFormRun(data.get(), &result);

    return exit_info { result };
  }

  void add_hot_key(const int KEY)
  {
    newtFormAddHotKey(data.get(), KEY);
  }

  void watch_fd(const int FILE_DESCRIPTOR, const int FLAGS)
  {
    newtFormWatchFd(data.get(), FILE_DESCRIPTOR, FLAGS);
  }

  void draw_form()
  {
    newtDrawForm(data.get());
  }

  template <generic_component component_t>
  void set_current(const component_t& comp)
  {
    newtFormSetCurrent(data.get(), static_cast<newtComponent>(comp));
  }

  void set_background(const int COLOR)
  {
    newtFormSetBackground(data.get(), COLOR);
  }

  void set_height(const int HEIGHT)
  {
    newtFormSetHeight(data.get(), HEIGHT);
  }

  void set_width(const int WIDTH)
  {
    newtFormSetWidth(data.get(), WIDTH);
  }

  component get_current()
  {
    return component { newtFormGetCurrent(data.get()), ptr_type::no_delete };
  }
};

template <component_or_collection... components_and_collections_t>
[[nodiscard]] inline std::pair<exit_info, form> fast_run(const int COLS, const int ROWS, const std::string_view TITLE, components_and_collections_t&... components)
{
  const grid GRID { COLS, ROWS, components... };
  const window WINDOW { GRID, TITLE };
  form user_form { components... };

  return { user_form.run(), std::move(user_form) };
}

class button : public component {
  public:
  explicit button(const std::string_view TEXT, const position POS = { 0, 0 }) noexcept
      : component(newtButton(POS.left, POS.top, TEXT.data()))
  {
  }
};

class compact_button : public component {
  public:
  explicit compact_button(const std::string_view TEXT, const position POS = { 0, 0 }) noexcept
      : component(newtCompactButton(POS.left, POS.top, TEXT.data()))
  {
  }
};

class label : public component {
  public:
  explicit label(const std::string_view TEXT, const position POS = { 0, 0 }) noexcept
      : component(newtLabel(POS.left, POS.top, TEXT.data()))
  {
  }

  void set_text(const std::string_view TEXT)
  {
    newtLabelSetText(data.get(), TEXT.data());
  }

  void set_colors(const int COLOR_SET)
  {
    newtLabelSetColors(data.get(), COLOR_SET);
  }
};

class entrybox : public component {
  const char* content;

  public:
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init, hicpp-member-init, hicpp-signed-bitwise) -- content gets initted by newtEntry
  explicit entrybox(const int WIDTH, const position POS = { 0, 0 }, std::string_view INITIAL_VALUE = { "" }, const int FLAGS = NEWT_ENTRY_SCROLL) noexcept
      : component(newtEntry(POS.left, POS.top, INITIAL_VALUE.data(), WIDTH, &content, FLAGS))
  {
  }

  void set_value(const std::string_view TEXT, const bool CURSOR_AT_END = true)
  {
    newtEntrySet(data.get(), TEXT.data(), static_cast<int>(CURSOR_AT_END));
  }

  std::string_view get_value()
  {
    return std::string_view { content };
  }

  /* TODO: Add filter api  <29-03-23, Giuseppe> */
};

class checkbox : public component {
  public:
  explicit checkbox(const std::string_view TEXT, const position POS = { 0, 0 }, const char DEFAULT_VAL = ' ', const std::string_view SEQ = std::string_view {}) noexcept
      : component(newtCheckbox(POS.left, POS.top, TEXT.data(), DEFAULT_VAL, SEQ.data(), nullptr))
  {
  }

  char get_value()
  {
    return newtCheckboxGetValue(data.get());
  }

  void set_value(const char VALUE)
  {
    newtCheckboxSetValue(data.get(), VALUE);
  }

  /* TODO: add set_flags flags <30-03-23, Giuseppe> */
};

class radio_button : public component {
  explicit radio_button(newtComponent other, ptr_type::deleter_ptr deleter_func = newtComponentDestroy)
      : component(other, deleter_func)
  {
  }

  public:
  radio_button()
      : component(nullptr, ptr_type::no_delete)
  {
  }

  explicit radio_button(std::string_view TEXT, const position POS = { 0, 0 }, const radio_button& PREVIUS = {}, bool IS_DEFAULT = false) noexcept
      : component(newtRadiobutton(POS.left, POS.top, TEXT.data(), static_cast<int>(IS_DEFAULT), static_cast<newtComponent>(PREVIUS)))
  {
  }

  [[nodiscard]] radio_button get_current() const
  {
    return radio_button { newtRadioGetCurrent(data.get()), ptr_type::no_delete };
  }

  void set_current()
  {
    newtRadioSetCurrent(data.get());
  }
};

class radio_button_collection {
  std::vector<radio_button> collection {};

  public:
  template <typename... T>
  explicit radio_button_collection(T... strings)
  {
    (add_radio_button(strings), ...);
  }

  [[nodiscard]] std::vector<radio_button>& get_components()

  {
    return collection;
  }

  [[nodiscard]] const std::vector<radio_button>& get_components() const
  {
    return collection;
  }

  [[nodiscard]] size_t get_current_index() const
  {
    return static_cast<size_t>(std::abs(std::distance(collection.begin(), std::find(collection.begin(), collection.end(), collection.front().get_current()))));
  }

  void set_current(const size_t INDEX)
  {
    collection[INDEX].set_current();
  }

  template <typename... T>
  void add_radio_button(std::string_view TEXT, const position POS = { 0, 0 }, const bool DEFAULT = false)
  {
    collection.empty()
        ? collection.emplace_back(TEXT, POS, radio_button {}, DEFAULT)
        : collection.emplace_back(TEXT, POS, collection.back(), DEFAULT);
  }
};

class scale : public component {
  public:
  scale(const int WIDTH, const long long FULL_VALUE, const position POS = { 0, 0 }) noexcept
      : component(newtScale(POS.left, POS.top, WIDTH, FULL_VALUE))
  {
  }

  void set_value(const unsigned long long VALUE)
  {
    newtScaleSet(data.get(), VALUE);
  }
};

class textbox : public component {
  public:
  explicit textbox(const size SIZE, const std::string_view TEXT, const position POS = { 0, 0 }, const bool IS_SCROLLABLE = true) noexcept
      // NOLINTNEXTLINE
      : component(newtTextbox(POS.left, POS.top, SIZE.width, SIZE.height, (IS_SCROLLABLE) ? NEWT_FLAG_SCROLL : 0))
  {
    newtTextboxSetText(data.get(), TEXT.data());
  }

  void set_text(const std::string_view TEXT)
  {
    newtTextboxSetText(data.get(), TEXT.data());
  }

  void set_height(const int HEIGHT)
  {
    newtTextboxSetHeight(data.get(), HEIGHT);
  }

  [[nodiscard]] int get_num_lines() const
  {
    return newtTextboxGetNumLines(data.get());
  }

  // NOLINTNEXTLINE -- still the best way
  void set_colors(const int NORMAL, const int ACTIVE)
  {
    newtTextboxSetColors(data.get(), NORMAL, ACTIVE);
  }
};

class textbox_reflowed : public component {
  std::string text;
  static const int FLEX_DEVIDER { 5 };

  public:
  void set_text(const std::string_view TEXT)
  {
    text = TEXT;
    newtTextboxSetText(data.get(), text.data());
  }

  explicit textbox_reflowed(const int WIDTH, const std::string_view TEXT, const position POS = { 0, 0 }) noexcept
      : component(newtTextboxReflowed(POS.left, POS.top, const_cast<char*>(TEXT.data()), WIDTH, static_cast<int>(WIDTH / FLEX_DEVIDER), static_cast<int>(WIDTH / FLEX_DEVIDER), 0)) // NOLINT -- newt takes char* instead of cosnt char*
  {
    set_text(TEXT);
  }

  void set_height(const int HEIGHT)
  {
    newtTextboxSetHeight(data.get(), HEIGHT);
  }

  [[nodiscard]] int get_num_lines() const
  {
    return newtTextboxGetNumLines(data.get());
  }

  // NOLINTNEXTLINE -- still the best way
  void set_colors(const int NORMAL, const int ACTIVE)
  {
    newtTextboxSetColors(data.get(), NORMAL, ACTIVE);
  }
};

}

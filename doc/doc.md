# Documentation

## Index

- [size, usize, position](#size-usize-position)
- [root_window](#root_window)
- [other functions](#other-functions)
- [grid](#grid)
- [window](#window)
- [component](#component)
- [scroll_bar](#scroll_bar)
- [form](#form)
- [button](#button)
- [compact_button](#compact_button)
- [label](#label)
- [entrybox](#entrybox)
- [checkbox](#checkbox)
- [radio_button](#radio_button)
- [radio_button_collection](#radio_button_collection)
- [scale](#scale)
- [textbox](#textbox)
- [textbox_reflowed](#textbox_reflowed)

## size, usize, position

- The `size` struct template is a simple 2D size class that can hold width and height values, uses `int` to store the values.
- The `usize` same as size but uses `unsigned int` since some newt api require it.
- The `position` struct has the same interfaces as `size` and `usize` but `width` and `height` are called `left` and `top`. It's used to express the position of the component instead of it's size.

### Public data members

```c++
int width { 0 } 
int height { 0 } 
```

### Constructors

```c++
size(const int, const int)
size(const size_base<other_number>)
```

1) A constructor that initializes the width and height to the specified values.

2) A templated constructor that initializes the size from another size_base object that may have a different integral type. The constructor uses static_cast to convert the other object's values to the integral type of this object.

### Operators

```c++
size operator+(const int) const
size operator-(const int) const
size operator*(const int) const
size operator/(const int) const
size operator+(const size) const
size operator-(const size) const
size operator*(const size) const
size operator/(const size) const
```

1) Returns a new `size` object that is the result of applying the operator with the specified `const int` value to both the `width` and `height` of the current object.

2) Returns a new `size` object that is the result of applying the operator with the correspondent other `size.widht` and `size.height` to the current object.

## root_window

The `root_window` class provides a set of static methods to manipulate the root window of the newt Terminal User Interface (TUI). The `root_window` class cannot be instantiated or copied, as it only contains static methods.

### Public Methods

```c++
static void init() noexcept
```

Initializes the newt library and clears the root window.

---

```c++
static void finish() noexcept
```

Shuts down the newt library.

---

```c++
static void draw_text(const position, const std::string_view)
```

Draws the given text string on the root window at the given position.

---

```c++
static void push_help_line(const std::string_view)
```

Pushes the given text string as a help line on the root window.

```c++
static void push_default_help_line()
```

Pushes the default help line on the root window.

---

```c++
static void clear_help_line()
```

Clears the help line on the root window.

---

```c++
static void pop_help_line()
```

Removes the last help line pushed onto the root window.

### Example Usage

```c++
root_window::init();
root_window::draw_text(position(2, 2), "Hello World!");
root_window::push_help_line("Press any key to quit");
newt::wait_for_key();
root_window::pop_help_line();
root_window::finish();
```

## Other functions

```c++
void refresh()
```

Refreshes the screen.

---

```c++
void bell()
```

Makes the terminal bell.

---

```c++
size get_screen_size()
```

Returns the `size` of the terminal screen as a size object, which contains the width and height of the screen.

---

```c++
usize get_screen_usize()
```

Same as `get_screen_size()` but retruns `usize`.

---

```c++
void clear_key_buffer()
```

Clears the keyboard buffer.

---

```c++
void wait_for_key()
```

Waits for a key press before continuing execution.

---

```c++
void reflow_text(std::string&, const int)
```

Reflows a given `std::string` to fit within the specified width. It takes a reference to the string to be reflowed and the desired width as `const int`.

---

```c++
std::string compute_filler(const std::string_view OLD, const std::string_view NEW)
```

Computes the filler string required to fill the gap between two strings of different lengths. It takes two string views as arguments, the old and new strings, and returns a string view containing the filler characters.

---

```c++
void inline resize_screen(const bool)
```

Resizes the terminal screen to the specified dimensions. The `const bool` argument specifies whether the screen should be redrawn after the resize.

---

```c++
void inline delay(const unsigned int)
```

Pauses program execution for the specified number of microseconds.

---

```c++
void inline cursor_on()
```

Turns on the cursor display.

---

```c++
void inline cursor_off()
```

Turns off the cursor display.

---

```c++
template <component_or_collection... components_and_collections_t>
[[nodiscard]] inline std::pair<exit_info, form> fast_run(const int COLS, const int ROWS, const std::string_view TITLE, components_and_collections_t&... components)
```

This is a function template called `fast_run` that takes as template arguments a variadic list of types that can be either a `component` or a `component_collection`. The function returns a `std::pair` of `exit_info` and `form`.  

The function takes several parameters:
- `COLS`: representing the number of columns in the grid.
- `ROWS`: representing the number of rows in the grid.
- `TITLE`: representing the title of the window.
- `components_and_collections_t&... components`: a variadic list of references to objects of the types specified in the template parameter list. These objects are used to populate the grid in the form.

Inside the function, a `grid` object is created using the specified number of rows and columns and the provided components and collections. A `window` object is created using the created grid and the provided title, the window is centered. A `form` object is created using the provided components and collections.

Finally, the `run` method of the `form` object is called, which will display the form to the user and wait for input. The function returns a `std::pair` containing the exit status of the form and the `form` object itself (this is mainly to allow the callee to access the components since the form would free them), which can be used for further processing.

## grid

The `grid` class represents a grid-based layout of user interface elements in the Newt library.

### Constructors

```c++
explicit grid(const int COLS, const int ROWS) noexcept

template <generic_component... component_types> explicit 
grid(const int COLS, const int ROWS, const component_types&...) noexcept
```

1) Constructs an empty `grid` with the specified number of columns and rows.

2) Constructs a `grid` with the specified number of `columns` and `rows`, and fills it with the specified `components`.

### Member Functions

```c++
void set_field(const int COL, const int ROW, const component_t& COMPONENT, const padding PADDING = {}, const int ANCHOR = anchor::NOWHERE, const int GROW = grow::NO)
```

Sets the specified component at the given column and row, with optional padding, anchor, and grow parameters.

---

```c++
std::pair<int, int> get_cols_rows()
```

Returns a `std::pair` containing the number of `cols` and `rows` in the `grid`.

---

```c++
template <generic_component... component_types> 
void set_fields(const component_types&...)
```

**TODO: UPDATE** Fills the `grid` with the specified components.

---

```c++
explicit operator newtGrid() const
```

Returns the underlying `newtGrid` data type.

---

### Structs and Enums

The `grid` class utilizes the following data structures and enum:

```c++
struct padding {
  int left { 0 };
  int top { 0 };
  int right { 0 };
  int bottom { 0 };
};
```

The `padding` struct defines the amount of padding on each side of a component within the `grid`.

---

```c++
enum anchor {
  NOWHERE = 0,
  LEFT = NEWT_ANCHOR_LEFT,
  RIGHT = NEWT_ANCHOR_RIGHT,
  TOP = NEWT_ANCHOR_TOP,
  BOTTOM = NEWT_ANCHOR_BOTTOM,
};
```

The `anchor` enum defines the anchor position for a component within the `grid`.

---

```c++
enum grow {
  NO = 0,
  X = NEWT_GRID_FLAG_GROWX,
  Y = NEWT_GRID_FLAG_GROWY,
};
```

The `grow` enum defines whether a `component` can grow in a certain direction within the `grid`.

## window

The `window` class allows the user to create a window. It provides several constructors to create a new window with a specified `size` and title, or to wrap an existing `grid` in a window.

### Constructors

```c++
window(const usize, const std::string_view = std::string_view {}) noexcept
window(const position, const usize, const std::string_view = std::string_view {}) noexcept
explicit window(const grid&, const std::string_view = {}) noexcept
```

1) Creates a new window with the specified `usize` and optional title. The window is centered on the screen.

2) Creates a new window with the specified `usize`, `position`, and optional title. The window is positioned at the specified `position`.

3) Wraps an existing `grid` in a window with the optional title.

## component

The `component` class is a wrapper around a `newtComponent` object that provides ownership management and convenience methods for working with components.  
**This class is the base class for all the components this library offers** and should not be used directly by the user.  
However an istance of this class is returned when a form gets runned since the newt api doesn't give a way to deduce the component type.

### Constructors

```c++
component(newtComponent) noexcept
component(component&) = default
component(component&&) = default
```

1) Constructs a `component` object that owns the given `newtComponent`.

2) Constructs a `component` object that *takes ownership* from the other `component`.

3) Move constructs a compoennt object that *takes owership* from the other `component`.

### Public Members

```c++
component& operator=(component&&) noexcept = default
```

Move assignment operator, *takes ownership* from the other `component`.

---

```c++
operator newtComponent() const
```

Conversion operator that returns the underlying `newtComponent`.

---

```c++
newtComponent get_owneship()
```

Returns the underlying `newtComponent` and *releases ownership* from the `component` object.

---

```c++
bool operator==(const component_t&) const
```

Comparison operator that checks if two `component` objects have the same underlying `newtComponent`.

## scroll_bar

The `scroll_bar` class is a derived class of the `component` class that represents a scroll bar component.

### Constructors

```c++
scroll_bar(const position POS, const int HEIGHT, const int NORMAL_COLOR_SET, const int THUMB_COLOR_SET) noexcept
```

Constructs a `scroll_bar` object with the specified position, height, and colors for the normal and thumb states of the scrollbar. The `scroll_bar` object takes ownership of a `newtVerticalScrollbar` object constructed with the given parameters.

### Public Members

```c++
void set(const int WHERE, const int TOTAL)
```

Sets the position of the scrollbar to the specified `WHERE` value and the total range to the specified `TOTAL` value.

---

```c++
void set_color(const int NORMAL, const int THUMB)
```

Sets the colors of the normal and thumb states of the scrollbar to the specified values.

## form

The `form` class is a subclass of `component` that represents a form widget in the newt UI library. 

### Constructors

```c++
explicit form(void* help_tag = nullptr, const int FLAGS = 0) noexcept
explicit form(scroll_bar& bar, void* help_tag = nullptr, const int FLAGS = 0) noexcept

template <component_or_collection... components_and_collections_t>
explicit form(components_and_collections_t&... components) noexcept
```

1) Constructs a new `form` object with an optional `help_tag` and `FLAGS`. The `help_tag` is an opaque data pointer that can be used to associate help text with the form.

2) Constructs a new `form` object that contains the given `scroll_bar` and has an optional `help_tag` and `FLAGS`.

3) Constructs a new `form` object that contains the given `components` and/or `collections`.

### Public Members

```c++
void add_component(component_t&)
```

Adds a `component` to the form.

---

```c++
void add_component(collection_t&)
```

Adds a `component_collection` to the form.

---

```c++
void add_components(collections_and_components_t&...)
```

Adds multiple `components` and/or `component_collection` to the form.

---

```c++
auto run() -> exit_info
```

Displays the form and waits for user input. Returns an `exit_info` object that describes how the user exited the form.

---

```c++
void add_hot_key(const int)
```

Registers a keyboard shortcut key for the form.

---

```c++
void watch_fd(const int FILE_DESCRIPTOR, const int FLAGS)
```

Sets up a file descriptor for the form to listen to.

---

```c++
void draw_form()
```

Draws the form on the screen.

---

```c++
void set_current(const component_t&)
```

Sets the current component of the form to the given `component`.

---

```c++
void set_background(const int)
```

Sets the background color of the form.

---

```c++
void set_height(const int)
```

Sets the height of the form.

---

```c++
void set_width(const int)
```

Sets the width of the form.

---

```c++
component get_current()
```

Returns the current component of the form as a `component` object.

## exit_info

The struct `exit_info` contains informations about how the user exited a `form`.

```c++
struct exit_info {
  exit_reason reason;
  std::variant<int, component> data;
};
```

- If the reason is `HOTKEY` than `data` contains the key pressed.
- If the reason is `COMPONENT` than `data` contains the `component` that caused the exit.
- If the reason is `TIMER` than `data` the watch variable.
- If the reason is `FDREADY` or `ERROR` than `data` is empty.

## button

The `button` class is a wrapper around a `newtButton` object that provides ownership management and convenience methods for working with buttons. It inherits from the `component` class.

### Constructors

```c++
button(const std::string_view, const position) noexcept
```

Constructs a `button` object with the specified text and position.

## compact_button

The `compact_button` class is a wrapper around a `newtCompactButton` object that represents a button with a simple text label that can be clicked. It uses less screen space than [button](#button).

### Constructors

```c++
explicit compact_button(const std::string_view, const position) noexcept
```

Constructs a `compact_button` object with the specified text and position.

## label

The `label` class is a derived class of the `component` class and represents a text label on a form in the Newt library.

### Constructors

```c++
explicit label(const std::string_view, const position) noexcept
```

Constructs a `label` object with the given text and position.

### Public Members

```c++
void set_text(const std::string_view)
```

Sets the text of the label to the given text.

---

```c++
void set_colors(const int)
```

Sets the colors of the label.

## entrybox

The `entrybox` class is a wrapper around a `newtEntry` object that provides ownership management and convenience methods for working with text input fields.

### Constructors

```c++
explicit entrybox(const int WIDTH, const position POS = { 0, 0 }, std::string_view INITIAL_VALUE = { "" }, const int FLAGS = NEWT_ENTRY_SCROLL) noexcept
```

Constructs an `entrybox` object with the specified `WIDTH` and `POS`. `INITIAL_VALUE` is the initial text value of the entrybox, and `FLAGS` are the display flags for the entrybox.

### Public Members

```c++
void set_value(const std::string_view TEXT, const bool CURSOR_AT_END = true)
```

Sets the text value of the entrybox to `TEXT`. If `CURSOR_AT_END` is true, the cursor is moved to the end of the text.

---

```c++
std::string_view get_value()
```

Returns the current text value of the entrybox as a `std::string_view`.

---

```c++
// TODO: Add filter api
```

TODO: Add a filter API for restricting input to the entrybox.

## checkbox

The `checkbox` class is a wrapper around a `newtCheckbox` object that provides ownership management and convenience methods for working with checkboxes.

### Constructors

```c++
checkbox(const std::string_view TEXT, const position POS = { 0, 0 }, const char DEFAULT_VAL = ' ', const std::string_view SEQ = std::string_view {}) noexcept
```

Constructs a `checkbox` object with the given `TEXT`, `POS`, `DEFAULT_VAL`, and `SEQ`.  

Checkkboxes in newt can cycle on a range of values, but by default there are only two states:
1) `' '`
2) `'*'`

The `SEQ` parameter can be used to define all the possible states for a checkbox.

### Public Members

```c++
char get_value()
```

Returns the value of the checkbox as a `char`.

---

```c++
void set_value(const char)
```

Sets the value of the checkbox.

## radio_button

The `radio_button` class is a wrapper around a `newtRadiobutton` object that provides ownership management and convenience methods for working with radio buttons.

### Constructors

```c++
radio_button()
explicit radio_button(std::string_view TEXT, const position POS = { 0, 0 }, const radio_button& PREVIUS = {}, bool IS_DEFAULT = false) noexcept
```

1) Constructs a `radio_button` object that does not own any `newtRadiobutton` object.

2) Constructs a `radio_button` object that owns the `newtRadiobutton` object created with the given parameters.

If the `PREVIUS` parameter is a default constructed `radio_button` then created radio_button also creates a new group.  
If the `PREVIUS` parameter is filled with an owning `radio_button` than the created radio_button is added to it's group.

### Public Members

```c++
[[nodiscard]] radio_button get_current() const
```

Returns a new `radio_button` object that does not owns the `newtRadiobutton` object that is currently selected in the group, or a default constructed `radio_button` object if no radio button is selected.

---

```c++
void set_current()
```

Makes the radio button the selected one in it's group.

## radio_button_collection

The `radio_button_collection` class represents a collection of `radio_button` objects. It provides methods for accessing and manipulating the collection as a whole, such as setting the current radio button and getting the index of the current radio button.

### Constructors

```c++
template <typename... T>
explicit radio_button_collection(T... strings)
```

Constructs a `radio_button_collection` object with a series of string arguments, each of which is used to create a new `radio_button` object that is added to the collection.

### Public Members

```c++
std::vector<radio_button>& get_components()
```

Returns a reference to the internal vector of `radio_button` objects.

---

```c++
const std::vector<radio_button>& get_components() const
```

Returns a const reference to the internal vector of `radio_button` objects.

---

```c++
size_t get_current_index() const
```

Returns the index of the current `radio_button` object in the collection.

---

```c++
void set_current(const size_t INDEX)
```

Sets the current `radio_button` object to the one at the specified index.

---

```c++
template <typename... T>
void add_radio_button(std::string_view TEXT, const position POS = { 0, 0 }, const bool DEFAULT = false)
```

Adds a new `radio_button` object to the collection with the specified text, position, and default value. If the collection is empty, the new radio button becomes the first radio button in the collection. Otherwise, the new radio button is added after the last radio button in the collection.

## scale

The `scale` class is a wrapper around a `newtScale` object that provides ownership management and a method for setting the value of the scale.

### Constructors

```c++
scale(const int WIDTH, const long long FULL_VALUE, const position POS = { 0, 0 }) noexcept
```

Constructs a `scale` object with the given `WIDTH` and `FULL_VALUE` parameters, and optionally a `position` parameter for its position on the form.

### Public Members

```c++
void set_value(const unsigned long long VALUE)
```

Sets the value of the scale to the given `VALUE`.

## textbox

The `textbox` class is a wrapper around a `newtTextbox` object that provides ownership management and convenience methods for working with textboxes.

### Constructors

```c++
explicit textbox(const size, std::string_view, position = { 0, 0 }, bool = true) noexcept
```

Constructs a `textbox` object with the given size, text, and position. If `IS_SCROLLABLE` is set to true, the textbox can be scrolled vertically.

### Public Members

```c++
void set_text(std::string_view)
```

Sets the text of the textbox. The text must be the same length or shorter.

---

```c++
void set_height(int)
```

Sets the height of the textbox.

---

```c++
int get_num_lines() const
```

Returns the number of lines of text in the textbox.

---

```c++
void set_colors(const int NORMALE,const int ACTIVE)
```

Sets the colors of the textbox. The `NORMAL` parameter specifies the color of the text when the textbox is not active, while the `ACTIVE` parameter specifies the color when the textbox is active.

## textbox_reflowed

The `textbox_reflowed` class is a specialization of the `component` class that represents a text box widget that can reflow its content based on the available width.

###### Constructors

```c++
explicit textbox_reflowed(const int WIDTH, const std::string_view TEXT, const position POS = { 0, 0 }) noexcept
```

Constructs a `textbox_reflowed` object with the given `WIDTH`, initial `TEXT` and `POS`.

### Public Members

```c++
void set_text(const std::string_view TEXT)
```

Sets the content of the text box to the given `TEXT`.

---

```c++
void set_height(const int HEIGHT)
```
Sets the height of the text box to the given `HEIGHT`.

---

```c++
int get_num_lines() const
```
Returns the number of lines of text in the text box.

---

```c++
void set_colors(const int NORMAL, const int ACTIVE)
```
Sets the colors of the text box in normal and active states.


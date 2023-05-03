# Documentation

## Index

- [size, usize, position](#size-usize-position)
- [root_window](#root_window)
- [other fucntions](#other-functions)
- [grid](#grid)
- [window](#window)
- [component](#component)

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

### Public Types

`ptr_type`  

A type alias for the `conditional_ownership_ptr` template, which is a smart pointer that can either own the object it points to or not, depending on how it was constructed and it's usage.

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

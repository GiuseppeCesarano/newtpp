# newtpp

Newtpp is a header-only C++ library that builds on the functionality of the newt C library to help users create simple terminal user interfaces quickly and easily. The library adds type safety, utility to build UI faster, and support for RAII.

## Features

- *Type safety*: newtpp provides type-safe C++ wrappers around the newt C library functions, reducing the risk of errors caused by incorrect use of function parameters.
- *Utility*: newtpp provides a set of high-level C++ classes and functions that make it easier to build terminal UIs.
- *RAII support*: newtpp's classes use RAII (Resource Acquisition Is Initialization) to manage resources automatically, reducing the risk of resource leaks.

## Requirements

- A C++20 compiler
- The newt C library

## Getting started

To use newtpp, simply include the relevant header files in your C++ project:

``` c++
#include "newtpp/newtpp.hpp"
```

when compiling link libnewt with `-lnewt` (clang++ or g++)

## Documentation

### Index

- [Examples](#Examples)
- [size, usize, position](#size,-usize,-position)
- [root_window](#root_window)
- [other fucntions](#other-functions)
- [grid](#grid)
- [window](#window)
- [component](#component)

# Examples

### size, usize, position

The size class template is a simple 2D size class that can hold width and height values.
The size class uses an integer to store those values.
The usize class has the same interfaces but uses unsigned int to store the values, since some newt api require unsigned int.

The position class also has the same interfaces but width and height are called left and top. It's used to express the position of the component instead of it's size

#### Public data members

int width { 0 }: the width of the size, initialized to 0.
int height { 0 }: the height of the size, initialized to 0.

#### Constructors

##### size(const int WIDTH, const int HEIGHT)

A constructor that initializes the width and height to the specified values.

#### size(const size_base<other_number> OTHER)

A templated constructor that initializes the size from another size_base object that may have a different integral type. The constructor uses static_cast to convert the other object's values to the integral type of this object.

#### Public member functions

##### size operator+(const int SCALAR) const

 Returns a new size object that is the result of adding the specified scalar value to both the width and height of the current object.

##### size operator-(const int SCALAR) const

 Returns a new size object that is the result of subtracting the specified scalar value from both the width and height of the current object.

##### size operator*(const int SCALAR) const

 Returns a new size object that is the result of multiplying the specified scalar value by both the width and height of the current object.

##### size operator/(const int SCALAR) const

 Returns a new size object that is the result of dividing both the width and height of the current object by the specified scalar value.

##### size operator+(const size OTHER) const

 Returns a new size object that is the result of adding the specified size object to the current object.

##### size operator-(const size OTHER) const

 Returns a new size object that is the result of subtracting the specified size object from the current object.

##### size operator*(const size OTHER) const

 Returns a new size object that is the result of multiplying the width and height of the current object by the width and height of the specified size object.

##### size operator/(const size OTHER) const

 Returns a new size object that is the result of dividing the width and height of the current object by the width and height of the specified size object.

### root_window

The `root_window` class provides a set of static methods to manipulate the root window of the newt Terminal User Interface (TUI). The root_window class cannot be instantiated or copied, as it only contains static methods.

#### Public Methods

##### static void init() noexcept

 Initializes the newt library and clears the root window.

##### static void finish() noexcept

 Shuts down the newt library.

##### static void draw_text(const position POS, const std::string_view TEXT)

Draws the given text string on the root window at the given position POS.

##### static void push_help_line(const std::string_view TEXT)

Pushes the given text string as a help line on the root window.

##### static void push_default_help_line()

Pushes the default help line on the root window.

##### static void clear_help_line()

Clears the help line on the root window.

##### static void pop_help_line()

Removes the last help line pushed onto the root window.

#### Example Usage

```c++
root_window::init();
root_window::draw_text(position(2, 2), "Hello World!");
root_window::push_help_line("Press 'q' to quit");
root_window::pop_help_line();
root_window::finish();
```

### Other functions

#### void refresh()

This function refreshes the screen.

#### void bell()

This function makes the terminal bell.

#### size get_screen_size()

This function returns the size of the terminal screen as a size object, which contains the width and height of the screen.

#### usize get_screen_usize()

Same as [get_screen_size()]("#get_screen_size(\)) but retruns usize.

#### void clear_key_buffer()

This function clears the keyboard buffer.

#### void wait_for_key()

This function waits for a key press before continuing execution.

#### void reflow_text(std::string& text, const int WIDTH)

This function reflows a given text string to fit within the specified width. It takes a reference to the string to be reflowed and the desired width as arguments.

#### std::string compute_filler(const std::string_view OLD, const std::string_view NEW)

This function computes the filler string required to fill the gap between two strings of different lengths. It takes two string views as arguments, the old and new strings, and returns a string view containing the filler characters.

#### void inline resize_screen(const bool REDRAW)

This function resizes the terminal screen to the specified dimensions. The REDRAW argument specifies whether the screen should be redrawn after the resize.

#### void inline delay(const unsigned int USECS)

This function pauses execution for the specified number of microseconds.

#### void inline cursor_on()

This function turns on the cursor display.

#### void inline cursor_off()

This function turns off the cursor display.

### grid

The grid class represents a grid-based layout of user interface elements in the Newt library.

#### Constructors

The grid class provides the following constructors:

##### explicit grid(const int COLS, const int ROWS) noexcept
constructs an empty grid with the specified number of columns and rows.

##### template <generic_component... component_types> explicit grid(const int COLS, const int ROWS, const component_types&... COMPONENTS) noexcept

Constructs a grid with the specified number of columns and rows, and fills it with the specified components.

#### Member Functions

The grid class provides the following member functions

##### void set_field(const int COL, const int ROW, const component_t& COMPONENT, const padding PADDING = {}, const int ANCHOR = anchor::NOWHERE, const int GROW = grow::NO)

sets the specified component at the given column and row, with optional padding, anchor, and grow parameters.

##### std::pair<int, int> get_cols_rows()

returns a pair containing the number of columns and rows in the grid.

##### template <generic_component... component_types> void set_fields(const component_types&... COMPONENTS)

fills the grid with the specified components.

##### explicit operator newtGrid() const

returns the underlying newtGrid data type.

#### structs and enums

The grid class utilizes the following data structures and enum:

##### struct padding

The padding struct defines the amount of padding on each side of a component within the grid. It contains the following members:

- `left` (default 0): The amount of padding on the left side of the component.
- `top` (default 0): The amount of padding on the top of the component.
- `right` (default 0): The amount of padding on the right side of the component.
- `bottom` (default 0): The amount of padding on the bottom of the component.

##### enum anchor

The anchor enum defines the anchor position for a component within the grid. It contains the following members:

- `NOWHERE` (value 0): The component is not anchored to any side or corner of the grid.
- `LEFT` (value NEWT_ANCHOR_LEFT): The component is anchored to the left side of the grid.
- `RIGHT` (value NEWT_ANCHOR_RIGHT): The component is anchored to the right side of the grid.
- `TOP` (value NEWT_ANCHOR_TOP): The component is anchored to the top of the grid.
- `BOTTOM` (value NEWT_ANCHOR_BOTTOM): The component is anchored to the bottom of the grid.

##### enum grow

The grow enum defines whether a component can grow in a certain direction within the grid. It contains the following members:

- `NO` (value 0): The component cannot grow in any direction.
- `X` (value NEWT_GRID_FLAG_GROWX): The component can grow horizontally.
- `Y` (value NEWT_GRID_FLAG_GROWY): The component can grow vertically.

### window
The window class allows the user to create a window. It provides several constructors to create a new window with a specified size and title, or to wrap an existing grid in a window.

#### Constructors

##### window(const usize SIZE, const std::string_view TITLE = std::string_view {}) noexcept

Creates a new window with the specified SIZE and optional TITLE. The window is centered on the screen.

##### window(const position POS, const usize SIZE, const std::string_view TITLE = std::string_view {}) noexcept

Creates a new window with the specified SIZE, POSITION, and optional TITLE. The window is positioned at the specified POSITION.

##### explicit window(const grid& GRID, const std::string_view TITLE = {}) noexcept

Wraps an existing grid in a window with the optional TITLE.

### component

The component class is a wrapper around a newtComponent object that provides ownership management and convenience methods for working with components.  
This class is the base class for all the components this library offers and should not be used directly by the user.  
However an istance of this class is returned when a form gets runned since the newt api doesn't give a way to deduce the component type.

#### Public Types

##### ptr_type

A type alias for the conditional_ownership_ptr template, which is a smart pointer that can either own the object it points to or not, depending on how it was constructed.

#### Constructors

##### component(newtComponent COMP) noexcept

Constructs a component object that owns the given newtComponent.

##### component(component& OTHER) = default

Constructs a component object that takes ownership from OTHER.

##### component(component&& OTHER) = default

Move constructs a compoennt object that takes owership from OTHER.

#### Public Members

##### component& operator=(component&&) noexcept = default

Move assignment operator, takes ownership from other.

##### operator newtComponent() const

Conversion operator that returns the underlying newtComponent.

##### newtComponent get_owneship()

Returns the underlying newtComponent and releases ownership from the component object.

##### bool operator==(const component_t& other) const

Comparison operator that checks if two component objects have the same underlying newtComponent.

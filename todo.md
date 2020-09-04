* Add key binding to fork and exec pdf viewer on selected notes
* Add search functionality
* Handle window resizes
  - On a resize:
    1. Check if the window meets some minimum size requirement
    2. Redraw all the windows
  - The window class should have an option to turn text wrapping on/off
  - Calls to `Window::printf` should not throw unless the requested coordinates are outside the window

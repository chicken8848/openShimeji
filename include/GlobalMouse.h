#ifndef GLOBAL_MOUSE_H
#define GLOBAL_MOUSE_H

#include <stdexcept>
#include <utility> // For std::pair

// Platform detection
#if defined(__linux__)
#define PLATFORM_LINUX
#include <X11/Xlib.h>
#elif defined(__APPLE__)
#define PLATFORM_MACOS
#include <ApplicationServices/ApplicationServices.h>
#else
#error "Unsupported platform"
#endif

// Function to get global mouse coordinates
std::pair<int, int> GetGlobalMouseCoordinates() {
#ifdef PLATFORM_LINUX
  // Linux: Use X11 to get the global mouse coordinates
  Display *display = XOpenDisplay(nullptr);
  if (!display) {
    throw std::runtime_error("Failed to open X display");
  }

  Window root, child;
  int rootX, rootY, winX, winY;
  unsigned int mask;
  XQueryPointer(display, DefaultRootWindow(display), &root, &child, &rootX,
                &rootY, &winX, &winY, &mask);
  XCloseDisplay(display);

  return {rootX, rootY};
#elif defined(PLATFORM_MACOS)
  // macOS: Use CoreGraphics to get the global mouse coordinates
  CGEventRef event = CGEventCreate(nullptr);
  CGPoint cursorPos = CGEventGetLocation(event);
  CFRelease(event);

  return {static_cast<int>(cursorPos.x), static_cast<int>(cursorPos.y)};
#endif
}

#endif // GLOBAL_MOUSE_H

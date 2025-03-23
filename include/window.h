
#include <iostream>
#include <string>
#include <vector>

#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h> // Quartz for macOS
#include <unistd.h>                                  // getpid()
#elif __linux__
#include <X11/Xatom.h>
#include <X11/Xlib.h> // X11 for Linux
#include <X11/Xutil.h>
#include <unistd.h> // getpid()
#endif

struct WindowInfo {
  unsigned long id;
  std::string title;
  int x, y, width, height;
};

#ifdef __linux__
int handleXError(Display *d, XErrorEvent *e) {
  if (e->error_code == BadWindow) {
    fprintf(stderr, "Ignoring BadWindow error (window closed?)\n");
    return 0; // Suppress termination
  }
  return 1; // Pass other errors through
}
#endif

std::vector<WindowInfo> getVisibleWindows() {
  std::vector<WindowInfo> windows;

#ifdef __APPLE__
  // Get the current process ID
  pid_t selfPID = getpid();

  // Get all on-screen windows
  CFArrayRef windowList = CGWindowListCopyWindowInfo(
      kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
  if (!windowList)
    return windows;

  CFIndex numWindows = CFArrayGetCount(windowList);
  for (CFIndex i = 0; i < numWindows; ++i) {
    CFDictionaryRef windowInfo =
        (CFDictionaryRef)CFArrayGetValueAtIndex(windowList, i);

    // Get window owner PID
    CFNumberRef ownerPIDRef =
        (CFNumberRef)CFDictionaryGetValue(windowInfo, kCGWindowOwnerPID);
    pid_t ownerPID;
    CFNumberGetValue(ownerPIDRef, kCFNumberIntType, &ownerPID);

    // Exclude our own program
    if (ownerPID == selfPID)
      continue;

    // Get window ID
    CFNumberRef windowIDRef =
        (CFNumberRef)CFDictionaryGetValue(windowInfo, kCGWindowNumber);
    unsigned long windowID;
    CFNumberGetValue(windowIDRef, kCFNumberLongType, &windowID);

    // Get window title
    CFStringRef titleRef =
        (CFStringRef)CFDictionaryGetValue(windowInfo, kCGWindowName);
    char title[256] = "(No title)";
    if (titleRef) {
      CFStringGetCString(titleRef, title, sizeof(title), kCFStringEncodingUTF8);
    }

    // Get window bounds
    CFDictionaryRef boundsRef =
        (CFDictionaryRef)CFDictionaryGetValue(windowInfo, kCGWindowBounds);
    CGRect bounds;
    if (boundsRef) {
      CGRectMakeWithDictionaryRepresentation(boundsRef, &bounds);
    }

    // Store in list
    windows.push_back({windowID, title, (int)bounds.origin.x,
                       (int)bounds.origin.y, (int)bounds.size.width,
                       (int)bounds.size.height});
  }

  CFRelease(windowList);

#elif __linux__
  XSetErrorHandler(handleXError);
  Display *display = XOpenDisplay(nullptr);
  if (!display)
    return windows;

  Window root = DefaultRootWindow(display);
  Window root_return, parent_return;
  Window *children;
  unsigned int num_children;

  if (XQueryTree(display, root, &root_return, &parent_return, &children,
                 &num_children)) {
    pid_t selfPID = getpid();
    for (unsigned int i = 0; i < num_children; ++i) {
      Window win = children[i];
      XWindowAttributes attr;
      if (XGetWindowAttributes(display, win, &attr) &&
          attr.map_state == IsViewable) {
        // Get PID of window owner
        Atom pidAtom = XInternAtom(display, "_NET_WM_PID", True);
        pid_t ownerPID = -1;
        if (pidAtom != None) {
          Atom actualType;
          int actualFormat;
          unsigned long nItems, bytesAfter;
          unsigned char *prop = nullptr;
          if (XGetWindowProperty(display, win, pidAtom, 0, 1, False,
                                 XA_CARDINAL, &actualType, &actualFormat,
                                 &nItems, &bytesAfter, &prop) == Success) {
            if (prop) {
              ownerPID = *(pid_t *)prop;
              XFree(prop);
            }
          }
        }

        // Exclude our own program
        if (ownerPID == selfPID)
          continue;

        // Get window title
        char *windowName = nullptr;
        XFetchName(display, win, &windowName);
        std::string title = windowName ? windowName : "(No title)";
        if (windowName)
          XFree(windowName);

        // Store window info
        windows.push_back(
            {win, title, attr.x, attr.y, attr.width, attr.height});
      }
    }
    XFree(children);
  }

  XCloseDisplay(display);
#endif

  return windows;
}

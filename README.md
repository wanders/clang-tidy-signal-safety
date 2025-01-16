Signal Safety clang-tidy plugin
===============================

**This uses attribute plugin functionality that does not exist in
upstream clang but needs [aw-attr-plugins](https://github.com/wanders/llvm-project/commits/aw-attr-plugins/) branch**

This clang tidy plugin checks that any function that has the
"signal_safe" attribute only calls functions defined in
[signal-safety(7)](https://man7.org/linux/man-pages/man7/signal-safety.7.html)
or other functions marked with the "signal_safe" attribute.

It also checks that functions set up as signal handlers (with
`signal()` or `struct sigaction`) has the "signal_safe" attribute.


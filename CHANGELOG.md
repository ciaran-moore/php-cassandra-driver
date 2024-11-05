# 1.0.0

- Removed TSRM_D / TSRM_DC uses across all files.
- Amended all php5to7_/PHP5TO7_ Macros and definitions to php8_/PHP8_
- Removed all `PHP_MAJOR_VERSION > 5/7` checks, leaving only the latest compatible declarations.
- Removed `-Wno-extra-semi` build flag to surpress warning `cc1: warning: command-line option ‘-Wno-extra-semi’ is valid for C++/ObjC++ but not for C`
- Cast input values to `zend_throw_exception_ex` to `(long long int)` where the expected format is `%lld` or `LL_FORMAT` is used.
- Removed unused `arginfo` parameters.
- Fixed broken build from trailing `else` statement
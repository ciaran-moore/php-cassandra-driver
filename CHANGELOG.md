# 1.0.0

Forked last working tag of Datastax PHP Driver implementation.
Removed TSRM_D / TSRM_DC uses across all files.
Amended all php5to7_/PHP5TO7_ Macros and definitions to php8_/PHP8_
Removed all `PHP_MAJOR_VERSION > 5/7` checks, leaving only the latest compatible declarations.
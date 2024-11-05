#ifndef PHP_DRIVER_GLOBALS_H
#define PHP_DRIVER_GLOBALS_H

ZEND_BEGIN_MODULE_GLOBALS(php_driver)
  CassUuidGen  *uuid_gen;
  pid_t         uuid_gen_pid;
  unsigned int  persistent_clusters;
  unsigned int  persistent_sessions;
  unsigned int  persistent_prepared_statements;
  php8_zval  type_varchar;
  php8_zval  type_text;
  php8_zval  type_blob;
  php8_zval  type_ascii;
  php8_zval  type_bigint;
  php8_zval  type_counter;
  php8_zval  type_int;
  php8_zval  type_varint;
  php8_zval  type_boolean;
  php8_zval  type_decimal;
  php8_zval  type_double;
  php8_zval  type_float;
  php8_zval  type_inet;
  php8_zval  type_timestamp;
  php8_zval  type_date;
  php8_zval  type_time;
  php8_zval  type_uuid;
  php8_zval  type_timeuuid;
  php8_zval  type_smallint;
  php8_zval  type_tinyint;
  php8_zval  type_duration;
  zend_resource stmt;
ZEND_END_MODULE_GLOBALS(php_driver)

ZEND_EXTERN_MODULE_GLOBALS(php_driver)

#endif /* PHP_DRIVER_GLOBALS_H */

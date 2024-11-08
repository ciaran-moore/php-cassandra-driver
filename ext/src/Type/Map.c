/**
 * Copyright 2015-2017 DataStax, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "php_driver.h"
#include "php_driver_types.h"
#include "util/types.h"

#include <zend_smart_str.h>
#include "src/Map.h"

zend_class_entry *php_driver_type_map_ce = NULL;

PHP_METHOD(TypeMap, __construct)
{
  zend_throw_exception_ex(php_driver_logic_exception_ce, 0 TSRMLS_CC,
    "Instantiation of a " PHP_DRIVER_NAMESPACE "\\Type\\Map type is not supported."
  );
  return;
}

PHP_METHOD(TypeMap, name)
{
  if (zend_parse_parameters_none() == FAILURE) {
    return;
  }

  PHP8_RETVAL_STRING("map");
}

PHP_METHOD(TypeMap, keyType)
{
  php_driver_type *self;

  if (zend_parse_parameters_none() == FAILURE) {
    return;
  }

  self = PHP_DRIVER_GET_TYPE(getThis());
  RETURN_ZVAL(PHP8_ZVAL_MAYBE_P(self->data.map.key_type), 1, 0);
}

PHP_METHOD(TypeMap, valueType)
{
  php_driver_type *self;

  if (zend_parse_parameters_none() == FAILURE) {
    return;
  }

  self = PHP_DRIVER_GET_TYPE(getThis());
  RETURN_ZVAL(PHP8_ZVAL_MAYBE_P(self->data.map.value_type), 1, 0);
}

PHP_METHOD(TypeMap, __toString)
{
  php_driver_type *self;
  smart_str string = PHP8_SMART_STR_INIT;

  if (zend_parse_parameters_none() == FAILURE) {
    return;
  }

  self = PHP_DRIVER_GET_TYPE(getThis());

  php_driver_type_string(self, &string TSRMLS_CC);
  smart_str_0(&string);

  PHP8_RETVAL_STRING(PHP8_SMART_STR_VAL(string));
  smart_str_free(&string);
}

PHP_METHOD(TypeMap, create)
{
  php_driver_map *map;
  php8_zval_args args = NULL;
  int argc = 0, i;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "*",
                            &args, &argc) == FAILURE) {
    return;
  }

  if (argc % 2 == 1) {
    PHP8_MAYBE_EFREE(args);
    zend_throw_exception_ex(php_driver_invalid_argument_exception_ce, 0 TSRMLS_CC,
                            "Not enough values, maps can only be created " \
                            "from an even number of values, where each odd " \
                            "value is a key and each even value is a value, " \
                            "e.g create(key, value, key, value, key, value)");
    return;
  }

  object_init_ex(return_value, php_driver_map_ce);
  map = PHP_DRIVER_GET_MAP(return_value);

  PHP8_ZVAL_COPY(PHP8_ZVAL_MAYBE_P(map->type), getThis());

  if (argc > 0) {
    for (i = 0; i < argc; i += 2) {
      if (!php_driver_map_set(map,
                              PHP8_ZVAL_ARG(args[i]),
                              PHP8_ZVAL_ARG(args[i + 1]) TSRMLS_CC)) {
        PHP8_MAYBE_EFREE(args);
        return;
      }
    }
    PHP8_MAYBE_EFREE(args);
  }
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_none, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_tostring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_value, 0, ZEND_RETURN_VALUE, 0)
  ZEND_ARG_VARIADIC_INFO(0, value)
ZEND_END_ARG_INFO()

static zend_function_entry php_driver_type_map_methods[] = {
  PHP_ME(TypeMap, __construct, arginfo_none,  ZEND_ACC_PRIVATE)
  PHP_ME(TypeMap, name,        arginfo_none,  ZEND_ACC_PUBLIC)
  PHP_ME(TypeMap, keyType,     arginfo_none,  ZEND_ACC_PUBLIC)
  PHP_ME(TypeMap, valueType,   arginfo_none,  ZEND_ACC_PUBLIC)
  PHP_ME(TypeMap, __toString,  arginfo_tostring,  ZEND_ACC_PUBLIC)
  PHP_ME(TypeMap, create,      arginfo_value, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

static zend_object_handlers php_driver_type_map_handlers;

static HashTable *
php_driver_type_map_gc(
      zend_object *object,
      php8_zval_gc table, int *n
)
{
  *table = NULL;
  *n = 0;
  return zend_std_get_properties(object TSRMLS_CC);
}

static HashTable *
php_driver_type_map_properties(
      zend_object *object
)
{
  php_driver_type *self  = PHP8_ZEND_OBJECT_GET(type, object);
  HashTable      *props = zend_std_get_properties(object TSRMLS_CC);

  PHP8_ZEND_HASH_UPDATE(props,
                           "keyType", sizeof("keyType"),
                           PHP8_ZVAL_MAYBE_P(self->data.map.key_type), sizeof(zval));
  Z_ADDREF_P(PHP8_ZVAL_MAYBE_P(self->data.map.key_type));

  PHP8_ZEND_HASH_UPDATE(props,
                           "valueType", sizeof("valueType"),
                           PHP8_ZVAL_MAYBE_P(self->data.map.value_type), sizeof(zval));
  Z_ADDREF_P(PHP8_ZVAL_MAYBE_P(self->data.map.value_type));

  return props;
}

static int
php_driver_type_map_compare(zval *obj1, zval *obj2)
{
  ZEND_COMPARE_OBJECTS_FALLBACK(obj1, obj2);
  php_driver_type* type1 = PHP_DRIVER_GET_TYPE(obj1);
  php_driver_type* type2 = PHP_DRIVER_GET_TYPE(obj2);

  return php_driver_type_compare(type1, type2 TSRMLS_CC);
}

static void
php_driver_type_map_free(php8_zend_object_free *object)
{
  php_driver_type *self = PHP8_ZEND_OBJECT_GET(type, object);

  if (self->data_type) cass_data_type_free(self->data_type);
  PHP8_ZVAL_MAYBE_DESTROY(self->data.map.key_type);
  PHP8_ZVAL_MAYBE_DESTROY(self->data.map.value_type);

  zend_object_std_dtor(&self->zval TSRMLS_CC);
  PHP8_MAYBE_EFREE(self);
}

static php8_zend_object
php_driver_type_map_new(zend_class_entry *ce)
{
  php_driver_type *self =
      PHP8_ZEND_OBJECT_ECALLOC(type, ce);

  self->type = CASS_VALUE_TYPE_MAP;
  self->data_type = cass_data_type_new(self->type);
  PHP8_ZVAL_UNDEF(self->data.map.key_type);
  PHP8_ZVAL_UNDEF(self->data.map.value_type);

  PHP8_ZEND_OBJECT_INIT_EX(type, type_map, self, ce);
}

void php_driver_define_TypeMap()
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, PHP_DRIVER_NAMESPACE "\\Type\\Map", php_driver_type_map_methods);
  php_driver_type_map_ce = php8_zend_register_internal_class_ex(&ce, php_driver_type_ce);
  memcpy(&php_driver_type_map_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  php_driver_type_map_handlers.get_properties  = php_driver_type_map_properties;
  php_driver_type_map_handlers.get_gc          = php_driver_type_map_gc;
  php_driver_type_map_handlers.compare         = php_driver_type_map_compare;


  php_driver_type_map_ce->ce_flags     |= ZEND_ACC_FINAL;
  php_driver_type_map_ce->create_object = php_driver_type_map_new;
}

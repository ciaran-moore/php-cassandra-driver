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
#include "util/result.h"
#include "util/ref.h"
#include "util/types.h"

#include "DefaultIndex.h"

zend_class_entry *php_driver_default_index_ce = NULL;

php8_zval
php_driver_create_index(php_driver_ref *schema,
                           const CassIndexMeta *meta)
{
  php8_zval result;
  php_driver_index *index;
  const char *name;
  size_t name_length;

  PHP8_ZVAL_UNDEF(result);

  PHP8_ZVAL_MAYBE_MAKE(result);
  object_init_ex(PHP8_ZVAL_MAYBE_P(result), php_driver_default_index_ce);

  index = PHP_DRIVER_GET_INDEX(PHP8_ZVAL_MAYBE_P(result));
  index->meta   = meta;
  index->schema = php_driver_add_ref(schema);

  cass_index_meta_name(meta, &name, &name_length);
  PHP8_ZVAL_MAYBE_MAKE(index->name);
  PHP8_ZVAL_STRINGL(PHP8_ZVAL_MAYBE_P(index->name), name, name_length);

  return result;
}

PHP_METHOD(DefaultIndex, name)
{
  php_driver_index *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_INDEX(getThis());
  RETURN_ZVAL(PHP8_ZVAL_MAYBE_P(self->name), 1, 0);
}

PHP_METHOD(DefaultIndex, target)
{
  php_driver_index *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_INDEX(getThis());
  if (PHP8_ZVAL_IS_UNDEF(self->target)) {
    const char *target;
    size_t target_length;
    cass_index_meta_target(self->meta, &target, &target_length);
    PHP8_ZVAL_MAYBE_MAKE(self->target);
    PHP8_ZVAL_STRINGL(PHP8_ZVAL_MAYBE_P(self->target), target, target_length);
  }

  RETURN_ZVAL(PHP8_ZVAL_MAYBE_P(self->target), 1, 0);
}

PHP_METHOD(DefaultIndex, kind)
{
  php_driver_index *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_INDEX(getThis());
  if (PHP8_ZVAL_IS_UNDEF(self->kind)) {
    PHP8_ZVAL_MAYBE_MAKE(self->kind);
    switch (cass_index_meta_type(self->meta)) {
      case CASS_INDEX_TYPE_KEYS:
        PHP8_ZVAL_STRING(PHP8_ZVAL_MAYBE_P(self->kind), "keys");
        break;
      case CASS_INDEX_TYPE_CUSTOM:
        PHP8_ZVAL_STRING(PHP8_ZVAL_MAYBE_P(self->kind), "custom");
        break;
      case CASS_INDEX_TYPE_COMPOSITES:
        PHP8_ZVAL_STRING(PHP8_ZVAL_MAYBE_P(self->kind), "composites");
        break;
      default:
        PHP8_ZVAL_STRING(PHP8_ZVAL_MAYBE_P(self->kind), "unknown");
        break;
    }
  }

  RETURN_ZVAL(PHP8_ZVAL_MAYBE_P(self->kind), 1, 0);
}

void php_driver_index_build_option(php_driver_index *index)
{
  const CassValue* options;

  PHP8_ZVAL_MAYBE_MAKE(index->options);
  array_init(PHP8_ZVAL_MAYBE_P(index->options));
  options = cass_index_meta_options(index->meta);
  if (options) {
    CassIterator* iterator = cass_iterator_from_map(options);
    while (cass_iterator_next(iterator)) {
      const char* key_str;
      size_t key_str_length;
      const char* value_str;
      size_t value_str_length;
      const CassValue* key = cass_iterator_get_map_key(iterator);
      const CassValue* value = cass_iterator_get_map_value(iterator);

      cass_value_get_string(key, &key_str, &key_str_length);
      cass_value_get_string(value, &value_str, &value_str_length);
      PHP8_ADD_ASSOC_STRINGL_EX(PHP8_ZVAL_MAYBE_P(index->options),
                                   key_str, key_str_length + 1,
                                   value_str, value_str_length);
    }
  }
}

PHP_METHOD(DefaultIndex, option)
{
  char *name;
  php8_size name_len;
  php_driver_index *self;
  php8_zval* result;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
                            &name, &name_len) == FAILURE) {
    return;
  }

  self = PHP_DRIVER_GET_INDEX(getThis());
  if (PHP8_ZVAL_IS_UNDEF(self->options)) {
    php_driver_index_build_option(self);
  }

  if (PHP8_ZEND_HASH_FIND(PHP8_Z_ARRVAL_MAYBE_P(self->options),
                         name, name_len + 1,
                         result)) {
    RETURN_ZVAL(PHP8_ZVAL_MAYBE_DEREF(result), 1, 0);
  }
  RETURN_FALSE;
}

PHP_METHOD(DefaultIndex, options)
{
  php_driver_index *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_INDEX(getThis());
  if (PHP8_ZVAL_IS_UNDEF(self->options)) {
    php_driver_index_build_option(self);
  }

  RETURN_ZVAL(PHP8_ZVAL_MAYBE_P(self->options), 1, 0);
}

PHP_METHOD(DefaultIndex, className)
{
  php_driver_index *self;
  php8_zval* result;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_INDEX(getThis());
  if (PHP8_ZVAL_IS_UNDEF(self->options)) {
    php_driver_index_build_option(self);
  }

  if (PHP8_ZEND_HASH_FIND(PHP8_Z_ARRVAL_MAYBE_P(self->options),
                         "class_name", sizeof("class_name"),
                         result)) {
    RETURN_ZVAL(PHP8_ZVAL_MAYBE_DEREF(result), 1, 0);
  }
  RETURN_FALSE;
}

PHP_METHOD(DefaultIndex, isCustom)
{
  php_driver_index *self;
  int is_custom;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_INDEX(getThis());
  if (PHP8_ZVAL_IS_UNDEF(self->options)) {
    php_driver_index_build_option(self);
  }

  is_custom =
      PHP8_ZEND_HASH_EXISTS(PHP8_Z_ARRVAL_MAYBE_P(self->options),
                               "class_name", sizeof("class_name"));
  RETURN_BOOL(is_custom);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_name, 0, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_none, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_driver_default_index_methods[] = {
  PHP_ME(DefaultIndex, name, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultIndex, kind, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultIndex, target, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultIndex, option, arginfo_name, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultIndex, options, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultIndex, className, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultIndex, isCustom, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

static zend_object_handlers php_driver_default_index_handlers;

static HashTable *
php_driver_type_default_index_gc(
      zend_object *object,
      php8_zval_gc table, int *n
)
{
  *table = NULL;
  *n = 0;
  return zend_std_get_properties(object TSRMLS_CC);
}

static HashTable *
php_driver_default_index_properties(
      zend_object *object
)
{
  HashTable *props = zend_std_get_properties(object TSRMLS_CC);

  return props;
}

static int
php_driver_default_index_compare(zval *obj1, zval *obj2)
{
  ZEND_COMPARE_OBJECTS_FALLBACK(obj1, obj2);
  if (Z_OBJCE_P(obj1) != Z_OBJCE_P(obj2))
    return 1; /* different classes */

  return Z_OBJ_HANDLE_P(obj1) != Z_OBJ_HANDLE_P(obj1);
}

static void
php_driver_default_index_free(php8_zend_object_free *object)
{
  php_driver_index *self = PHP8_ZEND_OBJECT_GET(index, object);

  PHP8_ZVAL_MAYBE_DESTROY(self->name);
  PHP8_ZVAL_MAYBE_DESTROY(self->kind);
  PHP8_ZVAL_MAYBE_DESTROY(self->target);
  PHP8_ZVAL_MAYBE_DESTROY(self->options);

  if (self->schema) {
    php_driver_del_ref(&self->schema);
    self->schema = NULL;
  }
  self->meta = NULL;

  zend_object_std_dtor(&self->zval TSRMLS_CC);
  PHP8_MAYBE_EFREE(self);
}

static php8_zend_object
php_driver_default_index_new(zend_class_entry *ce)
{
  php_driver_index *self =
      PHP8_ZEND_OBJECT_ECALLOC(index, ce);

  PHP8_ZVAL_UNDEF(self->name);
  PHP8_ZVAL_UNDEF(self->kind);
  PHP8_ZVAL_UNDEF(self->target);
  PHP8_ZVAL_UNDEF(self->options);

  self->schema = NULL;
  self->meta = NULL;

  PHP8_ZEND_OBJECT_INIT_EX(index, default_index, self, ce);
}

void php_driver_define_DefaultIndex()
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, PHP_DRIVER_NAMESPACE "\\DefaultIndex", php_driver_default_index_methods);
  php_driver_default_index_ce = zend_register_internal_class(&ce TSRMLS_CC);
  zend_class_implements(php_driver_default_index_ce TSRMLS_CC, 1, php_driver_index_ce);
  php_driver_default_index_ce->ce_flags     |= ZEND_ACC_FINAL;
  php_driver_default_index_ce->create_object = php_driver_default_index_new;

  memcpy(&php_driver_default_index_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  php_driver_default_index_handlers.get_properties  = php_driver_default_index_properties;
  php_driver_default_index_handlers.get_gc          = php_driver_type_default_index_gc;
  php_driver_default_index_handlers.compare         = php_driver_default_index_compare;
  php_driver_default_index_handlers.clone_obj = NULL;
}

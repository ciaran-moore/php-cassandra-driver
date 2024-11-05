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
#include "util/collections.h"
#include "util/hash.h"
#include "util/types.h"

#include "src/Type/Tuple.h"
#include "src/Tuple.h"

#include <zend_hash.h>

zend_class_entry *php_driver_tuple_ce = NULL;

void
php_driver_tuple_set(php_driver_tuple *tuple, ulong index, zval *object)
{
  PHP8_ZEND_HASH_INDEX_UPDATE(&tuple->values, index, object, sizeof(zval *));
  Z_TRY_ADDREF_P(object);
  tuple->dirty = 1;
}

static void
php_driver_tuple_populate(php_driver_tuple *tuple, zval *array)
{
  php8_ulong index;
  php_driver_type *type;
  php8_zval *current;
  php8_zval null;

  PHP8_ZVAL_MAYBE_MAKE(null);
  ZVAL_NULL(PHP8_ZVAL_MAYBE_P(null));

  type = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(tuple->type));

  PHP8_ZEND_HASH_FOREACH_NUM_KEY_VAL(&type->data.tuple.types, index, current) {
    php8_zval *value = NULL;
    (void) current;
    if (PHP8_ZEND_HASH_INDEX_FIND(&tuple->values, index, value)) {
      if (add_next_index_zval(array, PHP8_ZVAL_MAYBE_DEREF(value)) == SUCCESS)
        Z_TRY_ADDREF_P(PHP8_ZVAL_MAYBE_DEREF(value));
      else
        break;
    } else {
      if (add_next_index_zval(array, PHP8_ZVAL_MAYBE_P(null)) == SUCCESS)
        Z_TRY_ADDREF_P(PHP8_ZVAL_MAYBE_P(null));
      else
        break;
    }
  } PHP8_ZEND_HASH_FOREACH_END(&type->data.tuple.types);

  zval_ptr_dtor(&null);
}

/* {{{ Tuple::__construct(types) */
PHP_METHOD(Tuple, __construct)
{
  php_driver_tuple *self;
  php_driver_type *type;
  HashTable *types;
  php8_zval *current;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "h", &types) == FAILURE) {
    return;
  }

  self = PHP_DRIVER_GET_TUPLE(getThis());
  self->type = php_driver_type_tuple(TSRMLS_C);
  type = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(self->type));

  PHP8_ZEND_HASH_FOREACH_VAL(types, current) {
    zval *sub_type = PHP8_ZVAL_MAYBE_DEREF(current);
    php8_zval scalar_type;

    if (Z_TYPE_P(sub_type) == IS_STRING) {
      CassValueType value_type;
      if (!php_driver_value_type(Z_STRVAL_P(sub_type), &value_type TSRMLS_CC)) {
        return;
      }
      scalar_type = php_driver_type_scalar(value_type TSRMLS_CC);
      if (!php_driver_type_tuple_add(type,
                                        PHP8_ZVAL_MAYBE_P(scalar_type) TSRMLS_CC)) {
        return;
      }
    } else if (Z_TYPE_P(sub_type) == IS_OBJECT &&
               instanceof_function(Z_OBJCE_P(sub_type), php_driver_type_ce TSRMLS_CC)) {
      if (!php_driver_type_validate(sub_type, "type" TSRMLS_CC)) {
        return;
      }
      if (php_driver_type_tuple_add(type,
                                        sub_type TSRMLS_CC)) {
        Z_ADDREF_P(sub_type);
      } else {
        return;
      }
    } else {
      INVALID_ARGUMENT(sub_type, "a string or an instance of " PHP_DRIVER_NAMESPACE "\\Type");
    }

  } PHP8_ZEND_HASH_FOREACH_END(types);
}
/* }}} */

/* {{{ Tuple::type() */
PHP_METHOD(Tuple, type)
{
  php_driver_tuple *self = PHP_DRIVER_GET_TUPLE(getThis());
  RETURN_ZVAL(PHP8_ZVAL_MAYBE_P(self->type), 1, 0);
}

/* {{{ Tuple::values() */
PHP_METHOD(Tuple, values)
{
  php_driver_tuple *self = NULL;
  array_init(return_value);
  self = PHP_DRIVER_GET_TUPLE(getThis());
  php_driver_tuple_populate(self, return_value TSRMLS_CC);
}
/* }}} */

/* {{{ Tuple::set(int, mixed) */
PHP_METHOD(Tuple, set)
{
  php_driver_tuple *self = NULL;
  long index;
  php_driver_type *type;
  php8_zval *sub_type;
  zval *value;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &index, &value) == FAILURE)
    return;

  self = PHP_DRIVER_GET_TUPLE(getThis());
  type = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(self->type));

  if (index < 0 || index >= zend_hash_num_elements(&type->data.tuple.types)) {
    zend_throw_exception_ex(php_driver_invalid_argument_exception_ce, 0 TSRMLS_CC,
                            "Index out of bounds");
    return;
  }

  if (!PHP8_ZEND_HASH_INDEX_FIND(&type->data.tuple.types, index, sub_type) ||
      !php_driver_validate_object(value,
                                  PHP8_ZVAL_MAYBE_DEREF(sub_type) TSRMLS_CC)) {
    return;
  }

  php_driver_tuple_set(self, index, value TSRMLS_CC);
}
/* }}} */

/* {{{ Tuple::get(int) */
PHP_METHOD(Tuple, get)
{
  php_driver_tuple *self = NULL;
  long index;
  php_driver_type *type;
  php8_zval *value;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE)
    return;

  self = PHP_DRIVER_GET_TUPLE(getThis());
  type = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(self->type));

  if (index < 0 || index >= zend_hash_num_elements(&type->data.tuple.types)) {
    zend_throw_exception_ex(php_driver_invalid_argument_exception_ce, 0 TSRMLS_CC,
                            "Index out of bounds");
    return;
  }

  if (PHP8_ZEND_HASH_INDEX_FIND(&self->values, index, value)) {
    RETURN_ZVAL(PHP8_ZVAL_MAYBE_DEREF(value), 1, 0);
  }
}
/* }}} */

/* {{{ Tuple::count() */
PHP_METHOD(Tuple, count)
{
  php_driver_tuple *self = PHP_DRIVER_GET_TUPLE(getThis());
  php_driver_type *type = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(self->type));
  RETURN_LONG(zend_hash_num_elements(&type->data.tuple.types));
}
/* }}} */

/* {{{ Tuple::current() */
PHP_METHOD(Tuple, current)
{
  php8_ulong index;
  php_driver_tuple *self = PHP_DRIVER_GET_TUPLE(getThis());
  php_driver_type *type = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(self->type));

  if (PHP8_ZEND_HASH_GET_CURRENT_KEY_EX(&type->data.tuple.types, NULL, &index, &self->pos) == HASH_KEY_IS_LONG) {
    php8_zval *value;
    if (PHP8_ZEND_HASH_INDEX_FIND(&self->values, index, value)) {
      RETURN_ZVAL(PHP8_ZVAL_MAYBE_DEREF(value), 1, 0);
    }
  }
}
/* }}} */

/* {{{ Tuple::key() */
PHP_METHOD(Tuple, key)
{
  php8_ulong index;
  php_driver_tuple *self = PHP_DRIVER_GET_TUPLE(getThis());
  php_driver_type *type = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(self->type));
  if (PHP8_ZEND_HASH_GET_CURRENT_KEY_EX(&type->data.tuple.types, NULL, &index, &self->pos) == HASH_KEY_IS_LONG) {
    RETURN_LONG(index);
  }
}
/* }}} */

/* {{{ Tuple::next() */
PHP_METHOD(Tuple, next)
{
  php_driver_tuple *self = PHP_DRIVER_GET_TUPLE(getThis());
  php_driver_type *type = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(self->type));
  zend_hash_move_forward_ex(&type->data.tuple.types, &self->pos);
}
/* }}} */

/* {{{ Tuple::valid() */
PHP_METHOD(Tuple, valid)
{
  php_driver_tuple *self = PHP_DRIVER_GET_TUPLE(getThis());
  php_driver_type *type = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(self->type));
  RETURN_BOOL(zend_hash_has_more_elements_ex(&type->data.tuple.types, &self->pos) == SUCCESS);
}
/* }}} */

/* {{{ Tuple::rewind() */
PHP_METHOD(Tuple, rewind)
{
  php_driver_tuple *self = PHP_DRIVER_GET_TUPLE(getThis());
  php_driver_type *type = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(self->type));
  zend_hash_internal_pointer_reset_ex(&type->data.tuple.types, &self->pos);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo__construct, 0, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0, types)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_value, 0, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_index, 0, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_none, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_index_value, 0, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0, index)
  ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_current, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_key, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_next, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_rewind, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_valid, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(arginfo_count, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_driver_tuple_methods[] = {
  PHP_ME(Tuple, __construct, arginfo__construct, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
  PHP_ME(Tuple, type, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(Tuple, values, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(Tuple, set, arginfo_index_value, ZEND_ACC_PUBLIC)
  PHP_ME(Tuple, get, arginfo_index, ZEND_ACC_PUBLIC)
  /* Countable */
  PHP_ME(Tuple, count, arginfo_count, ZEND_ACC_PUBLIC)
  /* Iterator */
  PHP_ME(Tuple, current, arginfo_current, ZEND_ACC_PUBLIC)
  PHP_ME(Tuple, key, arginfo_key, ZEND_ACC_PUBLIC)
  PHP_ME(Tuple, next, arginfo_next, ZEND_ACC_PUBLIC)
  PHP_ME(Tuple, valid, arginfo_valid, ZEND_ACC_PUBLIC)
  PHP_ME(Tuple, rewind, arginfo_rewind, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

static php_driver_value_handlers php_driver_tuple_handlers;

static HashTable *
php_driver_tuple_gc(
      zend_object *object,
      php8_zval_gc table, int *n
)
{
  *table = NULL;
  *n = 0;
  return zend_std_get_properties(object TSRMLS_CC);
}

static HashTable *
php_driver_tuple_properties(
      zend_object *object
)
{
  php8_zval values;

  php_driver_tuple  *self = PHP8_ZEND_OBJECT_GET(tuple, object);

  HashTable             *props = zend_std_get_properties(object TSRMLS_CC);

  PHP8_ZEND_HASH_UPDATE(props,
                           "type", sizeof("type"),
                           PHP8_ZVAL_MAYBE_P(self->type), sizeof(zval));
  Z_ADDREF_P(PHP8_ZVAL_MAYBE_P(self->type));

  PHP8_ZVAL_MAYBE_MAKE(values);
  array_init(PHP8_ZVAL_MAYBE_P(values));
  php_driver_tuple_populate(self, PHP8_ZVAL_MAYBE_P(values) TSRMLS_CC);
  PHP8_ZEND_HASH_UPDATE(props, "values", sizeof("values"), PHP8_ZVAL_MAYBE_P(values), sizeof(zval));

  return props;
}

static int
php_driver_tuple_compare(zval *obj1, zval *obj2)
{
  ZEND_COMPARE_OBJECTS_FALLBACK(obj1, obj2);

  HashPosition pos1;
  HashPosition pos2;
  php8_zval *current1;
  php8_zval *current2;
  php_driver_tuple *tuple1;
  php_driver_tuple *tuple2;
  php_driver_type *type1;
  php_driver_type *type2;
  int result;

  if (Z_OBJCE_P(obj1) != Z_OBJCE_P(obj2))
    return 1; /* different classes */

  tuple1 = PHP_DRIVER_GET_TUPLE(obj1);
  tuple2 = PHP_DRIVER_GET_TUPLE(obj2);

  type1 = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(tuple1->type));
  type2 = PHP_DRIVER_GET_TYPE(PHP8_ZVAL_MAYBE_P(tuple2->type));

  result = php_driver_type_compare(type1, type2 TSRMLS_CC);
  if (result != 0) return result;

  if (zend_hash_num_elements(&tuple1->values) != zend_hash_num_elements(&tuple2->values)) {
    return zend_hash_num_elements(&tuple1->values) < zend_hash_num_elements(&tuple2->values) ? -1 : 1;
  }

  zend_hash_internal_pointer_reset_ex(&tuple1->values, &pos1);
  zend_hash_internal_pointer_reset_ex(&tuple2->values, &pos2);

  while (PHP8_ZEND_HASH_GET_CURRENT_DATA_EX(&tuple1->values, current1, &pos1) &&
         PHP8_ZEND_HASH_GET_CURRENT_DATA_EX(&tuple2->values, current2, &pos2)) {
    result = php_driver_value_compare(PHP8_ZVAL_MAYBE_DEREF(current1),
                                         PHP8_ZVAL_MAYBE_DEREF(current2) TSRMLS_CC);
    if (result != 0) return result;
    zend_hash_move_forward_ex(&tuple1->values, &pos1);
    zend_hash_move_forward_ex(&tuple2->values, &pos2);
  }

  return 0;
}

static unsigned
php_driver_tuple_hash_value(zval *obj)
{
  php8_zval *current;
  unsigned hashv = 0;
  php_driver_tuple *self = PHP_DRIVER_GET_TUPLE(obj);

  if (!self->dirty) return self->hashv;

  PHP8_ZEND_HASH_FOREACH_VAL(&self->values, current) {
    hashv = php_driver_combine_hash(hashv,
                                       php_driver_value_hash(PHP8_ZVAL_MAYBE_DEREF(current) TSRMLS_CC));
  } PHP8_ZEND_HASH_FOREACH_END(&self->values);

  self->hashv = hashv;
  self->dirty = 0;

  return hashv;
}

static void
php_driver_tuple_free(php8_zend_object_free *object)
{
  php_driver_tuple *self =
      PHP8_ZEND_OBJECT_GET(tuple, object);

  zend_hash_destroy(&self->values);
  PHP8_ZVAL_MAYBE_DESTROY(self->type);

  zend_object_std_dtor(&self->zval TSRMLS_CC);
  PHP8_MAYBE_EFREE(self);
}

static php8_zend_object
php_driver_tuple_new(zend_class_entry *ce)
{
  php_driver_tuple *self =
      PHP8_ZEND_OBJECT_ECALLOC(tuple, ce);

  zend_hash_init(&self->values, 0, NULL, ZVAL_PTR_DTOR, 0);
  self->pos = HT_INVALID_IDX;
  self->dirty = 1;
  PHP8_ZVAL_UNDEF(self->type);

  PHP8_ZEND_OBJECT_INIT(tuple, self, ce);
}

void php_driver_define_Tuple()
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, PHP_DRIVER_NAMESPACE "\\Tuple", php_driver_tuple_methods);
  php_driver_tuple_ce = zend_register_internal_class(&ce TSRMLS_CC);
  zend_class_implements(php_driver_tuple_ce TSRMLS_CC, 1, php_driver_value_ce);
  memcpy(&php_driver_tuple_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  php_driver_tuple_handlers.std.get_properties  = php_driver_tuple_properties;
  php_driver_tuple_handlers.std.get_gc          = php_driver_tuple_gc;
  php_driver_tuple_handlers.std.compare = php_driver_tuple_compare;
  php_driver_tuple_ce->ce_flags |= ZEND_ACC_FINAL;
  php_driver_tuple_ce->create_object = php_driver_tuple_new;
  zend_class_implements(php_driver_tuple_ce TSRMLS_CC, 2, zend_ce_countable, zend_ce_iterator);
  php_driver_tuple_handlers.hash_value = php_driver_tuple_hash_value;
  php_driver_tuple_handlers.std.clone_obj = NULL;
}
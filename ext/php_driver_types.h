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

#ifndef PHP_DRIVER_TYPES_H
#define PHP_DRIVER_TYPES_H

#define PHP_DRIVER_BEGIN_OBJECT_TYPE(type_name) \
  typedef struct php_driver_##type_name##_ {

#define PHP_DRIVER_END_OBJECT_TYPE(type_name) \
    zend_object zval;                            \
  } php_driver_##type_name;                       \
  static inline php_driver_##type_name *php_driver_##type_name##_object_fetch(zend_object *obj) { \
    return (php_driver_##type_name *)((char *)obj - XtOffsetOf(php_driver_##type_name, zval));        \
  }


#define PHP_DRIVER_GET_NUMERIC(obj) php_driver_numeric_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_BLOB(obj) php_driver_blob_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_TIMESTAMP(obj) php_driver_timestamp_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_DATE(obj) php_driver_date_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_TIME(obj) php_driver_time_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_UUID(obj) php_driver_uuid_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_INET(obj) php_driver_inet_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_COLLECTION(obj) php_driver_collection_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_MAP(obj) php_driver_map_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_SET(obj) php_driver_set_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_TUPLE(obj) php_driver_tuple_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_USER_TYPE_VALUE(obj) php_driver_user_type_value_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_CLUSTER(obj) php_driver_cluster_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_STATEMENT(obj) php_driver_statement_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_EXECUTION_OPTIONS(obj) php_driver_execution_options_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_ROWS(obj) php_driver_rows_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_FUTURE_ROWS(obj) php_driver_future_rows_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_CLUSTER_BUILDER(obj) php_driver_cluster_builder_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_FUTURE_PREPARED_STATEMENT(obj) php_driver_future_prepared_statement_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_FUTURE_VALUE(obj) php_driver_future_value_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_FUTURE_CLOSE(obj) php_driver_future_close_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_FUTURE_SESSION(obj) php_driver_future_session_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_SESSION(obj) php_driver_session_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_SSL(obj) php_driver_ssl_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_SSL_BUILDER(obj) php_driver_ssl_builder_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_SCHEMA(obj) php_driver_schema_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_KEYSPACE(obj) php_driver_keyspace_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_TABLE(obj) php_driver_table_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_COLUMN(obj) php_driver_column_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_INDEX(obj) php_driver_index_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_MATERIALIZED_VIEW(obj) php_driver_materialized_view_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_FUNCTION(obj) php_driver_function_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_AGGREGATE(obj) php_driver_aggregate_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_TYPE(obj) php_driver_type_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_RETRY_POLICY(obj) php_driver_retry_policy_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_TIMESTAMP_GEN(obj) php_driver_timestamp_gen_object_fetch(Z_OBJ_P(obj))
#define PHP_DRIVER_GET_DURATION(obj) php_driver_duration_object_fetch(Z_OBJ_P(obj))

typedef enum {
  PHP_DRIVER_BIGINT,
  PHP_DRIVER_DECIMAL,
  PHP_DRIVER_FLOAT,
  PHP_DRIVER_VARINT,
  PHP_DRIVER_SMALLINT,
  PHP_DRIVER_TINYINT
} php_driver_numeric_type;

PHP_DRIVER_BEGIN_OBJECT_TYPE(numeric)
  php_driver_numeric_type type;
  union {
    struct {
      cass_int8_t value;
    } tinyint;
    struct {
      cass_int16_t value;
    } smallint;
    struct {
      cass_int64_t value;
    } bigint;
    struct {
      cass_float_t value;
    } floating;
    struct {
      mpz_t value;
    } varint;
    struct {
      mpz_t value;
      long scale;
    } decimal;
  } data;
PHP_DRIVER_END_OBJECT_TYPE(numeric)

PHP_DRIVER_BEGIN_OBJECT_TYPE(timestamp)
    cass_int64_t timestamp;
PHP_DRIVER_END_OBJECT_TYPE(timestamp)

PHP_DRIVER_BEGIN_OBJECT_TYPE(date)
    cass_uint32_t date;
PHP_DRIVER_END_OBJECT_TYPE(date)

PHP_DRIVER_BEGIN_OBJECT_TYPE(time)
    cass_int64_t time;
PHP_DRIVER_END_OBJECT_TYPE(time)

PHP_DRIVER_BEGIN_OBJECT_TYPE(blob)
  cass_byte_t *data;
  size_t size;
PHP_DRIVER_END_OBJECT_TYPE(blob)

PHP_DRIVER_BEGIN_OBJECT_TYPE(uuid)
  CassUuid uuid;
PHP_DRIVER_END_OBJECT_TYPE(uuid)

PHP_DRIVER_BEGIN_OBJECT_TYPE(inet)
  CassInet inet;
PHP_DRIVER_END_OBJECT_TYPE(inet)

PHP_DRIVER_BEGIN_OBJECT_TYPE(duration)
  cass_int32_t months;
  cass_int32_t days;
  cass_int64_t nanos;
PHP_DRIVER_END_OBJECT_TYPE(duration)

PHP_DRIVER_BEGIN_OBJECT_TYPE(collection)
  php8_zval type;
  HashTable values;
  unsigned hashv;
  int dirty;
PHP_DRIVER_END_OBJECT_TYPE(collection)

typedef struct php_driver_map_entry_ php_driver_map_entry;

PHP_DRIVER_BEGIN_OBJECT_TYPE(map)
  php8_zval type;
  php_driver_map_entry *entries;
  unsigned hashv;
  int dirty;
  php_driver_map_entry *iter_curr;
  php_driver_map_entry *iter_temp;
PHP_DRIVER_END_OBJECT_TYPE(map)

typedef struct php_driver_set_entry_ php_driver_set_entry;

PHP_DRIVER_BEGIN_OBJECT_TYPE(set)
  php8_zval type;
  php_driver_set_entry *entries;
  unsigned hashv;
  int dirty;
  php_driver_set_entry *iter_curr;
  php_driver_set_entry *iter_temp;
  int iter_index;
PHP_DRIVER_END_OBJECT_TYPE(set)

PHP_DRIVER_BEGIN_OBJECT_TYPE(tuple)
  php8_zval type;
  HashTable values;
  HashPosition pos;
  unsigned hashv;
  int dirty;
PHP_DRIVER_END_OBJECT_TYPE(tuple)

PHP_DRIVER_BEGIN_OBJECT_TYPE(user_type_value)
  php8_zval type;
  HashTable values;
  HashPosition pos;
  unsigned hashv;
  int dirty;
PHP_DRIVER_END_OBJECT_TYPE(user_type_value)

PHP_DRIVER_BEGIN_OBJECT_TYPE(cluster)
  cass_byte_t *data;
  CassCluster *cluster;
  long default_consistency;
  int default_page_size;
  php8_zval default_timeout;
  cass_bool_t persist;
  char *hash_key;
  int hash_key_len;
PHP_DRIVER_END_OBJECT_TYPE(cluster)

typedef enum {
  PHP_DRIVER_SIMPLE_STATEMENT,
  PHP_DRIVER_PREPARED_STATEMENT,
  PHP_DRIVER_BATCH_STATEMENT
} php_driver_statement_type;

PHP_DRIVER_BEGIN_OBJECT_TYPE(statement)
  php_driver_statement_type type;
  union {
    struct {
      char *cql;
    } simple;
    struct {
      const CassPrepared *prepared;
    } prepared;
    struct {
      CassBatchType type;
      HashTable statements;
    } batch;
  } data;
PHP_DRIVER_END_OBJECT_TYPE(statement)

typedef struct {
  php8_zval statement;
  php8_zval arguments;
} php_driver_batch_statement_entry;

PHP_DRIVER_BEGIN_OBJECT_TYPE(execution_options)
  long consistency;
  long serial_consistency;
  int page_size;
  char *paging_state_token;
  size_t paging_state_token_size;
  php8_zval timeout;
  php8_zval arguments;
  php8_zval retry_policy;
  cass_int64_t timestamp;
PHP_DRIVER_END_OBJECT_TYPE(execution_options)

typedef enum {
  LOAD_BALANCING_DEFAULT = 0,
  LOAD_BALANCING_ROUND_ROBIN,
  LOAD_BALANCING_DC_AWARE_ROUND_ROBIN
} php_driver_load_balancing;

typedef void (*php_driver_free_function)(void *data);

typedef struct {
  size_t                  count;
  php_driver_free_function destruct;
  void                   *data;
} php_driver_ref;

PHP_DRIVER_BEGIN_OBJECT_TYPE(rows)
  php_driver_ref *statement;
  php_driver_ref *session;
  php8_zval rows;
  php8_zval next_rows;
  php_driver_ref *result;
  php_driver_ref *next_result;
  php8_zval future_next_page;
PHP_DRIVER_END_OBJECT_TYPE(rows)

PHP_DRIVER_BEGIN_OBJECT_TYPE(future_rows)
  php_driver_ref *statement;
  php_driver_ref *session;
  php8_zval rows;
  php_driver_ref *result;
  CassFuture *future;
PHP_DRIVER_END_OBJECT_TYPE(future_rows)

PHP_DRIVER_BEGIN_OBJECT_TYPE(cluster_builder)
  char *contact_points;
  int port;
  php_driver_load_balancing load_balancing_policy;
  char *local_dc;
  unsigned int used_hosts_per_remote_dc;
  cass_bool_t allow_remote_dcs_for_local_cl;
  cass_bool_t use_token_aware_routing;
  char *username;
  char *password;
  unsigned int connect_timeout;
  unsigned int request_timeout;
  php8_zval ssl_options;
  long default_consistency;
  int default_page_size;
  php8_zval default_timeout;
  cass_bool_t persist;
  int protocol_version;
  int io_threads;
  int core_connections_per_host;
  int max_connections_per_host;
  unsigned int reconnect_interval;
  cass_bool_t enable_latency_aware_routing;
  cass_bool_t enable_tcp_nodelay;
  cass_bool_t enable_tcp_keepalive;
  unsigned int tcp_keepalive_delay;
  php8_zval retry_policy;
  php8_zval timestamp_gen;
  cass_bool_t enable_schema;
  char *blacklist_hosts;
  char *whitelist_hosts;
  char *blacklist_dcs;
  char *whitelist_dcs;
  cass_bool_t enable_hostname_resolution;
  cass_bool_t enable_randomized_contact_points;
  unsigned int connection_heartbeat_interval;
PHP_DRIVER_END_OBJECT_TYPE(cluster_builder)

PHP_DRIVER_BEGIN_OBJECT_TYPE(future_prepared_statement)
  CassFuture *future;
  php8_zval prepared_statement;
PHP_DRIVER_END_OBJECT_TYPE(future_prepared_statement)

PHP_DRIVER_BEGIN_OBJECT_TYPE(future_value)
  php8_zval value;
PHP_DRIVER_END_OBJECT_TYPE(future_value)

PHP_DRIVER_BEGIN_OBJECT_TYPE(future_close)
  CassFuture *future;
PHP_DRIVER_END_OBJECT_TYPE(future_close)

PHP_DRIVER_BEGIN_OBJECT_TYPE(future_session)
  CassFuture *future;
  php_driver_ref *session;
  php8_zval default_session;
  cass_bool_t persist;
  char *hash_key;
  int hash_key_len;
  char *exception_message;
  CassError exception_code;
  char* session_keyspace;
  char* session_hash_key;
PHP_DRIVER_END_OBJECT_TYPE(future_session)

typedef struct {
  CassFuture *future;
  php_driver_ref *session;
} php_driver_psession;

typedef struct {
  CassFuture *future;
  php_driver_ref *ref;
} php_driver_pprepared_statement;

PHP_DRIVER_BEGIN_OBJECT_TYPE(session)
  php_driver_ref *session;
  long default_consistency;
  int default_page_size;
  char* keyspace;
  char* hash_key;
  php8_zval default_timeout;
  cass_bool_t persist;
PHP_DRIVER_END_OBJECT_TYPE(session)

PHP_DRIVER_BEGIN_OBJECT_TYPE(ssl)
  CassSsl *ssl;
PHP_DRIVER_END_OBJECT_TYPE(ssl)

PHP_DRIVER_BEGIN_OBJECT_TYPE(ssl_builder)
  int flags;
  char **trusted_certs;
  int trusted_certs_cnt;
  char *client_cert;
  char *private_key;
  char *passphrase;
PHP_DRIVER_END_OBJECT_TYPE(ssl_builder)

PHP_DRIVER_BEGIN_OBJECT_TYPE(schema)
  php_driver_ref *schema;
PHP_DRIVER_END_OBJECT_TYPE(schema)

PHP_DRIVER_BEGIN_OBJECT_TYPE(keyspace)
  php_driver_ref *schema;
  const CassKeyspaceMeta *meta;
PHP_DRIVER_END_OBJECT_TYPE(keyspace)

PHP_DRIVER_BEGIN_OBJECT_TYPE(table)
  php8_zval name;
  php8_zval options;
  php8_zval partition_key;
  php8_zval primary_key;
  php8_zval clustering_key;
  php8_zval clustering_order;
  php_driver_ref *schema;
  const CassTableMeta *meta;
PHP_DRIVER_END_OBJECT_TYPE(table)

PHP_DRIVER_BEGIN_OBJECT_TYPE(materialized_view)
  php8_zval name;
  php8_zval options;
  php8_zval partition_key;
  php8_zval primary_key;
  php8_zval clustering_key;
  php8_zval clustering_order;
  php8_zval base_table;
  php_driver_ref *schema;
  const CassMaterializedViewMeta *meta;
PHP_DRIVER_END_OBJECT_TYPE(materialized_view)

PHP_DRIVER_BEGIN_OBJECT_TYPE(column)
  php8_zval name;
  php8_zval type;
  int reversed;
  int frozen;
  php_driver_ref *schema;
  const CassColumnMeta *meta;
PHP_DRIVER_END_OBJECT_TYPE(column)

PHP_DRIVER_BEGIN_OBJECT_TYPE(index)
  php8_zval name;
  php8_zval kind;
  php8_zval target;
  php8_zval options;
  php_driver_ref *schema;
  const CassIndexMeta *meta;
PHP_DRIVER_END_OBJECT_TYPE(index)

PHP_DRIVER_BEGIN_OBJECT_TYPE(function)
  php8_zval simple_name;
  php8_zval arguments;
  php8_zval return_type;
  php8_zval signature;
  php8_zval language;
  php8_zval body;
  php_driver_ref *schema;
  const CassFunctionMeta *meta;
PHP_DRIVER_END_OBJECT_TYPE(function)

PHP_DRIVER_BEGIN_OBJECT_TYPE(aggregate)
  php8_zval simple_name;
  php8_zval argument_types;
  php8_zval state_function;
  php8_zval final_function;
  php8_zval initial_condition;
  php8_zval state_type;
  php8_zval return_type;
  php8_zval signature;
  php_driver_ref *schema;
  const CassAggregateMeta *meta;
PHP_DRIVER_END_OBJECT_TYPE(aggregate)

PHP_DRIVER_BEGIN_OBJECT_TYPE(type)
  CassValueType type;
  CassDataType *data_type;
  union {
    struct {
      php8_zval value_type;
    } collection;
    struct {
      php8_zval value_type;
    } set;
    struct {
      php8_zval key_type;
      php8_zval value_type;
    } map;
    struct {
      char *class_name;
    } custom;
    struct {
      char *keyspace;
      char *type_name;
      HashTable types;
    } udt;
    struct {
      HashTable types;
    } tuple;
  } data;
PHP_DRIVER_END_OBJECT_TYPE(type)

PHP_DRIVER_BEGIN_OBJECT_TYPE(retry_policy)
  CassRetryPolicy *policy;
PHP_DRIVER_END_OBJECT_TYPE(retry_policy)

PHP_DRIVER_BEGIN_OBJECT_TYPE(timestamp_gen)
  CassTimestampGen *gen;
PHP_DRIVER_END_OBJECT_TYPE(timestamp_gen)

typedef unsigned (*php_driver_value_hash_t)(zval *obj);

typedef struct {
  zend_object_handlers std;
  php_driver_value_hash_t hash_value;
} php_driver_value_handlers;

extern PHP_DRIVER_API zend_class_entry *php_driver_value_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_numeric_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_bigint_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_smallint_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_tinyint_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_blob_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_decimal_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_float_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_inet_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_timestamp_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_date_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_time_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_uuid_interface_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_uuid_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_timeuuid_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_varint_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_custom_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_duration_ce;

extern PHP_DRIVER_API zend_class_entry *php_driver_set_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_map_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_collection_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_tuple_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_user_type_value_ce;

/* Exceptions */
void php_driver_define_Exception();
void php_driver_define_InvalidArgumentException();
void php_driver_define_DomainException();
void php_driver_define_LogicException();
void php_driver_define_RuntimeException();
void php_driver_define_TimeoutException();
void php_driver_define_ExecutionException();
void php_driver_define_ReadTimeoutException();
void php_driver_define_WriteTimeoutException();
void php_driver_define_UnavailableException();
void php_driver_define_TruncateException();
void php_driver_define_ValidationException();
void php_driver_define_InvalidQueryException();
void php_driver_define_InvalidSyntaxException();
void php_driver_define_UnauthorizedException();
void php_driver_define_UnpreparedException();
void php_driver_define_ConfigurationException();
void php_driver_define_AlreadyExistsException();
void php_driver_define_AuthenticationException();
void php_driver_define_ProtocolException();
void php_driver_define_ServerException();
void php_driver_define_IsBootstrappingException();
void php_driver_define_OverloadedException();
void php_driver_define_DivideByZeroException();
void php_driver_define_RangeException();

/* Types */
void php_driver_define_Value();
void php_driver_define_Numeric();
void php_driver_define_Bigint();
void php_driver_define_Smallint();
void php_driver_define_Tinyint();
void php_driver_define_Blob();
void php_driver_define_Collection();
void php_driver_define_Decimal();
void php_driver_define_Float();
void php_driver_define_Inet();
void php_driver_define_Map();
void php_driver_define_Set();
void php_driver_define_Timestamp();
void php_driver_define_Date();
void php_driver_define_Time();
void php_driver_define_Tuple();
void php_driver_define_UserTypeValue();
void php_driver_define_UuidInterface();
void php_driver_define_Uuid();
void php_driver_define_Timeuuid();
void php_driver_define_Varint();
void php_driver_define_Custom();
void php_driver_define_Duration();

/* Classes */
extern PHP_DRIVER_API zend_class_entry *php_driver_core_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_cluster_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_default_cluster_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_cluster_builder_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_default_cluster_builder_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_ssl_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_ssl_builder_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_future_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_future_prepared_statement_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_future_rows_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_future_session_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_future_value_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_future_close_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_session_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_default_session_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_runtime_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_timeout_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_logic_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_domain_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_invalid_argument_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_server_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_overloaded_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_is_bootstrapping_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_execution_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_truncate_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_write_timeout_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_read_timeout_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_truncate_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_unavailable_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_validation_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_invalid_syntax_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_unauthorized_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_invalid_query_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_configuration_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_already_exists_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_unprepared_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_protocol_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_authentication_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_divide_by_zero_exception_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_range_exception_ce;

extern PHP_DRIVER_API zend_class_entry *php_driver_statement_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_simple_statement_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_prepared_statement_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_batch_statement_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_execution_options_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_rows_ce;

void php_driver_define_Core();
void php_driver_define_Cluster();
void php_driver_define_ClusterBuilder();
void php_driver_define_DefaultCluster();
void php_driver_define_Future();
void php_driver_define_FuturePreparedStatement();
void php_driver_define_FutureRows();
void php_driver_define_FutureSession();
void php_driver_define_FutureValue();
void php_driver_define_FutureClose();
void php_driver_define_Session();
void php_driver_define_DefaultSession();
void php_driver_define_SSLOptions();
void php_driver_define_SSLOptionsBuilder();
void php_driver_define_Statement();
void php_driver_define_SimpleStatement();
void php_driver_define_PreparedStatement();
void php_driver_define_BatchStatement();
void php_driver_define_ExecutionOptions();
void php_driver_define_Rows();

extern PHP_DRIVER_API zend_class_entry *php_driver_schema_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_default_schema_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_keyspace_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_default_keyspace_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_table_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_default_table_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_column_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_default_column_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_index_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_default_index_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_materialized_view_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_default_materialized_view_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_function_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_default_function_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_aggregate_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_default_aggregate_ce;

void php_driver_define_Schema();
void php_driver_define_DefaultSchema();
void php_driver_define_Keyspace();
void php_driver_define_DefaultKeyspace();
void php_driver_define_Table();
void php_driver_define_DefaultTable();
void php_driver_define_Column();
void php_driver_define_DefaultColumn();
void php_driver_define_Index();
void php_driver_define_DefaultIndex();
void php_driver_define_MaterializedView();
void php_driver_define_DefaultMaterializedView();
void php_driver_define_Function();
void php_driver_define_DefaultFunction();
void php_driver_define_Aggregate();
void php_driver_define_DefaultAggregate();

extern PHP_DRIVER_API zend_class_entry *php_driver_type_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_type_scalar_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_type_collection_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_type_set_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_type_map_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_type_tuple_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_type_user_type_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_type_custom_ce;

void php_driver_define_Type();
void php_driver_define_TypeScalar();
void php_driver_define_TypeCollection();
void php_driver_define_TypeSet();
void php_driver_define_TypeMap();
void php_driver_define_TypeTuple();
void php_driver_define_TypeUserType();
void php_driver_define_TypeCustom();

extern PHP_DRIVER_API zend_class_entry *php_driver_retry_policy_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_retry_policy_default_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_retry_policy_downgrading_consistency_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_retry_policy_fallthrough_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_retry_policy_logging_ce;

void php_driver_define_RetryPolicy();
void php_driver_define_RetryPolicyDefault();
void php_driver_define_RetryPolicyDowngradingConsistency();
void php_driver_define_RetryPolicyFallthrough();
void php_driver_define_RetryPolicyLogging();

extern PHP_DRIVER_API zend_class_entry *php_driver_timestamp_gen_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_timestamp_gen_monotonic_ce;
extern PHP_DRIVER_API zend_class_entry *php_driver_timestamp_gen_server_side_ce;

void php_driver_define_TimestampGenerator();
void php_driver_define_TimestampGeneratorMonotonic();
void php_driver_define_TimestampGeneratorServerSide();

extern int php_le_php_driver_cluster();
extern int php_le_php_driver_session();
extern int php_le_php_driver_prepared_statement();

#endif /* PHP_DRIVER_TYPES_H */

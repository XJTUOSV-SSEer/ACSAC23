#include "CryptoEnclave_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define ADD_ASSIGN_OVERFLOW(a, b) (	\
	((a) += (b)) < (b)	\
)


typedef struct ms_ecall_init_t {
	unsigned char* ms_keyF;
	size_t ms_len;
} ms_ecall_init_t;

typedef struct ms_ecall_addDoc_t {
	char* ms_doc_id;
	size_t ms_id_length;
	char* ms_content;
	int ms_content_length;
} ms_ecall_addDoc_t;

typedef struct ms_ecall_delDoc_t {
	char* ms_doc_id;
	size_t ms_id_length;
} ms_ecall_delDoc_t;

typedef struct ms_ecall_search_t {
	const char* ms_keyword;
	size_t ms_len;
} ms_ecall_search_t;

typedef struct ms_ecall_test_t {
	char* ms_encrypted_content;
	size_t ms_length_content;
} ms_ecall_test_t;

typedef struct ms_ecall_hash_sha256_test_t {
	unsigned char* ms_message;
	size_t ms_message_length;
} ms_ecall_hash_sha256_test_t;

typedef struct ms_ecall_get_Proof_ID_u_t {
	char* ms_ID_u;
	int ms_id_len;
	unsigned char* ms_h_u;
	int ms_h_u_len;
} ms_ecall_get_Proof_ID_u_t;

typedef struct ms_ecall_get_Proof_w_t {
	char* ms_w;
	int ms_w_len;
	unsigned char* ms_h_w;
	int ms_h_w_len;
} ms_ecall_get_Proof_w_t;

typedef struct ms_ecall_Enclave_search_t {
	char* ms_w;
	int ms_w_len;
	char* ms_id_u;
	int ms_id_u_len;
	unsigned char* ms_bs_w_out;
	int ms_bs_w_out_len;
	unsigned char* ms_bs_u_out;
	int ms_bs_u_out_len;
	unsigned char* ms_K_w;
	int ms_K_w_len;
	unsigned char* ms_K_u;
	int ms_K_u_len;
	int ms_c_w;
	int ms_c_u;
} ms_ecall_Enclave_search_t;

typedef struct ms_ocall_test2_t {
	char* ms_encrypted_content;
	size_t ms_length_content;
} ms_ocall_test2_t;

typedef struct ms_ocall_test_t {
	int* ms_mint;
	char* ms_mchar;
	char* ms_mstring;
	int ms_len;
} ms_ocall_test_t;

typedef struct ms_ocall_print_string_t {
	const char* ms_str;
} ms_ocall_print_string_t;

typedef struct ms_ocall_print_bytes_t {
	unsigned char* ms_ptr;
	uint32_t ms_len;
} ms_ocall_print_bytes_t;

typedef struct ms_ocall_transfer_encrypted_entries_t {
	const void* ms_t1_u_arr;
	const void* ms_t1_v_arr;
	const void* ms_t2_u_arr;
	const void* ms_t2_v_arr;
	int ms_pair_count;
	int ms_rand_size;
} ms_ocall_transfer_encrypted_entries_t;

typedef struct ms_ocall_retrieve_encrypted_doc_t {
	const char* ms_del_id;
	size_t ms_del_id_len;
	unsigned char* ms_encrypted_content;
	size_t ms_maxLen;
	int* ms_length_content;
	size_t ms_int_len;
} ms_ocall_retrieve_encrypted_doc_t;

typedef struct ms_ocall_del_encrypted_doc_t {
	const char* ms_del_id;
	size_t ms_del_id_len;
} ms_ocall_del_encrypted_doc_t;

typedef struct ms_ocall_retrieve_M_c_t {
	unsigned char* ms__u_prime;
	size_t ms__u_prime_size;
	unsigned char* ms__v_prime;
	size_t ms_maxLen;
	int* ms__v_prime_size;
	size_t ms_int_len;
} ms_ocall_retrieve_M_c_t;

typedef struct ms_ocall_del_M_c_value_t {
	const unsigned char* ms__u_prime;
	size_t ms__u_prime_size;
} ms_ocall_del_M_c_value_t;

typedef struct ms_ocall_query_tokens_entries_t {
	const void* ms_Q_w_u_arr;
	const void* ms_Q_w_id_arr;
	int ms_pair_count;
	int ms_rand_size;
} ms_ocall_query_tokens_entries_t;

typedef struct ms_ocall_dec_e_t {
	unsigned char* ms_sum_e;
	unsigned char* ms_res;
	int ms_dec;
} ms_ocall_dec_e_t;

typedef struct ms_ocall_send_Proof_res_t {
	unsigned char* ms_Proof_res;
	int ms_len;
} ms_ocall_send_Proof_res_t;

typedef struct ms_sgx_oc_cpuidex_t {
	int* ms_cpuinfo;
	int ms_leaf;
	int ms_subleaf;
} ms_sgx_oc_cpuidex_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_setwait_untrusted_events_ocall_t {
	int ms_retval;
	const void* ms_waiter;
	const void* ms_self;
} ms_sgx_thread_setwait_untrusted_events_ocall_t;

typedef struct ms_sgx_thread_set_multiple_untrusted_events_ocall_t {
	int ms_retval;
	const void** ms_waiters;
	size_t ms_total;
} ms_sgx_thread_set_multiple_untrusted_events_ocall_t;

static sgx_status_t SGX_CDECL sgx_ecall_init(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_init_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_init_t* ms = SGX_CAST(ms_ecall_init_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	unsigned char* _tmp_keyF = ms->ms_keyF;
	size_t _tmp_len = ms->ms_len;
	size_t _len_keyF = _tmp_len;
	unsigned char* _in_keyF = NULL;

	CHECK_UNIQUE_POINTER(_tmp_keyF, _len_keyF);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_keyF != NULL && _len_keyF != 0) {
		if ( _len_keyF % sizeof(*_tmp_keyF) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_keyF = (unsigned char*)malloc(_len_keyF);
		if (_in_keyF == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_keyF, _len_keyF, _tmp_keyF, _len_keyF)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ecall_init(_in_keyF, _tmp_len);

err:
	if (_in_keyF) free(_in_keyF);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_addDoc(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_addDoc_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_addDoc_t* ms = SGX_CAST(ms_ecall_addDoc_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_doc_id = ms->ms_doc_id;
	size_t _tmp_id_length = ms->ms_id_length;
	size_t _len_doc_id = _tmp_id_length;
	char* _in_doc_id = NULL;
	char* _tmp_content = ms->ms_content;
	int _tmp_content_length = ms->ms_content_length;
	size_t _len_content = _tmp_content_length;
	char* _in_content = NULL;

	CHECK_UNIQUE_POINTER(_tmp_doc_id, _len_doc_id);
	CHECK_UNIQUE_POINTER(_tmp_content, _len_content);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_doc_id != NULL && _len_doc_id != 0) {
		if ( _len_doc_id % sizeof(*_tmp_doc_id) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_doc_id = (char*)malloc(_len_doc_id);
		if (_in_doc_id == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_doc_id, _len_doc_id, _tmp_doc_id, _len_doc_id)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_content != NULL && _len_content != 0) {
		if ( _len_content % sizeof(*_tmp_content) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_content = (char*)malloc(_len_content);
		if (_in_content == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_content, _len_content, _tmp_content, _len_content)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ecall_addDoc(_in_doc_id, _tmp_id_length, _in_content, _tmp_content_length);

err:
	if (_in_doc_id) free(_in_doc_id);
	if (_in_content) free(_in_content);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_delDoc(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_delDoc_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_delDoc_t* ms = SGX_CAST(ms_ecall_delDoc_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_doc_id = ms->ms_doc_id;
	size_t _tmp_id_length = ms->ms_id_length;
	size_t _len_doc_id = _tmp_id_length;
	char* _in_doc_id = NULL;

	CHECK_UNIQUE_POINTER(_tmp_doc_id, _len_doc_id);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_doc_id != NULL && _len_doc_id != 0) {
		if ( _len_doc_id % sizeof(*_tmp_doc_id) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_doc_id = (char*)malloc(_len_doc_id);
		if (_in_doc_id == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_doc_id, _len_doc_id, _tmp_doc_id, _len_doc_id)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ecall_delDoc(_in_doc_id, _tmp_id_length);

err:
	if (_in_doc_id) free(_in_doc_id);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_search(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_search_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_search_t* ms = SGX_CAST(ms_ecall_search_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const char* _tmp_keyword = ms->ms_keyword;
	size_t _tmp_len = ms->ms_len;
	size_t _len_keyword = _tmp_len;
	char* _in_keyword = NULL;

	CHECK_UNIQUE_POINTER(_tmp_keyword, _len_keyword);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_keyword != NULL && _len_keyword != 0) {
		if ( _len_keyword % sizeof(*_tmp_keyword) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_keyword = (char*)malloc(_len_keyword);
		if (_in_keyword == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_keyword, _len_keyword, _tmp_keyword, _len_keyword)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ecall_search((const char*)_in_keyword, _tmp_len);

err:
	if (_in_keyword) free(_in_keyword);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_test(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_test_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_test_t* ms = SGX_CAST(ms_ecall_test_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_encrypted_content = ms->ms_encrypted_content;
	size_t _tmp_length_content = ms->ms_length_content;
	size_t _len_encrypted_content = _tmp_length_content;
	char* _in_encrypted_content = NULL;

	CHECK_UNIQUE_POINTER(_tmp_encrypted_content, _len_encrypted_content);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_encrypted_content != NULL && _len_encrypted_content != 0) {
		if ( _len_encrypted_content % sizeof(*_tmp_encrypted_content) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_encrypted_content = (char*)malloc(_len_encrypted_content);
		if (_in_encrypted_content == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_encrypted_content, _len_encrypted_content, _tmp_encrypted_content, _len_encrypted_content)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ecall_test(_in_encrypted_content, _tmp_length_content);

err:
	if (_in_encrypted_content) free(_in_encrypted_content);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_hash_sha256_test(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_hash_sha256_test_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_hash_sha256_test_t* ms = SGX_CAST(ms_ecall_hash_sha256_test_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	unsigned char* _tmp_message = ms->ms_message;
	size_t _tmp_message_length = ms->ms_message_length;
	size_t _len_message = _tmp_message_length;
	unsigned char* _in_message = NULL;

	CHECK_UNIQUE_POINTER(_tmp_message, _len_message);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_message != NULL && _len_message != 0) {
		if ( _len_message % sizeof(*_tmp_message) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_message = (unsigned char*)malloc(_len_message);
		if (_in_message == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_message, _len_message, _tmp_message, _len_message)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ecall_hash_sha256_test(_in_message, _tmp_message_length);

err:
	if (_in_message) free(_in_message);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_get_Proof_ID_u(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_get_Proof_ID_u_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_get_Proof_ID_u_t* ms = SGX_CAST(ms_ecall_get_Proof_ID_u_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_ID_u = ms->ms_ID_u;
	int _tmp_id_len = ms->ms_id_len;
	size_t _len_ID_u = _tmp_id_len;
	char* _in_ID_u = NULL;
	unsigned char* _tmp_h_u = ms->ms_h_u;
	int _tmp_h_u_len = ms->ms_h_u_len;
	size_t _len_h_u = _tmp_h_u_len;
	unsigned char* _in_h_u = NULL;

	CHECK_UNIQUE_POINTER(_tmp_ID_u, _len_ID_u);
	CHECK_UNIQUE_POINTER(_tmp_h_u, _len_h_u);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_ID_u != NULL && _len_ID_u != 0) {
		if ( _len_ID_u % sizeof(*_tmp_ID_u) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_ID_u = (char*)malloc(_len_ID_u);
		if (_in_ID_u == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_ID_u, _len_ID_u, _tmp_ID_u, _len_ID_u)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_h_u != NULL && _len_h_u != 0) {
		if ( _len_h_u % sizeof(*_tmp_h_u) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_h_u = (unsigned char*)malloc(_len_h_u);
		if (_in_h_u == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_h_u, _len_h_u, _tmp_h_u, _len_h_u)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ecall_get_Proof_ID_u(_in_ID_u, _tmp_id_len, _in_h_u, _tmp_h_u_len);

err:
	if (_in_ID_u) free(_in_ID_u);
	if (_in_h_u) free(_in_h_u);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_get_Proof_w(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_get_Proof_w_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_get_Proof_w_t* ms = SGX_CAST(ms_ecall_get_Proof_w_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_w = ms->ms_w;
	int _tmp_w_len = ms->ms_w_len;
	size_t _len_w = _tmp_w_len;
	char* _in_w = NULL;
	unsigned char* _tmp_h_w = ms->ms_h_w;
	int _tmp_h_w_len = ms->ms_h_w_len;
	size_t _len_h_w = _tmp_h_w_len;
	unsigned char* _in_h_w = NULL;

	CHECK_UNIQUE_POINTER(_tmp_w, _len_w);
	CHECK_UNIQUE_POINTER(_tmp_h_w, _len_h_w);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_w != NULL && _len_w != 0) {
		if ( _len_w % sizeof(*_tmp_w) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_w = (char*)malloc(_len_w);
		if (_in_w == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_w, _len_w, _tmp_w, _len_w)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_h_w != NULL && _len_h_w != 0) {
		if ( _len_h_w % sizeof(*_tmp_h_w) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_h_w = (unsigned char*)malloc(_len_h_w);
		if (_in_h_w == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_h_w, _len_h_w, _tmp_h_w, _len_h_w)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ecall_get_Proof_w(_in_w, _tmp_w_len, _in_h_w, _tmp_h_w_len);

err:
	if (_in_w) free(_in_w);
	if (_in_h_w) free(_in_h_w);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_Enclave_search(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_Enclave_search_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_Enclave_search_t* ms = SGX_CAST(ms_ecall_Enclave_search_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_w = ms->ms_w;
	int _tmp_w_len = ms->ms_w_len;
	size_t _len_w = _tmp_w_len;
	char* _in_w = NULL;
	char* _tmp_id_u = ms->ms_id_u;
	int _tmp_id_u_len = ms->ms_id_u_len;
	size_t _len_id_u = _tmp_id_u_len;
	char* _in_id_u = NULL;
	unsigned char* _tmp_bs_w_out = ms->ms_bs_w_out;
	int _tmp_bs_w_out_len = ms->ms_bs_w_out_len;
	size_t _len_bs_w_out = _tmp_bs_w_out_len;
	unsigned char* _in_bs_w_out = NULL;
	unsigned char* _tmp_bs_u_out = ms->ms_bs_u_out;
	int _tmp_bs_u_out_len = ms->ms_bs_u_out_len;
	size_t _len_bs_u_out = _tmp_bs_u_out_len;
	unsigned char* _in_bs_u_out = NULL;
	unsigned char* _tmp_K_w = ms->ms_K_w;
	int _tmp_K_w_len = ms->ms_K_w_len;
	size_t _len_K_w = _tmp_K_w_len;
	unsigned char* _in_K_w = NULL;
	unsigned char* _tmp_K_u = ms->ms_K_u;
	int _tmp_K_u_len = ms->ms_K_u_len;
	size_t _len_K_u = _tmp_K_u_len;
	unsigned char* _in_K_u = NULL;

	CHECK_UNIQUE_POINTER(_tmp_w, _len_w);
	CHECK_UNIQUE_POINTER(_tmp_id_u, _len_id_u);
	CHECK_UNIQUE_POINTER(_tmp_bs_w_out, _len_bs_w_out);
	CHECK_UNIQUE_POINTER(_tmp_bs_u_out, _len_bs_u_out);
	CHECK_UNIQUE_POINTER(_tmp_K_w, _len_K_w);
	CHECK_UNIQUE_POINTER(_tmp_K_u, _len_K_u);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_w != NULL && _len_w != 0) {
		if ( _len_w % sizeof(*_tmp_w) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_w = (char*)malloc(_len_w);
		if (_in_w == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_w, _len_w, _tmp_w, _len_w)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_id_u != NULL && _len_id_u != 0) {
		if ( _len_id_u % sizeof(*_tmp_id_u) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_id_u = (char*)malloc(_len_id_u);
		if (_in_id_u == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_id_u, _len_id_u, _tmp_id_u, _len_id_u)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_bs_w_out != NULL && _len_bs_w_out != 0) {
		if ( _len_bs_w_out % sizeof(*_tmp_bs_w_out) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_bs_w_out = (unsigned char*)malloc(_len_bs_w_out);
		if (_in_bs_w_out == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_bs_w_out, _len_bs_w_out, _tmp_bs_w_out, _len_bs_w_out)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_bs_u_out != NULL && _len_bs_u_out != 0) {
		if ( _len_bs_u_out % sizeof(*_tmp_bs_u_out) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_bs_u_out = (unsigned char*)malloc(_len_bs_u_out);
		if (_in_bs_u_out == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_bs_u_out, _len_bs_u_out, _tmp_bs_u_out, _len_bs_u_out)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_K_w != NULL && _len_K_w != 0) {
		if ( _len_K_w % sizeof(*_tmp_K_w) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_K_w = (unsigned char*)malloc(_len_K_w);
		if (_in_K_w == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_K_w, _len_K_w, _tmp_K_w, _len_K_w)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_K_u != NULL && _len_K_u != 0) {
		if ( _len_K_u % sizeof(*_tmp_K_u) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_K_u = (unsigned char*)malloc(_len_K_u);
		if (_in_K_u == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_K_u, _len_K_u, _tmp_K_u, _len_K_u)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ecall_Enclave_search(_in_w, _tmp_w_len, _in_id_u, _tmp_id_u_len, _in_bs_w_out, _tmp_bs_w_out_len, _in_bs_u_out, _tmp_bs_u_out_len, _in_K_w, _tmp_K_w_len, _in_K_u, _tmp_K_u_len, ms->ms_c_w, ms->ms_c_u);

err:
	if (_in_w) free(_in_w);
	if (_in_id_u) free(_in_id_u);
	if (_in_bs_w_out) free(_in_bs_w_out);
	if (_in_bs_u_out) free(_in_bs_u_out);
	if (_in_K_w) free(_in_K_w);
	if (_in_K_u) free(_in_K_u);
	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[9];
} g_ecall_table = {
	9,
	{
		{(void*)(uintptr_t)sgx_ecall_init, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_addDoc, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_delDoc, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_search, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_test, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_hash_sha256_test, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_get_Proof_ID_u, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_get_Proof_w, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_Enclave_search, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[17][9];
} g_dyn_entry_table = {
	17,
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_test2(char* encrypted_content, size_t length_content)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_encrypted_content = length_content;

	ms_ocall_test2_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_test2_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(encrypted_content, _len_encrypted_content);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (encrypted_content != NULL) ? _len_encrypted_content : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_test2_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_test2_t));
	ocalloc_size -= sizeof(ms_ocall_test2_t);

	if (encrypted_content != NULL) {
		ms->ms_encrypted_content = (char*)__tmp;
		if (_len_encrypted_content % sizeof(*encrypted_content) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, encrypted_content, _len_encrypted_content)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_encrypted_content);
		ocalloc_size -= _len_encrypted_content;
	} else {
		ms->ms_encrypted_content = NULL;
	}
	
	ms->ms_length_content = length_content;
	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_test(int* mint, char* mchar, char* mstring, int len)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_mint = 1 * sizeof(int);
	size_t _len_mchar = 1;
	size_t _len_mstring = len;

	ms_ocall_test_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_test_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(mint, _len_mint);
	CHECK_ENCLAVE_POINTER(mchar, _len_mchar);
	CHECK_ENCLAVE_POINTER(mstring, _len_mstring);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (mint != NULL) ? _len_mint : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (mchar != NULL) ? _len_mchar : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (mstring != NULL) ? _len_mstring : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_test_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_test_t));
	ocalloc_size -= sizeof(ms_ocall_test_t);

	if (mint != NULL) {
		ms->ms_mint = (int*)__tmp;
		if (_len_mint % sizeof(*mint) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, mint, _len_mint)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_mint);
		ocalloc_size -= _len_mint;
	} else {
		ms->ms_mint = NULL;
	}
	
	if (mchar != NULL) {
		ms->ms_mchar = (char*)__tmp;
		if (_len_mchar % sizeof(*mchar) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, mchar, _len_mchar)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_mchar);
		ocalloc_size -= _len_mchar;
	} else {
		ms->ms_mchar = NULL;
	}
	
	if (mstring != NULL) {
		ms->ms_mstring = (char*)__tmp;
		if (_len_mstring % sizeof(*mstring) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, mstring, _len_mstring)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_mstring);
		ocalloc_size -= _len_mstring;
	} else {
		ms->ms_mstring = NULL;
	}
	
	ms->ms_len = len;
	status = sgx_ocall(1, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_string(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_print_string_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_string_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_string_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_string_t));
	ocalloc_size -= sizeof(ms_ocall_print_string_t);

	if (str != NULL) {
		ms->ms_str = (const char*)__tmp;
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}
	
	status = sgx_ocall(2, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_bytes(unsigned char* ptr, uint32_t len)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_ptr = len;

	ms_ocall_print_bytes_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_bytes_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(ptr, _len_ptr);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (ptr != NULL) ? _len_ptr : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_bytes_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_bytes_t));
	ocalloc_size -= sizeof(ms_ocall_print_bytes_t);

	if (ptr != NULL) {
		ms->ms_ptr = (unsigned char*)__tmp;
		if (_len_ptr % sizeof(*ptr) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, ptr, _len_ptr)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_ptr);
		ocalloc_size -= _len_ptr;
	} else {
		ms->ms_ptr = NULL;
	}
	
	ms->ms_len = len;
	status = sgx_ocall(3, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_transfer_encrypted_entries(const void* t1_u_arr, const void* t1_v_arr, const void* t2_u_arr, const void* t2_v_arr, int pair_count, int rand_size)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_t1_u_arr = pair_count * rand_size;
	size_t _len_t1_v_arr = pair_count * rand_size;
	size_t _len_t2_u_arr = pair_count * rand_size;
	size_t _len_t2_v_arr = pair_count * rand_size;

	ms_ocall_transfer_encrypted_entries_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_transfer_encrypted_entries_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(t1_u_arr, _len_t1_u_arr);
	CHECK_ENCLAVE_POINTER(t1_v_arr, _len_t1_v_arr);
	CHECK_ENCLAVE_POINTER(t2_u_arr, _len_t2_u_arr);
	CHECK_ENCLAVE_POINTER(t2_v_arr, _len_t2_v_arr);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (t1_u_arr != NULL) ? _len_t1_u_arr : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (t1_v_arr != NULL) ? _len_t1_v_arr : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (t2_u_arr != NULL) ? _len_t2_u_arr : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (t2_v_arr != NULL) ? _len_t2_v_arr : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_transfer_encrypted_entries_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_transfer_encrypted_entries_t));
	ocalloc_size -= sizeof(ms_ocall_transfer_encrypted_entries_t);

	if (t1_u_arr != NULL) {
		ms->ms_t1_u_arr = (const void*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, t1_u_arr, _len_t1_u_arr)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_t1_u_arr);
		ocalloc_size -= _len_t1_u_arr;
	} else {
		ms->ms_t1_u_arr = NULL;
	}
	
	if (t1_v_arr != NULL) {
		ms->ms_t1_v_arr = (const void*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, t1_v_arr, _len_t1_v_arr)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_t1_v_arr);
		ocalloc_size -= _len_t1_v_arr;
	} else {
		ms->ms_t1_v_arr = NULL;
	}
	
	if (t2_u_arr != NULL) {
		ms->ms_t2_u_arr = (const void*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, t2_u_arr, _len_t2_u_arr)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_t2_u_arr);
		ocalloc_size -= _len_t2_u_arr;
	} else {
		ms->ms_t2_u_arr = NULL;
	}
	
	if (t2_v_arr != NULL) {
		ms->ms_t2_v_arr = (const void*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, t2_v_arr, _len_t2_v_arr)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_t2_v_arr);
		ocalloc_size -= _len_t2_v_arr;
	} else {
		ms->ms_t2_v_arr = NULL;
	}
	
	ms->ms_pair_count = pair_count;
	ms->ms_rand_size = rand_size;
	status = sgx_ocall(4, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_retrieve_encrypted_doc(const char* del_id, size_t del_id_len, unsigned char* encrypted_content, size_t maxLen, int* length_content, size_t int_len)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_del_id = del_id_len;
	size_t _len_encrypted_content = maxLen;
	size_t _len_length_content = int_len * sizeof(int);

	ms_ocall_retrieve_encrypted_doc_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_retrieve_encrypted_doc_t);
	void *__tmp = NULL;

	void *__tmp_encrypted_content = NULL;
	void *__tmp_length_content = NULL;

	CHECK_ENCLAVE_POINTER(del_id, _len_del_id);
	CHECK_ENCLAVE_POINTER(encrypted_content, _len_encrypted_content);
	CHECK_ENCLAVE_POINTER(length_content, _len_length_content);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (del_id != NULL) ? _len_del_id : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (encrypted_content != NULL) ? _len_encrypted_content : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (length_content != NULL) ? _len_length_content : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_retrieve_encrypted_doc_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_retrieve_encrypted_doc_t));
	ocalloc_size -= sizeof(ms_ocall_retrieve_encrypted_doc_t);

	if (del_id != NULL) {
		ms->ms_del_id = (const char*)__tmp;
		if (_len_del_id % sizeof(*del_id) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, del_id, _len_del_id)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_del_id);
		ocalloc_size -= _len_del_id;
	} else {
		ms->ms_del_id = NULL;
	}
	
	ms->ms_del_id_len = del_id_len;
	if (encrypted_content != NULL) {
		ms->ms_encrypted_content = (unsigned char*)__tmp;
		__tmp_encrypted_content = __tmp;
		if (_len_encrypted_content % sizeof(*encrypted_content) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp_encrypted_content, 0, _len_encrypted_content);
		__tmp = (void *)((size_t)__tmp + _len_encrypted_content);
		ocalloc_size -= _len_encrypted_content;
	} else {
		ms->ms_encrypted_content = NULL;
	}
	
	ms->ms_maxLen = maxLen;
	if (length_content != NULL) {
		ms->ms_length_content = (int*)__tmp;
		__tmp_length_content = __tmp;
		if (_len_length_content % sizeof(*length_content) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp_length_content, 0, _len_length_content);
		__tmp = (void *)((size_t)__tmp + _len_length_content);
		ocalloc_size -= _len_length_content;
	} else {
		ms->ms_length_content = NULL;
	}
	
	ms->ms_int_len = int_len;
	status = sgx_ocall(5, ms);

	if (status == SGX_SUCCESS) {
		if (encrypted_content) {
			if (memcpy_s((void*)encrypted_content, _len_encrypted_content, __tmp_encrypted_content, _len_encrypted_content)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (length_content) {
			if (memcpy_s((void*)length_content, _len_length_content, __tmp_length_content, _len_length_content)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_del_encrypted_doc(const char* del_id, size_t del_id_len)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_del_id = del_id_len;

	ms_ocall_del_encrypted_doc_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_del_encrypted_doc_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(del_id, _len_del_id);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (del_id != NULL) ? _len_del_id : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_del_encrypted_doc_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_del_encrypted_doc_t));
	ocalloc_size -= sizeof(ms_ocall_del_encrypted_doc_t);

	if (del_id != NULL) {
		ms->ms_del_id = (const char*)__tmp;
		if (_len_del_id % sizeof(*del_id) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, del_id, _len_del_id)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_del_id);
		ocalloc_size -= _len_del_id;
	} else {
		ms->ms_del_id = NULL;
	}
	
	ms->ms_del_id_len = del_id_len;
	status = sgx_ocall(6, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_retrieve_M_c(unsigned char* _u_prime, size_t _u_prime_size, unsigned char* _v_prime, size_t maxLen, int* _v_prime_size, size_t int_len)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len__u_prime = _u_prime_size;
	size_t _len__v_prime = maxLen;
	size_t _len__v_prime_size = int_len * sizeof(int);

	ms_ocall_retrieve_M_c_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_retrieve_M_c_t);
	void *__tmp = NULL;

	void *__tmp__v_prime = NULL;
	void *__tmp__v_prime_size = NULL;

	CHECK_ENCLAVE_POINTER(_u_prime, _len__u_prime);
	CHECK_ENCLAVE_POINTER(_v_prime, _len__v_prime);
	CHECK_ENCLAVE_POINTER(_v_prime_size, _len__v_prime_size);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (_u_prime != NULL) ? _len__u_prime : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (_v_prime != NULL) ? _len__v_prime : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (_v_prime_size != NULL) ? _len__v_prime_size : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_retrieve_M_c_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_retrieve_M_c_t));
	ocalloc_size -= sizeof(ms_ocall_retrieve_M_c_t);

	if (_u_prime != NULL) {
		ms->ms__u_prime = (unsigned char*)__tmp;
		if (_len__u_prime % sizeof(*_u_prime) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, _u_prime, _len__u_prime)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len__u_prime);
		ocalloc_size -= _len__u_prime;
	} else {
		ms->ms__u_prime = NULL;
	}
	
	ms->ms__u_prime_size = _u_prime_size;
	if (_v_prime != NULL) {
		ms->ms__v_prime = (unsigned char*)__tmp;
		__tmp__v_prime = __tmp;
		if (_len__v_prime % sizeof(*_v_prime) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp__v_prime, 0, _len__v_prime);
		__tmp = (void *)((size_t)__tmp + _len__v_prime);
		ocalloc_size -= _len__v_prime;
	} else {
		ms->ms__v_prime = NULL;
	}
	
	ms->ms_maxLen = maxLen;
	if (_v_prime_size != NULL) {
		ms->ms__v_prime_size = (int*)__tmp;
		__tmp__v_prime_size = __tmp;
		if (_len__v_prime_size % sizeof(*_v_prime_size) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp__v_prime_size, 0, _len__v_prime_size);
		__tmp = (void *)((size_t)__tmp + _len__v_prime_size);
		ocalloc_size -= _len__v_prime_size;
	} else {
		ms->ms__v_prime_size = NULL;
	}
	
	ms->ms_int_len = int_len;
	status = sgx_ocall(7, ms);

	if (status == SGX_SUCCESS) {
		if (_v_prime) {
			if (memcpy_s((void*)_v_prime, _len__v_prime, __tmp__v_prime, _len__v_prime)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (_v_prime_size) {
			if (memcpy_s((void*)_v_prime_size, _len__v_prime_size, __tmp__v_prime_size, _len__v_prime_size)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_del_M_c_value(const unsigned char* _u_prime, size_t _u_prime_size)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len__u_prime = _u_prime_size;

	ms_ocall_del_M_c_value_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_del_M_c_value_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(_u_prime, _len__u_prime);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (_u_prime != NULL) ? _len__u_prime : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_del_M_c_value_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_del_M_c_value_t));
	ocalloc_size -= sizeof(ms_ocall_del_M_c_value_t);

	if (_u_prime != NULL) {
		ms->ms__u_prime = (const unsigned char*)__tmp;
		if (_len__u_prime % sizeof(*_u_prime) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, _u_prime, _len__u_prime)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len__u_prime);
		ocalloc_size -= _len__u_prime;
	} else {
		ms->ms__u_prime = NULL;
	}
	
	ms->ms__u_prime_size = _u_prime_size;
	status = sgx_ocall(8, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_query_tokens_entries(const void* Q_w_u_arr, const void* Q_w_id_arr, int pair_count, int rand_size)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_Q_w_u_arr = pair_count * rand_size;
	size_t _len_Q_w_id_arr = pair_count * rand_size;

	ms_ocall_query_tokens_entries_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_query_tokens_entries_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(Q_w_u_arr, _len_Q_w_u_arr);
	CHECK_ENCLAVE_POINTER(Q_w_id_arr, _len_Q_w_id_arr);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (Q_w_u_arr != NULL) ? _len_Q_w_u_arr : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (Q_w_id_arr != NULL) ? _len_Q_w_id_arr : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_query_tokens_entries_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_query_tokens_entries_t));
	ocalloc_size -= sizeof(ms_ocall_query_tokens_entries_t);

	if (Q_w_u_arr != NULL) {
		ms->ms_Q_w_u_arr = (const void*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, Q_w_u_arr, _len_Q_w_u_arr)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_Q_w_u_arr);
		ocalloc_size -= _len_Q_w_u_arr;
	} else {
		ms->ms_Q_w_u_arr = NULL;
	}
	
	if (Q_w_id_arr != NULL) {
		ms->ms_Q_w_id_arr = (const void*)__tmp;
		if (memcpy_s(__tmp, ocalloc_size, Q_w_id_arr, _len_Q_w_id_arr)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_Q_w_id_arr);
		ocalloc_size -= _len_Q_w_id_arr;
	} else {
		ms->ms_Q_w_id_arr = NULL;
	}
	
	ms->ms_pair_count = pair_count;
	ms->ms_rand_size = rand_size;
	status = sgx_ocall(9, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_dec_e(unsigned char* sum_e, unsigned char* res, int dec)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_sum_e = sum_e ? strlen(sum_e) + 1 : 0;
	size_t _len_res = res ? strlen(res) + 1 : 0;

	ms_ocall_dec_e_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_dec_e_t);
	void *__tmp = NULL;

	void *__tmp_res = NULL;

	CHECK_ENCLAVE_POINTER(sum_e, _len_sum_e);
	CHECK_ENCLAVE_POINTER(res, _len_res);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (sum_e != NULL) ? _len_sum_e : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (res != NULL) ? _len_res : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_dec_e_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_dec_e_t));
	ocalloc_size -= sizeof(ms_ocall_dec_e_t);

	if (sum_e != NULL) {
		ms->ms_sum_e = (unsigned char*)__tmp;
		if (_len_sum_e % sizeof(*sum_e) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, sum_e, _len_sum_e)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_sum_e);
		ocalloc_size -= _len_sum_e;
	} else {
		ms->ms_sum_e = NULL;
	}
	
	if (res != NULL) {
		ms->ms_res = (unsigned char*)__tmp;
		__tmp_res = __tmp;
		if (_len_res % sizeof(*res) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp_res, ocalloc_size, res, _len_res)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_res);
		ocalloc_size -= _len_res;
	} else {
		ms->ms_res = NULL;
	}
	
	ms->ms_dec = dec;
	status = sgx_ocall(10, ms);

	if (status == SGX_SUCCESS) {
		if (res) {
			size_t __tmp_len_res;
			if (memcpy_s((void*)res, _len_res, __tmp_res, _len_res)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
			((char*)res)[_len_res - 1] = '\0';
			__tmp_len_res = strlen(res) + 1;
			memset(res +__tmp_len_res - 1, 0, _len_res -__tmp_len_res);
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_send_Proof_res(unsigned char* Proof_res, int len)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_Proof_res = len;

	ms_ocall_send_Proof_res_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_send_Proof_res_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(Proof_res, _len_Proof_res);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (Proof_res != NULL) ? _len_Proof_res : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_send_Proof_res_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_send_Proof_res_t));
	ocalloc_size -= sizeof(ms_ocall_send_Proof_res_t);

	if (Proof_res != NULL) {
		ms->ms_Proof_res = (unsigned char*)__tmp;
		if (_len_Proof_res % sizeof(*Proof_res) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, Proof_res, _len_Proof_res)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_Proof_res);
		ocalloc_size -= _len_Proof_res;
	} else {
		ms->ms_Proof_res = NULL;
	}
	
	ms->ms_len = len;
	status = sgx_ocall(11, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_cpuinfo = 4 * sizeof(int);

	ms_sgx_oc_cpuidex_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_oc_cpuidex_t);
	void *__tmp = NULL;

	void *__tmp_cpuinfo = NULL;

	CHECK_ENCLAVE_POINTER(cpuinfo, _len_cpuinfo);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (cpuinfo != NULL) ? _len_cpuinfo : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_oc_cpuidex_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_oc_cpuidex_t));
	ocalloc_size -= sizeof(ms_sgx_oc_cpuidex_t);

	if (cpuinfo != NULL) {
		ms->ms_cpuinfo = (int*)__tmp;
		__tmp_cpuinfo = __tmp;
		if (_len_cpuinfo % sizeof(*cpuinfo) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp_cpuinfo, 0, _len_cpuinfo);
		__tmp = (void *)((size_t)__tmp + _len_cpuinfo);
		ocalloc_size -= _len_cpuinfo;
	} else {
		ms->ms_cpuinfo = NULL;
	}
	
	ms->ms_leaf = leaf;
	ms->ms_subleaf = subleaf;
	status = sgx_ocall(12, ms);

	if (status == SGX_SUCCESS) {
		if (cpuinfo) {
			if (memcpy_s((void*)cpuinfo, _len_cpuinfo, __tmp_cpuinfo, _len_cpuinfo)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_wait_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);

	ms->ms_self = self;
	status = sgx_ocall(13, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_set_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_untrusted_event_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);

	ms->ms_waiter = waiter;
	status = sgx_ocall(14, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_setwait_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);

	ms->ms_waiter = waiter;
	ms->ms_self = self;
	status = sgx_ocall(15, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_waiters = total * sizeof(void*);

	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(waiters, _len_waiters);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (waiters != NULL) ? _len_waiters : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_multiple_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);

	if (waiters != NULL) {
		ms->ms_waiters = (const void**)__tmp;
		if (_len_waiters % sizeof(*waiters) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, waiters, _len_waiters)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_waiters);
		ocalloc_size -= _len_waiters;
	} else {
		ms->ms_waiters = NULL;
	}
	
	ms->ms_total = total;
	status = sgx_ocall(16, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}


#include "CryptoEnclave_u.h"
#include <errno.h>

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

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_test2(void* pms)
{
	ms_ocall_test2_t* ms = SGX_CAST(ms_ocall_test2_t*, pms);
	ocall_test2(ms->ms_encrypted_content, ms->ms_length_content);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_test(void* pms)
{
	ms_ocall_test_t* ms = SGX_CAST(ms_ocall_test_t*, pms);
	ocall_test(ms->ms_mint, ms->ms_mchar, ms->ms_mstring, ms->ms_len);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_print_string(void* pms)
{
	ms_ocall_print_string_t* ms = SGX_CAST(ms_ocall_print_string_t*, pms);
	ocall_print_string(ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_print_bytes(void* pms)
{
	ms_ocall_print_bytes_t* ms = SGX_CAST(ms_ocall_print_bytes_t*, pms);
	ocall_print_bytes(ms->ms_ptr, ms->ms_len);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_transfer_encrypted_entries(void* pms)
{
	ms_ocall_transfer_encrypted_entries_t* ms = SGX_CAST(ms_ocall_transfer_encrypted_entries_t*, pms);
	ocall_transfer_encrypted_entries(ms->ms_t1_u_arr, ms->ms_t1_v_arr, ms->ms_t2_u_arr, ms->ms_t2_v_arr, ms->ms_pair_count, ms->ms_rand_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_retrieve_encrypted_doc(void* pms)
{
	ms_ocall_retrieve_encrypted_doc_t* ms = SGX_CAST(ms_ocall_retrieve_encrypted_doc_t*, pms);
	ocall_retrieve_encrypted_doc(ms->ms_del_id, ms->ms_del_id_len, ms->ms_encrypted_content, ms->ms_maxLen, ms->ms_length_content, ms->ms_int_len);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_del_encrypted_doc(void* pms)
{
	ms_ocall_del_encrypted_doc_t* ms = SGX_CAST(ms_ocall_del_encrypted_doc_t*, pms);
	ocall_del_encrypted_doc(ms->ms_del_id, ms->ms_del_id_len);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_retrieve_M_c(void* pms)
{
	ms_ocall_retrieve_M_c_t* ms = SGX_CAST(ms_ocall_retrieve_M_c_t*, pms);
	ocall_retrieve_M_c(ms->ms__u_prime, ms->ms__u_prime_size, ms->ms__v_prime, ms->ms_maxLen, ms->ms__v_prime_size, ms->ms_int_len);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_del_M_c_value(void* pms)
{
	ms_ocall_del_M_c_value_t* ms = SGX_CAST(ms_ocall_del_M_c_value_t*, pms);
	ocall_del_M_c_value(ms->ms__u_prime, ms->ms__u_prime_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_query_tokens_entries(void* pms)
{
	ms_ocall_query_tokens_entries_t* ms = SGX_CAST(ms_ocall_query_tokens_entries_t*, pms);
	ocall_query_tokens_entries(ms->ms_Q_w_u_arr, ms->ms_Q_w_id_arr, ms->ms_pair_count, ms->ms_rand_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_dec_e(void* pms)
{
	ms_ocall_dec_e_t* ms = SGX_CAST(ms_ocall_dec_e_t*, pms);
	ocall_dec_e(ms->ms_sum_e, ms->ms_res, ms->ms_dec);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_ocall_send_Proof_res(void* pms)
{
	ms_ocall_send_Proof_res_t* ms = SGX_CAST(ms_ocall_send_Proof_res_t*, pms);
	ocall_send_Proof_res(ms->ms_Proof_res, ms->ms_len);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_sgx_oc_cpuidex(void* pms)
{
	ms_sgx_oc_cpuidex_t* ms = SGX_CAST(ms_sgx_oc_cpuidex_t*, pms);
	sgx_oc_cpuidex(ms->ms_cpuinfo, ms->ms_leaf, ms->ms_subleaf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall(ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall(ms->ms_waiter);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_sgx_thread_setwait_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_setwait_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_setwait_untrusted_events_ocall(ms->ms_waiter, ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL CryptoEnclave_sgx_thread_set_multiple_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_multiple_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_multiple_untrusted_events_ocall(ms->ms_waiters, ms->ms_total);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[17];
} ocall_table_CryptoEnclave = {
	17,
	{
		(void*)CryptoEnclave_ocall_test2,
		(void*)CryptoEnclave_ocall_test,
		(void*)CryptoEnclave_ocall_print_string,
		(void*)CryptoEnclave_ocall_print_bytes,
		(void*)CryptoEnclave_ocall_transfer_encrypted_entries,
		(void*)CryptoEnclave_ocall_retrieve_encrypted_doc,
		(void*)CryptoEnclave_ocall_del_encrypted_doc,
		(void*)CryptoEnclave_ocall_retrieve_M_c,
		(void*)CryptoEnclave_ocall_del_M_c_value,
		(void*)CryptoEnclave_ocall_query_tokens_entries,
		(void*)CryptoEnclave_ocall_dec_e,
		(void*)CryptoEnclave_ocall_send_Proof_res,
		(void*)CryptoEnclave_sgx_oc_cpuidex,
		(void*)CryptoEnclave_sgx_thread_wait_untrusted_event_ocall,
		(void*)CryptoEnclave_sgx_thread_set_untrusted_event_ocall,
		(void*)CryptoEnclave_sgx_thread_setwait_untrusted_events_ocall,
		(void*)CryptoEnclave_sgx_thread_set_multiple_untrusted_events_ocall,
	}
};
sgx_status_t ecall_init(sgx_enclave_id_t eid, unsigned char* keyF, size_t len)
{
	sgx_status_t status;
	ms_ecall_init_t ms;
	ms.ms_keyF = keyF;
	ms.ms_len = len;
	status = sgx_ecall(eid, 0, &ocall_table_CryptoEnclave, &ms);
	return status;
}

sgx_status_t ecall_addDoc(sgx_enclave_id_t eid, char* doc_id, size_t id_length, char* content, int content_length)
{
	sgx_status_t status;
	ms_ecall_addDoc_t ms;
	ms.ms_doc_id = doc_id;
	ms.ms_id_length = id_length;
	ms.ms_content = content;
	ms.ms_content_length = content_length;
	status = sgx_ecall(eid, 1, &ocall_table_CryptoEnclave, &ms);
	return status;
}

sgx_status_t ecall_delDoc(sgx_enclave_id_t eid, char* doc_id, size_t id_length)
{
	sgx_status_t status;
	ms_ecall_delDoc_t ms;
	ms.ms_doc_id = doc_id;
	ms.ms_id_length = id_length;
	status = sgx_ecall(eid, 2, &ocall_table_CryptoEnclave, &ms);
	return status;
}

sgx_status_t ecall_search(sgx_enclave_id_t eid, const char* keyword, size_t len)
{
	sgx_status_t status;
	ms_ecall_search_t ms;
	ms.ms_keyword = keyword;
	ms.ms_len = len;
	status = sgx_ecall(eid, 3, &ocall_table_CryptoEnclave, &ms);
	return status;
}

sgx_status_t ecall_test(sgx_enclave_id_t eid, char* encrypted_content, size_t length_content)
{
	sgx_status_t status;
	ms_ecall_test_t ms;
	ms.ms_encrypted_content = encrypted_content;
	ms.ms_length_content = length_content;
	status = sgx_ecall(eid, 4, &ocall_table_CryptoEnclave, &ms);
	return status;
}

sgx_status_t ecall_hash_sha256_test(sgx_enclave_id_t eid, unsigned char* message, size_t message_length)
{
	sgx_status_t status;
	ms_ecall_hash_sha256_test_t ms;
	ms.ms_message = message;
	ms.ms_message_length = message_length;
	status = sgx_ecall(eid, 5, &ocall_table_CryptoEnclave, &ms);
	return status;
}

sgx_status_t ecall_get_Proof_ID_u(sgx_enclave_id_t eid, char* ID_u, int id_len, unsigned char* h_u, int h_u_len)
{
	sgx_status_t status;
	ms_ecall_get_Proof_ID_u_t ms;
	ms.ms_ID_u = ID_u;
	ms.ms_id_len = id_len;
	ms.ms_h_u = h_u;
	ms.ms_h_u_len = h_u_len;
	status = sgx_ecall(eid, 6, &ocall_table_CryptoEnclave, &ms);
	return status;
}

sgx_status_t ecall_get_Proof_w(sgx_enclave_id_t eid, char* w, int w_len, unsigned char* h_w, int h_w_len)
{
	sgx_status_t status;
	ms_ecall_get_Proof_w_t ms;
	ms.ms_w = w;
	ms.ms_w_len = w_len;
	ms.ms_h_w = h_w;
	ms.ms_h_w_len = h_w_len;
	status = sgx_ecall(eid, 7, &ocall_table_CryptoEnclave, &ms);
	return status;
}

sgx_status_t ecall_Enclave_search(sgx_enclave_id_t eid, char* w, int w_len, char* id_u, int id_u_len, unsigned char* bs_w_out, int bs_w_out_len, unsigned char* bs_u_out, int bs_u_out_len, unsigned char* K_w, int K_w_len, unsigned char* K_u, int K_u_len, int c_w, int c_u)
{
	sgx_status_t status;
	ms_ecall_Enclave_search_t ms;
	ms.ms_w = w;
	ms.ms_w_len = w_len;
	ms.ms_id_u = id_u;
	ms.ms_id_u_len = id_u_len;
	ms.ms_bs_w_out = bs_w_out;
	ms.ms_bs_w_out_len = bs_w_out_len;
	ms.ms_bs_u_out = bs_u_out;
	ms.ms_bs_u_out_len = bs_u_out_len;
	ms.ms_K_w = K_w;
	ms.ms_K_w_len = K_w_len;
	ms.ms_K_u = K_u;
	ms.ms_K_u_len = K_u_len;
	ms.ms_c_w = c_w;
	ms.ms_c_u = c_u;
	status = sgx_ecall(eid, 8, &ocall_table_CryptoEnclave, &ms);
	return status;
}


#include "EnclaveUtils.h"
#include "CryptoEnclave_t.h"

#include "sgx_trts.h"
#include "sgx_tcrypto.h"
#include "../common/data_type.h"

void printf( const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print_string(buf);
}

void print_bytes(uint8_t *ptr, uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    printf("%x", *(ptr + i));
  }

  printf("\n");
}


int  cmp(const uint8_t *value1, const uint8_t *value2, uint32_t len){
    for (uint32_t i = 0; i < len; i++) {
        if (*(value1+i) != *(value2+i)) {
        return -1;
        }
    }

    return 0;
}

void  clear(uint8_t *dest, uint32_t len){
    for (uint32_t i = 0; i < len; i++) {
        *(dest + i) = 0;
    }
}

//将文档 content分词，分词符号为","
std::vector<std::string>  wordTokenize(char *content,int content_length){
    char delim[] = ",";//" ,.-";
    std::vector<std::string> result;

    char *content_cpy = (char*)malloc(content_length);
    memcpy(content_cpy,content,content_length);

    char *token = strtok(content_cpy,delim);
    while (token != NULL)
    {
        result.push_back(token); 
        token =  strtok(NULL,delim);
    }

    free(token);
    free(content_cpy);
    
    return result;
}

//PRF
void prf_F_improve(const void *key,const void *plaintext,size_t plaintext_len, entryKey *k ){

    //k->content_length = AESGCM_MAC_SIZE + AESGCM_IV_SIZE + plaintext_len; //important- has to be size_t
	//k->content = (char *) malloc(k->content_length);
	enc_aes_gcm(key,plaintext,plaintext_len,k->content,k->content_length);

}

void prf_Enc_improve(const void *key,const void *plaintext,size_t plaintext_len, entryValue *v){

    //v->message_length = AESGCM_MAC_SIZE + AESGCM_IV_SIZE + plaintext_len; //important- has to be size_t
	//v->message = (char *) malloc(v->message_length);
	enc_aes_gcm(key,plaintext,plaintext_len,v->message,v->message_length);
}


void prf_Dec_Improve(const void *key,const void *ciphertext,size_t ciphertext_len, entryValue *value ){


    //value->message_length = ciphertext_len - AESGCM_MAC_SIZE - AESGCM_IV_SIZE;
	//value->message = (char *) malloc(value->message_length);
    dec_aes_gcm(key,ciphertext,ciphertext_len,value->message,value->message_length);
}

void enc_aes_gcm(const void *key, const void *plaintext, size_t plaintext_len, void *ciphertext, size_t ciphertext_len)
{
  uint8_t p_dst[ciphertext_len] = {0};

  //p_dst = mac + iv + cipher
	sgx_rijndael128GCM_encrypt(
    (sgx_aes_gcm_128bit_key_t*)key,
		(uint8_t *) plaintext, plaintext_len,
		p_dst + AESGCM_MAC_SIZE + AESGCM_IV_SIZE, //where  the cipher should be stored
		(uint8_t *)gcm_iv, AESGCM_IV_SIZE,
		NULL, 0,
		(sgx_aes_gcm_128bit_tag_t *) p_dst);	//the tag should be the first 16 bytes and auto dumped out

  memcpy(p_dst + AESGCM_MAC_SIZE, gcm_iv, AESGCM_IV_SIZE);

  //copy tag+iv+cipher to ciphertext
  memcpy(ciphertext,p_dst,ciphertext_len);

}

void dec_aes_gcm(const void *key, const void *ciphertext, size_t ciphertext_len, void *plaintext, size_t plaintext_len){
    
    uint8_t p_dst[plaintext_len] = {0};

	sgx_status_t ret = sgx_rijndael128GCM_decrypt(
		(sgx_aes_gcm_128bit_key_t*)key,
		(uint8_t *) (ciphertext + AESGCM_MAC_SIZE + AESGCM_IV_SIZE), plaintext_len,
		p_dst,
		(uint8_t *)gcm_iv, AESGCM_IV_SIZE,
		NULL, 0,
		(sgx_aes_gcm_128bit_tag_t *) ciphertext); //get the first 16 bit tag to verify

	memcpy(plaintext, p_dst, plaintext_len);

}

int hash_SHA256(const void *msg,int msg_len,void* value){
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    ret = sgx_sha256_msg((uint8_t*)msg,msg_len,(sgx_sha256_hash_t*)value);//32位，256bit
    if (ret == SGX_SUCCESS) {
        return 1;
    }
    else {
        printf("[*] hash error line 87: %d\n", ret);
        return 0;
    }  
}

int hash_SHA256(const void *key,int key_len,const void* p,int p_len,void* value){
    unsigned char* msg = (unsigned char*)malloc(key_len+p_len);
    int msg_len = key_len+p_len;
    memcpy(msg,key,key_len);
    memcpy(msg+key_len,p,p_len);
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    ret = sgx_sha256_msg((uint8_t*)msg,msg_len,(sgx_sha256_hash_t*)value);//32位，256bit
    free(msg);
    if (ret == SGX_SUCCESS) {
        return 1;
    }
    else {
        printf("[*] hash error line 87: %d\n", ret);
        return 0;
    }  
}

//generating 128bit output digest
int hash_SHA128(const void *key, const void *msg, int msg_len, void *value){
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    ret = sgx_rijndael128_cmac_msg(
            (sgx_cmac_128bit_key_t *)key,
            (const uint8_t*)msg,
            msg_len,
            (sgx_cmac_128bit_tag_t*)value); //16位，128bit
     
    if (ret == SGX_SUCCESS) {
        return 1;
    }
    else {
        printf("[*] hash error line 87: %d\n", ret);
        return 0;
    }  
}

//make sure the key is 16 bytes and appended to the digest
//这是在hash结束后的value后面加上16位的key
int hash_SHA128_key(const void *key, int key_len, const void *msg, int msg_len, void *value){
    
    int result;
    result = hash_SHA128(key,msg,msg_len,value);
    if (result==1) {
        //这是在hash结束后的value后面加上16位的key
        memcpy(value+ENTRY_HASH_KEY_LEN_128,key,key_len); 
        return 1;
    } else{
        printf("[*] hash error line 163: %d\n", result);
        return 0;
    }
}

void Hashxor(unsigned char* hash1,unsigned char* hash2,int len,unsigned char* res){
    for(int i=0;i<len;i++){
        res[i] = hash1[i] ^ hash2[i];
    }
}

void Hashand(unsigned char* hash1,unsigned char* hash2,int len,unsigned char* res){
    for(int i=0;i<len;i++){
        res[i] = hash1[i] & hash2[i];
    }
}


void getMultiHash(std::vector<bool> bitmap,unsigned char* res,unsigned char* K,int K_len){
    std::string input = unsignedcharToString(K,K_len);
    bool first = true;
    for(int i=0;i<bitmap.size();i++){
        if(bitmap[i] == 1){
            unsigned char* temp = (unsigned char*)malloc(SHA_256_SIZE);
            std::string hash_input = input + std::to_string(i);
            unsigned char* message = (unsigned char*)hash_input.c_str();
            hash_SHA256(message,hash_input.length(),temp);
            if(first){
                memcpy(res,temp,SHA_256_SIZE);
                first = false;
            }else{
                Hashxor(res,temp,SHA_256_SIZE,res);
            }
            message = NULL;
            free(temp);
        }
    }
}

std::vector<bool> bitmap_transfer(unsigned char* bs,int bs_len){
    std::vector<bool> res;
    for(int i=0;i<bs_len;i++){
        if(bs[i] == '0'){
            res.push_back(0);
        }else if(bs[i] == '1'){
            res.push_back(1);
        }
    }
    return res;
}

std::string charToString(char* c,int len){
    std::string res = "";
    for(int i = 0;i<len;i++){
        res+=c[i];
    }
    return res;
}

std::string unsignedcharToString(unsigned char* uc,int len){
    std::string res = "";
    for(int i = 0;i<len;i++){
        res+=uc[i];
    }
    return res;
}


bool unsignedcharCmp(unsigned char* uc1,unsigned char* uc2,int len){
    for(int i=0;i<len;i++){
        if(uc1[i] != uc2[i]){
            return false;
        }
    }
    return true;
}
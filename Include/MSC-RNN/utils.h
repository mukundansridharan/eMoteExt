#define min(a,b) ((b)>(a))?a:b
#define max(a,b) ((a)>(b))?a:b

// Copy utils
inline void copyUIntVecToLL(uint* invec, ll* outvec, int vec_len)
{
	copy(invec,invec+vec_len, outvec);
}

//Vector-scalar multiplication
void mulVecScal(ll* vec, ll scal, int vec_len, ll* out){
	for(int i=0; i < vec_len; i++)
		out[i] = *(vec+i)*scal;
} 

//Vector-scalar division
void divVecScal(ll* vec, ll scal, int vec_len, ll* out){
	for(int i=0; i < vec_len; i++)
		out[i] = *(vec+i)/scal;
} 

//Matrix-vector multiplication
void mulMatVec(ll* mat, ll* vec, int mat_rows, int vec_len, ll* out){
	for(int i=0; i < mat_rows; i++){
		out[i] = 0;
		for(int j=0; j < vec_len; j++)
			out[i] += *((mat+i*vec_len)+j)*(*(vec+j));
	}
}

//Vector-vector addition
void addVecs(ll* vec1, ll* vec2, int vec_len, ll* out){
	for(int i=0; i < vec_len; i++)
		out[i] = *(vec1+i)+*(vec2+i);
}

//Vector-vector subtraction
void subVecs(ll* vec1, ll* vec2, int vec_len, ll* out){
	for(int i=0; i < vec_len; i++)
		out[i] = *(vec1+i)-*(vec2+i);
}

//Vector-vector multiplication (Hadamard)
void mulVecs(ll* vec1, ll* vec2, int vec_len, ll* out){
	for(int i=0; i < vec_len; i++)
		out[i] = *(vec1+i)*(*(vec2+i));
}

// Standardization with scaling
void stdScaleInput(ll* in_vec, ll* mean, ll* stdev, ll scale, int vec_len, ll* out_vec){
	for(int i=0; i<vec_len; i++)
		*(out_vec+i) = scale*(*(in_vec+i)-mean[i])/stdev[i];
}

// quantTanh
inline void quantTanh(ll* vec, int vec_len, ll scale, ll* out_vec){
	for(int i=0; i<vec_len; i++)
		*(out_vec+i) = max(-scale, min(scale, *(vec+i)));
}

// quantSigm
inline void quantSigm(ll* vec, int vec_len, ll scale, ll* out_vec){
	for(int i=0; i<vec_len; i++)
		*(out_vec+i) = max(min((*(vec+i)+scale)/2, scale),0);
}

// Print utils
#ifdef DBG
void util_printVec(ll* vec, int vec_len){
	for(int i=0; i < vec_len; i++)
		cout << vec[i] << '\t';
	cout << "\n\n";
}

void util_printMatrix(uint* mat, int row_len, int col_len){
	for(int i=0; i < row_len; i++){
		for(int j=0; j < col_len; j++)
			cout << *(mat+i*col_len+j) << '\t';
		cout << endl;
	}
	cout << "\n\n";
}
#endif

// Extract instance
void extract_instance(uint* src, uint*dst, int row_start, int row_end, int vec_len){
	for(int t=0, k=row_start * vec_len; k < row_end * vec_len; t++, k++)
		*(dst+t) = *(src + k);
}

// Matrix slice utils
void util_slice2D(uint* src, uint*dst, int row_index, int vec_len){
	for(int j=0; j < vec_len; j++)
		*(dst+j) = *((src+row_index*vec_len)+j);
}

void util_slice3D(uint* src, uint*dst, int row_index, int col_len, int vec_len){
	uint *slice_beg = (src + row_index * col_len * vec_len);
	for(int k=0; k < col_len * vec_len; k++)
			*(dst+k) = *(slice_beg + k);
}
#ifndef MOTE
// String builder util
string strBuild(ll i, char delim)
{
    string s;
    stringstream out;
    out << i;
    out << delim;
    s = out.str();
    return s;
}
#endif
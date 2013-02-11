inline
SMLXMatrix::SMLXMatrix() :
		mat(0), nr(0), nc(0), msize(0), maxsize(0), owned(0), flags(0)
{
}


inline
SMLXMatrix::SMLXMatrix(short nrow, short ncol) :
		mat(0), msize(0), maxsize(0), owned(0), flags(0)
{
	Resize(nrow, ncol);
}


inline
SMLXMatrix::SMLXMatrix(short nrow, short ncol, const float* initialValues) :
		mat(0), msize(0), maxsize(0), owned(0), flags(0)
{
	Resize(nrow, ncol);
	assert(offsetof(SMLXMatrix, mat) == 0);
	if(((unsigned int)initialValues) & 0xf)
		GenericAssign(*this, *(SMLXMatrix*)&initialValues);
	else
		Assign(*this, *(SMLXMatrix*)&initialValues);
	MarkAsDefined();
}

// SMLMatrix3f is defined in smlmath.h
inline
SMLXMatrix::SMLXMatrix(const SMLMatrix3f& m33) :
		mat(0), msize(0), maxsize(0), owned(0), flags(0)
{
	Resize(3, 3);
	void* pm = (void*)&m33;
	Assign(*this, *(SMLXMatrix*)&pm);
	MarkAsDefined();
}

// SMLMatrix4f is defined in smlmath.h
inline
SMLXMatrix::SMLXMatrix(const SMLMatrix4f& m44) :
		mat(0), msize(0), maxsize(0), owned(0), flags(0)
{
	Resize(4, 4);
	void* pm = (void*)&m44;
	Assign(*this, *(SMLXMatrix*)&pm);
	MarkAsDefined();
}


inline
SMLXMatrix::SMLXMatrix(const SMLXMatrix& B) :
		mat(0), msize(0), maxsize(0), owned(0), flags(0)
{
	Resize(B);
	Assign(*this, B);
	MarkAsDefined();
#ifndef NDEBUG	
	subExpressionsCount = 0;
#endif	
}


inline
SMLXMatrix::~SMLXMatrix() {
	if (owned) {
		mat = (float*)((unsigned int)mat - (owned-1));
		delete [] mat;
		owned = 0;
	}
}


inline
void SMLXMatrix::Export(float* dst) const {
	msize = nr * nc;
	for (short i = 0; i < msize; i++)
	  dst[i] = mat[i];
}


// Cast to SMLMatrix3f.
inline
SMLXMatrix::operator const SMLMatrix3f&() const {
	assert(nr == 3 && nc == 3);
	return *(const SMLMatrix3f*) mat;
}


// Cast to SMLMatrix4f.
inline
SMLXMatrix::operator const SMLMatrix4f&() const {
	assert(nr == 4 && nc == 4);
	return *(const SMLMatrix4f*) mat;
}


inline
void SMLXMatrix::Cross(const SMLVec3f& V) {	
  // this = [V]x (cross operator)
	Resize(3, 3);
	float* d = Data();
	d[0] = 0.0f; d[1] = -V.z; d[2] =  V.y;
	d[3] =  V.z; d[4] = 0.0f; d[5] = -V.x;
	d[6] = -V.y; d[7] =  V.x; d[8] = 0.0f;
	MarkAsDefined();
}


inline
void SMLXMatrix::AdjustSize(short nrow, short ncol) {
	assert(msize >= nrow * ncol);
	assert(nrow < sizeof(lutSizes)/sizeof(int));
	assert(ncol < sizeof(lutSizes)/sizeof(int));
	nr = nrow;
	nc = ncol;
	msize = nrow * ncol;
}


inline
float* SMLXMatrix::operator[](int i) {
	assert(i >= 0 && i < nr);
	return& mat[i * nc];
}


inline
const float* SMLXMatrix::operator[](int i) const {
	assert(i >= 0 && i < nr);
	return &mat[i * nc];
}


inline
int SMLXMatrix::Index(int i, int j) const {
	assert(i < nr && j < nc);
	return i * nc + j;
}


inline
float SMLXMatrix::Get(int i, int j) const {
	return mat[Index(i, j)];
}


inline
void SMLXMatrix::Set(float initialValue) {
	Assign(*this, initialValue);
	MarkAsDefined();
}


inline
void SMLXMatrix::Zero() {
	Set(0.0);
}


inline
void SMLXMatrix::Transpose() {
	SMLXTmpMatrix& tm = TMat(nr, nr);
	tm.Identity();
	*this = ::Transpose(*this) * tm;
}


inline
void SMLXMatrix::Transpose(const SMLXMatrix& m) {
	*this = m;
	Transpose();
}


inline
void SMLXMatrix::Identity() {
	// Do not assert to enable | I Z | matrices.
	// assert(nr == nc);
	Set(0.0);
	for (short i = 0; i < msize; i += nc + 1)
		mat[i] = 1.0;
}


inline
void SMLXMatrix::Identity(short nrow, short ncol) {
	Resize(nrow, ncol);
	Identity();
}


inline
void SMLXMatrix::Identity(short nrow) {
	Resize(nrow, nrow);
	Identity();
}


inline
void SMLXMatrix::Negate() {
	// Not optimized.
	for (short i = 0; i < msize; i++)
		mat[i] = -mat[i];
}


inline
void SMLXMatrix::IdentityMinus() {
	// Not optimized.
	assert(nr == nc);
	Negate();
	for (short i = 0; i < msize; i += nc + 1)
		mat[i] += 1;
}


inline
float SMLXMatrix::MaxAbs() const
{
	// Not optimized.
	float mv = -1;
	msize = nr * nc;
	for (short i = 0; i < msize; i++) {
		float fm = FastAbs(mat[i]);
		if (fm > mv) mv = fm;
	}
	return mv;
}


inline
const float* SMLXMatrix::Data() const
{
	return mat;
}


inline
float* SMLXMatrix::Data()
{
	return mat;
}


inline
void SMLXMatrix::operator=(const SMLXMatrix& B) 
{
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][0];
#endif
	assert(this != &B);
	Resize(B);
	Assign(*this, B);
	MarkAsDefined();
}


inline
void SMLXMatrix::operator*=(float scale) 
{
	Scale(scale);
}


inline
void SMLXMatrix::Swap(const SMLXMatrix& B) {
	assert (nr == B.nr && nc == B.nc);
	__asm mov eax, dword ptr [this]
	__asm mov ebx, dword ptr [B]
	swapmem(0)
	swapmem(4)
	swapmem(8)
}


void VerifyXMatrix(const SMLXMatrix&);


inline 
float Scalar(const SMLXMatrix& A, const SMLXMatrix& B) {
	assert(A.ncols() == 1 && B.ncols() == 1 && A.nrows() == B.nrows());
	const float* ad = A.Data();
	const float* bd = B.Data();
	int n = A.nrows();
	float s = 0;
	for (int i = 0; i < n; i++)
		s += ad[i] * bd[i];
	return s;
}


inline 
float Scalar(const SMLXMatrix& A) {
	return Scalar(A, A);
}


inline 
void SMLXMatrix::Resize(const SMLXMatrix& A) {
	Resize(A.nr, A.nc);
}


inline 
void SMLXMatrix::Resize(short nrow, short ncol) {
	assert(nrow < sizeof(lutSizes)/sizeof(int));
	assert(ncol < sizeof(lutSizes)/sizeof(int));
	nr = nrow;
	nc = ncol;
	msize = nr * nc;
	if (msize > maxsize) {
		if (owned) 
		{
			mat = (float*)((unsigned int)mat - (owned-1));
			delete [] mat;
		}
		maxsize = msize;
		float* mat1 = new float[msize+4];
		mat = (float*)(((unsigned int)mat1 +0xf) & 0xfffffff0); //align
		owned = 1+(unsigned int)mat - (unsigned int)mat1;
	}
}


inline 
SMLXTmpMatrix& Inverse(const SMLXMatrix& A) {
	SMLXTmpMatrix& mi = A.TMat();
	mi.Invert();
	return mi;
}


inline 
void SMLXMatrix::Transform(SMLXSpatialVector const &A, SMLXSpatialVector& B) const {
	B = *this * A;
}


inline SMLXTransposedMatrix& Transpose(const SMLXMatrix& m) {
	return (SMLXTransposedMatrix&)m;
}


inline 
void SMLXMatrix::TransformTransposed(SMLXSpatialVector const &A, SMLXSpatialVector& B) const {
	B = ::Transpose(*this) * A;
}

/////////////////////////////////////////////////////////////////////////
// Each SMLXMatrix operation (+, -, =, *) is actually call
// to an appropriate function (from the array name##Pool[][]).
// Notes:
// 1. Matrices with non-optimized sizes will be processed via
//    call of function Generic##name, where name is the operation.
/////////////////////////////////////////////////////////////////////////

// Generic##name from name##Misses array will be used for name##Pool[-1][-1],
// as well as for gaps in the table.
// Note: lutSizes[i] is equal to -1 for nonoptimized sizes.
#define pfun1(op, AR, AC) (void (__fastcall *)(SMLXMatrix&, const SMLXMatrix&)) ## op ## AR ## x ## AC

#ifdef _PIII_CODE_
#define ftable_vmm(name, op)           \
static void* name ## Misses[8][7] = {      \
	0, 0, 0, 0, 0, 0, Generic ## name, \
	Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, \
	pfun1(op, 1, 1), pfun1(op, 1, 2), pfun1(op, 1, 3), pfun1(op, 1, 4), pfun1(op, 1, 5), pfun1(op, 1, 6), Generic ## name, \
	pfun1(op, 2, 1), pfun1(op, 2, 2), pfun1(op, 2, 3), pfun1(op, 2, 4), pfun1(op, 2, 5), pfun1(op, 2, 6), Generic ## name, \
	pfun1(op, 3, 1), pfun1(op, 3, 2), pfun1(op, 3, 3), pfun1(op, 3, 4), pfun1(op, 3, 5), pfun1(op, 3, 6), Generic ## name, \
	pfun1(op, 4, 1), pfun1(op, 4, 2), pfun1(op, 4, 3), pfun1(op, 4, 4), pfun1(op, 4, 5), pfun1(op, 4, 6), Generic ## name, \
	pfun1(op, 5, 1), pfun1(op, 5, 2), pfun1(op, 5, 3), pfun1(op, 5, 4), pfun1(op, 5, 5), pfun1(op, 5, 6), Generic ## name, \
	pfun1(op, 6, 1), pfun1(op, 6, 2), pfun1(op, 6, 3), pfun1(op, 6, 4), pfun1(op, 6, 5), pfun1(op, 6, 6), Generic ## name, \
};
#else
#define ftable_vmm(name, op)           \
static void* name ## Misses[8][7] = {      \
	0, 0, 0, 0, 0, 0, Generic ## name, \
	Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name,/*};\
static void* name ## Pool[6][7] = {*/    \
	pfun1(op, 1, 1), Generic ## name, pfun1(op, 1, 3), pfun1(op, 1, 4), pfun1(op, 1, 5), pfun1(op, 1, 6), Generic ## name, \
	Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, \
	pfun1(op, 3, 1), Generic ## name, pfun1(op, 3, 3),Generic ## name , pfun1(op, 3, 5), pfun1(op, 3, 6), Generic ## name, \
	pfun1(op, 4, 1), Generic ## name, Generic ## name, pfun1(op, 4, 4), Generic ## name, Generic ## name, Generic ## name, \
	pfun1(op, 5, 1), Generic ## name, pfun1(op, 5, 3),Generic ## name , pfun1(op, 5, 5), pfun1(op, 5, 6), Generic ## name, \
	pfun1(op, 6, 1), Generic ## name, pfun1(op, 6, 3),Generic ## name , pfun1(op, 6, 5), pfun1(op, 6, 6), Generic ## name, \
};
#endif


#define pfun2(op, AR, AC) (SMLXTmpMatrix& (__fastcall *)(const SMLXMatrix&, const SMLXMatrix&))## op ## AR ## x ## AC
#ifdef _PIII_CODE_
#define ftable_mmm(name, op)           \
static void* name ## Misses[8][7] = {      \
	0, 0, 0, 0, 0, 0, Generic ## name, \
	Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, \
	pfun2(op, 1, 1), pfun2(op, 1, 2), pfun2(op, 1, 3), pfun2(op, 1, 4), pfun2(op, 1, 5), pfun2(op, 1, 6), Generic ## name, \
	pfun2(op, 2, 1), pfun2(op, 2, 2), pfun2(op, 2, 3), pfun2(op, 2, 4), pfun2(op, 2, 5), pfun2(op, 2, 6), Generic ## name, \
	pfun2(op, 3, 1), pfun2(op, 3, 2), pfun2(op, 3, 3), pfun2(op, 3, 4), pfun2(op, 3, 5), pfun2(op, 3, 6), Generic ## name, \
	pfun2(op, 4, 1), pfun2(op, 4, 2), pfun2(op, 4, 3), pfun2(op, 4, 4), pfun2(op, 4, 5), pfun2(op, 4, 6), Generic ## name, \
	pfun2(op, 5, 1), pfun2(op, 5, 2), pfun2(op, 5, 3), pfun2(op, 5, 4), pfun2(op, 5, 5), pfun2(op, 5, 6), Generic ## name, \
	pfun2(op, 6, 1), pfun2(op, 6, 2), pfun2(op, 6, 3), pfun2(op, 6, 4), pfun2(op, 6, 5), pfun2(op, 6, 6), Generic ## name, \
};
#else
#define ftable_mmm(name, op)           \
static void* name ## Misses[8][7] = {      \
	0, 0, 0, 0, 0, 0, Generic ## name, \
	Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, /*};\
static void* name ## Pool[6][7] = {*/    \
	pfun2(op, 1, 1), Generic ## name, pfun2(op, 1, 3), pfun2(op, 1, 4), pfun2(op, 1, 5), pfun2(op, 1, 6), Generic ## name, \
	Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, Generic ## name, \
	pfun2(op, 3, 1), Generic ## name, pfun2(op, 3, 3), Generic ## name, pfun2(op, 3, 5), pfun2(op, 3, 6), Generic ## name, \
	pfun2(op, 4, 1), Generic ## name, Generic ## name, pfun2(op, 4, 4), Generic ## name, Generic ## name, Generic ## name, \
	pfun2(op, 5, 1), Generic ## name, pfun2(op, 5, 3), Generic ## name, pfun2(op, 5, 5), pfun2(op, 5, 6), Generic ## name, \
	pfun2(op, 6, 1), Generic ## name, pfun2(op, 6, 3), Generic ## name, pfun2(op, 6, 5), pfun2(op, 6, 6), Generic ## name, \
};
#endif



typedef void  (__fastcall *VOIDMX2) (const SMLXMatrix&, const SMLXMatrix&);
typedef void  (__fastcall *VOIDMXV) (const SMLXMatrix&, float);
typedef void* (__fastcall *PVOIDMX2)(const SMLXMatrix&, const SMLXMatrix&);


ftable_vmm(AddTo, AddTo/*operator+=*/);
inline 
void SMLXMatrix::operator+=(const SMLXMatrix& B) {
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][2];
#endif
	if (IsDefined()) {
		void* pf = AddToMisses/*Pool*/[2+SMLXMatrix::lutSizes[nr]][SMLXMatrix::lutSizes[nc]];
		((VOIDMX2&)pf)(*this, B);
	} else
	*this = B;
#ifndef NDEBUG
	subExpressionsCount = 0;
#endif
}


ftable_vmm(SubFrom, SubFrom/*operator-=*/);
inline 
void SMLXMatrix::operator-=(const SMLXMatrix& B) {
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][3];
#endif
	if (!IsDefined()) {
		Resize(B);
		Zero();
	}
	void* pf = SubFromMisses/*Pool*/[2+SMLXMatrix::lutSizes[nr]][SMLXMatrix::lutSizes[nc]];
	((VOIDMX2&)pf)(*this, B);
#ifndef NDEBUG
	subExpressionsCount = 0;
#endif
}


ftable_vmm(Assign, Assign);
inline 
void Assign(SMLXMatrix& A, const SMLXMatrix& B) {
	void* pf = AssignMisses/*Pool*/[2+SMLXMatrix::lutSizes[A.nr]][SMLXMatrix::lutSizes[A.nc]];
	((VOIDMX2&)pf)(A, B);
#ifndef NDEBUG
	subExpressionsCount = 0;
	// VerifyXMatrix(A);
#endif
}

#define fav(op, nr, nc) (void (__fastcall *)(SMLXMatrix&, float)):: ## op ## nr ## x ## nc
#ifdef _PIII_CODE_
static void* AssignValueMisses[8][7] = {
	0, 0, 0, 0, 0, 0, GenericAssignValue,
	GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue, 
	fav(AssignValue, 1, 1), fav(AssignValue, 1, 2), fav(AssignValue, 1, 3), fav(AssignValue, 1, 4), fav(AssignValue, 1, 5), fav(AssignValue, 1, 6), GenericAssignValue,
	fav(AssignValue, 2, 1), fav(AssignValue, 2, 2), fav(AssignValue, 2, 3), fav(AssignValue, 2, 4), fav(AssignValue, 2, 5), fav(AssignValue, 2, 6), GenericAssignValue,
	fav(AssignValue, 3, 1), fav(AssignValue, 3, 2), fav(AssignValue, 3, 3), fav(AssignValue, 3, 4), fav(AssignValue, 3, 5), fav(AssignValue, 3, 6), GenericAssignValue,
	fav(AssignValue, 4, 1), fav(AssignValue, 4, 2), fav(AssignValue, 4, 3), fav(AssignValue, 4, 4), fav(AssignValue, 4, 5), fav(AssignValue, 4, 6), GenericAssignValue,
	fav(AssignValue, 5, 1), fav(AssignValue, 5, 2), fav(AssignValue, 5, 3), fav(AssignValue, 5, 4), fav(AssignValue, 5, 5), fav(AssignValue, 5, 6), GenericAssignValue,
	fav(AssignValue, 6, 1), fav(AssignValue, 6, 2), fav(AssignValue, 6, 3), fav(AssignValue, 6, 4), fav(AssignValue, 6, 5), fav(AssignValue, 6, 6), GenericAssignValue,
};
#else
static void* AssignValueMisses[8][7] = {
	0, 0, 0, 0, 0, 0, GenericAssignValue,
	GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue,/*};
static void* AssignValuePool[6][7] = {*/
	fav(AssignValue, 1, 1) , GenericAssignValue, fav(AssignValue, 1, 3) , fav(AssignValue, 1, 4) , fav(AssignValue, 1, 5) , fav(AssignValue, 1, 6) , GenericAssignValue,
	GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue, GenericAssignValue,
	fav(AssignValue, 3, 1) , GenericAssignValue, fav(AssignValue, 3, 3) , GenericAssignValue, fav(AssignValue, 3, 5) , fav(AssignValue, 3, 6) , GenericAssignValue,
	fav(AssignValue, 4, 1) , GenericAssignValue, GenericAssignValue, fav(AssignValue, 4, 4) , GenericAssignValue, GenericAssignValue, GenericAssignValue,
	fav(AssignValue, 5, 1) , GenericAssignValue, fav(AssignValue, 5, 3) , GenericAssignValue, fav(AssignValue, 5, 5) , fav(AssignValue, 5, 6) , GenericAssignValue,
	fav(AssignValue, 6, 1) , GenericAssignValue, fav(AssignValue, 6, 3) , GenericAssignValue, fav(AssignValue, 6, 5) , fav(AssignValue, 6, 6) , GenericAssignValue,
};
#endif



inline 
void Assign(SMLXMatrix& A, float value) {
	void* pf = AssignValueMisses/*Pool*/[2+SMLXMatrix::lutSizes[A.nr]][SMLXMatrix::lutSizes[A.nc]];
	((VOIDMXV&)pf)(A, value);
}


inline 
void SMLXMatrix::operator=(const SMLXTmpMatrix& B) {
	Resize(B);
	MarkAsDefined();
	// Assign(*this, (SMLXMatrix&)B);
	Swap(B);
#ifndef NDEBUG
	subExpressionsCount = 0;
	// VerifyXMatrix(*this);
#endif
}


inline 
SMLXTmpMatrix& SMLXMatrix::operator+(SMLXTmpMatrix& B) const {
	return B += *this, B;
}


inline 
SMLXTmpMatrix& SMLXMatrix::operator-(SMLXTmpMatrix& B) const {
	return B.NSub(*this), B;
}


inline 
SMLXTmpMatrix& SMLXMatrix::TMat(const SMLXMatrix& A) {
	return A.TMat();
}


inline 
SMLXTmpMatrix& SMLXMatrix::TMat() const {
	SMLXTmpMatrix& tm = TMat(nr, nc);
	(SMLXMatrix&)tm = *this;
	return tm;
}



inline 
SMLXTmpMatrix& __fastcall GenericTMat(short nrow, short ncol) {
	static int pp = 0;
	SMLXTmpMatrix& tm = SMLXMatrix::pool[pp++%SMLXMatrix::poolSize];
	tm.Resize(nrow, ncol);
	tm.MarkAsDefined();
	return tm;
}

#define fTMat(nr, nc) (SMLXTmpMatrix& (__fastcall *)()) TMat ## nr ## x ## nc
#ifdef _PIII_CODE_
static void* TmatMisses[8][7] = {
	0, 0, 0, 0, 0, 0, GenericTMat,
	GenericTMat, GenericTMat, GenericTMat, GenericTMat, GenericTMat, GenericTMat, GenericTMat,
	fTMat(1, 1), fTMat(1, 2), fTMat(1, 3), fTMat(1, 4), fTMat(1, 5), fTMat(1, 6), GenericTMat,
	fTMat(2, 1), fTMat(2, 2), fTMat(2, 3), fTMat(2, 4), fTMat(2, 5), fTMat(2, 6), GenericTMat,
	fTMat(3, 1), fTMat(3, 2), fTMat(3, 3), fTMat(3, 4), fTMat(3, 5), fTMat(3, 6), GenericTMat,
	fTMat(4, 1), fTMat(4, 2), fTMat(4, 3), fTMat(4, 4), fTMat(4, 5), fTMat(4, 6), GenericTMat,
	fTMat(5, 1), fTMat(5, 2), fTMat(5, 3), fTMat(5, 4), fTMat(5, 5), fTMat(5, 6), GenericTMat,
	fTMat(6, 1), fTMat(6, 2), fTMat(6, 3), fTMat(6, 4), fTMat(6, 5), fTMat(6, 6), GenericTMat,
};
#else
static void* TmatMisses[8][7] = {
	0, 0, 0, 0, 0, 0, GenericTMat,
	GenericTMat, GenericTMat, GenericTMat, GenericTMat, GenericTMat, GenericTMat, GenericTMat,/*};
static void* TmatPool[6][7] = {*/
	fTMat(1, 1), GenericTMat, fTMat(1, 3), fTMat(1, 4), fTMat(1, 5), fTMat(1, 6), GenericTMat,
	GenericTMat, GenericTMat, GenericTMat, GenericTMat, GenericTMat, GenericTMat, GenericTMat,
	fTMat(3, 1), GenericTMat, fTMat(3, 3), GenericTMat, fTMat(3, 5), fTMat(3, 6), GenericTMat,
	fTMat(4, 1), GenericTMat, GenericTMat, fTMat(4, 4), GenericTMat, GenericTMat, GenericTMat,
	fTMat(5, 1), GenericTMat, fTMat(5, 3), GenericTMat, fTMat(5, 5), fTMat(5, 6), GenericTMat,
	fTMat(6, 1), GenericTMat, fTMat(6, 3), GenericTMat, fTMat(6, 5), fTMat(6, 6), GenericTMat,
};
#endif


inline 
SMLXTmpMatrix& __fastcall SMLXMatrix::TMat(short nrow, short ncol) {
	__asm push  ecx
	__asm push  edx
	void* pf = TmatMisses/*Pool*/[2+SMLXMatrix::lutSizes[nrow]][SMLXMatrix::lutSizes[ncol]];
	__asm pop   edx
	__asm pop   ecx
	__asm call  dword ptr [pf]
	__asm mov   dword ptr [pf], eax
	((SMLXTmpMatrix*) pf)->nr = nrow;
	((SMLXTmpMatrix*) pf)->nc = ncol;
	return *(SMLXTmpMatrix*) pf;
}


inline 
SMLXTmpMatrix& SMLXMatrix::TMat(const SMLXMatrix& A, short nrow, short ncol) {
	// With memory reservation
	SMLXTmpMatrix& tm = TMat(nrow, ncol);
	tm.Resize(A);
	(SMLXMatrix&)tm = A;
	return tm;
}


ftable_mmm(Add, Add/*operator+*/);
inline 
SMLXTmpMatrix& __fastcall operator+(const SMLXMatrix& A, const SMLXMatrix& B) {
#ifdef COUNT_OPERATIONS
	++SMLXMatrix::mCount[A.nr][A.nc][4];
#endif
	void* pf = AddMisses/*Pool*/[2+SMLXMatrix::lutSizes[A.nr]][SMLXMatrix::lutSizes[A.nc]];
	pf = ((PVOIDMX2&)pf)(A, B);
	(int&)((SMLXTmpMatrix*) pf)->nr = (int&)A.nr;
	return *(SMLXTmpMatrix*) pf;
}


ftable_mmm(Sub, Sub/*operator-*/);
inline 
SMLXTmpMatrix& __fastcall operator-(const SMLXMatrix& A, const SMLXMatrix& B) {
#ifdef COUNT_OPERATIONS
	++SMLXMatrix::mCount[A.nr][A.nc][5];
#endif
	void* pf = SubMisses/*Pool*/[2+SMLXMatrix::lutSizes[A.nr]][SMLXMatrix::lutSizes[A.nc]];
	pf = ((PVOIDMX2&)pf)(A, B);
	(int&)((SMLXTmpMatrix*) pf)->nr = (int&)A.nr;
	return *(SMLXTmpMatrix*) pf;
}


inline 
SMLXTmpMatrix& SMLXTmpMatrix::operator+(const SMLXMatrix& A) {
	*this += A;
	return *this;
}


inline 
SMLXTmpMatrix& SMLXTmpMatrix::operator-(const SMLXMatrix& A) {
	*this -= A;
	return *this;
}


inline 
SMLXTmpMatrix& SMLXTmpMatrix::operator*(float scale) {
	*this *= scale;
	return *this;
}


SMLLIBENTRY extern void* ftable_mm00[];
SMLLIBENTRY extern void* ftable_mm01[];
SMLLIBENTRY extern void* ftable_mm10[];
SMLLIBENTRY extern void* ftable_mm11[];


inline 
int hash(int nr, int nc, int n) {
	return 7 * (7 * (SMLXMatrix::lutSizes[nr] + 1) + SMLXMatrix::lutSizes[nc] + 1) + SMLXMatrix::lutSizes[n] + 1;
	// If 1 <= {nr, nc, n} <= 6, then
	// the following lines will be correct (and maybe a little bit faster):
	// assert(1 <= nr && nr <= 6 && 1 <= nc && nc <= 6 && 1 <= n && n <= 6);
	// return 7 * (7 * nr + nc) + n;
}


inline 
SMLXTmpMatrix& __fastcall operator*(const SMLXMatrix& A, const SMLXMatrix& B) {
#ifdef COUNT_OPERATIONS
	++SMLXMatrix::mCount[A.nr][A.nc][6];
	++SMLXMatrix::mCount[B.nr][B.nc][6];
#endif
	void* pf = ftable_mm00[hash(A.nr, A.nc, B.nc)];
	pf = ((PVOIDMX2&)pf)(A, B);
	((SMLXTmpMatrix*) pf)->nr = A.nr;
	((SMLXTmpMatrix*) pf)->nc = B.nc;
//
//  SMLXTmpMatrix& check = SMLXMatrix::TMat(((SMLXTmpMatrix*) pf)->nr, ((SMLXTmpMatrix*) pf)->nc);
//  check.Mult(A, B, 0, 0);
//  check -= *(SMLXTmpMatrix*) pf;
//  float diff = check.MaxAbs();
//  assert(diff < 1.0e-3);
//
	return *(SMLXTmpMatrix*) pf;
}


inline 
SMLXTmpMatrix& __fastcall operator*(const SMLXTransposedMatrix& A, const SMLXMatrix& B) {
#ifdef COUNT_OPERATIONS
	++SMLXMatrix::mCount[A.nc][A.nr][6];
	++SMLXMatrix::mCount[B.nr][B.nc][6];
#endif
	void* pf = ftable_mm10[hash(A.nr, A.nc, B.nc)];
	pf = ((PVOIDMX2&)pf)(A, B);
	((SMLXTmpMatrix*) pf)->nr = A.nc;
	((SMLXTmpMatrix*) pf)->nc = B.nc;
//
//  SMLXTmpMatrix& check = SMLXMatrix::TMat(((SMLXTmpMatrix*) pf)->nr, ((SMLXTmpMatrix*) pf)->nc);
//  check.Mult(A, B, 1, 0);
//  check -= *(SMLXTmpMatrix*) pf;
//  float diff = check.MaxAbs();
//  assert(diff < 1.0e-3);
//
	return *(SMLXTmpMatrix*) pf;
}


inline 
SMLXTmpMatrix& __fastcall operator*(const SMLXMatrix& A, const SMLXTransposedMatrix& B) {
#ifdef COUNT_OPERATIONS
	++SMLXMatrix::mCount[A.nr][A.nc][6];
	++SMLXMatrix::mCount[B.nc][B.nr][6];
#endif
	void* pf = ftable_mm01[hash(A.nr, A.nc, B.nr)];
	pf = ((PVOIDMX2&)pf)(A, B);
	((SMLXTmpMatrix*) pf)->nr = A.nr;
	((SMLXTmpMatrix*) pf)->nc = B.nr;
//
//  SMLXTmpMatrix& check = SMLXMatrix::TMat(((SMLXTmpMatrix*) pf)->nr, ((SMLXTmpMatrix*) pf)->nc);
//  check.Mult(A, B, 0, 1);
//  check -= *(SMLXTmpMatrix*) pf;
//  float diff = check.MaxAbs();
//  assert(diff < 1.0e-3);
//
	return *(SMLXTmpMatrix*) pf;
}


inline 
SMLXTmpMatrix& __fastcall operator*(const SMLXTransposedMatrix& A, const SMLXTransposedMatrix& B) {
#ifdef COUNT_OPERATIONS
	++SMLXMatrix::mCount[A.nc][A.nr][6];
	++SMLXMatrix::mCount[B.nc][B.nr][6];
#endif
	void* pf = ftable_mm11[hash(A.nr, A.nc, B.nr)];
	pf = ((PVOIDMX2&)pf)(A, B);
	((SMLXTmpMatrix*) pf)->nr = A.nc;
	((SMLXTmpMatrix*) pf)->nc = B.nr;
//
//  SMLXTmpMatrix& check = SMLXMatrix::TMat(((SMLXTmpMatrix*) pf)->nr, ((SMLXTmpMatrix*) pf)->nc);
//  check.Mult(A, B, 1, 1);
//  check -= *(SMLXTmpMatrix*) pf;
//  float diff = check.MaxAbs();
//  assert(diff  < 1.0e-3);
//
	return *(SMLXTmpMatrix*) pf;
}


inline
SMLXSpatialVector::SMLXSpatialVector() 
{
	msize = maxsize = nr = 6; nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
}


inline
SMLXSpatialVector::SMLXSpatialVector(float v0, float v1, float v2, float v3, float v4, float v5) 
{
	msize = maxsize = nr = 6; nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
	vec[0] = v0;
	vec[1] = v1;
	vec[2] = v2;
	vec[3] = v3;
	vec[4] = v4;
	vec[5] = v5;
}


inline
SMLXSpatialVector::SMLXSpatialVector(float v0, float v1, float v2, float v3, float v4) 
{
	msize = maxsize = nr = 5; nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
	vec[0] = v0;
	vec[1] = v1;
	vec[2] = v2;
	vec[3] = v3;
	vec[4] = v4;
}


inline
SMLXSpatialVector::SMLXSpatialVector(float v0, float v1, float v2, float v3) 
{
	msize = maxsize = nr = 4; nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
	vec[0] = v0;
	vec[1] = v1;
	vec[2] = v2;
	vec[3] = v3;
}


inline
SMLXSpatialVector::SMLXSpatialVector(float v0, float v1, float v2) 
{
	msize = maxsize = nr = 3; nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
	vec[0] = v0;
	vec[1] = v1;
	vec[2] = v2;
}


inline
SMLXSpatialVector::SMLXSpatialVector(float v0, float v1) 
{
	msize = maxsize = nr = 2; nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
	vec[0] = v0;
	vec[1] = v1;
}


inline
SMLXSpatialVector::SMLXSpatialVector(float v0) 
{
	msize = maxsize = nr = 1; nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
	vec[0] = v0;
}


inline
SMLXSpatialVector::SMLXSpatialVector(const float* initialValues) 
{
	msize = maxsize = nr = 6; nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
	if(((unsigned int)initialValues) & 0xf)
		GenericAssign(*this, *(SMLXMatrix*)&initialValues);
	else
		Assign(*this, *(SMLXMatrix*)&initialValues);
	MarkAsDefined();
}


inline
SMLXSpatialVector::SMLXSpatialVector(const SMLXSpatialVector& A)
	
{
	msize = maxsize = nr = A.nrows(); nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
	Assign(*this, A);
	MarkAsDefined();
}


inline
SMLXSpatialVector::SMLXSpatialVector(const SMLVec3f& A)
	
{
	msize = maxsize = nr = 3; nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
	vec[0] = A.x;
	vec[1] = A.y;
	vec[2] = A.z;
	MarkAsDefined();
}


inline
SMLXSpatialVector::SMLXSpatialVector(const SMLVec4f& A)
	
{
	msize = maxsize = nr = 4; nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
	vec[0] = A.x;
	vec[1] = A.y;
	vec[2] = A.z;
	vec[3] = A.w;
	MarkAsDefined();
}


inline
SMLXSpatialVector::SMLXSpatialVector(const SMLXMatrix& A)	
{
	assert(A.ncols() == 1 && A.nrows() <= 6);
	msize = maxsize = nr = A.nrows(); nc = 1;
	unsigned int addr =(unsigned int)&m_vecData[0];
	vec = (float*)((addr+0x0f) & 0xfffffff0);
	mat = vec;
	Assign(*this, A);
	MarkAsDefined();
}


inline
void SMLXSpatialVector::AdjustSize(short nrow) 
{
	assert(nrow <= 6);
	SMLXMatrix::AdjustSize(nrow, 1);
}


inline
void SMLXSpatialVector::Copy(const SMLXMatrix& A, int row) 
{
	const float* pr = A[row];
	Assign(*this, *(SMLXMatrix*)&pr);
	MarkAsDefined();
}


inline
void SMLXSpatialVector::operator=(const SMLXMatrix& A) 
{
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][0];
#endif
	assert(this != &A);
	assert(A.ncols() == 1 && A.nrows() <= 6);
	nr = A.nrows();
	Assign(*this, A);
	MarkAsDefined();
}


inline
void SMLXSpatialVector::operator=(const SMLXSpatialVector& A) 
{
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][0];
#endif
	assert(this != &A);
	nr = A.nrows();
	Assign(*this, A);
	MarkAsDefined();
}


inline
void SMLXSpatialVector::operator*=(float scale) 
{
	Scale(scale);
}


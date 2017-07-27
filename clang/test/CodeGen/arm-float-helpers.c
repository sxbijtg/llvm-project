// REQUIRES: arm-registered-target
// RUN: %clang_cc1 -emit-llvm -o - -triple arm-none-linux-gnueabi %s | FileCheck %s
// RUN: %clang_cc1 -emit-llvm -o - -triple arm-none-linux-gnueabihf %s | FileCheck %s
// RUN: %clang_cc1 -emit-llvm -o - -triple arm-none-linux-gnueabi -target-feature "+soft-float" -target-feature "+soft-float-abi" %s | FileCheck %s
// RUN: %clang_cc1 -emit-llvm -o - -triple arm-none-linux-gnueabi -target-feature "+soft-float" %s | FileCheck %s
// RUN: %clang_cc1 -emit-llvm -o - -triple arm-none-eabi %s | FileCheck %s
// RUN: %clang_cc1 -emit-llvm -o - -triple arm-none-eabi -meabi gnu %s | FileCheck %s
// RUN: %clang_cc1 -emit-llvm -o - -triple arm-none-eabi %s | FileCheck %s
// RUN: %clang_cc1 -emit-llvm -o - -triple arm-none-eabi -target-feature "+soft-float" -target-feature "+soft-float-abi" -meabi gnu %s | FileCheck %s
// RUN: %clang_cc1 -emit-llvm -o - -triple arm-none-eabi -target-feature "+soft-float" -meabi gnu %s | FileCheck %s
// RUN: %clang_cc1 -emit-llvm -o - -triple arm-none-eabihf %s | FileCheck %s
// RUN: %clang_cc1 -emit-llvm -o - -triple arm-none-eabihf -meabi gnu %s | FileCheck %s

// The Runtime ABI for the ARM Architecture IHI0043 section 4.1.2 The
// floating-point helper functions to always use the base AAPCS (soft-float)
// calling convention.
//
// These helper functions such as __aeabi_fadd are not explicitly called by
// clang, instead they are generated by the ARMISelLowering when they are
// needed; clang relies on llvm to use the base AAPCS.
//
// In this test we check that clang is not directly calling the __aeabi_
// functions. We rely on llvm to test that the base AAPCS is used for any
// __aeabi_ function from 4.1.2 that is used.
//
// When compiled to an object file with -mfloat-abi=soft each function F
// below should result in a call to __aeabi_F. If clang is changed to call any
// of these functions directly the test will need to be altered to check that
// arm_aapcscc is used.
//
// Note that it is only the functions in 4.1.2 that must use the base AAPCS,
// other runtime functions such as the _Complex helper routines are not covered.

float fadd(float a, float b) { return a + b; }
// CHECK-LABEL: define float @fadd(float %a, float %b)
// CHECK-NOT: __aeabi_fadd
// CHECK: %add = fadd float %0, %1

float fdiv(float a, float b) { return a / b; }
// CHECK-LABEL: define float @fdiv(float %a, float %b)
// CHECK-NOT: __aeabi_fdiv
// CHECK: %div = fdiv float %0, %1

float fmul(float a, float b) { return a * b; }
// CHECK-LABEL: define float @fmul(float %a, float %b)
// CHECK-NOT: __aeabi_fmul
// CHECK: %mul = fmul float %0, %1

float fsub(float a, float b) { return a - b; }
// CHECK-LABEL: define float @fsub(float %a, float %b)
// CHECK-NOT: __aeabi_fsub
// CHECK: %sub = fsub float %0, %1

int fcmpeq(float a, float b) { return a == b; }
// CHECK-LABEL: define i32 @fcmpeq(float %a, float %b)
// CHECK-NOT: __aeabi_fcmpeq
// CHECK: %cmp = fcmp oeq float %0, %1

int fcmplt(float a, float b) { return a < b; }
// CHECK-LABEL: define i32 @fcmplt(float %a, float %b)
// CHECK-NOT: __aeabi_fcmplt
// CHECK: %cmp = fcmp olt float %0, %1

int fcmple(float a, float b) { return a <= b; }
// CHECK-LABEL: define i32 @fcmple(float %a, float %b)
// CHECK-NOT: __aeabi_fcmple
// CHECK: %cmp = fcmp ole float %0, %1

int fcmpge(float a, float b) { return a >= b; }
// CHECK-LABEL: define i32 @fcmpge(float %a, float %b)
// CHECK-NOT: __aeabi_fcmpge
// CHECK: %cmp = fcmp oge float %0, %1

int fcmpgt(float a, float b) { return a > b; }
// CHECK-LABEL: define i32 @fcmpgt(float %a, float %b)
// CHECK-NOT: __aeabi_fcmpgt
// CHECK: %cmp = fcmp ogt float %0, %1

int fcmpun(float a, float b) { return __builtin_isunordered(a, b); }
// CHECK-LABEL: define i32 @fcmpun(float %a, float %b)
// CHECK-NOT: __aeabi_fcmpun
// CHECK: %cmp = fcmp uno double %conv, %conv1

double dadd(double a, double b) { return a + b; }
// CHECK-LABEL: define double @dadd(double %a, double %b)
// CHECK-NOT: __aeabi_dadd
// CHECK: %add = fadd double %0, %1

double ddiv(double a, double b) { return a / b; }
// CHECK-LABEL: define double @ddiv(double %a, double %b)
// CHECK-NOT: __aeabi_ddiv
// CHECK: %div = fdiv double %0, %1

double dmul(double a, double b) { return a * b; }
// CHECK-LABEL: define double @dmul(double %a, double %b)
// CHECK-NOT: __aeabi_dmul
// CHECK: %mul = fmul double %0, %1

double dsub(double a, double b) { return a - b; }
// CHECK-LABEL: define double @dsub(double %a, double %b)
// CHECK-NOT: __aeabi_dsub
// CHECK: %sub = fsub double %0, %1

int dcmpeq(double a, double b) { return a == b; }
// CHECK-LABEL: define i32 @dcmpeq(double %a, double %b)
// CHECK-NOT: __aeabi_dcmpeq
// CHECK: %cmp = fcmp oeq double %0, %1

int dcmplt(double a, double b) { return a < b; }
// CHECK-LABEL: define i32 @dcmplt(double %a, double %b)
// CHECK-NOT: __aeabi_dcmplt
// CHECK: %cmp = fcmp olt double %0, %1

int dcmple(double a, double b) { return a <= b; }
// CHECK-LABEL: define i32 @dcmple(double %a, double %b)
// CHECK-NOT: __aeabi_dcmple
// CHECK: %cmp = fcmp ole double %0, %1

int dcmpge(double a, double b) { return a >= b; }
// CHECK-LABEL: define i32 @dcmpge(double %a, double %b)
// CHECK-NOT: __aeabi_dcmpge
// CHECK: %cmp = fcmp oge double %0, %1

int dcmpgt(double a, double b) { return a > b; }
// CHECK-LABEL: define i32 @dcmpgt(double %a, double %b)
// CHECK-NOT: __aeabi_dcmpgt
// CHECK: %cmp = fcmp ogt double %0, %1

int dcmpun(double a, double b) { return __builtin_isunordered(a, b); }
// CHECK-LABEL: define i32 @dcmpun(double %a, double %b)
// CHECK-NOT: __aeabi_dcmpun
// CHECK: %cmp = fcmp uno double %0, %1

int d2iz(double a) { return (int)a; }
// CHECK-LABEL: define i32 @d2iz(double %a)
// CHECK-NOT: __aeabi_d2iz
// CHECK: %conv = fptosi double %0 to i32

unsigned int d2uiz(double a) { return (unsigned int)a; }
// CHECK-LABEL: define i32 @d2uiz(double %a)
// CHECK-NOT: __aeabi_d2uiz
// CHECK: %conv = fptoui double %0 to i32

long long d2lz(double a) { return (long long)a; }
// CHECK-LABEL: define i64 @d2lz(double %a)
// CHECK-NOT: __aeabi_d2lz
// CHECK: %conv = fptosi double %0 to i64

unsigned long long d2ulz(double a) { return (unsigned long long)a; }
// CHECK-LABEL: define i64 @d2ulz(double %a)
// CHECK-NOT: __aeabi_d2ulz
// CHECK: %conv = fptoui double %0 to i64

int f2iz(float a) { return (int)a; }
// CHECK-LABEL: define i32 @f2iz(float %a)
// CHECK-NOT: __aeabi_f2iz
// CHECK: %conv = fptosi float %0 to i32

unsigned int f2uiz(float a) { return (unsigned int)a; }
// CHECK-LABEL: define i32 @f2uiz(float %a)
// CHECK-NOT: __aeabi_f2uiz
// CHECK: %conv = fptoui float %0 to i32

long long f2lz(float a) { return (long long)a; }
// CHECK-LABEL: define i64 @f2lz(float %a)
// CHECK-NOT: __aeabi_f2lz
// CHECK: %conv = fptosi float %0 to i64

unsigned long long f2ulz(float a) { return (unsigned long long)a; }
// CHECK-LABEL: define i64 @f2ulz(float %a)
// CHECK-NOT: __aeabi_f2ulz
// CHECK: %conv = fptoui float %0 to i64

float d2f(double a) { return (float)a; }
// CHECK-LABEL: define float @d2f(double %a)
// CHECK-NOT: __aeabi_d2f
// CHECK: %conv = fptrunc double %0 to float

double f2d(float a) { return (double)a; }
// CHECK-LABEL: define double @f2d(float %a)
// CHECK-NOT: __aeabi_f2d
// CHECK: %conv = fpext float %0 to double

double i2d(int a) { return (double)a; }
// CHECK-LABEL: define double @i2d(i32 %a)
// CHECK-NOT: __aeabi_i2d
// CHECK: %conv = sitofp i32 %0 to double

double ui2d(unsigned int a) { return (double)a; }
// CHECK-LABEL: define double @ui2d(i32 %a)
// CHECK-NOT: __aeabi_ui2d
// CHECK: %conv = uitofp i32 %0 to double

double l2d(long long a) { return (double)a; }
// CHECK-LABEL: define double @l2d(i64 %a)
// CHECK-NOT: __aeabi_l2d
// CHECK: %conv = sitofp i64 %0 to double

double ul2d(unsigned long long a) { return (unsigned long long)a; }
// CHECK-LABEL: define double @ul2d(i64 %a)
// CHECK-NOT: __aeabi_ul2d
// CHECK: %conv = uitofp i64 %0 to double

float i2f(int a) { return (int)a; }
// CHECK-LABEL: define float @i2f(i32 %a)
// CHECK-NOT: __aeabi_i2f
// CHECK: %conv = sitofp i32 %0 to float

float ui2f(unsigned int a) { return (unsigned int)a; }
// CHECK-LABEL: define float @ui2f(i32 %a)
// CHECK-NOT: __aeabi_ui2f
// CHECK: %conv = uitofp i32 %0 to float

float l2f(long long a) { return (long long)a; }
// CHECK-LABEL: define float @l2f(i64 %a)
// CHECK-NOT: __aeabi_l2f
// CHECK: %conv = sitofp i64 %0 to float

float ul2f(unsigned long long a) { return (unsigned long long)a; }
// CHECK-LABEL: define float @ul2f(i64 %a)
// CHECK-NOT: __aeabi_ul2f
// CHECK: %conv = uitofp i64 %0 to float

// Functions in section 4.1.2 not used by llvm and don't easily map directly to
// C source code.
// cfcmpeq
// cfcmple
// cfrcmple
// cdcmpeq
// cdcmple
// cdrcmple
// frsub
// drsub

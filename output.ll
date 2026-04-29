; ModuleID = 'StackLangModule'
source_filename = "StackLangModule"

@fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@fmt.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@fmt.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @fmt, i32 15)
  %1 = call i32 (ptr, ...) @printf(ptr @fmt.1, i32 5)
  %2 = call i32 (ptr, ...) @printf(ptr @fmt.2, i32 4)
  %3 = call i32 (ptr, ...) @printf(ptr @fmt.3, i32 48)
  ret i32 0
}

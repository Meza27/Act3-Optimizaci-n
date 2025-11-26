; ModuleID = 'Calc'
source_filename = "Calc"

@str_printf_fmt = private unnamed_addr constant [12 x i8] c"Result: %f\0A\00", align 1

define i32 @main() {
begin:
  %0 = call i32 (ptr, ...) @printf(ptr @str_printf_fmt, double 9.000000e+00)
  br label %end

end:                                              ; preds = %begin
  ret i32 0
}

declare i32 @printf(ptr, ...)

; ModuleID = 'Calc'
source_filename = "Calc"

@str_printf_fmt = private unnamed_addr constant [12 x i8] c"Result: %f\0A\00", align 1

define i32 @main() {
begin:
  br label %end

end:                                              ; preds = %begin
  ret i32 0
}

declare i32 @printf(ptr, ...)

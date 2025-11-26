; ModuleID = 'Calc'
source_filename = "Calc"

@str_printf_fmt = private unnamed_addr constant [12 x i8] c"Result: %f\0A\00", align 1

; Function Attrs: nofree nounwind
define noundef i32 @main() local_unnamed_addr #0 {
begin:
  %0 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @str_printf_fmt, double 6.000000e+01)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }

#define LOG(format, ...) \
   fprintf(stderr, format, __VA_ARGS__)
   
#define LOG_FUNC(x) fprintf (stderr, "\nF: " #x  " = %s \t", x)

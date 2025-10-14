export PATH="/c/Program Files/PostgreSQL/16/bin:$PATH"

gcc -I"/c/Program Files/PostgreSQL/18/include/" \
    -I"/c/Program Files/PostgreSQL/18/include/server/" \
    -I"/c/Program Files/PostgreSQL/18/include/server/port/win32" \
    -I"/c/Program Files/PostgreSQL/18/include/server/port/win32_msvc" \
    -L"/c/Program Files/PostgreSQL/18/lib" \
    -shared -o myfunc.dll main.c \
    -lpq -lpgport -lpgcommon
pause
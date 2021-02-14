
# DBJ + IOWOW.IO

Complete reshuffle of very interesting http://iowow.io
( IOWOW - The C11 persistent key/value database engine based on [skip list](https://en.wikipedia.org/wiki/Skip_list) )

For refactoring, I have adopted the "jump head first into the deep end" approach in here. 

Idea is to "make it work" on WIN32. Without CMake and/or VSCode but with VisualStudio 2019 only. Thus and basically: Use VS only and make it work with as little effort as possible.

## What has been done

- The testing/benchmarking code has been taken out into separate folder `[tests](tests)`
  - idea is to make tests work one by one
- CMake has been removed
- Currently (2021 FEB) all is in one VStudio 2019 (VS) solution and project
  - obviously the authors idea was/is to separate the source into some DLL's
  - I will use lib's.
    - I am still not 100% sure where are the lib's exact boundaries
      - that is mainly because I first removed all the CMake related artefacts
- Compiler I am using is `clang-cl` as packaged with VS
  - that is a CLANG 10.0.0 front end, that understands [cl.exe switches](https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options?view=msvc-160)
- First testing that works, should be [`tests/log/tests/iwlog_test1.c`](tests/log_tests/iwlog_test1.c)
- As testing/benchmarking framework, I am using [my windows version](https://github.com/dbj-data/ubut) of [utest.h](https://github.com/sheredom/utest.h)/[ubench.h](https://github.com/sheredom/ubench.h)
  - thus, one `main()` and lot's but not that many `UTEST()`'s 
- > right now I am at the famous "it compiles but it does not link" point, where I have realized it will be (probably) most productive if, I "simplify": 
`kv_file/src/platform/win32/win32.c` 
    - I am not 100% sure here, will have to look into the linking problem a bit deeper.

## Caveat Emptor

My day job is not programming. It is architecting and managing. Thus this all depends on my bandwidth.  Stay tuned :)

Please respect [the licences associated](https://raw.githubusercontent.com/DBJDBJ/kv_file/master/LICENSE?token=AABDOA6Q2V22IXHNCBORZSTAFFQOM).

Questions and suggestions are welcome.
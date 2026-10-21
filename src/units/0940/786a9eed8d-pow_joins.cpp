// lane-m48-b crt: pow-chain composition join (D9 mechanism; linker directive only, defines nothing).
// task-8 pow-runtime-closure-r1/source/cookie_alias.rsp: the original CRT cookie cell 007A3180 read by the pow error
// ports (sse_error.cpp:12, x87_error.cpp:14) is the task-31 legacy cookie owner (image value BB40E64E), never the
// modern compiler ___security_cookie.
#pragma comment(linker, "/alternatename:_task8_crt_security_cookie_007a3180=_legacy_security_cookie")

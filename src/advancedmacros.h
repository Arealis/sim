#ifndef ADVANCEDMACROS_H
#define ADVANCEDMACROS_H

//Number of evaluation passes
#define EVAL1(...) __VA_ARGS__
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL4(...) EVAL2(EVAL2(__VA_ARGS__))
#define EVAL8(...) EVAL4(EVAL4(__VA_ARGS__))
#define EVAL16(...) EVAL8(EVAL8(__VA_ARGS__))
#define EVAL32(...) EVAL16(EVAL16(__VA_ARGS__))
#define EVAL64(...) EVAL32(EVAL32(__VA_ARGS__))
#define EVAL128(...) EVAL64(EVAL64(__VA_ARGS__))
#define EVAL256(...) EVAL128(EVAL128(__VA_ARGS__))
#define EVAL512(...) EVAL256(EVAL256(__VA_ARGS__))
#define EVAL1024(...) EVAL512(EVAL512(__VA_ARGS__))
#define EVAL(...) EVAL1024(__VA_ARGS__)

//Common Values
#define PASS(...) __VA_ARGS__
#define EMPTY()
#define COMMA() ,
#define PLUS() +
#define ZERO() 0
#define ONE() 1

//Defers
#define DEFER1(id) id EMPTY()
#define DEFER2(id) id EMPTY EMPTY()()

//Common operations
#define CAT(a, ...) a ## __VA_ARGS__
#define CAT3(a, b, ...) a ## b ## __VA_ARGS__
#define ADD(a,b) (a) + (b)
#define FIRST(a, ...) a
#define SECOND(a, b, ...) b

#define IS_PROBE(...) SECOND(__VA_ARGS__, 0)
#define PROBE() ~, 1

#define NOT(x) IS_PROBE(CAT(_NOT_, x))
#define _NOT_0 PROBE()

#define BOOL(x) NOT(NOT(x))

#define OR(a,b) CAT3(_OR_, a, b)
#define _OR_00 0
#define _OR_01 1
#define _OR_10 1
#define _OR_11 1

#define AND(a,b) CAT3(_AND_, a, b)
#define _AND_00 0
#define _AND_01 0
#define _AND_10 0
#define _AND_11 1

#define _IF(c) CAT(_IF_,c)
#define IF(c) _IF(BOOL(c))
#define _IF_0(...)
#define _IF_1(...) __VA_ARGS__

#define _IF_ELSE(c) CAT(_IF_ELSE_,c)
#define IF_ELSE(c) _IF_ELSE(BOOL(c))
#define _IF_ELSE_0(t,f) f
#define _IF_ELSE_1(t,f) t

#define HAS_ARGS(...) BOOL(FIRST(_END_OF_ARGUMENTS_ __VA_ARGS__)(0))
#define _END_OF_ARGUMENTS_(...) BOOL(FIRST(__VA_ARGS__))


//The recursive part
#define MAP_INNER(op,sep,bit,cur_val, ...) \
  op(cur_val, bit) \
  IF(HAS_ARGS(__VA_ARGS__))( \
    sep() DEFER2(_MAP_INNER)()(op, sep, ADD(bit,bit), ##__VA_ARGS__) \
  )

#define MAP(...) \
   EVAL64(MAP_INNER(__VA_ARGS__))

#define _MAP_INNER() MAP_INNER



#endif // ADVANCEDMACROS_H

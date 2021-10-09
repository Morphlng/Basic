# Basic

An interpretive language realized by C++

[中文仓库](https://gitee.com/Morphlng/basic)

## Decription

A Basic(literally) language interpreter.

1. Lexer. Separate input into a list of Tokens
2. Parser. Read the Tokens and generate a AST Tree
3. Interpreter. Analyze the AST Tree and Interpretes it.

## How to install

1. clone the repo
2. under root folder, use `make`
3. in folder `output`, there will be a *basic* executable file
4. `make clean` can clear the `.o` files
5. `make clean_all` can clear everything that's compiled

## Grammar

> For complete Grammar Definition see grammar.txt

### Variable

We have type of `Number`、`String`、`List`、`Dict`、`Function`, you can declare one as follow:

```pascal
VAR num = 1                     // both Int and Float are Number
VAR str = "hello"
VAR list = [1,"2",[3,4]]        // List can store any type of value
VAR dict = { name:"David", age:18 }
VAR lambda = FUNC (a,b)->a+b    // you can define a lambda function, or a function with name(see below)
```

#### Number

For type `Number`, you can do these things：

1. Arithmetic

```basic
basic > 123 + 456
579
basic > 456 - 123
333
basic > 123 * 456
56088
basic > 233 / 1314
0.177321
basic > 426 ^ 3
77308776
```

2. Bool logic
   
```basic
basic > 123 < 456
1
basic > 123 > 456
0
basic > 123 == 123
1
basic > 123 != 456
1
basic > 123 <= 456
1
basic > 123 >= 456
0
```

3. Logic

```basic
basic > 123 AND 456 // Equivalent to & in C
72
basic > 123 OR 456 // Equivalent to | in C
507
basic > NOT 1
0
```

4. True/False

There is not type of Boolean, so `1 stands for True`, and `0 stands for False`

#### String

For type `String`, you can `add(+)`、`multiply(*)` and `index[]`：

```pascal
basic > VAR str = "123"
basic > str + "456"
"123456"
basic > str * 3
"123123123"
basic > 3 * str
"123123123"
basic > str[0]
"1"
```

> Please Notice: These operations **are not Mutable**

#### List

For type `List`, you can `add(+)`、`concat(*)`、`del(-)`、`index([])`：

```pascal
basic > VAR list1 = []
basic > VAR list2 = [4,5,6]
basic > list1 + 1
[1]
basic > list1 * list2
[4,5,6]
basic > list2 - 1
[5,6]
basic > list2[0]
4
```

> Please Notice: These operations **are not Mutable**

#### Dict

For type `Dict`, you can `index by key["key"]` or `attr_by(a.name)`

```javascript
basic > VAR dict = { name:"David", age:18}
basic > dict["name"]
"David"
basic > dict.gender
undefined
basic > dict
{ age:18, gender:undefined, name:"David" }
```

> You might notice that, attr_by an undefined attribute will automatically insert and set to undefined

#### Function

Function can be anonymous or have a name. Also it can be single line or multiline, see example:

1. Multiline

```basic
basic > FUNC add(a,b):
...         IF IS_NUM(a) AND IS_NUM(b) THEN:
...             PRINT(a+b)
...         ELSE:
...             PRINT("operator must be Numbers");
...         END;
...     END
<function add>
basic > add(1,5)
6
basic > add("1",5)
operator must be Numbers
```

2. Single-line(use ->)

```basic
basic > VAR check = FUNC (a)-> IF IS_NUM(a) THEN PRINT("value is Number") ELIF IS_STR(a) THEN PRINT("value is String") ELSE PRINT("value is other type")
basic > check(1)
value is Number
basic > check("1")
value is String
```

### Control statements

#### IF expression

1. Single-Line

```basic
basic > VAR age = 14
14
basic > IF age < 13 THEN PRINT("child") ELIF age < 18 THEN PRINT("teenager") ELSE PRINT("adult")
"teenager"
```

2. Multi-Line With END

> Whether you need to end with END, depend on the last condition statement is multi-line or single-line. In the case below, Else followed by ":", means that it's a multiline statement.

```basic
basic > IF age<13 THEN:
...         PRINT("child");
...     ELIF age<18 THEN:
...         PRINT("teenager");
...     ELSE:
...         PRINT("Adult");
...     END
```

3. Cross-Line

```basic
basic > IF age<13 THEN:
...         PRINT("child") ELIF age<18 THEN:
...             PRINT("teenager") ELSE PRINT("Adult") 
```

#### While loop

`WHILE <condition> THEN <expr>`

1. Single-Line

> It's not really useable, since the "i" won't auto-increase

```basic
basic > VAR i = 0
basic > WHILE i < 10 THEN VAR i = i + 1
```

2. Multi-Line

```basic
basic > VAR i = 1
basic > WHILE i<10 THEN:
...         PRINT(i);
...         VAR i = i + 1;
...     END
```

#### For loop

`FOR <var_name> = <start_value> TO <end_value> (STEP <step_length>)? THEN <expr>`

1. Single-Line

> For loop have auto-increment, so you can do some basic things with single-line statement.

```basic
basic > FOR i = 1 TO 10 STEP 2 THEN PRINT(i)
13579
```

2. Multi-Line

```basic
basic > FOR i=1 TO 9 THEN:
...         FOR j = 1 TO 9 THEN:
...             PRINT(i*j);
...             PRINT("\t");
...         END;
...         PRINT("\n");
...     END
```

### built-in Functions

提供了几个内置函数, 这些函数的功能可能还会有变化. 

- `PRINT(value)`. print the given value
- `PRINTS(list, ends_with)`. call it like: PRINTS([1,2],"\n")
- `PRINT_RET(value)`. this will return the value that have been given.
- `INPUT()`. receive user input as String
- `INPUT_NUM()`. receive user input as Number
- `CLEAR()`. clear the terminal
- `IS_NUM(value)`. check if value is a Number
- `IS_STR(value)`. check if value is a String
- `IS_List(value)`. check if value is a List
- `IS_FUNC(value)`. check if value is a Function
- `APPEND(list, elem)`. **mutable function**, append elem to the back of list
- `POP(list, index)`. **mutable function**, delete elem at index in list, return elem. 
- `POP_BACK(list)`. **mutable function**, delete the last elem in list, return elem
- `POP_FRONT(list)`. **mutable function**, delete the first elem in list, return elem
- `EXTEND(list1, list2)`. **mutable function**, append list2 to list1.
- `RUN(filepath)`. You can save basic code in file, then use RUN to execute. This also equivalent to import
- SWAP(&a,&b). **You should call this function by Ref**, see below Reference.

### 4.4 CONTINUE、BREAK、RETURN

1. RETURN

```basic
basic > FUNC test():
...         VAR foo = 5;
...         RETURN foo;
...     END
basic > test()
5
```

2. For-loop

```basic
basic > VAR list = []
basic > FOR i = 1 TO 10 THEN:
...         IF i == 4 THEN:
...             CONTINUE;
...         ELIF i == 8 THEN BREAK;
...         APPEND(list, i)
...     END
basic > list
[1,2,3,5,6,7]
```

3. While-loop

```basic
basic > VAR i = 0
basic > VAR list = []
basic > WHILE i < 10 THEN:
...         VAR i = i + 1;
...         IF i==4 THEN CONTINUE;
...         IF i==8 THEN BREAK;
...         APPEND(a,i);
...     END
basic > list
[1,2,3,5,6,7]
```

### Reference

Sometime, we want to pass arguments by its value, othertime, reference. So I'll introduce `&` to represent reference:

```javascript
basic > VAR a = 1, b = "hello"
basic > SWAP(a,b)
// passing by value, it won't do anything
basic > SWAP(&a,&b)
// passing by reference, a will be "hello" while b will be 1
```

For your own function, call it like that:

```javascript
basic > FUNC test(a)-> VAR &a = a + 1
basic > VAR a = 1
basic > test(&a)
2
```

> Please notice: such as `VAR &IDENTIFIER = expr`, I call it *mutation*, that requires the IDENTIFIER to be already existed. While `VAR IDENTIFIER = expr` is called *definition*, so the IDENTIFIER can be brand new.

### Comment

line that starts with "#" is comment.

## Command line Arguments

In order to ease the use of running script, I implemented command line mode beside interactive mode. Available args are as follows:

```bash
$ ./basic -h
Welcome to Basic!
Usage: ./basic [options...]

Options:
        -f,--file : Execute Basic script from given file_path [default: none]
        -t,--text : Execute Basic script from given text [default: none]
               -i : A flag to toggle interactive mode [implicit: "true", default: false]
     -v,--verbose : A flag to toggle verbose [implicit: "true", default: false]
       -D,--Debug : A flag to toggle debug mode [implicit: "true", default: false]
        -h,--help : print help [implicit: "true", default: false]
```

When input no args or explicitly `-i`, will enter interactive mode.

## Credits

|              [David Callanan](https://github.com/davidcallanan)              |
| :--------------------------------------------------------------------------: |
| <img width="60" src="https://avatars.githubusercontent.com/u/40340425?v=4"/> |
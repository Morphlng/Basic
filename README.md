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
3. `make clean` can clear the `.o` files
4. `make clean_all` can clear everything that's compiled

## Grammar

> For complete Grammar Definition see grammar.txt

### Variable

We have type of `Number`、`String`、`List`、`Function`, you can declare one as follow:

```pascal
VAR num = 1                     // both Int and Float are Number
VAR str = "hello"
VAR list = [1,"2",[3,4]]        // List can store any type of value
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

For type `String`,you can `add(+)`、`multiply(*)` and `index[]`：

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

For type `List`,you can `add(+)`、`concat(*)`、`del(-)`、`index([])`：

```pascal
basic > VAR list1 = []
basic > VAR list2 = [4,5,6]
basic > list1 + 1
[1]
basic > list1 * list2
[4,5,6]
basic > list2-1
[5,6]
basic > list2[0]
4
```

> Please Notice: These operations **are not Mutable**

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

提供了几个内置函数, 这些函数的功能可能还会有变化。

- `PRINT(value)`。print the given value
- `PRINTS(list, ends_with)`。call it like: PRINTS([1,2],"\n")
- `PRINT_RET(value)`。this will return the value that have been given.
- `INPUT()`。receive user input as String
- `INPUT_NUM()`。receive user input as Number
- `CLEAR()`。clear the terminal
- `IS_NUM(value)`。check if value is a Number
- `IS_STR(value)`。check if value is a String
- `IS_List(value)`。check if value is a List
- `IS_FUNC(value)`。check if value is a Function
- `APPEND(list, elem)`。**mutable function**, append elem to the back of list
- `POP(list, index)`。**mutable function**, delete elem at index in list, return elem。
- `POP_BACK(list)`。**mutable function**, delete the last elem in list, return elem
- `POP_FRONT(list)`。**mutable function**, delete the first elem in list, return elem
- `EXTEND(list1, list2)`。**mutable function**, append list2 to list1.

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

### Comment

line that starts with "#" is comment.

## Run Script

Use `Run(filepath)` to run code written in script：

```basic
# This is a demostration of the language

FUNC mul_table():
	FOR i = 1 TO 9 THEN:
		FOR j = 1 TO i THEN:
			PRINTS([i, "*", j, "=", i*j]," ")
		END
		PRINT("\n")
	END
END

mul_table()	
```

save the code above in `multiplication_table.txt`, and then:

```basic
basic > RUN("./scripts/multiplication_table.txt")
1 * 1 = 1

2 * 1 = 2  2 * 2 = 4

3 * 1 = 3  3 * 2 = 6  3 * 3 = 9

4 * 1 = 4  4 * 2 = 8  4 * 3 = 12  4 * 4 = 16

5 * 1 = 5  5 * 2 = 10  5 * 3 = 15  5 * 4 = 20  5 * 5 = 25

6 * 1 = 6  6 * 2 = 12  6 * 3 = 18  6 * 4 = 24  6 * 5 = 30  6 * 6 = 36

7 * 1 = 7  7 * 2 = 14  7 * 3 = 21  7 * 4 = 28  7 * 5 = 35  7 * 6 = 42  7 * 7 = 49

8 * 1 = 8  8 * 2 = 16  8 * 3 = 24  8 * 4 = 32  8 * 5 = 40  8 * 6 = 48  8 * 7 = 56  8 * 8 = 64

9 * 1 = 9  9 * 2 = 18  9 * 3 = 27  9 * 4 = 36  9 * 5 = 45  9 * 6 = 54  9 * 7 = 63  9 * 8 = 72  9 * 9 = 81

[<function mul_table>]
```

## Credits

|              [David Callanan](https://github.com/davidcallanan)              |
| :--------------------------------------------------------------------------: |
| <img width="60" src="https://avatars.githubusercontent.com/u/40340425?v=4"/> |
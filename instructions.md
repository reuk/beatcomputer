# all instructions?

NOP
ADD
SUB
MUL
DIV
BAND
BOR
BNOT
BNOR
BNAND
BXOR
AND
OR
NOT
MOVE
LOAD
SAVE
JUMP
JE
JNE
JL
JLE
JG
JGE
RND

# do nothing
NOP

# dest = x + y
ADD dest register register
ADD dest register const

# dest = x - y
SUB dest register register
SUB dest register const

# dest = x * y
MUL dest register register
MUL dest register const

# dest = x / y
DIV dest register register
DIV dest register const

# dest = x & y
BAND dest register register
BAND dest register const

# dest = x | y
BOR dest register register
BOR dest register const

# dest = ~x
BNOT dest register
BNOT dest const

# dest = ~(x | y)
BNOR dest register register
BNOR dest register const

# dest = ~(x & y)
BNAND dest register register
BNAND dest register const

# dest = x ^ y
BXOR dest register register
BXOR dest register const

# dest = x && y
AND dest register register
AND dest register const

# dest = x || y
OR dest register register
OR dest register const

# dest = ! x
NOT dest register
NOT dest const

# dest = x
MOVE dest register
MOVE dest const

# dest = memory[address]
LOAD dest address

# memory[address] = dest
SAVE dest address

# ip = address
JUMP address

# jump if equal
JE register register address

# jump if not equal
JNE register register address

# jump if less than
JL register register address

# jump if less than or equal
JLE register register address

# jump if greater than
JG register register address

# jump if greater than or equal
JGE register register address

# dest = rng
RND dest

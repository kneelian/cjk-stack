# misc insns

- 嘸 - m4 `NOP`
- 死 - sei2 `DIE`

- 叛 - toggle debug

# stack manipulation

- 擰 - swap main and side stacks
- 殲 - dump whole main stack
- 棄 - pop one element from main stack

- 換 - swap top two: ab -> ba
- 紡 - rotate top three: abc -> cab

- 哥 - copy from side to main

# push constants

for numbers 0x0 .. 0xa:

- 零 - zero
- 壹 - one
- 貳 - two
- 參 - three
- 肆 - four
- 伍 - five
- 陸 - six
- 柒 - seven
- 捌 - eight
- 玖 - nine
- 拾 - ten

# arithmetics

- 增 - increment
- 低 - decrement
- 加 - add
- 減 - subtract
- 除 - divide
- 乘 - multiply
- 餘 - remainder/modulo

- 基 - log base e
- 根 - root
- 冪 - pow

- 階 - factorial of integer

# type conversion

- 整 - round (from float)
- 彎 - bend (to float)

# conditionals

- 越 - bigger (n1 >? n2), produces predicate
- 敢 - is-it-so? (consumes predicate, skips next insn if true)

# flow control

- 安 - where? push current location to side
- 跳 - jump - pop side and jump unconditionally
- 落 - fall - pop side and jump if nonzero

- 宣 - register subroutine
- 終 - finish subroutine
- 嗌 - invoke subroutine

# printing

- 報 - peek and print to terminal
